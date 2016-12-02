//=============================================================================
// Basic.fx by Frank Luna (C) 2011 All Rights Reserved.
//
// Basic effect that currently supports transformations, lighting, and texturing.
//=============================================================================

#include "LightHelper.fx"


// G���� ����
SURFACE_DATA UnpackGBuffer(float4 PosL, float2 Tex)
{
	SURFACE_DATA Out;
	
	// �� �ؽ�ó �̱�
	Out.DepthTex     = gGDepthTex   .Sample(samLinear, Tex);
	Out.DiffuseTex   = gGDiffuseTex .Sample(samLinear, Tex);
	Out.PositionTex  = gGPositionTex.Sample(samPoint , Tex);
	Out.SpecularTex  = gGSpecularTex.Sample(samLinear, Tex);
	Out.TanNormalTex = gGNormalTex  .Sample(samLinear, Tex);

	// Ÿ��Ʈ ���
	// ��ǥ -1 ~ 1���̷� �ٲٱ� (Ȥ�� ����)
	Out.TanNormalTex = (Out.TanNormalTex * 2.0f) - 1.0f;
	
	////--------------------------------------------------------------//
	//// ��ǥ Z���
	////--------------------------------------------------------------//
	//// 1.
	////// ��ǥ = ( �� - �Ͼ� ) *  ���̹��� + �Ͼ�
	//float PositionTexZ = (gFar - gNear) * Out.DepthTex.x + gNear;
	//
	//float4 pos = float4(PosL.xy, Out.DepthTex.x, 1.0f);
	//pos = mul(pos, gProjInvTranspose); // ��� �ȱ��                   // <-- ���� ����ĵ��� ��ũ���� ��������� ������� �ƴ�. -_-;;
	//pos.z = PositionTexZ;
	//pos = mul(pos, gViewInvTranspose); // ����� �ű��.
	//pos.xyz /= pos.w;
	//Out.PositionTex = pos;
	////--------------------------------------------------------------//
	//
	//// ���� ��ġ ���ϱ�
	//float4 Position = float4(Out.PositionTex.xy, PositionTexZ, 1.0f);
	//Position = mul(Position, gViewInvTranspose);
	////Position.z *= Position.w;
	//Out.PositionTex = Position;
	//
	//// 2
	//Out.PositionTex = (Out.PositionTex  * 2.0f) - 1.0f;        // ����
	//
	//
	//- �̰� �ٸ���---//
	//pos = mul(pos, gViewInvTranspose); // ����� �ű��.

	//--------------------------------------------------------------//
	return Out;
}

// ���ؽ�
GVertexOut VS(GVertexIn vin)
{
	GVertexOut vout;

	// Transform to world space space.
	vout.PosL = float4(vin.PosL, 1.0f);

	// Output vertex attributes for interpolation across triangle.
	// ������ ��ȯ����� ����.
	vout.Tex = mul(float4(vin.Tex, 0.0f, 1.0f), gTexTFMtx).xy;

	

	return vout;
}

