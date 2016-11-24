//=============================================================================
// Basic.fx by Frank Luna (C) 2011 All Rights Reserved.
//
// Basic effect that currently supports transformations, lighting, and texturing.
//=============================================================================

#include "LightHelper.fx"


// 애니 상태에 따른 색상혼합
void curAniState(float2 AniData, inout float4 _DiffuseTex)
{
	// 애니메이션 텍스처 선택 (매트릭스 만들기)
	switch (AniData.x)
	{
		// e_Idle = 0, e_Run = 2, e_Walk = 3, e_Attack1 = 6, e_Attack2 = 7, e_Attack3 = 8
		default:
		case 0:
		case 2:
		case 3:
		case 6:
		case 7:
		case 8:
			break;

		// e_Damage (red)
		case 1:
			// 지속 프레임
			if (AniData.y < 0.2f)
			{
				_DiffuseTex.x  *= 2.0f;
				_DiffuseTex.xy *= 1.2f;
			}
			break;

		// e_Death (gray)
		case 4:
			{
			    float3 _incDownColor = _DiffuseTex.xyz / AniData.y;
			    float3 _stdDownColor = _DiffuseTex.xyz * 0.2f;

				// 일정 색상까지만
				if (_incDownColor.x > _stdDownColor.x)
				    _DiffuseTex.xyz = _incDownColor;
				else
					_DiffuseTex.xyz = _stdDownColor;
			}
			break;

		// e_DeathWait (gray)
		case 5:
			_DiffuseTex.xyz *= 0.2f;
			break;

		// e_Stun (yellow)
		case 9:
			// 지속 프레임
			if (AniData.y < 0.2f) // 프레임
			{
				_DiffuseTex.xy *= 2.0f;
				_DiffuseTex.z  *= 1.2f;
			}
			break;
	}
}

// G 버퍼 패킹
PS_GBUFFER_OUT PackGBuffer(PNTVertexAniOut pin)
{
	PS_GBUFFER_OUT Out;

	// Interpolating normal can unnormalize it, so normalize it.
	pin.NormalW = normalize(pin.NormalW);

	// Default to multiplicative identity.
	// 텍스처 사용 유무
	float4 DepthTex      = float4(1, 1, 1, 1);
	float4 DiffuseTex    = float4(1, 1, 1, 1);
	float4 SpecularTex   = float4(1, 1, 1, 1);
	float4 BumpNormalTex = float4(1, 1, 1, 1);
	float3 TangentNormal = float3(1, 1, 1);

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

	// 현재 애니 상태에 DiffuseTex 색상혼합
	curAniState(pin.AniData, DiffuseTex);

	// 출력	
	Out.Depth = Depth;
	Out.Color = DiffuseTex;

	//--------------------------------------------------------------//
	// 포지션 맵 저장
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

	Out.Normal = float4(TangentNormal, 0.0f) * 0.5 + 0.5; // <--- 희안하게 노멀라이즈하면 에러남
	//Out.Normal   = float4(pin.NormalW, 1.0f)  * 0.5 + 0.5;
	Out.Specular = SpecularTex;

	return Out;
}


// 텍스처 선택
void GetTexMtx(Texture2D _selectTex, float2 _TexSelect, float _TexWidth, inout float4x4 _Mtx)
{
	// 텍셀에 순차적으로 접근하는 방법: 1 / (텍스쳐 너비 - 1.0f) <-- [-1.0f는 300 텍스처는 0~299까지 이므로..]

	// Tex 행 꺼내기 
	float4 tex_col1 = _selectTex.SampleLevel(samPoint, _TexSelect, 0);

	_TexSelect.x += 1 / (_TexWidth - 1.0f);
	float4 tex_col2 = _selectTex.SampleLevel(samPoint, _TexSelect, 0);

	_TexSelect.x += 1 / (_TexWidth - 1.0f);
	float4 tex_col3 = _selectTex.SampleLevel(samPoint, _TexSelect, 0);

	_TexSelect.x += 1 / (_TexWidth - 1.0f);
	float4 tex_col4 = _selectTex.SampleLevel(samPoint, _TexSelect, 0);

	// 매트릭스 만들기
	_Mtx._11 = tex_col1.x; 	 _Mtx._12 = tex_col1.y; 	_Mtx._13 = tex_col1.z;  	_Mtx._14 = tex_col1.w;
	_Mtx._21 = tex_col2.x; 	 _Mtx._22 = tex_col2.y; 	_Mtx._23 = tex_col2.z;  	_Mtx._24 = tex_col2.w;
	_Mtx._31 = tex_col3.x; 	 _Mtx._32 = tex_col3.y; 	_Mtx._33 = tex_col3.z;  	_Mtx._34 = tex_col3.w;
	_Mtx._41 = tex_col4.x; 	 _Mtx._42 = tex_col4.y; 	_Mtx._43 = tex_col4.z;  	_Mtx._44 = tex_col4.w;
}

// 매트릭스 선택
void SelectMtx(float _Anikey, float2 _TexSelect, float _TexWidth, inout float4x4 _Mtx)
{
	// 애니메이션 텍스처 선택 (매트릭스 만들기)
	switch (_Anikey)
	{
		// e_Idle = 0,
		default:
		case 0:
			// Tex 행 꺼내기
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

		// e_DeathWait = 5,
		case 5:
			GetTexMtx(gDeathWaitTex, _TexSelect, _TexWidth, _Mtx);
			break;

		// e_Attack1 = 6,
		case 6:
			GetTexMtx(gAttack1Tex, _TexSelect, _TexWidth, _Mtx);
			break;

		// e_Attack2 = 7,
		case 7:
			GetTexMtx(gAttack2Tex, _TexSelect, _TexWidth, _Mtx);
			break;

		// e_Attack3 = 8,
		case 8:
			GetTexMtx(gAttack3Tex, _TexSelect, _TexWidth, _Mtx);
			break;

		// e_Stun = 9
		case 9:
			GetTexMtx(gStunTex, _TexSelect, _TexWidth, _Mtx);
			break;
	}
}

