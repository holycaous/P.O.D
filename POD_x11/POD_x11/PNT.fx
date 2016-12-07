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

	// �Ȱ�
	float3 toEye    = gEyePosW - pin.PosW;
	float distToEye = length(toEye);
	float fogLerp   = saturate((distToEye - 180.0f) / 4800.0f);
	Out.Color = lerp(Out.Color, float4(0.15f, 0.15f, 0.2f, 0.0f), fogLerp);



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

	Out.Shadow = pin.ShadowPosH;
	//Out.Shadow = CalcShadowFactor(samShadow, gShadowMap, pin.ShadowPosH);
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

	// ������
	vout.ShadowPosH = mul(float4(vout.PosW, 1.0f), gShadowTransform);

	return vout;
}

// �ι�° �Ű������� ����, �ؽ�ó ��� ������ ������.
PS_GBUFFER_OUT PS(PNTVertexOut pin)/* : SV_Target*/
{
	// G���۸� ��ŷ�Ѵ�.
	return PackGBuffer(pin);
}


// ���ؽ�
ShadowVertexOut SDVS(PNTVertexIn vin)
{
	ShadowVertexOut vout;

	// Transform to world space space.
	float3 PosW = mul(float4(vin.PosL, 1.0f), vin.World).xyz;

	// Transform to homogeneous clip space.
	vout.PosH = mul(float4(PosW, 1.0f), gLightViewProj);
	vout.Tex  = mul(float4(vin.Tex, 0.0f, 1.0f), gTexTFMtx).xy;

	return vout;
}

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

		//SetRasterizerState(0);
		SetRasterizerState(NoCull);
		SetDepthStencilState(LessDSS, 0);
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

		SetRasterizerState(NoCull);
		SetDepthStencilState(LessDSS, 0);
	}
}


technique11 SDPongTex
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, SDVS()));
		SetGeometryShader(NULL);
		SetPixelShader(NULL);

		SetRasterizerState(Depth);
		SetDepthStencilState(LessDSS, 0);
	}
}

// ����Ʈ + �ؽ�ó ����
technique11 SDCartoonTex
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, SDVS()));
		SetGeometryShader(NULL);
		SetPixelShader(NULL);

		SetRasterizerState(Depth);
	}
}
