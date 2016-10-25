//=============================================================================
// Basic.fx by Frank Luna (C) 2011 All Rights Reserved.
//
// Basic effect that currently supports transformations, lighting, and texturing.
//=============================================================================

#include "LightHelper.fx"

// 버텍스
TLVertexOut VS(TLVertexIn vin)
{
	TLVertexOut vout;

	// Transform to world space space.
	vout.PosW = mul(float4(vin.PosL, 1.0f), gWorld).xyz;
	vout.NormalW = mul(vin.NormalL, (float3x3)gWorldInvTranspose); 

	// Transform to homogeneous clip space.
	vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj);

	// Output vertex attributes for interpolation across triangle.
	// 어차피 변환결과는 같음.
	vout.Tex = mul(float4(vin.Tex, 0.0f, 1.0f), gTexTFMtx).xy;

	// 매트릭스 만들기 용도
	vout.WN = mul(vin.NormalL,  (float3x3)gWorld);
	vout.WT = mul(vin.Tangent,  (float3x3)gWorld);
	vout.WB = mul(vin.BiNormal, (float3x3)gWorld);

	//vout.WN = vin.NormalL;
	//vout.WT = vin.Tangent;
	//vout.WB = vin.BiNormal;
	return vout;
}


// 두번째 매개변수를 통해, 텍스처 사용 유무를 가른다.
float4 PS(TLVertexOut pin, uniform int gLightCount, uniform bool gUseTexure, uniform int gShaderMode) : SV_Target
{
	// Interpolating normal can unnormalize it, so normalize it.
	pin.NormalW = normalize(pin.NormalW); 

	// The toEye vector is used in lighting.
	float3 toEye = gEyePosW - pin.PosW;  

	// Cache the distance to the eye from this surface point.
	float distToEye = length(toEye);

	// Normalize.
	toEye /= distToEye;

	// Default to multiplicative identity.
	// 텍스처 사용 유무
	float4 DiffuseTex    = float4(1, 1, 1, 1);
	float4 SpecularTex   = float4(1, 1, 1, 1);
	float4 NormalTex     = float4(1, 1, 1, 1);
	float3 TangentNormal = float3(1, 1, 1);
	float3 World_light   = float3(1, 1, 1);
	float  DotNomalMap   = 1.0f;

	// 텍스처를 사용하는가?

	if (gUseTexure)
	{
		// Sample texture.
		// 텍셀 구하기
		DiffuseTex  = gDiffuseTex.Sample(samAnisotropic, pin.Tex);
		SpecularTex = gSpecularTex.Sample(samAnisotropic, pin.Tex);
		NormalTex   = gNormalTex.Sample(samAnisotropic, pin.Tex);

		// TBN 매트릭스 생성 & 역행렬 만들기
		float3x3 TBN = float3x3(normalize(pin.WT), normalize(pin.WB), normalize(pin.WN));
		TBN = transpose(TBN);

		// 읽어들인 노멀맵의 정보를 월드맵으로 옮긴다.
		TangentNormal = normalize(NormalTex * 2 - 1);

		float3 gSpotLight_Direction = normalize(gSpotLight.Position.rgb - pin.PosW);
		World_light = mul(TBN, gSpotLight_Direction);

		DotNomalMap = dot(TangentNormal.rgb, World_light);
	}

	//
	// Lighting.
	//

	// 텍스처가 없으면 어차피, 다른맵은 적용 할 필요가 없음. 
	float4 litColor = DiffuseTex;

		// 라이트 사용 갯수
	if (gLightCount > 0)
	{
		// Start with a sum of zero. 
		float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
		float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
		float4 spec    = float4(0.0f, 0.0f, 0.0f, 0.0f);

		// 디렉셔널 라이트 
		// [unroll] <- 포문 사용전 필요한듯
		float4 A, D, S;

		ComputeDirectionalLight(gMaterial, gDirLight, pin.NormalW, toEye, A, D, S);
		ambient += A;
		diffuse += D;
		spec    += S;

		// 포인트 라이트
		ComputePointLight(gMaterial, gPointLight, pin.PosW, pin.NormalW, toEye, A, D, S);
		ambient += A;
		diffuse += D;
		spec    += S;
		//
		// 스폿 라이트
		ComputeSpotLight(gMaterial, gSpotLight, pin.PosW, pin.NormalW, toEye, A, D, S);
		ambient += A;
		diffuse += D;
		spec    += S;


		// 렌더링 모드
		switch (gShaderMode)
		{
		case 0:
			// 퐁 셰이더
			litColor = (DiffuseTex * (ambient + diffuse) + spec) * SpecularTex;
			break;

		case 1:
			// 카툰 렌더링
			float4 FinLight = ceil((ambient + diffuse) * 4) / 4;
			litColor = (DiffuseTex * FinLight + spec) * SpecularTex;
			break;
		}
	}

	// Common to take alpha from diffuse material and texture.
	litColor.a = gMaterial.Diffuse.a * DiffuseTex.a;

	// 법선 매핑 곱하기
	litColor.r *= DotNomalMap;
	litColor.g *= DotNomalMap;
	litColor.b *= DotNomalMap;
	return litColor;
}


