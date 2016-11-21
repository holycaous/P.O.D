//=============================================================================
// Basic.fx by Frank Luna (C) 2011 All Rights Reserved.
//
// Basic effect that currently supports transformations, lighting, and texturing.
//=============================================================================

#include "LightHelper.fx"


// G 버퍼 패킹
PS_GBUFFER_OUT PackGBuffer(PNTVertexOut pin)
{
	PS_GBUFFER_OUT Out;
	
	// Interpolating normal can unnormalize it, so normalize it.
	pin.NormalW = normalize(pin.NormalW);

	// Default to multiplicative identity.
	// 텍스처 사용 유무
	float4 DepthTex       = float4(1, 1, 1, 1);
	float4 DiffuseTex     = float4(1, 1, 1, 1);
	float4 SpecularTex    = float4(1, 1, 1, 1);
	float4 BumpNormalTex  = float4(1, 1, 1, 1);
	float3 TangentNormal  = float3(1, 1, 1);
	
	// Sample texture.
	// 텍셀 구하기
	DiffuseTex    = gDiffuseTex .Sample(samAnisotropic, pin.Tex);
	SpecularTex   = gSpecularTex.Sample(samAnisotropic, pin.Tex);
	BumpNormalTex = gNormalTex  .Sample(samAnisotropic, pin.Tex);

	// 포지션맵 만들기
	// 좌표를 뷰포트로 옮긴다.
	float4 PosV = mul(float4(pin.PosW, 1.0f), gView); 
	//float4 PosP = mul(float4(pin.PosW, 1.0f), gViewProj); //<<---------- 뷰 프로젝션으로 넘기고 있음 
	PosV /= PosV.w; // 데카르트 좌표계로 넘기기 위해 (동차좌표로 보내기 위해)
	//PosP /= PosP.w;

	// 깊이 맵 만들기
	// (파-니어) : 1 = (좌표-니어) : ?
	// 깊이 버퍼 = (좌표-니어) / (파 - 니어)
	// 좌표 = ( 파 - 니어 ) *  깊이버퍼 + 니어
	float  Depth = (PosV.z - gNear) / (gFar - gNear);


	// 탄젠트 노멀 만들기
	// 읽어들인 노멀맵의 정보를 월드맵으로 옮긴다.
	// 노말맵 안엔 0~255 가 있어.
	// 그래서, 쉐이더로 말하면 -1 ~ 1 사이로 바꿔주는게 그 공식이야
	BumpNormalTex = normalize((BumpNormalTex * 2.0f) - 1.0f);

	//--------------------------------------------------------------//
	// 노멀 Z 계산 <--------- 이거 아닌거 같음. 예시 맵이 탄젠트.. -_-;; 
	//--------------------------------------------------------------//
	//float NormalZ = sqrt(1 - (pin.NormalW.x * pin.NormalW.x) - (pin.NormalW.y - pin.NormalW.y));
	//pin.NormalW.z = NormalZ;
	//--------------------------------------------------------------//

	// 탄젠트법선 계산
	TangentNormal = normalize(pin.NormalW + BumpNormalTex.x * pin.WT + BumpNormalTex.y * pin.WB);

	//--------------------------------------------------------------//
	// 탄젠트 법선 Z 구하기 
	//--------------------------------------------------------------//
	// 엑스제곱 + 와이제곱 + 제트제곱 == 1 <-- 비례식
	//float TangetNormalZ = sqrt(1 - (TangentNormal.x * TangentNormal.x) - (TangentNormal.y - TangentNormal.y));
	//TangentNormal.z = TangetNormalZ;
	//--------------------------------------------------------------//

	// 출력	
	Out.Depth = Depth;
	Out.Color = DiffuseTex;

	//--------------------------------------------------------------//
	// 포지션 맵 저장
	//--------------------------------------------------------------//
	// 1.
	//Out.Position = PosV;

	// 2.
	//Out.Position = float4(PosP.xyz, 1.0f) * 0.5 + 0.5; // PosP <---

	// 3.
	Out.Position = float4(pin.PosW, 1.0f);	             // PosW <---

	// 4. 
	//Out.Position = pin.PosH * 0.5 + 0.5;				 // PosP <---   
	//--------------------------------------------------------------//

	Out.Normal = float4(TangentNormal, 0.0f) * 0.5 + 0.5; // <--- 희안하게 노멀라이즈하면 에러남
	//Out.Normal   = float4(pin.NormalW, 1.0f)  * 0.5 + 0.5;
	Out.Specular = SpecularTex;

	return Out;
}