// �ι�° �Ű������� ����, �ؽ�ó ��� ������ ������.
float4 PS(GVertexOut pin, uniform int gShaderMode) : SV_Target
{
	// ȭ�� ��ǥ�� ��´�
	SURFACE_DATA sData = UnpackGBuffer(pin.PosL, pin.Tex);

	// �븻 �� ���
	float  DotDirectLightNomalMap = 0.0f;
	float  DotPointLightNomalMap  = 0.0f;
	//float  DotNomalMap = 0.0f;

	// �ƹ��͵� ó�����ص� ��ǻ��
	float4 litColor = sData.DiffuseTex;

	// �� Pos, LookDir ���⺤��
	float3 toEye = gEyePosW - sData.PositionTex.xyz;
	float distToEye = length(toEye);
	toEye /= distToEye;

	// �׽�Ʈ 1
	// ��ǻ� ���
	//return sData.DiffuseTex;
	//
	// ź��Ʈ ��ָ� ���
	//return sData.TanNormalTex;
	//
	// ����ŧ����
	//return sData.SpecularTex;
	//
	// ���̸�
	//return sData.DepthTex;
	//
	// �����Ǹ�
	//return sData.PositionTex;


	// �𷺼ų� ����Ʈ
	float3 gDirectionLight_Dir = normalize(-gDirLight.Direction);
	DotDirectLightNomalMap     = saturate(dot(sData.TanNormalTex.xyz, gDirectionLight_Dir)) * 2.0f;
	
	// �׽�Ʈ 1
	// ����Ʈ ����Ʈ
	//float gPointLight_Length = length(gPointLight.Position.xyz - sData.PositionTex.xyz);
	//
	//// �Ÿ� Ȯ��
	//[flatten]
	//if (gPointLight_Length < gPointLight.Range)
	//{
	//	// �Ÿ� ���� % �� ���Ѵ�.
	//	float gFinPointLength = gPointLight.Range / gPointLight_Length;
	//
	//	// ����Ʈ �������� �ִٸ�,
	//	float3 gPointLight_Dir = normalize(gPointLight.Position.xyz - sData.PositionTex.xyz);
	//		DotPointLightNomalMap += saturate(dot(sData.TanNormalTex.xyz, gPointLight_Dir)) * pow(gFinPointLength, 2.f) * 0.01f;		  // +=
	//}
	//	
	////// ���� ����Ʈ
	//float gSpotLight_Length = length(sData.PositionTex.xyz - gSpotLight.Position.xyz);
	//
	//// �Ÿ� Ȯ��
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

	// �𷺼ų� ����Ʈ 
	ComputeDirectionalLight(gMaterial, gDirLight, sData.TanNormalTex.xyz, toEye, A, D, S);
	ambient += A;
	diffuse += D;
	spec    += S;
	
	//// ����Ʈ ����Ʈ
	//[flatten]
	//if (gPointLight_Length < gPointLight.Range)
	//{
	//	// �Ÿ� ���� % �� ���Ѵ�.
	//	float gFinPointLength = gPointLight.Range / gPointLight_Length;
	//
	//	ComputePointLight(gMaterial, gPointLight, sData.PositionTex.xyz, sData.TanNormalTex.xyz, toEye, A, D, S);
	//	ambient += A * gFinPointLength * 0.2f; //(gPointLight_Length < 0.0 ? 0.0 : gFinPointLength);
	//	diffuse += D * gFinPointLength * 0.2f; //(gPointLight_Length < 0.0 ? 0.0 : gFinPointLength);
	//	spec    += S * gFinPointLength * 0.2f; //(gPointLight_Length < 0.0 ? 0.0 : gFinPointLength);
	//}
	//
	//
	//// ���� ����Ʈ
	//if (gSpotLight_Length < gSpotLight.Range)
	//{
	//	ComputeSpotLight(gMaterial, gSpotLight, sData.PositionTex.xyz, sData.TanNormalTex.xyz, toEye, A, D, S);
	//	ambient += A;
	//	diffuse += D;
	//	spec    += S;
	//}

	// ������ ���
	switch (gShaderMode)
	{
	case 0:
		// �� ���̴�
		litColor = ((diffuse + ambient) * sData.DiffuseTex) + (spec * sData.SpecularTex);
		break;

	case 1:
		// ī�� ������
		float4 FinLight = ceil((ambient + diffuse) * 4) / 4;
			litColor = FinLight * sData.DiffuseTex + (spec * sData.SpecularTex);
		break;
	}

	// Common to take alpha from diffuse material and texture.
	litColor.a = gMaterial.Diffuse.a * sData.DiffuseTex.a;

	//---------------------------------------------------------//
	// �׽�Ʈ 2
	// �����ʸ� ���
	//litColor = DotNomalMap;
	//return litColor;
	//
	//---------------------------------------------------------//
	// �׽�Ʈ 3
	// �̰����� ���
	//litColor = DotNomalMap * sData.DiffuseTex + sData.SpecularTex;
	//return litColor; 

	//---------------------------------------------------------//
	// �Ϸ� 
	// ���� ���� ���ϱ�
	litColor.xyz *= (DotDirectLightNomalMap + DotPointLightNomalMap);
	//litColor.xyz *= (length(DotDirectLightNomalMap + DotPointLightNomalMap) < 1.5f ? DotDirectLightNomalMap + DotPointLightNomalMap : 1.5f );
	return litColor; 
	//---------------------------------------------------------//
}


// �������� ����Ʈ�� �����.
// �Ű������� ���������� ���������ν�, ��� �� �� �ְ� ����.

// ����Ʈ + �ؽ�ó ����
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
