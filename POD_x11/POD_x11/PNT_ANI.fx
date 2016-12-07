//=============================================================================
// Basic.fx by Frank Luna (C) 2011 All Rights Reserved.
//
// Basic effect that currently supports transformations, lighting, and texturing.
//=============================================================================

#include "LightHelper.fx"

// G ���� ��ŷ
PS_GBUFFER_OUT PackGBuffer(inout PNTVertexAniOut pin)
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
	
	// ī�޶� ���⺤��
	float3 vCameraDir = normalize(gEyePosW - pin.PosW);

	// ������Ʈ
	float  rimWidth      = 0.82f;
	float  RimLightColor = smoothstep(1.0f - rimWidth, 1.2f, 1 - max(0, dot(TangentNormal, vCameraDir)));
	float4 FinRimLight = float4(RimLightColor * 0.6f, RimLightColor * 0.45f, RimLightColor * 0.42f, 0.0f);

	// ������ �޾������� ���� ó��
	if (pin.AniData.x == 1.0f)
	{
		// ���ӽð� (������ �����ϸ� �پ������..??)
		if (pin.AniData.y > 0.5f && pin.AniData.y < 2.5f)
		{
			DiffuseTex.r *= max(DiffuseTex.r * 2.0f, 1.4f);
			DiffuseTex.g *= max(DiffuseTex.g * 1.2f, 1.2f);
		}
	}

	Out.Color = DiffuseTex + FinRimLight;

	// �Ȱ�
	float3 toEye    = gEyePosW - pin.PosW;
	float distToEye = length(toEye);
	float fogLerp   = saturate((distToEye - 180.0f) / 4800.0f);
	Out.Color = lerp(Out.Color, float4(0.15f, 0.15f, 0.2f, 0.0f), fogLerp);


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

	Out.Shadow = pin.ShadowPosH;
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

// ��Ʈ���� ����
void SelectMtx(float _Anikey, float2 _TexSelect, float _TexWidth, inout float4x4 _Mtx)
{
	// �ִϸ��̼� �ؽ�ó ���� (��Ʈ���� �����)
	switch (_Anikey)
	{
		// e_Idle = 0,
		default:
		case 0:
			// Tex �� ������
			GetTexMtx(gIdleTex, _TexSelect, _TexWidth, _Mtx);
			break;

		// e_Damage = 1,
		case 1:
			GetTexMtx(gDamageTex, _TexSelect, _TexWidth, _Mtx);
			break;

		// e_Run = 2,
		case 2:
			GetTexMtx(gRunTex, _TexSelect, _TexWidth, _Mtx);
			break;

		// e_Walk = 3,
		case 3:
			GetTexMtx(gWalkTex, _TexSelect, _TexWidth, _Mtx);
			break;

		// e_Death = 4,
		case 4:
			GetTexMtx(gDeathTex, _TexSelect, _TexWidth, _Mtx);
			break;

		// e_Attack = 5,
		case 5:
			GetTexMtx(gAttackTex, _TexSelect, _TexWidth, _Mtx);
			break;
	}
}

// ��Ų �� ����
void SelectSkinModel(float _Anikey, float2 _TexSelect, float _TexWidth, inout float4x4 _Mtx)
{
	// �ִϸ��̼� �ؽ�ó ���� (��Ʈ���� �����)
	switch (_Anikey)
	{
	// e_Idle = 0,
	default:
	case 0:
		// Tex �� ������
		GetTexMtx(gIdleModelTex, _TexSelect, _TexWidth, _Mtx);
		break;

	// e_Damage = 1,
	case 1:
		GetTexMtx(gDamageModelTex, _TexSelect, _TexWidth, _Mtx);
		break;

	// e_Run = 2,
	case 2:
		GetTexMtx(gRunModelTex, _TexSelect, _TexWidth, _Mtx);
		break;

	// e_Walk = 3,
	case 3:
		GetTexMtx(gWalkModelTex, _TexSelect, _TexWidth, _Mtx);
		break;

	// e_Death = 4,
	case 4:
		GetTexMtx(gDeathModelTex, _TexSelect, _TexWidth, _Mtx);
		break;

	// e_Attack = 5,
	case 5:
		GetTexMtx(gAttackModelTex, _TexSelect, _TexWidth, _Mtx);
		break;
	}
}

