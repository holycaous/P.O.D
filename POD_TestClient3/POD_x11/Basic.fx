//=============================================================================
// Basic.fx by Frank Luna (C) 2011 All Rights Reserved.
//
// Basic effect that currently supports transformations, lighting, and texturing.
//=============================================================================

#include "LightHelper.fx"

// ���ؽ�
TLVertexOut VS(TLVertexIn vin)
{
	TLVertexOut vout;

	// Transform to world space space.
	vout.PosW = mul(float4(vin.PosL, 1.0f), gWorld).xyz;
	vout.NormalW = mul(vin.NormalL, (float3x3)gWorldInvTranspose); 

	// Transform to homogeneous clip space.
	vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj);

	// Output vertex attributes for interpolation across triangle.
	// ������ ��ȯ����� ����.
	vout.Tex = mul(float4(vin.Tex, 0.0f, 1.0f), gTexTFMtx).xy;

	// ��Ʈ���� ����� �뵵
	vout.WN = mul(vin.NormalL,  (float3x3)gWorld);
	vout.WT = mul(vin.Tangent,  (float3x3)gWorld);
	vout.WB = mul(vin.BiNormal, (float3x3)gWorld);

	//vout.WN = vin.NormalL;
	//vout.WT = vin.Tangent;
	//vout.WB = vin.BiNormal;
	return vout;
}


// �ι�° �Ű������� ����, �ؽ�ó ��� ������ ������.
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
	// �ؽ�ó ��� ����
	float4 DiffuseTex    = float4(1, 1, 1, 1);
	float4 SpecularTex   = float4(1, 1, 1, 1);
	float4 NormalTex     = float4(1, 1, 1, 1);
	float3 TangentNormal = float3(1, 1, 1);
	float3 World_light   = float3(1, 1, 1);
	float  DotNomalMap   = 1.0f;

	// �ؽ�ó�� ����ϴ°�?

	if (gUseTexure)
	{
		// Sample texture.
		// �ؼ� ���ϱ�
		DiffuseTex  = gDiffuseTex.Sample(samAnisotropic, pin.Tex);
		SpecularTex = gSpecularTex.Sample(samAnisotropic, pin.Tex);
		NormalTex   = gNormalTex.Sample(samAnisotropic, pin.Tex);

		// TBN ��Ʈ���� ���� & ����� �����
		float3x3 TBN = float3x3(normalize(pin.WT), normalize(pin.WB), normalize(pin.WN));
		TBN = transpose(TBN);

		// �о���� ��ָ��� ������ ��������� �ű��.
		TangentNormal = normalize(NormalTex * 2 - 1);

		float3 gSpotLight_Direction = normalize(gSpotLight.Position.rgb - pin.PosW);
		World_light = mul(TBN, gSpotLight_Direction);

		DotNomalMap = dot(TangentNormal.rgb, World_light);
	}

	//
	// Lighting.
	//

	// �ؽ�ó�� ������ ������, �ٸ����� ���� �� �ʿ䰡 ����. 
	float4 litColor = DiffuseTex;

		// ����Ʈ ��� ����
	if (gLightCount > 0)
	{
		// Start with a sum of zero. 
		float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
		float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
		float4 spec    = float4(0.0f, 0.0f, 0.0f, 0.0f);

		// �𷺼ų� ����Ʈ 
		// [unroll] <- ���� ����� �ʿ��ѵ�
		float4 A, D, S;

		ComputeDirectionalLight(gMaterial, gDirLight, pin.NormalW, toEye, A, D, S);
		ambient += A;
		diffuse += D;
		spec    += S;

		// ����Ʈ ����Ʈ
		ComputePointLight(gMaterial, gPointLight, pin.PosW, pin.NormalW, toEye, A, D, S);
		ambient += A;
		diffuse += D;
		spec    += S;
		//
		// ���� ����Ʈ
		ComputeSpotLight(gMaterial, gSpotLight, pin.PosW, pin.NormalW, toEye, A, D, S);
		ambient += A;
		diffuse += D;
		spec    += S;


		// ������ ���
		switch (gShaderMode)
		{
		case 0:
			// �� ���̴�
			litColor = (DiffuseTex * (ambient + diffuse) + spec) * SpecularTex;
			break;

		case 1:
			// ī�� ������
			float4 FinLight = ceil((ambient + diffuse) * 4) / 4;
			litColor = (DiffuseTex * FinLight + spec) * SpecularTex;
			break;
		}
	}

	// Common to take alpha from diffuse material and texture.
	litColor.a = gMaterial.Diffuse.a * DiffuseTex.a;

	// ���� ���� ���ϱ�
	litColor.r *= DotNomalMap;
	litColor.g *= DotNomalMap;
	litColor.b *= DotNomalMap;
	return litColor;
}


// �������� ����Ʈ�� �����.
// �Ű������� ���������� ���������ν�, ��� �� �� �ְ� ����.
technique11 Light1
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS(1, false, 0)));
	}
}

// ����Ʈ + �ؽ�ó ����
technique11 PongTex
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS(1, true, 0)));
	}
}

// ����Ʈ + �ؽ�ó ����
technique11 CartoonTex
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS(1, true, 1)));
	}
}


// 3��

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
// �ؽ�ó ����
Texture2D gDiffuseTex;
Texture2D gSpecularTex;
Texture2D gNormalTex;

// �ؽ�ó ���ø�
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
	float2 Tex         : TEXCOORD0; // ���� �뵵
	float3 DirLightW   : POSITION1;
	float3 PointLightW : POSITION2;
	float3 SpotLightW  : POSITION3;
};

