//=============================================================================
// Basic.fx by Frank Luna (C) 2011 All Rights Reserved.
//
// Basic effect that currently supports transformations, lighting, and texturing.
//=============================================================================

#include "LightHelper.fx"


// G ���� ��ŷ
PS_GBUFFER_OUT PackGBuffer(PNTVertexAniOut pin)
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

	// ���	
	Out.Depth = Depth;
	Out.Color = DiffuseTex;

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
void GetTexMtx(Texture2D _selectTex, float2 _TexSelect, inout float4x4 _Mtx)
{
	// Tex �� ������
	float4 tex_col1 = _selectTex.SampleLevel(samLinear, _TexSelect, 0);

	++_TexSelect.x;
	float4 tex_col2 = _selectTex.SampleLevel(samLinear, _TexSelect, 0);

	++_TexSelect.x;
	float4 tex_col3 = _selectTex.SampleLevel(samLinear, _TexSelect, 0);

	++_TexSelect.x;
	float4 tex_col4 = _selectTex.SampleLevel(samLinear, _TexSelect, 0);

	// ��Ʈ���� �����
	_Mtx._11 = tex_col1.x; 	_Mtx._12 = tex_col1.y; 	_Mtx._13 = tex_col1.z; 	_Mtx._14 = tex_col1.w;
	_Mtx._21 = tex_col2.x; 	_Mtx._22 = tex_col2.y; 	_Mtx._23 = tex_col2.z; 	_Mtx._24 = tex_col2.w;
	_Mtx._31 = tex_col3.x; 	_Mtx._32 = tex_col3.y; 	_Mtx._33 = tex_col3.z; 	_Mtx._34 = tex_col3.w;
	_Mtx._41 = tex_col4.x; 	_Mtx._42 = tex_col4.y; 	_Mtx._43 = tex_col4.z; 	_Mtx._44 = tex_col4.w;
}

// ��Ʈ���� ����
void SelectMtx(float _Anikey, float2 _TexSelect, inout float4x4 _Mtx)
{
	// �ִϸ��̼� �ؽ�ó ���� (��Ʈ���� �����)
	switch (_Anikey)
	{
		// e_Idle = 0,
		default:
		case 0:
			// Tex �� ������
			GetTexMtx(gIdleTex, _TexSelect, _Mtx);
			break;

		// e_Damage = 1,
		case 1:
			GetTexMtx(gDamageTex, _TexSelect, _Mtx);
			break;

		// e_Run = 2,
		case 2:
			GetTexMtx(gRunTex, _TexSelect, _Mtx);
			break;

		// e_Walk = 3,
		case 3:
			GetTexMtx(gWalkTex, _TexSelect, _Mtx);
			break;

		// e_Death = 4,
		case 4:
			GetTexMtx(gDeathTex, _TexSelect, _Mtx);
			break;

		// e_DeathWait = 5,
		case 5:
			GetTexMtx(gDeathWaitTex, _TexSelect, _Mtx);
			break;

		// e_Attack1 = 6,
		case 6:
			GetTexMtx(gAttack1Tex, _TexSelect, _Mtx);
			break;

		// e_Attack2 = 7,
		case 7:
			GetTexMtx(gAttack2Tex, _TexSelect, _Mtx);
			break;

		// e_Attack3 = 8,
		case 8:
			GetTexMtx(gAttack3Tex, _TexSelect, _Mtx);
			break;

		// e_Stun = 9
		case 9:
			GetTexMtx(gStunTex, _TexSelect, _Mtx);
			break;
	}
}