// 스키닝 계산
PNTVertexAniOut CalSkin(PNTVertexAniIn vin)
{
	// 출력할 버텍스 정보
	PNTVertexAniOut vout;
	
	// 계산할 공간
	float3 _PosL      = { 0.0f, 0.0f, 0.0f };
	float3 _NormalL   = { 0.0f, 0.0f, 0.0f };
	float3 _TanL      = { 0.0f, 0.0f, 0.0f };
	float3 _BiNormalL = { 0.0f, 0.0f, 0.0f };


	// 현재 프레임이 애니 키 
	float    _AniKey  = vin.AniData.y;
	float4x4 _MadeMtx = { 1.0f, 0.0f, 0.0f, 0.0f, 
		                  0.0f, 1.0f, 0.0f, 0.0f,
					      0.0f, 0.0f, 1.0f, 0.0f,
						  0.0f, 0.0f, 0.0f, 1.0f };

	// 가중치 계산
	float _weight[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	_weight[0] = vin.Weights.x;
	_weight[1] = vin.Weights.y;
	_weight[2] = vin.Weights.z;
	_weight[3] = 1.0f - _weight[0] - _weight[1] - _weight[2];

	// 애니 키 선택
	float2 _TexSelect;
	_TexSelect.y = _AniKey / (vin.AniData.w - 1.0f);  // 일단, 소수부 버리기 나중에 보간 해줘야함

	// 최대 4개 까지
	for (int i = 0; i < 4; ++i)
	{
		//-------------------------------------------------------------------------------//
		// 텍스처 추출
		//-------------------------------------------------------------------------------//
		// 본 선택							                             
													  					         // 행렬 픽셀이 4칸씩 뛰므로	( 텍셀 1개 == 한 행 이므로, 4개를 얻어야 함 ) 
		_TexSelect.x = ((float)vin.BoneIndices[i] * 4.0f) / (vin.AniData.z - 1.0f);     // 그래서 uv처리할때 Tex U쪽에 * 4 이런거 해줘야할 듯 (4개씩 얻고..)

		// 매트릭스 선택
		SelectMtx(vin.AniData.x, _TexSelect, vin.AniData.z, _MadeMtx);
		
		//-------------------------------------------------------------------------------//
		// 스키닝 계산
		//-------------------------------------------------------------------------------//
		float4 Skined_pos = mul(float4(vin.PosL, 1.0f), _MadeMtx);
		_PosL += _weight[i] * (Skined_pos.xyz / Skined_pos.w);
		//_PosL      += _weight[i] * mul(float4(vin.PosL, 1.0f), _MadeMtx).xyz;
		//_NormalL   += _weight[i] * mul(vin.NormalL , (float3x3)_MadeMtx);
		//_TanL      += _weight[i] * mul(vin.Tangent , (float3x3)_MadeMtx);
		//_BiNormalL += _weight[i] * mul(vin.BiNormal, (float3x3)_MadeMtx);
		//-------------------------------------------------------------------------------//
	}

	//--------------------------------------------------------------------------------//
	// 원래 하던거
	//--------------------------------------------------------------------------------//
	// 최종적으로 여기다 스키닝 된 정점 정보를 덮어 써야함.

	vout.PosW    = mul(float4(_PosL, 1.0f), vin.World).xyz;          // W
	//vout.NormalW = mul(_NormalL, (float3x3)gWorldInvTranspose);    // W  // 역전치월드를 로컬에 곱해주면, 오로지 회전 부분만 로컬 노멀에 적용, (회전유지, 이동X, 스케일 1로 초기화)
	
	// 매트릭스 만들기 용도
	//vout.WT = mul(_TanL     , (float3x3)vin.World);	               // W
	//vout.WB = mul(_BiNormalL, (float3x3)vin.World);	               // W

	// 동차절단공간으로 변환
	vout.PosH = mul(float4(vout.PosW, 1.0f), gViewProj); // WVP

	// 어차피 변환결과는 같음. ( 거의 로컬 TM 행렬임 )
	vout.Tex = mul(float4(vin.Tex, 0.0f, 1.0f), gTexTFMtx).xy;

	// 애니 데이터 정보
	vout.AniData = vin.AniData.xy;

	//--------------------------------------------------------------------------------//
	// 디퍼드 렌더링이기때문에.. 딱히 저 위에 안해줘도..??
	//--------------------------------------------------------------------------------//
	vout.NormalW = mul(vin.NormalL, (float3x3)vin.World);

	// 매트릭스 만들기 용도
	vout.WT = mul(vin.Tangent , (float3x3)vin.World);
	vout.WB = mul(vin.BiNormal, (float3x3)vin.World);

	return vout;
}


// 버텍스
PNTVertexAniOut VS(PNTVertexAniIn vin)
{
	return CalSkin(vin);
}

// 두번째 매개변수를 통해, 텍스처 사용 유무를 가른다.
PS_GBUFFER_OUT PS(PNTVertexAniOut pin)/* : SV_Target*/
{
	// G버퍼를 패킹한다.
	return PackGBuffer(pin);
}

// 셰이더 본문
technique11 PongTexAni
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
}

