//=============================================================================
// Basic.fx by Frank Luna (C) 2011 All Rights Reserved.
//
// Basic effect that currently supports transformations, lighting, and texturing.
//=============================================================================

#include "LightHelper.fx"


// G ���� ��ŷ
PS_GBUFFER_OUT PackGBuffer(PNTVertexOut pin)
{
	PS_GBUFFER_OUT Out;
	
	// Interpolating normal can unnormalize it, so normalize it.
	pin.NormalW = normalize(pin.NormalW);

	// Default to multiplicative identity.
	// �ؽ�ó ��� ����
	float4 DepthTex       = float4(1, 1, 1, 1);
	float4 DiffuseTex     = float4(1, 1, 1, 1);
	float4 SpecularTex    = float4(1, 1, 1, 1);
	float4 BumpNormalTex  = float4(1, 1, 1, 1);
	float3 TangentNormal  = float3(1, 1, 1);
	
	// Sample texture.
	// �ؼ� ���ϱ�
	DiffuseTex    = gDiffuseTex .Sample(samAnisotropic, pin.Tex);
	SpecularTex   = gSpecularTex.Sample(samAnisotropic, pin.Tex);
	BumpNormalTex = gNormalTex  .Sample(samAnisotropic, pin.Tex);

	// �����Ǹ� �����
	// ��ǥ�� ����Ʈ�� �ű��.
	float4 PosV = mul(float4(pin.PosW, 1.0f), gView); 
	//float4 PosP = mul(float4(pin.PosW, 1.0f), gViewProj); //<<---------- �� ������������ �ѱ�� ���� 
	PosV /= PosV.w; // ��ī��Ʈ ��ǥ��� �ѱ�� ���� (������ǥ�� ������ ����)
	//PosP /= PosP.w;

	// ���� �� �����
	// (��-�Ͼ�) : 1 = (��ǥ-�Ͼ�) : ?
	// ���� ���� = (��ǥ-�Ͼ�) / (�� - �Ͼ�)
	// ��ǥ = ( �� - �Ͼ� ) *  ���̹��� + �Ͼ�
	float  Depth = (PosV.z - gNear) / (gFar - gNear);


	// ź��Ʈ ��� �����
	// �о���� ��ָ��� ������ ��������� �ű��.
	// �븻�� �ȿ� 0~255 �� �־�.
	// �׷���, ���̴��� ���ϸ� -1 ~ 1 ���̷� �ٲ��ִ°� �� �����̾�
	BumpNormalTex = normalize((BumpNormalTex * 2.0f) - 1.0f);

	//--------------------------------------------------------------//
	// ��� Z ��� <--------- �̰� �ƴѰ� ����. ���� ���� ź��Ʈ.. -_-;; 
	//--------------------------------------------------------------//
	//float NormalZ = sqrt(1 - (pin.NormalW.x * pin.NormalW.x) - (pin.NormalW.y - pin.NormalW.y));
	//pin.NormalW.z = NormalZ;
	//--------------------------------------------------------------//

	// ź��Ʈ���� ���
	TangentNormal = normalize(pin.NormalW + BumpNormalTex.x * pin.WT + BumpNormalTex.y * pin.WB);

	//--------------------------------------------------------------//
	// ź��Ʈ ���� Z ���ϱ� 
	//--------------------------------------------------------------//
	// �������� + �������� + ��Ʈ���� == 1 <-- ��ʽ�
	//float TangetNormalZ = sqrt(1 - (TangentNormal.x * TangentNormal.x) - (TangentNormal.y - TangentNormal.y));
	//TangentNormal.z = TangetNormalZ;
	//--------------------------------------------------------------//

	// ���	
	Out.Depth = Depth;
	Out.Color = DiffuseTex;

	//--------------------------------------------------------------//
	// ������ �� ����
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

	Out.Normal = float4(TangentNormal, 0.0f) * 0.5 + 0.5; // <--- ����ϰ� ��ֶ������ϸ� ������
	//Out.Normal   = float4(pin.NormalW, 1.0f)  * 0.5 + 0.5;
	Out.Specular = SpecularTex;

	return Out;
}

// ���ؽ�
PNTVertexOut VS(PNTVertexIn vin)
{
	PNTVertexOut vout;

	// Transform to world space space.
	vout.PosW    = mul(float4(vin.PosL, 1.0f), vin.World).xyz;
	vout.NormalW = mul(vin.NormalL, (float3x3)vin.World);

	// Transform to homogeneous clip space.
	vout.PosH = mul(float4(vout.PosW, 1.0f), gViewProj);

	// Output vertex attributes for interpolation across triangle.
	// ������ ��ȯ����� ����.
	vout.Tex = mul(float4(vin.Tex, 0.0f, 1.0f), gTexTFMtx).xy;

	// ��Ʈ���� ����� �뵵
	vout.WT = mul(vin.Tangent , (float3x3)vin.World);
	vout.WB = mul(vin.BiNormal, (float3x3)vin.World);

	return vout;
}

// �ι�° �Ű������� ����, �ؽ�ó ��� ������ ������.
PS_GBUFFER_OUT PS(PNTVertexOut pin)/* : SV_Target*/
{
	// G���۸� ��ŷ�Ѵ�.
	return PackGBuffer(pin);
}


