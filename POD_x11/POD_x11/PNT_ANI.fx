//=============================================================================
// Basic.fx by Frank Luna (C) 2011 All Rights Reserved.
//
// Basic effect that currently supports transformations, lighting, and texturing.
//=============================================================================

#include "LightHelper.fx"

// G 버퍼 패킹
PS_GBUFFER_OUT PackGBuffer(inout PNTVertexAniOut pin)
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
	
	// 카메라 방향벡터
	float3 vCameraDir = normalize(gEyePosW - pin.PosW);

	// 림라이트
	float  rimWidth      = 0.82f;
	float  RimLightColor = smoothstep(1.0f - rimWidth, 1.2f, 1 - max(0, dot(TangentNormal, vCameraDir)));
	float4 FinRimLight = float4(RimLightColor * 0.6f, RimLightColor * 0.45f, RimLightColor * 0.42f, 0.0f);

	// 데미지 받았을때만 별도 처리
	if (pin.AniData.x == 1.0f)
	{
		// 지속시간 (프레임 보간하면 줄어들지도..??)
		if (pin.AniData.y > 0.5f && pin.AniData.y < 2.5f)
		{
			DiffuseTex.r *= max(DiffuseTex.r * 2.0f, 1.4f);
			DiffuseTex.g *= max(DiffuseTex.g * 1.2f, 1.2f);
		}
	}

	Out.Color = DiffuseTex + FinRimLight;

	// 안개
	float3 toEye    = gEyePosW - pin.PosW;
	float distToEye = length(toEye);
	float fogLerp   = saturate((distToEye - 180.0f) / 4800.0f);
	Out.Color = lerp(Out.Color, float4(0.15f, 0.15f, 0.2f, 0.0f), fogLerp);


	// 출력	
	Out.Depth = Depth;
	//Out.Color = DiffuseTex + FinRimLight + FinRimLight;

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

	Out.Shadow = pin.ShadowPosH;
	return Out;
}

// 텍스처 선택
void GetTexMtx(Texture2D _selectTex, float2 _TexSelect, float _TexWidth, inout float4x4 _Mtx)
{
	// 텍셀에 순차적으로 접근하는 방법: 1 / (텍스쳐 너비 - 1.0f) <-- [-1.0f는 300 텍스처는 0~299까지 이므로..]
	// Tex 행 꺼내기 - tex2Dlod, Load

	_Mtx[0] = _selectTex.SampleLevel(samPoint, _TexSelect, 0);

	_TexSelect.x += (1.0f / (_TexWidth - 1.0f));
	_Mtx[1] = _selectTex.SampleLevel(samPoint, _TexSelect, 0);

	_TexSelect.x += (1.0f / (_TexWidth - 1.0f));
	_Mtx[2] = _selectTex.SampleLevel(samPoint, _TexSelect, 0);

	_TexSelect.x += (1.0f / (_TexWidth - 1.0f));
	_Mtx[3] = _selectTex.SampleLevel(samPoint, _TexSelect, 0);
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

		// e_Attack = 5,
		case 5:
			GetTexMtx(gAttackTex, _TexSelect, _TexWidth, _Mtx);
			break;
	}
}

