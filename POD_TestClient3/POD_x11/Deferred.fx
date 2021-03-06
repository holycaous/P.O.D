//=============================================================================
// Basic.fx by Frank Luna (C) 2011 All Rights Reserved.
//
// Basic effect that currently supports transformations, lighting, and texturing.
//=============================================================================

#include "LightHelper.fx"


// G버퍼 언팩
SURFACE_DATA UnpackGBuffer(float2 Tex)
{
	SURFACE_DATA Out;
	
	// 각 텍스처 뽑기
	Out.DepthTex     = gGDepthTex   .Sample(samLinear, Tex);
	Out.DiffuseTex   = gGDiffuseTex .Sample(samLinear, Tex);
	Out.PositionTex  = gGPositionTex.Sample(samLinear, Tex);
	Out.SpecularTex  = gGSpecularTex.Sample(samLinear, Tex);
	Out.TanNormalTex = gGNormalTex  .Sample(samLinear, Tex);

	// 타젠트 노멀
	// 좌표 -1 ~ 1사이로 바꾸기 (혹은 복구)
	Out.TanNormalTex = (Out.TanNormalTex * 2.0f) - 1.0f;
	
	//--------------------------------------------------------------//
	// 좌표 Z얻기
	//--------------------------------------------------------------//
	// 1.
	//// 좌표 = ( 파 - 니어 ) *  깊이버퍼 + 니어
	//float PositionTexZ = (gFar - gNear) * Out.DepthTex.x + gNear;
	
	//// 최종 위치 구하기
	//float4 Position = float4(Out.PositionTex.xy, PositionTexZ, 1.0f);
	//Position = mul(Position, gViewInvTranspose);
	////Position.z *= Position.w;
	//Out.PositionTex = Position;

	//// 2
	//Out.PositionTex = (Out.PositionTex  * 2.0f) - 1.0f;        // 복원
	//Out.PositionTex = mul(Out.PositionTex, gProjInvTranspose); // 뷰로 옴긴다
	//Out.PositionTex = mul(Out.PositionTex, gViewInvTranspose); // 월드로 옮긴다.
	//Out.PositionTex.z = PositionTexZ;
	//--------------------------------------------------------------//
	return Out;
}

// 버텍스
GVertexOut VS(GVertexIn vin)
{
	GVertexOut vout;

	// Transform to world space space.
	vout.PosW = mul(float4(vin.PosL, 1.0f), vin.World).xyz;
	//vout.PosW = vin.PosL;

	// Transform to homogeneous clip space.
	vout.PosH = mul(float4(vout.PosW, 1.0f), gProj);

	// Output vertex attributes for interpolation across triangle.
	// 어차피 변환결과는 같음.
	vout.Tex = mul(float4(vin.Tex, 0.0f, 1.0f), gTexTFMtx).xy;

	return vout;
}

// 두번째 매개변수를 통해, 텍스처 사용 유무를 가른다.
float4 PS(GVertexOut pin, uniform int gShaderMode) : SV_Target
{
	// 화면 좌표를 얻는다
	SURFACE_DATA sData = UnpackGBuffer(pin.Tex);

	// 노말 맵 결과
	float  DotNomalMap = 1.0f;

	// 아무것도 처리안해도 디퓨즈
	float4 litColor = sData.DiffuseTex;

	// 각 Pos, LookDir 방향벡터
	float3 toEye    = gEyePosW - sData.PositionTex.xyz;
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

	// 화면에 빛을 안비추는 방법..
	// 디렉셔널 라이트
	float3 gDirectionLight_Dir = normalize(-gDirLight.Direction);
		DotNomalMap = saturate(dot(sData.TanNormalTex.xyz, gDirectionLight_Dir));
	
	// 테스트 1
	// 포인트 라이트
	//float gPointLight_Length = length(gPointLight.Position.xyz - sData.PositionTex.xyz);
	
	// 거리 확인
	//[flatten]
	//if (gPointLight_Length < gPointLight.Range)
	//{
		// 라이트 영역내에 있다면,
		float3 gPointLight_Dir = normalize(gPointLight.Position.xyz - sData.PositionTex.xyz);
			DotNomalMap += saturate(dot(sData.TanNormalTex.xyz, gPointLight_Dir));		 		 // +=
	//}
	
	////// 스팟 라이트
	//float gSpotLight_Length = length(sData.PositionTex.xyz - gSpotLight.Position.xyz);
	//
	//// 거리 확인
	////if (gSpotLight_Length < gSpotLight.Range)
	//{
	//	float3 gSpotLight_Dir = normalize(gSpotLight.Position.xyz - sData.PositionTex.xyz);
	//		DotNomalMap = saturate(dot(sData.TanNormalTex.xyz, gSpotLight_Dir));				 // +=
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
	ComputeDirectionalLight(sData.DiffuseTex, gMaterial, gDirLight, sData.TanNormalTex.xyz, toEye, A, D, S);
	ambient += A;
	diffuse += D;
	spec    += S;
	
	// 포인트 라이트
	//if (gPointLight_Length < gPointLight.Range)
	//{
		ComputePointLight(sData.DiffuseTex, gMaterial, gPointLight, sData.PositionTex.xyz, sData.TanNormalTex.xyz, toEye, A, D, S);
		ambient += A;
		diffuse += D;
		spec    += S;
	//}

	//// 스폿 라이트
	//if (gSpotLight_Length < gSpotLight.Range)
	//{
	//	ComputeSpotLight(sData.DiffuseTex, gMaterial, gSpotLight, sData.PositionTex.xyz, sData.TanNormalTex.xyz, toEye, A, D, S);
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

	//---------------------------------------------------------//
	// 테스트 3
	// 이것저것 출력
	//litColor = DotNomalMap * sData.DiffuseTex + sData.SpecularTex;
	//return litColor; 

	//---------------------------------------------------------//
	// 완료 
	// 법선 매핑 곱하기
	litColor.xyz *= DotNomalMap;
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
	}
}
