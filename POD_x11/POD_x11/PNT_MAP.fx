//=============================================================================
// Basic.fx by Frank Luna (C) 2011 All Rights Reserved.
//
// Basic effect that currently supports transformations, lighting, and texturing.
//=============================================================================

#include "LightHelper.fx"

// G 버퍼 패킹
PS_GBUFFER_OUT PackGBuffer(inout PNTVertexMapOut pin)
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
	
	//// 카메라 방향벡터
	//float3 vCameraDir = normalize(gEyePosW - pin.PosW);

	//// 림라이트
	//float  rimWidth      = 0.2f;
	//float  RimLightColor = smoothstep(1.0f - rimWidth, 1.0f, 1 - max(0.0f, dot(TangentNormal, normalize(-gDirLight.Direction))));
	//float4 FinRimLight = float4(RimLightColor * 0.7f, RimLightColor * 0.15f, RimLightColor * 0.01f, 1.0f);

	Out.Color = DiffuseTex/* + FinRimLight*/;

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

// 스킨 모델 선택
void SelectSkinModel(float _Anikey, float2 _TexSelect, float _TexWidth, inout float4x4 _Mtx)
{
	// 애니메이션 텍스처 선택 (매트릭스 만들기)
	switch (_Anikey)
	{
	// 기본 맵,
	default:
	case 0:
		// Tex 행 꺼내기
		GetTexMtx(gMapTex, _TexSelect, _TexWidth, _Mtx);
		break;
	}
}

// 맵 계산
PNTVertexMapOut CalMap(inout PNTVertexMapIn vin)
{
	// 출력할 버텍스 정보
	PNTVertexMapOut vout;
	
	// 현재 프레임이 스킨 모델 
	float4x4 _MadeSkinMtx = { 1.0f, 0.0f, 0.0f, 0.0f, 
		                      0.0f, 1.0f, 0.0f, 0.0f,
					          0.0f, 0.0f, 1.0f, 0.0f,
					 	      0.0f, 0.0f, 0.0f, 1.0f };

	//-------------------------------------------------------------------------------//
	// 맵 모델 데이터 추출
	//-------------------------------------------------------------------------------//		
	// 맵 모델 선택
	float2 _TexModelSelect;
	_TexModelSelect.x = 0.0f;
	_TexModelSelect.y = vin.VtxInfo.x / (vin.VtxInfo.y - 1.0f);	// 버택스 번호, 버택스 갯수	

	// 스킨 모델 선택
	SelectSkinModel(vin.TexData.x, _TexModelSelect, 4.1f, _MadeSkinMtx);


	//--------------------------------------------------------------------------------//
	// 변환
	//--------------------------------------------------------------------------------//
	vout.PosW    = mul(float4(_MadeSkinMtx[0].xyz, 1.0f), vin.World).xyz;      // W
	vout.NormalW = mul(_MadeSkinMtx[1].xyz, (float3x3)vin.World);			   // W 
	
	// 매트릭스 만들기 용도
	vout.WT = mul(_MadeSkinMtx[2].xyz, (float3x3)vin.World);	               // W
	vout.WB = mul(_MadeSkinMtx[3].xyz, (float3x3)vin.World);	               // W

	// 동차절단공간으로 변환
	vout.PosH = mul(float4(vout.PosW, 1.0f), gViewProj); // WVP

	// 어차피 변환결과는 같음. ( 거의 로컬 TM 행렬임 )
	vout.Tex = mul(float4(vin.Tex, 0.0f, 1.0f), gTexTFMtx).xy;

	// 원래는 이렇게 하라고 되어있었지만...
	//vout.NormalW = mul(_NormalL, (float3x3)gWorldInvTranspose);    // W  // 역전치월드를 로컬에 곱해주면, 오로지 회전 부분만 로컬 노멀에 적용, (회전유지, 이동X, 스케일 1로 초기화)
	return vout;
}


// 버텍스
PNTVertexMapOut VS(PNTVertexMapIn vin)
{
	return CalMap(vin);
}

// 두번째 매개변수를 통해, 텍스처 사용 유무를 가른다.
PS_GBUFFER_OUT PS(PNTVertexMapOut pin)/* : SV_Target*/
{
	// G버퍼를 패킹한다.
	return PackGBuffer(pin);
}

// 셰이더 본문
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

