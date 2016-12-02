//=============================================================================
// Basic.fx by Frank Luna (C) 2011 All Rights Reserved.
//
// Basic effect that currently supports transformations, lighting, and texturing.
//=============================================================================

#include "LightHelper.fx"


// G버퍼 언팩
SURFACE_DATA UnpackGBuffer(float4 PosL, float2 Tex)
{
	SURFACE_DATA Out;
	
	// 각 텍스처 뽑기
	Out.DepthTex     = gGDepthTex   .Sample(samLinear, Tex);
	Out.DiffuseTex   = gGDiffuseTex .Sample(samLinear, Tex);
	Out.PositionTex  = gGPositionTex.Sample(samPoint , Tex);
	Out.SpecularTex  = gGSpecularTex.Sample(samLinear, Tex);
	Out.TanNormalTex = gGNormalTex  .Sample(samLinear, Tex);

	// 타젠트 노멀
	// 좌표 -1 ~ 1사이로 바꾸기 (혹은 복구)
	Out.TanNormalTex = (Out.TanNormalTex * 2.0f) - 1.0f;
	
	////--------------------------------------------------------------//
	//// 좌표 Z얻기
	////--------------------------------------------------------------//
	//// 1.
	////// 좌표 = ( 파 - 니어 ) *  깊이버퍼 + 니어
	//float PositionTexZ = (gFar - gNear) * Out.DepthTex.x + gNear;
	//
	//float4 pos = float4(PosL.xy, Out.DepthTex.x, 1.0f);
	//pos = mul(pos, gProjInvTranspose); // 뷰로 옴긴다                   // <-- 현재 역행렬들이 스크린을 대상으로한 역행렬이 아님. -_-;;
	//pos.z = PositionTexZ;
	//pos = mul(pos, gViewInvTranspose); // 월드로 옮긴다.
	//pos.xyz /= pos.w;
	//Out.PositionTex = pos;
	////--------------------------------------------------------------//
	//
	//// 최종 위치 구하기
	//float4 Position = float4(Out.PositionTex.xy, PositionTexZ, 1.0f);
	//Position = mul(Position, gViewInvTranspose);
	////Position.z *= Position.w;
	//Out.PositionTex = Position;
	//
	//// 2
	//Out.PositionTex = (Out.PositionTex  * 2.0f) - 1.0f;        // 복원
	//
	//
	//- 이건 다른거---//
	//pos = mul(pos, gViewInvTranspose); // 월드로 옮긴다.

	//--------------------------------------------------------------//
	return Out;
}

// 버텍스
GVertexOut VS(GVertexIn vin)
{
	GVertexOut vout;

	// Transform to world space space.
	vout.PosL = float4(vin.PosL, 1.0f);

	// Output vertex attributes for interpolation across triangle.
	// 어차피 변환결과는 같음.
	vout.Tex = mul(float4(vin.Tex, 0.0f, 1.0f), gTexTFMtx).xy;

	

	return vout;
}