// 버텍스
PNTVertexOut VS(PNTVertexIn vin)
{
	PNTVertexOut vout;

	// Transform to world space space.
	vout.PosW    = mul(float4(vin.PosL, 1.0f), vin.World).xyz;
	vout.NormalW = mul(vin.NormalL, (float3x3)vin.World);

	// Transform to homogeneous clip space.
	vout.PosH = mul(float4(vout.PosW, 1.0f), gViewProj);

	// Output vertex attributes for interpolation across triangle.
	// 어차피 변환결과는 같음.
	vout.Tex = mul(float4(vin.Tex, 0.0f, 1.0f), gTexTFMtx).xy;

	// 매트릭스 만들기 용도
	vout.WT = mul(vin.Tangent , (float3x3)vin.World);
	vout.WB = mul(vin.BiNormal, (float3x3)vin.World);

	return vout;
}

// 두번째 매개변수를 통해, 텍스처 사용 유무를 가른다.
PS_GBUFFER_OUT PS(PNTVertexOut pin)/* : SV_Target*/
{
	// G버퍼를 패킹한다.
	return PackGBuffer(pin);
}


//
//// 두번째 매개변수를 통해, 텍스처 사용 유무를 가른다.
//float4 PS(PNTVertexOut pin, uniform int gLightCount, uniform bool gUseTexure, uniform int gShaderMode) : SV_Target
//{
//	//// 포지션맵 테스트
//	//float4 position;
//	//position.xyz = pin.PosW;
//	//position.w = 1.0f;
//	//return position;
//
//	// Interpolating normal can unnormalize it, so normalize it.
//	pin.NormalW = normalize(pin.NormalW);
//
//	// The toEye vector is used in lighting.
//	float3 toEye = gEyePosW - pin.PosW;
//
//	// Cache the distance to the eye from this surface point.
//	float distToEye = length(toEye);
//	
//	// Normalize.
//	toEye /= distToEye;
//
//	// Default to multiplicative identity.
//	// 텍스처 사용 유무
//	float4 DiffuseTex     = float4(1, 1, 1, 1);
//	float4 SpecularTex    = float4(1, 1, 1, 1);
//	float4 BumpNormalTex  = float4(1, 1, 1, 1);
//	float3 TangentNormal  = float3(1, 1, 1);
//	float3 TanWorld_light = float3(1, 1, 1);
//	float  DotNomalMap    = 1.0f;
//
//	// 텍스처를 사용하는가?
//	if (gUseTexure)
//	{
//		// Sample texture.
//		// 텍셀 구하기
//		DiffuseTex    = gDiffuseTex .Sample(samAnisotropic, pin.Tex);
//		SpecularTex   = gSpecularTex.Sample(samAnisotropic, pin.Tex);
//		BumpNormalTex = gNormalTex  .Sample(samAnisotropic, pin.Tex);
//
//
//		// 읽어들인 노멀맵의 정보를 월드맵으로 옮긴다.
//		// 노말맵 안엔 0~255 가 있어.
//		// 그래서, 쉐이더로 말하면 -1 ~ 1 사이로 바꿔주는게 그 공식이야
//		BumpNormalTex = normalize((BumpNormalTex * 2.0f) - 1.0f);
//
//		// 탄젠트법선 계산
//		TangentNormal = pin.NormalW + BumpNormalTex.x * pin.WT + BumpNormalTex.y * pin.WB;
//
//		// 탄젠트법선 노멀 라이즈
//		TangentNormal = normalize(TangentNormal);
//
//		//----------------------------------------------------------//
//		// 노말 맵이라하면, 법선이다.
//		// 그말은 라이트랑 내적할 수 있다.
//
//		// 디렉셔널 라이트
//		float3 gDirectionLight_Dir = normalize(-gDirLight.Direction);
//		DotNomalMap = saturate(dot(TangentNormal.xyz, gDirectionLight_Dir));
//
//		// 포인트 라이트
//		float gPointLight_Length = length(gPointLight.Position.xyz - pin.PosW);
//
//		// 거리 확인
//		if (gPointLight_Length < gPointLight.Range)
//		{
//			float3 gPointLight_Dir = normalize(gPointLight.Position.xyz - pin.PosW);
//			DotNomalMap += saturate(dot(TangentNormal.xyz, gPointLight_Dir));
//		}
//
//		// 스팟 라이트
//		float gSpotLight_Length = length(gSpotLight.Position.xyz - pin.PosW);
//		
//		// 거리 확인
//		if (gSpotLight_Length < gSpotLight.Range)
//		{
//			float3 gSpotLight_Dir = normalize(gSpotLight.Position.xyz - pin.PosW);
//			DotNomalMap += saturate(dot(TangentNormal.xyz, gSpotLight_Dir));
//		}
//		//----------------------------------------------------------//
//	}
//
//	// 디퓨즈맵 테스트
//	//return DiffuseTex;
//
//	// 스팩큘러맵 테스트
//	//return SpecularTex;
//
//
//	//
//	// Lighting.
//	//
//
//	// 텍스처가 없으면 어차피, 다른맵은 적용 할 필요가 없음. 
//	float4 litColor = DiffuseTex;
//
//	// 라이트 사용 갯수
//	if (gLightCount > 0)
//	{
//		// Start with a sum of zero. 
//		float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
//		float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
//		float4 spec    = float4(0.0f, 0.0f, 0.0f, 0.0f);
//
//		// 디렉셔널 라이트 
//		// [unroll] <- 포문 사용전 필요한듯 <-- 실행중지할때까지 루프를 펼친다. 이게 무슨의미인지, 무슨 이득일지 모르겠음.
//		float4 A, D, S;
//
//		ComputeDirectionalLight(gMaterial, gDirLight, TangentNormal, toEye, A, D, S);
//		ambient += A;
//		diffuse += D;
//		spec    += S;
//
//		// 포인트 라이트
//		ComputePointLight(gMaterial, gPointLight, pin.PosW, TangentNormal, toEye, A, D, S);
//		ambient += A;
//		diffuse += D;
//		spec    += S;
//		
//		// 스폿 라이트
//		ComputeSpotLight(gMaterial, gSpotLight, pin.PosW, TangentNormal, toEye, A, D, S);
//		ambient += A;
//		diffuse += D;
//		spec    += S;
//
//		// 렌더링 모드
//		switch (gShaderMode)
//		{
//		case 0:
//			// 퐁 셰이더
//			litColor = ((diffuse + ambient) * DiffuseTex) + (spec * SpecularTex);
//			break;
//
//		case 1:
//			// 카툰 렌더링
//			float4 FinLight = ceil((ambient + diffuse) * 4) / 4;
//			litColor = FinLight * DiffuseTex + (spec * SpecularTex);
//			break; 
//		}
//
//		// Common to take alpha from diffuse material and texture.
//		litColor.a = gMaterial.Diffuse.a * DiffuseTex.a;
//	}
//
//	//---------------------------------------------------------//
//	// 테스트 2
//	// 법선맵만 출력
//	//litColor = DotNomalMap;
//	//return litColor;
//
//	//---------------------------------------------------------//
//	// 완료 
//	// 법선 매핑 곱하기
//	litColor *= DotNomalMap;
//	return litColor;
//	//---------------------------------------------------------//
//}


// 여러개의 라이트를 만든다.
// 매개변수를 직접적으로 설정함으로써, 골라 쓸 수 있게 만듬.

// 라이트 + 텍스처 버전
technique11 PongTex
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
}

// 라이트 + 텍스처 버전
technique11 CartoonTex
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
}