//
//// �ι�° �Ű������� ����, �ؽ�ó ��� ������ ������.
//float4 PS(PNTVertexOut pin, uniform int gLightCount, uniform bool gUseTexure, uniform int gShaderMode) : SV_Target
//{
//	//// �����Ǹ� �׽�Ʈ
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
//	// �ؽ�ó ��� ����
//	float4 DiffuseTex     = float4(1, 1, 1, 1);
//	float4 SpecularTex    = float4(1, 1, 1, 1);
//	float4 BumpNormalTex  = float4(1, 1, 1, 1);
//	float3 TangentNormal  = float3(1, 1, 1);
//	float3 TanWorld_light = float3(1, 1, 1);
//	float  DotNomalMap    = 1.0f;
//
//	// �ؽ�ó�� ����ϴ°�?
//	if (gUseTexure)
//	{
//		// Sample texture.
//		// �ؼ� ���ϱ�
//		DiffuseTex    = gDiffuseTex .Sample(samAnisotropic, pin.Tex);
//		SpecularTex   = gSpecularTex.Sample(samAnisotropic, pin.Tex);
//		BumpNormalTex = gNormalTex  .Sample(samAnisotropic, pin.Tex);
//
//
//		// �о���� ��ָ��� ������ ��������� �ű��.
//		// �븻�� �ȿ� 0~255 �� �־�.
//		// �׷���, ���̴��� ���ϸ� -1 ~ 1 ���̷� �ٲ��ִ°� �� �����̾�
//		BumpNormalTex = normalize((BumpNormalTex * 2.0f) - 1.0f);
//
//		// ź��Ʈ���� ���
//		TangentNormal = pin.NormalW + BumpNormalTex.x * pin.WT + BumpNormalTex.y * pin.WB;
//
//		// ź��Ʈ���� ��� ������
//		TangentNormal = normalize(TangentNormal);
//
//		//----------------------------------------------------------//
//		// �븻 ���̶��ϸ�, �����̴�.
//		// �׸��� ����Ʈ�� ������ �� �ִ�.
//
//		// �𷺼ų� ����Ʈ
//		float3 gDirectionLight_Dir = normalize(-gDirLight.Direction);
//		DotNomalMap = saturate(dot(TangentNormal.xyz, gDirectionLight_Dir));
//
//		// ����Ʈ ����Ʈ
//		float gPointLight_Length = length(gPointLight.Position.xyz - pin.PosW);
//
//		// �Ÿ� Ȯ��
//		if (gPointLight_Length < gPointLight.Range)
//		{
//			float3 gPointLight_Dir = normalize(gPointLight.Position.xyz - pin.PosW);
//			DotNomalMap += saturate(dot(TangentNormal.xyz, gPointLight_Dir));
//		}
//
//		// ���� ����Ʈ
//		float gSpotLight_Length = length(gSpotLight.Position.xyz - pin.PosW);
//		
//		// �Ÿ� Ȯ��
//		if (gSpotLight_Length < gSpotLight.Range)
//		{
//			float3 gSpotLight_Dir = normalize(gSpotLight.Position.xyz - pin.PosW);
//			DotNomalMap += saturate(dot(TangentNormal.xyz, gSpotLight_Dir));
//		}
//		//----------------------------------------------------------//
//	}
//
//	// ��ǻ��� �׽�Ʈ
//	//return DiffuseTex;
//
//	// ����ŧ���� �׽�Ʈ
//	//return SpecularTex;
//
//
//	//
//	// Lighting.
//	//
//
//	// �ؽ�ó�� ������ ������, �ٸ����� ���� �� �ʿ䰡 ����. 
//	float4 litColor = DiffuseTex;
//
//	// ����Ʈ ��� ����
//	if (gLightCount > 0)
//	{
//		// Start with a sum of zero. 
//		float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
//		float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
//		float4 spec    = float4(0.0f, 0.0f, 0.0f, 0.0f);
//
//		// �𷺼ų� ����Ʈ 
//		// [unroll] <- ���� ����� �ʿ��ѵ� <-- ���������Ҷ����� ������ ��ģ��. �̰� �����ǹ�����, ���� �̵����� �𸣰���.
//		float4 A, D, S;
//
//		ComputeDirectionalLight(gMaterial, gDirLight, TangentNormal, toEye, A, D, S);
//		ambient += A;
//		diffuse += D;
//		spec    += S;
//
//		// ����Ʈ ����Ʈ
//		ComputePointLight(gMaterial, gPointLight, pin.PosW, TangentNormal, toEye, A, D, S);
//		ambient += A;
//		diffuse += D;
//		spec    += S;
//		
//		// ���� ����Ʈ
//		ComputeSpotLight(gMaterial, gSpotLight, pin.PosW, TangentNormal, toEye, A, D, S);
//		ambient += A;
//		diffuse += D;
//		spec    += S;
//
//		// ������ ���
//		switch (gShaderMode)
//		{
//		case 0:
//			// �� ���̴�
//			litColor = ((diffuse + ambient) * DiffuseTex) + (spec * SpecularTex);
//			break;
//
//		case 1:
//			// ī�� ������
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
//	// �׽�Ʈ 2
//	// �����ʸ� ���
//	//litColor = DotNomalMap;
//	//return litColor;
//
//	//---------------------------------------------------------//
//	// �Ϸ� 
//	// ���� ���� ���ϱ�
//	litColor *= DotNomalMap;
//	return litColor;
//	//---------------------------------------------------------//
//}


// �������� ����Ʈ�� �����.
// �Ű������� ���������� ���������ν�, ��� �� �� �ְ� ����.

// ����Ʈ + �ؽ�ó ����
technique11 PongTex
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
}

// ����Ʈ + �ؽ�ó ����
technique11 CartoonTex
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
}