// 두번째 매개변수를 통해, 텍스처 사용 유무를 가른다.
float4 PS(GVertexOut pin, uniform int gShaderMode) : SV_Target
{
	// 화면 좌표를 얻는다
	SURFACE_DATA sData = UnpackGBuffer(pin.PosL, pin.Tex);

	// 노말 맵 결과
	float  DotDirectLightNomalMap = 0.0f;
	float  DotPointLightNomalMap  = 0.0f;
	//float  DotNomalMap = 0.0f;

	// 아무것도 처리안해도 디퓨즈
	float4 litColor = sData.DiffuseTex;

	// 각 Pos, LookDir 방향벡터
	float3 toEye = gEyePosW - sData.PositionTex.xyz;
	float distToEye = length(toEye);
	toEye /= distToEye;

	// 테스트 1
	// 디퓨즈만 출력
	//return sData.DiffuseTex;
	//
	// 탄젠트 노멀만 출력
	//return sData.TanNormalTex;
	//
	// 스팩큘러맵
	//return sData.SpecularTex;
	//
	// 깊이맵
	//return sData.DepthTex;
	//
	// 포지션맵
	//return sData.PositionTex;


	// 디렉셔널 라이트
	float3 gDirectionLight_Dir = normalize(-gDirLight.Direction);
	DotDirectLightNomalMap     = saturate(dot(sData.TanNormalTex.xyz, gDirectionLight_Dir)) * 2.0f;
	
	// 테스트 1
	// 포인트 라이트
	//float gPointLight_Length = length(gPointLight.Position.xyz - sData.PositionTex.xyz);
	//
	//// 거리 확인
	//[flatten]
	//if (gPointLight_Length < gPointLight.Range)
	//{
	//	// 거리 마다 % 를 구한다.
	//	float gFinPointLength = gPointLight.Range / gPointLight_Length;
	//
	//	// 라이트 영역내에 있다면,
	//	float3 gPointLight_Dir = normalize(gPointLight.Position.xyz - sData.PositionTex.xyz);
	//		DotPointLightNomalMap += saturate(dot(sData.TanNormalTex.xyz, gPointLight_Dir)) * pow(gFinPointLength, 2.f) * 0.01f;		  // +=
	//}
	//	
	////// 스팟 라이트
	//float gSpotLight_Length = length(sData.PositionTex.xyz - gSpotLight.Position.xyz);
	//
	//// 거리 확인
	//if (gSpotLight_Length < gSpotLight.Range)
	//{
	//	float3 gSpotLight_Dir = normalize(gSpotLight.Position.xyz - sData.PositionTex.xyz);
	//		DotNomalMap = saturate(dot(sData.TanNormalTex.xyz, gSpotLight_Dir));				   // +=
	//}

	//
	// Lighting.
	//

	// Start with a sum of zero.
	float4 A, D, S;

	float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 spec    = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// 디렉셔널 라이트 
	ComputeDirectionalLight(gMaterial, gDirLight, sData.TanNormalTex.xyz, toEye, A, D, S);
	ambient += A;
	diffuse += D;
	spec    += S;
	
	//// 포인트 라이트
	//[flatten]
	//if (gPointLight_Length < gPointLight.Range)
	//{
	//	// 거리 마다 % 를 구한다.
	//	float gFinPointLength = gPointLight.Range / gPointLight_Length;
	//
	//	ComputePointLight(gMaterial, gPointLight, sData.PositionTex.xyz, sData.TanNormalTex.xyz, toEye, A, D, S);
	//	ambient += A * gFinPointLength * 0.2f; //(gPointLight_Length < 0.0 ? 0.0 : gFinPointLength);
	//	diffuse += D * gFinPointLength * 0.2f; //(gPointLight_Length < 0.0 ? 0.0 : gFinPointLength);
	//	spec    += S * gFinPointLength * 0.2f; //(gPointLight_Length < 0.0 ? 0.0 : gFinPointLength);
	//}
	//
	//
	//// 스폿 라이트
	//if (gSpotLight_Length < gSpotLight.Range)
	//{
	//	ComputeSpotLight(gMaterial, gSpotLight, sData.PositionTex.xyz, sData.TanNormalTex.xyz, toEye, A, D, S);
	//	ambient += A;
	//	diffuse += D;
	//	spec    += S;
	//}

	// 렌더링 모드
	switch (gShaderMode)
	{
	case 0:
		// 퐁 셰이더
		litColor = ((diffuse + ambient) * sData.DiffuseTex) + (spec * sData.SpecularTex);
		break;

	case 1:
		// 카툰 렌더링
		float4 FinLight = ceil((ambient + diffuse) * 4) / 4;
			litColor = FinLight * sData.DiffuseTex + (spec * sData.SpecularTex);
		break;
	}

	// Common to take alpha from diffuse material and texture.
	litColor.a = gMaterial.Diffuse.a * sData.DiffuseTex.a;

	//---------------------------------------------------------//
	// 테스트 2
	// 법선맵만 출력
	//litColor = DotNomalMap;
	//return litColor;
	//
	//---------------------------------------------------------//
	// 테스트 3
	// 이것저것 출력
	//litColor = DotNomalMap * sData.DiffuseTex + sData.SpecularTex;
	//return litColor; 

	//---------------------------------------------------------//
	// 완료 
	// 법선 매핑 곱하기
	litColor.xyz *= (DotDirectLightNomalMap + DotPointLightNomalMap);
	//litColor.xyz *= (length(DotDirectLightNomalMap + DotPointLightNomalMap) < 1.5f ? DotDirectLightNomalMap + DotPointLightNomalMap : 1.5f );
	return litColor; 
	//---------------------------------------------------------//
}


// 여러개의 라이트를 만든다.
// 매개변수를 직접적으로 설정함으로써, 골라 쓸 수 있게 만듬.

// 라이트 + 텍스처 버전
technique11 Deferred
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS(0)));

		SetRasterizerState(0);
		SetDepthStencilState(LessDSS, 0);
	}
}