// 여러개의 라이트를 만든다.
// 매개변수를 직접적으로 설정함으로써, 골라 쓸 수 있게 만듬.
technique11 Light1
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS(1, false, 0)));
	}
}

// 라이트 + 텍스처 버전
technique11 PongTex
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS(1, true, 0)));
	}
}

// 라이트 + 텍스처 버전
technique11 CartoonTex
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS(1, true, 1)));
	}
}


// 3번

/*
//=============================================================================
// Basic.fx by Frank Luna (C) 2011 All Rights Reserved.
//
// Basic effect that currently supports transformations, lighting, and texturing.
//=============================================================================

#include "UtillFX.fx"
#include "LightHelper.fx"

cbuffer cbPerFrame
{
	DirectionalLight gDirLight;
	PointLight       gPointLight;
	SpotLight        gSpotLight;
	float3           gEyePosW;

	float  gFogStart;
	float  gFogRange;
	float4 gFogColor;
};

cbuffer cbPerObject
{
	float4x4 gWorld;
	float4x4 gWorldInvTranspose;
	float4x4 gWorldViewProj;
	float4x4 gTexTFMtx;
	float4x4 gTMMtx;
	Material gMaterial;
};

// Nonnumeric values cannot be added to a cbuffer.
// 텍스처 버퍼
Texture2D gDiffuseTex;
Texture2D gSpecularTex;
Texture2D gNormalTex;

// 텍스처 샘플링
SamplerState samAnisotropic
{
	Filter = ANISOTROPIC;
	MaxAnisotropy = 4;

	AddressU = WRAP;
	AddressV = WRAP;
};

struct VertexIn
{
	float3 PosL     : POSITION;
	float3 NormalL  : NORMAL;
	float2 Tex      : TEXCOORD;
	float3 Tangent  : TANGENT;
	float3 BiNormal : BINORMAL;
};

struct VertexOut
{
	float4 PosH        : SV_POSITION;
	float3 PosW        : POSITION0;
	float3 NormalW     : NORMAL;
	float2 Tex         : TEXCOORD0; // 보정 용도
	float3 DirLightW   : POSITION1;
	float3 PointLightW : POSITION2;
	float3 SpotLightW  : POSITION3;
};

// 버텍스
VertexOut VS(VertexIn vin)
{
	VertexOut vout;

	// Transform to world space space.
	vout.PosW = mul(float4(vin.PosL, 1.0f), gWorld).xyz;
	vout.NormalW = mul(vin.NormalL, (float3x3)gWorldInvTranspose); // 법선매핑 쓰면 이거 안씀

	// Transform to homogeneous clip space.
	vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj);

	// Output vertex attributes for interpolation across triangle.
	// 어차피 변환결과는 같음.
	vout.Tex = mul(float4(vin.Tex, 0.0f, 1.0f), gTexTFMtx).xy;

	// TBN 매트릭스 생성
	float3x3 TBN = float3x3(
		normalize(mul(vin.Tangent, (float3x3)gWorld)),
		normalize(mul(vin.BiNormal, (float3x3)gWorld)),
		normalize(mul(vin.NormalL, (float3x3)gWorld)));

	// TBN 역행렬 만들기
	TBN = transpose(TBN);

	// 각 라이트 카메라와의 방향 얻고, 저장하기
	vout.DirLightW = mul(TBN, normalize(gDirLight.Direction));
	vout.PointLightW = mul(TBN, normalize(gPointLight.Position.rgb - vout.PosW));
	vout.SpotLightW = mul(TBN, normalize(gSpotLight.Position.rgb - vout.PosW));

	return vout;
}


// 두번째 매개변수를 통해, 텍스처 사용 유무를 가른다.
float4 PS(VertexOut pin, uniform int gLightCount, uniform bool gUseTexure, uniform int gShaderMode) : SV_Target
{
	// Interpolating normal can unnormalize it, so normalize it.
	pin.NormalW = normalize(pin.NormalW);  // 법선매핑 쓰면 이거 안씀

	// The toEye vector is used in lighting.
	float3 toEye = gEyePosW - pin.PosW;  // 법선매핑 쓰면 이거 안씀

		// Cache the distance to the eye from this surface point.
		float distToEye = length(toEye);

	// Normalize.
	toEye /= distToEye;

	// Default to multiplicative identity.
	// 텍스처 사용 유무
	float4 DiffuseTex = float4(1, 1, 1, 1);
		float4 SpecularTex = float4(1, 1, 1, 1);
		float4 NormalTex = float4(1, 1, 1, 1);
		float3 TangentNormal = float3(1, 1, 1);

		// 텍스처를 사용하는가?
		float DotNomalMap = 1.0f;
	float3 World_light;
	if (gUseTexure)
	{
		// Sample texture.
		// 텍셀 구하기
		DiffuseTex = gDiffuseTex.Sample(samAnisotropic, pin.Tex);
		SpecularTex = gSpecularTex.Sample(samAnisotropic, pin.Tex);
		NormalTex = gNormalTex.Sample(samAnisotropic, pin.Tex);

		// 읽어들인 노멀맵의 정보를 월드맵으로 옮긴다.
		TangentNormal = normalize(NormalTex * 2 - 1);

		// 월드맵에 적용
		World_light = pin.SpotLightW;

		// 계산된 내적 값 
		DotNomalMap = dot(TangentNormal.rgb, World_light);
	}

	//
	// Lighting.
	//

	// 텍스처가 없으면 어차피, 다른맵은 적용 할 필요가 없음. 
	float4 litColor = DiffuseTex;

		// 라이트 사용 갯수
	if (gLightCount > 0)
	{
		// Start with a sum of zero. 
		float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
			float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
			float4 spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

			// 디렉셔널 라이트 
			// [unroll] <- 포문 사용전 필요한듯
			float4 A, D, S;

		ComputeDirectionalLight(gMaterial, gDirLight, pin.NormalW, toEye, A, D, S);
		ambient += A;
		diffuse += D;
		spec += S;

		// 포인트 라이트
		ComputePointLight(gMaterial, gPointLight, pin.PosW, pin.NormalW, toEye, A, D, S);
		ambient += A;
		diffuse += D;
		spec += S;
		//
		// 스폿 라이트
		ComputeSpotLight(gMaterial, gSpotLight, pin.PosW, pin.NormalW, toEye, A, D, S);
		ambient += A;
		diffuse += D;
		spec += S;


		// 렌더링 모드
		switch (gShaderMode)
		{
		case 0:
			// 퐁 셰이더
			litColor = (DiffuseTex * (ambient + diffuse) + spec) * SpecularTex;
			break;

		case 1:
			// 카툰 렌더링
			float4 FinLight = ceil((ambient + diffuse) * 4) / 4;
				litColor = (DiffuseTex * FinLight + spec) * SpecularTex;
			break;
		}
	}

	// Common to take alpha from diffuse material and texture.
	litColor.a = gMaterial.Diffuse.a * DiffuseTex.a;

	// 법선 매핑 곱하기
	litColor.r *= DotNomalMap;
	litColor.g *= DotNomalMap;
	litColor.b *= DotNomalMap;
	return litColor;
}


// 여러개의 라이트를 만든다.
// 매개변수를 직접적으로 설정함으로써, 골라 쓸 수 있게 만듬.
technique11 Light1
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS(1, false, 0)));
	}
}

// 라이트 + 텍스처 버전
technique11 PongTex
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS(1, true, 0)));
	}
}

// 라이트 + 텍스처 버전
technique11 CartoonTex
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS(1, true, 1)));
	}
}

// 2번 
*/