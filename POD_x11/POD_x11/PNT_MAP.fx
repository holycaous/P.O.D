//=============================================================================
// Basic.fx by Frank Luna (C) 2011 All Rights Reserved.
//
// Basic effect that currently supports transformations, lighting, and texturing.
//=============================================================================

#include "LightHelper.fx"

// G ���� ��ŷ
PS_GBUFFER_OUT PackGBuffer(inout PNTVertexMapOut pin)
{
	PS_GBUFFER_OUT Out;

	// Interpolating normal can unnormalize it, so normalize it.
	pin.NormalW = normalize(pin.NormalW);

	// Default to multiplicative identity.
	// �ؽ�ó ��� ����
	float4 DepthTex      = float4(1, 1, 1, 1);
	float4 DiffuseTex    = float4(1, 1, 1, 1);
	float4 SpecularTex   = float4(1, 1, 1, 1);
	float4 BumpNormalTex = float4(1, 1, 1, 1);
	float3 TangentNormal = float3(1, 1, 1);

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
	
	//// ī�޶� ���⺤��
	//float3 vCameraDir = normalize(gEyePosW - pin.PosW);

	//// ������Ʈ
	//float  rimWidth      = 0.2f;
	//float  RimLightColor = smoothstep(1.0f - rimWidth, 1.0f, 1 - max(0.0f, dot(TangentNormal, normalize(-gDirLight.Direction))));
	//float4 FinRimLight = float4(RimLightColor * 0.7f, RimLightColor * 0.15f, RimLightColor * 0.01f, 1.0f);

	Out.Color = DiffuseTex/* + FinRimLight*/;

	// ���	
	Out.Depth = Depth;
	//Out.Color = DiffuseTex + FinRimLight + FinRimLight;

	//--------------------------------------------------------------//
	// ������ �� ����
	//--------------------------------------------------------------//

	// 1.
	//Out.Position = PosV;
	//
	// 2.
	//Out.Position = float4(PosP.xyz, 1.0f) * 0.5 + 0.5; // PosP <---
	//
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

// �ؽ�ó ����
void GetTexMtx(Texture2D _selectTex, float2 _TexSelect, float _TexWidth, inout float4x4 _Mtx)
{
	// �ؼ��� ���������� �����ϴ� ���: 1 / (�ؽ��� �ʺ� - 1.0f) <-- [-1.0f�� 300 �ؽ�ó�� 0~299���� �̹Ƿ�..]
	// Tex �� ������ - tex2Dlod, Load

	_Mtx[0] = _selectTex.SampleLevel(samPoint, _TexSelect, 0);

	_TexSelect.x += (1.0f / (_TexWidth - 1.0f));
	_Mtx[1] = _selectTex.SampleLevel(samPoint, _TexSelect, 0);

	_TexSelect.x += (1.0f / (_TexWidth - 1.0f));
	_Mtx[2] = _selectTex.SampleLevel(samPoint, _TexSelect, 0);

	_TexSelect.x += (1.0f / (_TexWidth - 1.0f));
	_Mtx[3] = _selectTex.SampleLevel(samPoint, _TexSelect, 0);
}

// ��Ų �� ����
void SelectSkinModel(float _Anikey, float2 _TexSelect, float _TexWidth, inout float4x4 _Mtx)
{
	// �ִϸ��̼� �ؽ�ó ���� (��Ʈ���� �����)
	switch (_Anikey)
	{
	// �⺻ ��,
	default:
	case 0:
		// Tex �� ������
		GetTexMtx(gMapTex, _TexSelect, _TexWidth, _Mtx);
		break;
	}
}

// �� ���
PNTVertexMapOut CalMap(inout PNTVertexMapIn vin)
{
	// ����� ���ؽ� ����
	PNTVertexMapOut vout;
	
	// ���� �������� ��Ų �� 
	float4x4 _MadeSkinMtx = { 1.0f, 0.0f, 0.0f, 0.0f, 
		                      0.0f, 1.0f, 0.0f, 0.0f,
					          0.0f, 0.0f, 1.0f, 0.0f,
					 	      0.0f, 0.0f, 0.0f, 1.0f };

	//-------------------------------------------------------------------------------//
	// �� �� ������ ����
	//-------------------------------------------------------------------------------//		
	// �� �� ����
	float2 _TexModelSelect;
	_TexModelSelect.x = 0.0f;
	_TexModelSelect.y = vin.VtxInfo.x / (vin.VtxInfo.y - 1.0f);	// ���ý� ��ȣ, ���ý� ����	

	// ��Ų �� ����
	SelectSkinModel(vin.TexData.x, _TexModelSelect, 4.1f, _MadeSkinMtx);


	//--------------------------------------------------------------------------------//
	// ��ȯ
	//--------------------------------------------------------------------------------//
	vout.PosW    = mul(float4(_MadeSkinMtx[0].xyz, 1.0f), vin.World).xyz;      // W
	vout.NormalW = mul(_MadeSkinMtx[1].xyz, (float3x3)vin.World);			   // W 
	
	// ��Ʈ���� ����� �뵵
	vout.WT = mul(_MadeSkinMtx[2].xyz, (float3x3)vin.World);	               // W
	vout.WB = mul(_MadeSkinMtx[3].xyz, (float3x3)vin.World);	               // W

	// �������ܰ������� ��ȯ
	vout.PosH = mul(float4(vout.PosW, 1.0f), gViewProj); // WVP

	// ������ ��ȯ����� ����. ( ���� ���� TM ����� )
	vout.Tex = mul(float4(vin.Tex, 0.0f, 1.0f), gTexTFMtx).xy;

	// ������ �̷��� �϶�� �Ǿ��־�����...
	//vout.NormalW = mul(_NormalL, (float3x3)gWorldInvTranspose);    // W  // ����ġ���带 ���ÿ� �����ָ�, ������ ȸ�� �κи� ���� ��ֿ� ����, (ȸ������, �̵�X, ������ 1�� �ʱ�ȭ)
	return vout;
}


// ���ؽ�
PNTVertexMapOut VS(PNTVertexMapIn vin)
{
	return CalMap(vin);
}

// �ι�° �Ű������� ����, �ؽ�ó ��� ������ ������.
PS_GBUFFER_OUT PS(PNTVertexMapOut pin)/* : SV_Target*/
{
	// G���۸� ��ŷ�Ѵ�.
	return PackGBuffer(pin);
}

// ���̴� ����
technique11 PongTexMap
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS()));

		SetRasterizerState(0);
		SetDepthStencilState(LessDSS, 0);
	}
}