// ��Ű�� ���
PNTVertexAniOut CalSkin(inout PNTVertexAniIn vin)
{
	// ����� ���ؽ� ����
	PNTVertexAniOut vout;
	
	// ����� ����
	float3 _PosL      = { 0.0f, 0.0f, 0.0f };
	float3 _NormalL   = { 0.0f, 0.0f, 0.0f };
	float3 _TanL      = { 0.0f, 0.0f, 0.0f };
	float3 _BiNormalL = { 0.0f, 0.0f, 0.0f };


	// ���� �������� �ִ� Ű 
	float4x4 _MadeMtx = { 1.0f, 0.0f, 0.0f, 0.0f, 
		                  0.0f, 1.0f, 0.0f, 0.0f,
					      0.0f, 0.0f, 1.0f, 0.0f,
						  0.0f, 0.0f, 0.0f, 1.0f };

	// ���� �������� ��Ų �� 
	float4x4 _MadeSkinMtx = { 1.0f, 0.0f, 0.0f, 0.0f, 
		                      0.0f, 1.0f, 0.0f, 0.0f,
					          0.0f, 0.0f, 1.0f, 0.0f,
					 	      0.0f, 0.0f, 0.0f, 1.0f };

	// ����ġ ���
	float _weight[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	      _weight[0] = vin.Weights.x;
	      _weight[1] = vin.Weights.y;
	      _weight[2] = vin.Weights.z;
	      _weight[3] = 1.0f - _weight[0] - _weight[1] - _weight[2];


	//-------------------------------------------------------------------------------//
	// ��Ű�� �� ������ ����
	//-------------------------------------------------------------------------------//		
	// ��Ű�� �� ����
	float2 _TexModelSelect;
	_TexModelSelect.x = 0.0f;
	_TexModelSelect.y = vin.VtxInfo.x / (vin.VtxInfo.y - 1.0f);	// ���ý� ��ȣ, ���ý� ����	

	// ��Ų �� ����
	SelectSkinModel(vin.AniData.x, _TexModelSelect, 4.1f, _MadeSkinMtx);


	//-------------------------------------------------------------------------------//
	// ��Ű�� �ִ� ������ ����
	//-------------------------------------------------------------------------------//								  					                      
	// �ִ� Ű ����
	float2 _TexSelect;
	_TexSelect.y = (float)((int)vin.AniData.y) / (vin.AniData.w - 1.0f);   // �ϴ�, �Ҽ��� ������ ���߿� ���� �������

	// �ִ� 4�� ����
	for (int i = 0; i < 4; ++i)
	{
		//-------------------------------------------------------------------------------//
		// ��Ű�� �ؽ�ó ����
		//-------------------------------------------------------------------------------//
		// �� ����							                             
		_TexSelect.x = ((float)vin.BoneIndices[i] * 4.0f) / (vin.AniData.z - 1.0f);        

		// ��Ʈ���� ����
		SelectMtx(vin.AniData.x, _TexSelect, vin.AniData.z, _MadeMtx);
		
		//-------------------------------------------------------------------------------//
		// ��Ű�� ���
		//-------------------------------------------------------------------------------//
		
		// 1.
		//float4 Skined_pos = mul(float4(vin.PosL, 1.0f), _MadeMtx);
		//_PosL += _weight[i] * (Skined_pos.xyz / Skined_pos.w);

		// 2.
		_PosL      += (_weight[i] * mul(float4(_MadeSkinMtx[0].xyz, 1.0f), _MadeMtx).xyz);
		_NormalL   +=  _weight[i] * mul(_MadeSkinMtx[1].xyz, (float3x3)_MadeMtx);
		_TanL      +=  _weight[i] * mul(_MadeSkinMtx[2].xyz, (float3x3)_MadeMtx);
		_BiNormalL +=  _weight[i] * mul(_MadeSkinMtx[3].xyz, (float3x3)_MadeMtx);


		// �׽�Ʈ�� (���� �̿��Ѱ�)
		//_PosL += _weight[i] * mul(float4(vin.PosL, 1.0f), gBoneTransforms[vin.BoneIndices[i]]).xyz;

		//-------------------------------------------------------------------------------//
	}

	//--------------------------------------------------------------------------------//
	// ���� �ϴ���
	//--------------------------------------------------------------------------------//
	// ���������� ����� ��Ű�� �� ���� ������ ���� �����.

	// �������� �޾��� �� ���� ó�� <-- �����ſ�
	//if (vin.AniData.x == 1.0f)
	//{
	//	if (vin.AniData.y < 10.0f)
	//	{
	//		float _WaveX = sin(vin.AniData.y * 5.0f) * pow(0.5f, vin.AniData.y);
	//		_PosL.z += _WaveX * 2.5f;
	//	}
	//}

	vout.PosW    = mul(float4(_PosL, 1.0f), vin.World).xyz;        // W
	vout.NormalW = mul(_NormalL, (float3x3)vin.World);			   // W 
	
	// ��Ʈ���� ����� �뵵
	vout.WT = mul(_TanL     , (float3x3)vin.World);	               // W
	vout.WB = mul(_BiNormalL, (float3x3)vin.World);	               // W

	// �������ܰ������� ��ȯ
	vout.PosH = mul(float4(vout.PosW, 1.0f), gViewProj); // WVP

	// ������ ��ȯ����� ����. ( ���� ���� TM ����� )
	vout.Tex = mul(float4(vin.Tex, 0.0f, 1.0f), gTexTFMtx).xy;

	// �ִ� ������
	vout.AniData = vin.AniData.xy;

	// ������
	vout.ShadowPosH = mul(float4(vout.PosW, 1.0f), gShadowTransform);
	
	// ������ �̷��� �϶�� �Ǿ��־�����...
	//vout.NormalW = mul(_NormalL, (float3x3)gWorldInvTranspose);    // W  // ����ġ���带 ���ÿ� �����ָ�, ������ ȸ�� �κи� ���� ��ֿ� ����, (ȸ������, �̵�X, ������ 1�� �ʱ�ȭ)
	return vout;
}


// ��Ű�� ���
ShadowVertexOut SDCalSkin(inout PNTVertexAniIn vin)
{
	// ����� ���ؽ� ����
	ShadowVertexOut vout;
	
	// ����� ����
	float3 _PosL      = { 0.0f, 0.0f, 0.0f };
	float3 _NormalL   = { 0.0f, 0.0f, 0.0f };
	float3 _TanL      = { 0.0f, 0.0f, 0.0f };
	float3 _BiNormalL = { 0.0f, 0.0f, 0.0f };


	// ���� �������� �ִ� Ű 
	float4x4 _MadeMtx = { 1.0f, 0.0f, 0.0f, 0.0f, 
		                  0.0f, 1.0f, 0.0f, 0.0f,
					      0.0f, 0.0f, 1.0f, 0.0f,
						  0.0f, 0.0f, 0.0f, 1.0f };

	// ���� �������� ��Ų �� 
	float4x4 _MadeSkinMtx = { 1.0f, 0.0f, 0.0f, 0.0f, 
		                      0.0f, 1.0f, 0.0f, 0.0f,
					          0.0f, 0.0f, 1.0f, 0.0f,
					 	      0.0f, 0.0f, 0.0f, 1.0f };

	// ����ġ ���
	float _weight[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	      _weight[0] = vin.Weights.x;
	      _weight[1] = vin.Weights.y;
	      _weight[2] = vin.Weights.z;
	      _weight[3] = 1.0f - _weight[0] - _weight[1] - _weight[2];


	//-------------------------------------------------------------------------------//
	// ��Ű�� �� ������ ����
	//-------------------------------------------------------------------------------//		
	// ��Ű�� �� ����
	float2 _TexModelSelect;
	_TexModelSelect.x = 0.0f;
	_TexModelSelect.y = vin.VtxInfo.x / (vin.VtxInfo.y - 1.0f);	// ���ý� ��ȣ, ���ý� ����	

	// ��Ų �� ����
	SelectSkinModel(vin.AniData.x, _TexModelSelect, 4.1f, _MadeSkinMtx);

	//-------------------------------------------------------------------------------//
	// ��Ű�� �ִ� ������ ����
	//-------------------------------------------------------------------------------//								  					                      
	// �ִ� Ű ����
	float2 _TexSelect;
	_TexSelect.y = (float)((int)vin.AniData.y) / (vin.AniData.w - 1.0f);   // �ϴ�, �Ҽ��� ������ ���߿� ���� �������

	// �ִ� 4�� ����
	for (int i = 0; i < 4; ++i)
	{
		//-------------------------------------------------------------------------------//
		// ��Ű�� �ؽ�ó ����
		//-------------------------------------------------------------------------------//
		// �� ����							                             
		_TexSelect.x = ((float)vin.BoneIndices[i] * 4.0f) / (vin.AniData.z - 1.0f);        

		// ��Ʈ���� ����
		SelectMtx(vin.AniData.x, _TexSelect, vin.AniData.z, _MadeMtx);
		
		//-------------------------------------------------------------------------------//
		// ��Ű�� ���
		//-------------------------------------------------------------------------------//
		_PosL      += (_weight[i] * mul(float4(_MadeSkinMtx[0].xyz, 1.0f), _MadeMtx).xyz);

	}

	//--------------------------------------------------------------------------------//
	// ���� �ϴ���
	//--------------------------------------------------------------------------------//

	float3 PosW = mul(float4(_PosL, 1.0f), vin.World).xyz;      // W
	
	// �������ܰ������� ��ȯ
	vout.PosH = mul(float4(PosW, 1.0f), gLightViewProj); // WVP

	// ������ ��ȯ����� ����. ( ���� ���� TM ����� )
	vout.Tex = mul(float4(vin.Tex, 0.0f, 1.0f), gTexTFMtx).xy;

	return vout;
}


// ���ؽ�
PNTVertexAniOut VS(PNTVertexAniIn vin)
{
	return CalSkin(vin);
}

// �ι�° �Ű������� ����, �ؽ�ó ��� ������ ������.
PS_GBUFFER_OUT PS(PNTVertexAniOut pin)/* : SV_Target*/
{
	// G���۸� ��ŷ�Ѵ�.
	return PackGBuffer(pin);
}

// ���ؽ�
ShadowVertexOut SDVS(PNTVertexAniIn vin)
{
	return SDCalSkin(vin);
}

// ���̴� ����
technique11 PongTexAni
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

// ���̴� ����
technique11 SDPongTexAni
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, SDVS()));
		SetGeometryShader(NULL);
		SetPixelShader(NULL);

		SetRasterizerState(Depth);
	}
}