// ���ؽ�
VertexOut VS(VertexIn vin)
{
	VertexOut vout;

	// Transform to world space space.
	vout.PosW = mul(float4(vin.PosL, 1.0f), gWorld).xyz;
	vout.NormalW = mul(vin.NormalL, (float3x3)gWorldInvTranspose); // �������� ���� �̰� �Ⱦ�

	// Transform to homogeneous clip space.
	vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj);

	// Output vertex attributes for interpolation across triangle.
	// ������ ��ȯ����� ����.
	vout.Tex = mul(float4(vin.Tex, 0.0f, 1.0f), gTexTFMtx).xy;

	// TBN ��Ʈ���� ����
	float3x3 TBN = float3x3(
		normalize(mul(vin.Tangent, (float3x3)gWorld)),
		normalize(mul(vin.BiNormal, (float3x3)gWorld)),
		normalize(mul(vin.NormalL, (float3x3)gWorld)));

	// TBN ����� �����
	TBN = transpose(TBN);

	// �� ����Ʈ ī�޶���� ���� ���, �����ϱ�
	vout.DirLightW = mul(TBN, normalize(gDirLight.Direction));
	vout.PointLightW = mul(TBN, normalize(gPointLight.Position.rgb - vout.PosW));
	vout.SpotLightW = mul(TBN, normalize(gSpotLight.Position.rgb - vout.PosW));

	return vout;
}


// �ι�° �Ű������� ����, �ؽ�ó ��� ������ ������.
float4 PS(VertexOut pin, uniform int gLightCount, uniform bool gUseTexure, uniform int gShaderMode) : SV_Target
{
	// Interpolating normal can unnormalize it, so normalize it.
	pin.NormalW = normalize(pin.NormalW);  // �������� ���� �̰� �Ⱦ�

	// The toEye vector is used in lighting.
	float3 toEye = gEyePosW - pin.PosW;  // �������� ���� �̰� �Ⱦ�

		// Cache the distance to the eye from this surface point.
		float distToEye = length(toEye);

	// Normalize.
	toEye /= distToEye;

	// Default to multiplicative identity.
	// �ؽ�ó ��� ����
	float4 DiffuseTex = float4(1, 1, 1, 1);
		float4 SpecularTex = float4(1, 1, 1, 1);
		float4 NormalTex = float4(1, 1, 1, 1);
		float3 TangentNormal = float3(1, 1, 1);

		// �ؽ�ó�� ����ϴ°�?
		float DotNomalMap = 1.0f;
	float3 World_light;
	if (gUseTexure)
	{
		// Sample texture.
		// �ؼ� ���ϱ�
		DiffuseTex = gDiffuseTex.Sample(samAnisotropic, pin.Tex);
		SpecularTex = gSpecularTex.Sample(samAnisotropic, pin.Tex);
		NormalTex = gNormalTex.Sample(samAnisotropic, pin.Tex);

		// �о���� ��ָ��� ������ ��������� �ű��.
		TangentNormal = normalize(NormalTex * 2 - 1);

		// ����ʿ� ����
		World_light = pin.SpotLightW;

		// ���� ���� �� 
		DotNomalMap = dot(TangentNormal.rgb, World_light);
	}

	//
	// Lighting.
	//

	// �ؽ�ó�� ������ ������, �ٸ����� ���� �� �ʿ䰡 ����. 
	float4 litColor = DiffuseTex;

		// ����Ʈ ��� ����
	if (gLightCount > 0)
	{
		// Start with a sum of zero. 
		float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
			float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
			float4 spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

			// �𷺼ų� ����Ʈ 
			// [unroll] <- ���� ����� �ʿ��ѵ�
			float4 A, D, S;

		ComputeDirectionalLight(gMaterial, gDirLight, pin.NormalW, toEye, A, D, S);
		ambient += A;
		diffuse += D;
		spec += S;

		// ����Ʈ ����Ʈ
		ComputePointLight(gMaterial, gPointLight, pin.PosW, pin.NormalW, toEye, A, D, S);
		ambient += A;
		diffuse += D;
		spec += S;
		//
		// ���� ����Ʈ
		ComputeSpotLight(gMaterial, gSpotLight, pin.PosW, pin.NormalW, toEye, A, D, S);
		ambient += A;
		diffuse += D;
		spec += S;


		// ������ ���
		switch (gShaderMode)
		{
		case 0:
			// �� ���̴�
			litColor = (DiffuseTex * (ambient + diffuse) + spec) * SpecularTex;
			break;

		case 1:
			// ī�� ������
			float4 FinLight = ceil((ambient + diffuse) * 4) / 4;
				litColor = (DiffuseTex * FinLight + spec) * SpecularTex;
			break;
		}
	}

	// Common to take alpha from diffuse material and texture.
	litColor.a = gMaterial.Diffuse.a * DiffuseTex.a;

	// ���� ���� ���ϱ�
	litColor.r *= DotNomalMap;
	litColor.g *= DotNomalMap;
	litColor.b *= DotNomalMap;
	return litColor;
}


// �������� ����Ʈ�� �����.
// �Ű������� ���������� ���������ν�, ��� �� �� �ְ� ����.
technique11 Light1
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS(1, false, 0)));
	}
}

// ����Ʈ + �ؽ�ó ����
technique11 PongTex
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS(1, true, 0)));
	}
}

// ����Ʈ + �ؽ�ó ����
technique11 CartoonTex
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS(1, true, 1)));
	}
}

// 2�� 
*/