// ��Ű�� ���
PNTVertexAniOut CalSkin(PNTVertexAniIn vin)
{
	// ����� ���ؽ� ����
	PNTVertexAniOut vout;
	
	// ����� ����
	float3 _PosL      = { 0.0f, 0.0f, 0.0f };
	float3 _NormalL   = { 0.0f, 0.0f, 0.0f };
	float3 _TanL      = { 0.0f, 0.0f, 0.0f };
	float3 _BiNormalL = { 0.0f, 0.0f, 0.0f };


	// ���� �������� �ִ� Ű 
	float    _AniKey  = vin.AniData.y;
	float4x4 _MadeMtx = { 1.0f, 0.0f, 0.0f, 0.0f, 
		                  0.0f, 1.0f, 0.0f, 0.0f,
					      0.0f, 0.0f, 1.0f, 0.0f,
						  0.0f, 0.0f, 0.0f, 1.0f };

	// ����ġ ���
	float _weight[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	_weight[0] = vin.Weights.x;
	_weight[1] = vin.Weights.y;
	_weight[2] = vin.Weights.z;
	_weight[3] = 1.0f - _weight[0] - _weight[1] - _weight[2];

	// �ִ� Ű ����
	float2 _TexSelect;
	_TexSelect.y = (int)_AniKey;           // int������ �޾Ƽ�, �ڿ��� �ٹ��� (�׽�Ʈ ����) <-- ���߿� ����������� - 0.12, 0.5 �̷���

	// �ִ� 4�� ����
	for (int i = 0; i < 4; ++i)
	{
		//-------------------------------------------------------------------------------//
		// �ؽ�ó ����
		//-------------------------------------------------------------------------------//
		// �� ����
		_TexSelect.x = vin.BoneIndices[i] * 4; // ��� �ȼ��� 4ĭ�� �ٹǷ�	   // �ؼ� 1�� == �� �� �̹Ƿ�, 4���� ���� �� 
																		   // �׷��� uvó���Ҷ� Tex U�ʿ� * 4 �̷��� ������� �� (4���� ���..)
		// ��Ʈ���� ����
		SelectMtx(vin.AniData.x, _TexSelect, _MadeMtx);

		//-------------------------------------------------------------------------------//
		// ��Ű�� ���
		//-------------------------------------------------------------------------------//
		_PosL      += _weight[i] * mul(float4(vin.PosL, 1.0f), _MadeMtx).xyz;
		_NormalL   += _weight[i] * mul(vin.NormalL , (float3x3)_MadeMtx);
		_TanL      += _weight[i] * mul(vin.Tangent , (float3x3)_MadeMtx);
		_BiNormalL += _weight[i] * mul(vin.BiNormal, (float3x3)_MadeMtx);
		//-------------------------------------------------------------------------------//

	}

	//--------------------------------------------------------------------------------//
	// ���� �ϴ���
	//--------------------------------------------------------------------------------//
	// ���������� ����� ��Ű�� �� ���� ������ ���� �����.

	// Transform to world space space.
	vout.PosW    = mul(float4(_PosL, 1.0f), vin.World).xyz;        // W
	vout.NormalW = mul(_NormalL, (float3x3)gWorldInvTranspose);    // W  // ����ġ���带 ���ÿ� �����ָ�, ������ ȸ�� �κи� ���� ��ֿ� ����, (ȸ������, �̵�X, ������ 1�� �ʱ�ȭ)
	
	// ��Ʈ���� ����� �뵵
	vout.WT = mul(_TanL     , (float3x3)vin.World);	               // W
	vout.WB = mul(_BiNormalL, (float3x3)vin.World);	               // W

	// �������ܰ������� ��ȯ
	vout.PosH = mul(float4(vout.PosW, 1.0f), gViewProj); // WVP

	// ������ ��ȯ����� ����. ( ���� ���� TM ����� )
	vout.Tex = mul(float4(vin.Tex, 0.0f, 1.0f), gTexTFMtx).xy;
	//--------------------------------------------------------------------------------//



	return vout;
}


// ���ؽ�
PNTVertexAniOut VS(PNTVertexAniIn vin)
{
	// �ϵ���� ��Ű�� ���
	return CalSkin(vin);
}

// �ι�° �Ű������� ����, �ؽ�ó ��� ������ ������.
PS_GBUFFER_OUT PS(PNTVertexAniOut pin)/* : SV_Target*/
{
	// G���۸� ��ŷ�Ѵ�.
	return PackGBuffer(pin);
}

// ���̴� ����
technique11 PongTexAni
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
}