// 스킨 모델 선택
void SelectSkinModel(float _Anikey, float2 _TexSelect, float _TexWidth, inout float4x4 _Mtx)
{
	// 애니메이션 텍스처 선택 (매트릭스 만들기)
	switch (_Anikey)
	{
	// e_Idle = 0,
	default:
	case 0:
		// Tex 행 꺼내기
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

// 스키닝 계산
PNTVertexAniOut CalSkin(inout PNTVertexAniIn vin)
{
	// 출력할 버텍스 정보
	PNTVertexAniOut vout;
	
	// 계산할 공간
	float3 _PosL      = { 0.0f, 0.0f, 0.0f };
	float3 _NormalL   = { 0.0f, 0.0f, 0.0f };
	float3 _TanL      = { 0.0f, 0.0f, 0.0f };
	float3 _BiNormalL = { 0.0f, 0.0f, 0.0f };


	// 현재 프레임이 애니 키 
	float4x4 _MadeMtx = { 1.0f, 0.0f, 0.0f, 0.0f, 
		                  0.0f, 1.0f, 0.0f, 0.0f,
					      0.0f, 0.0f, 1.0f, 0.0f,
						  0.0f, 0.0f, 0.0f, 1.0f };

	// 현재 프레임이 스킨 모델 
	float4x4 _MadeSkinMtx = { 1.0f, 0.0f, 0.0f, 0.0f, 
		                      0.0f, 1.0f, 0.0f, 0.0f,
					          0.0f, 0.0f, 1.0f, 0.0f,
					 	      0.0f, 0.0f, 0.0f, 1.0f };

	// 가중치 계산
	float _weight[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	      _weight[0] = vin.Weights.x;
	      _weight[1] = vin.Weights.y;
	      _weight[2] = vin.Weights.z;
	      _weight[3] = 1.0f - _weight[0] - _weight[1] - _weight[2];


	//-------------------------------------------------------------------------------//
	// 스키닝 모델 데이터 추출
	//-------------------------------------------------------------------------------//		
	// 스키닝 모델 선택
	float2 _TexModelSelect;
	_TexModelSelect.x = 0.0f;
	_TexModelSelect.y = vin.VtxInfo.x / (vin.VtxInfo.y - 1.0f);	// 버택스 번호, 버택스 갯수	

	// 스킨 모델 선택
	SelectSkinModel(vin.AniData.x, _TexModelSelect, 4.1f, _MadeSkinMtx);


	//-------------------------------------------------------------------------------//
	// 스키닝 애니 데이터 추출
	//-------------------------------------------------------------------------------//								  					                      
	// 애니 키 선택
	float2 _TexSelect;
	_TexSelect.y = (float)((int)vin.AniData.y) / (vin.AniData.w - 1.0f);   // 일단, 소수부 버리기 나중에 보간 해줘야함

	// 최대 4개 까지
	for (int i = 0; i < 4; ++i)
	{
		//-------------------------------------------------------------------------------//
		// 스키닝 텍스처 추출
		//-------------------------------------------------------------------------------//
		// 본 선택							                             
		_TexSelect.x = ((float)vin.BoneIndices[i] * 4.0f) / (vin.AniData.z - 1.0f);        

		// 매트릭스 선택
		SelectMtx(vin.AniData.x, _TexSelect, vin.AniData.z, _MadeMtx);
		
		//-------------------------------------------------------------------------------//
		// 스키닝 계산
		//-------------------------------------------------------------------------------//
		
		// 1.
		//float4 Skined_pos = mul(float4(vin.PosL, 1.0f), _MadeMtx);
		//_PosL += _weight[i] * (Skined_pos.xyz / Skined_pos.w);

		// 2.
		_PosL      += (_weight[i] * mul(float4(_MadeSkinMtx[0].xyz, 1.0f), _MadeMtx).xyz);
		_NormalL   +=  _weight[i] * mul(_MadeSkinMtx[1].xyz, (float3x3)_MadeMtx);
		_TanL      +=  _weight[i] * mul(_MadeSkinMtx[2].xyz, (float3x3)_MadeMtx);
		_BiNormalL +=  _weight[i] * mul(_MadeSkinMtx[3].xyz, (float3x3)_MadeMtx);


		// 테스트용 (버퍼 이용한거)
		//_PosL += _weight[i] * mul(float4(vin.PosL, 1.0f), gBoneTransforms[vin.BoneIndices[i]]).xyz;

		//-------------------------------------------------------------------------------//
	}

	//--------------------------------------------------------------------------------//
	// 원래 하던거
	//--------------------------------------------------------------------------------//
	// 최종적으로 여기다 스키닝 된 정점 정보를 덮어 써야함.

	// 데미지를 받았을 때 진동 처리 <-- 개무거움
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
	
	// 매트릭스 만들기 용도
	vout.WT = mul(_TanL     , (float3x3)vin.World);	               // W
	vout.WB = mul(_BiNormalL, (float3x3)vin.World);	               // W

	// 동차절단공간으로 변환
	vout.PosH = mul(float4(vout.PosW, 1.0f), gViewProj); // WVP

	// 어차피 변환결과는 같음. ( 거의 로컬 TM 행렬임 )
	vout.Tex = mul(float4(vin.Tex, 0.0f, 1.0f), gTexTFMtx).xy;

	// 애니 데이터
	vout.AniData = vin.AniData.xy;

	// 쉐도우
	vout.ShadowPosH = mul(float4(vout.PosW, 1.0f), gShadowTransform);
	
	// 원래는 이렇게 하라고 되어있었지만...
	//vout.NormalW = mul(_NormalL, (float3x3)gWorldInvTranspose);    // W  // 역전치월드를 로컬에 곱해주면, 오로지 회전 부분만 로컬 노멀에 적용, (회전유지, 이동X, 스케일 1로 초기화)
	return vout;
}


// 스키닝 계산
ShadowVertexOut SDCalSkin(inout PNTVertexAniIn vin)
{
	// 출력할 버텍스 정보
	ShadowVertexOut vout;
	
	// 계산할 공간
	float3 _PosL      = { 0.0f, 0.0f, 0.0f };
	float3 _NormalL   = { 0.0f, 0.0f, 0.0f };
	float3 _TanL      = { 0.0f, 0.0f, 0.0f };
	float3 _BiNormalL = { 0.0f, 0.0f, 0.0f };


	// 현재 프레임이 애니 키 
	float4x4 _MadeMtx = { 1.0f, 0.0f, 0.0f, 0.0f, 
		                  0.0f, 1.0f, 0.0f, 0.0f,
					      0.0f, 0.0f, 1.0f, 0.0f,
						  0.0f, 0.0f, 0.0f, 1.0f };

	// 현재 프레임이 스킨 모델 
	float4x4 _MadeSkinMtx = { 1.0f, 0.0f, 0.0f, 0.0f, 
		                      0.0f, 1.0f, 0.0f, 0.0f,
					          0.0f, 0.0f, 1.0f, 0.0f,
					 	      0.0f, 0.0f, 0.0f, 1.0f };

	// 가중치 계산
	float _weight[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	      _weight[0] = vin.Weights.x;
	      _weight[1] = vin.Weights.y;
	      _weight[2] = vin.Weights.z;
	      _weight[3] = 1.0f - _weight[0] - _weight[1] - _weight[2];


	//-------------------------------------------------------------------------------//
	// 스키닝 모델 데이터 추출
	//-------------------------------------------------------------------------------//		
	// 스키닝 모델 선택
	float2 _TexModelSelect;
	_TexModelSelect.x = 0.0f;
	_TexModelSelect.y = vin.VtxInfo.x / (vin.VtxInfo.y - 1.0f);	// 버택스 번호, 버택스 갯수	

	// 스킨 모델 선택
	SelectSkinModel(vin.AniData.x, _TexModelSelect, 4.1f, _MadeSkinMtx);

	//-------------------------------------------------------------------------------//
	// 스키닝 애니 데이터 추출
	//-------------------------------------------------------------------------------//								  					                      
	// 애니 키 선택
	float2 _TexSelect;
	_TexSelect.y = (float)((int)vin.AniData.y) / (vin.AniData.w - 1.0f);   // 일단, 소수부 버리기 나중에 보간 해줘야함

	// 최대 4개 까지
	for (int i = 0; i < 4; ++i)
	{
		//-------------------------------------------------------------------------------//
		// 스키닝 텍스처 추출
		//-------------------------------------------------------------------------------//
		// 본 선택							                             
		_TexSelect.x = ((float)vin.BoneIndices[i] * 4.0f) / (vin.AniData.z - 1.0f);        

		// 매트릭스 선택
		SelectMtx(vin.AniData.x, _TexSelect, vin.AniData.z, _MadeMtx);
		
		//-------------------------------------------------------------------------------//
		// 스키닝 계산
		//-------------------------------------------------------------------------------//
		_PosL      += (_weight[i] * mul(float4(_MadeSkinMtx[0].xyz, 1.0f), _MadeMtx).xyz);

	}

	//--------------------------------------------------------------------------------//
	// 원래 하던거
	//--------------------------------------------------------------------------------//

	float3 PosW = mul(float4(_PosL, 1.0f), vin.World).xyz;      // W
	
	// 동차절단공간으로 변환
	vout.PosH = mul(float4(PosW, 1.0f), gLightViewProj); // WVP

	// 어차피 변환결과는 같음. ( 거의 로컬 TM 행렬임 )
	vout.Tex = mul(float4(vin.Tex, 0.0f, 1.0f), gTexTFMtx).xy;

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

// 버텍스
ShadowVertexOut SDVS(PNTVertexAniIn vin)
{
	return SDCalSkin(vin);
}

// 셰이더 본문
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

// 셰이더 본문
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

