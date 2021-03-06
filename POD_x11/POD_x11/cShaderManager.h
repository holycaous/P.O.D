#pragma once

#include <d3dcompiler.h>

extern cCam gCam;

// 쉐이더 매니저
class cShaderManager : public cSingleton<cShaderManager>
{
	cCoreStorage*  mCoreStorage  = cCoreStorage ::GetInstance();
	cLightManager* mLightManager = cLightManager::GetInstance();
	cMapManager*   mMapManager   = cMapManager  ::GetInstance();
	cShadowMap *   mShadowMap    = cShadowMap   ::GetInstance();
	cHDRManager*   mHDRManager   = cHDRManager  ::GetInstance();

public:
	// 쉐이더 종류
	map<int, EffectStorage*> mShader;

	// 렌더링 모드
	SHADER_TYPE mShaderMode;

public:
	void Init()
	{
		// FX 초기화 & 빌드
		InitFX();
	}

	void ClearClass()
	{
		// 쉐이더 삭제
		for (map<int, EffectStorage*>::iterator itor = mShader.begin(); itor != mShader.end();)
		{
			// 쉐이더 변수 내부 객체 완전 삭제
			SafeDelete(itor->second);

			// 다음 변수로
			mShader.erase(itor++);
		}
		// 쉐이더 변수 삭제
		mShader.clear();
	}

public:
	// Get & Set
	// 쉐이더 변수 Set 하기 ( 오버로딩 )
	void SetShaderValue(SHADER_VAL_TYPE _ValueEnum, char* _Name, ID3D11ShaderResourceView* _value)
	{
		mShader[mShaderMode]->mfxResource[_Name]->SetResource(_value);
	}

	void SetShaderValue(SHADER_VAL_TYPE _ValueEnum, char* _Name, SHADER_TYPE _ShaderMode, ID3D11ShaderResourceView* _value)
	{
		mShader[_ShaderMode]->mfxResource[_Name]->SetResource(_value);
	}

	void SetShaderValue(SHADER_VAL_TYPE _ValueEnum, char* _Name, SHADER_TYPE _ShaderMode, ID3D11UnorderedAccessView* _value)
	{
		mShader[_ShaderMode]->mfxUAV[_Name]->SetUnorderedAccessView(_value);
	}

	// 스킨 행렬 저장
	void SetShaderMtxArray(SHADER_VAL_TYPE _ValueEnum, char* _Name, XMFLOAT4X4* _Mtx, int _MtxSize)
	{
		mShader[mShaderMode]->mfxMtx[_Name]->SetMatrixArray(reinterpret_cast<float*>(_Mtx), 0, _MtxSize);
	}

	// 스킨 행렬 저장
	void SetCommonShaderMtxArray(SHADER_VAL_TYPE _ValueEnum, char* _Name, XMFLOAT4X4* _Mtx, int _MtxSize)
	{
		mShader[e_ShaderDeferred]->mfxMtx[_Name]->SetMatrixArray(reinterpret_cast<float*>(_Mtx), 0, _MtxSize);
	}

	// '기본' 쉐이더 변수 업데이트
	void SetBasicShaderValue(InitMetaData* mNowModel, int _Num)
	{
		// 기본 정보 세팅
		XMMATRIX world             = XMLoadFloat4x4(&mNowModel->mObjData[_Num].mWdMtx);
		XMMATRIX worldInvTranspose = cMathHelper::InverseTranspose(world);
		XMMATRIX worldViewProj     = world * gCam.ViewProj();
		XMMATRIX WorldView		   = gCam.View() * world;

		// 카메라 값 세팅
		XMMATRIX view      = gCam.View();
		XMMATRIX proj      = gCam.Proj();
		XMMATRIX viewInvTranspose = cMathHelper::InverseTranspose(gCam.View());
		XMMATRIX projInvTranspose = cMathHelper::InverseTranspose(gCam.Proj());


		// viewInvTranspose
		// -> 이동은 무시 하고
		// -> 회전은 그데로 살리고
		// -> 스케일은
		// -> 다시 1로
		// -> 만들기 위해서
		// -> 하는거야

		// 모델 매트릭스 업데이트
		UpdateWorldMtx(world, worldInvTranspose, worldViewProj, view, viewInvTranspose, proj, projInvTranspose);

		// 카메라 위치 업데이트
		UpdateCamPos(gCam.GetPosition());
	}

	// '기본' UI 쉐이더 변수 업데이트
	void SetUIBasicShaderValue(InitMetaData* mNowModel, int _Num)
	{
		// 기본 정보 세팅
		XMMATRIX world             = XMLoadFloat4x4(&mNowModel->mObjData[_Num].mWdMtx);
		XMMATRIX worldInvTranspose = cMathHelper::InverseTranspose(world);
		XMMATRIX UIworldViewProj   = world * gCam.UIViewProj();
		XMMATRIX WorldView		   = gCam.View() * world;

		// 카메라 값 세팅
		XMMATRIX UIview      = gCam.UIView();
		XMMATRIX UIproj      = gCam.UIProj();
		XMMATRIX UIviewInvTranspose = cMathHelper::InverseTranspose(gCam.UIView());
		XMMATRIX UIprojInvTranspose = cMathHelper::InverseTranspose(gCam.UIProj());


		// viewInvTranspose
		// -> 이동은 무시 하고
		// -> 회전은 그데로 살리고
		// -> 스케일은
		// -> 다시 1로
		// -> 만들기 위해서
		// -> 하는거야

		// 모델 매트릭스 업데이트
		UpdateUIWorldMtx(world, worldInvTranspose, UIworldViewProj, UIview, UIviewInvTranspose, UIproj, UIprojInvTranspose);

		// 카메라 위치 업데이트
		UpdateCamPos(gCam.GetPosition());
	}

	// HDR 1번 패스 업데이트
	void HDR_1Set(D3DX11_TECHNIQUE_DESC& TechDesc)
	{
		// 1번패스
		SetShaderValue(e_ShaderValUAV, "gAverageLum1", mHDRManager->mDownScale1DUAV); // 결과

		// 쉐이더 정보 얻기 (어떤 쉐이더를 사용할지 결정하는 곳)
		GetDesc(&TechDesc, e_Basic);
	}

	// HDR 2번 패스 업데이트
	void HDR_2Set(D3DX11_TECHNIQUE_DESC& TechDesc)
	{
		// 2번패스
		SetShaderValue(e_ShaderValResource, "gAverageValues1D", mHDRManager->mDownScale1DSRV); // 입력
		SetShaderValue(e_ShaderValUAV     , "gAverageLum2"    , mHDRManager->mAvgLumUAV);      // 결과

		// 쉐이더 정보 얻기 (어떤 쉐이더를 사용할지 결정하는 곳)
		GetDesc(&TechDesc, e_Shadow);
	}

	// '기본' 쉐이더 변수 업데이트
	void SetBasicShaderValueIns(TECH_TYPE _TechType)
	{
		//-----------------------------------------------------------------------------------//
		// 계산 쉐이더 전용
		//-----------------------------------------------------------------------------------//
		if (mShaderMode == e_ShaderHDR_CS)
		{
			SetShaderValue(e_ShaderVal, "gTDownScaleCB"   , mHDRManager->mTDownScaleCB);
			SetShaderValue(e_ShaderVal, "gTFinalPassCB"   , mHDRManager->mTFinalPassCB);
		}
#ifdef POSTEFFECT_ON
		else if (mShaderMode == e_ShaderFinHDR)
		{
			SetShaderValue(e_ShaderVal, "gTDownScaleCB", mHDRManager->mTDownScaleCB);
			SetShaderValue(e_ShaderVal, "gTFinalPassCB", mHDRManager->mTFinalPassCB);

			// HDR 입력
			SetShaderValue(e_ShaderValResource, "gAvgLum", mHDRManager->mAvgLumSRV);
		}
#endif
		//-----------------------------------------------------------------------------------//
		// 일반 쉐이더 전용
		//-----------------------------------------------------------------------------------//
		else
		{
			// 기본 정보 세팅
			XMMATRIX world = XMMatrixIdentity();
			XMMATRIX worldInvTranspose = cMathHelper::InverseTranspose(world);

			// 카메라 값 세팅
			XMMATRIX view = gCam.View();
			XMMATRIX proj = gCam.Proj();
			XMMATRIX viewProj = gCam.ViewProj();
			XMMATRIX viewInvTranspose = cMathHelper::InverseTranspose(gCam.View());
			XMMATRIX projInvTranspose = cMathHelper::InverseTranspose(gCam.Proj());

			//// 이런식으로 접근 가능함, 모델 매트릭스에 ----------
			//XMFLOAT4X4 _world;
			//XMMATRIX modelScale  = XMMatrixScaling(0.5f, 0.5f, 0.5f);
			//XMMATRIX modelOffset = XMMatrixTranslation(0.0f, 1.0f, 0.0f);
			//XMStoreFloat4x4(&_world, XMMatrixMultiply(modelScale, modelOffset));
			////---------------------------------------------------

			// 모델 매트릭스 업데이트
			UpdateWorldMtxIns(world, worldInvTranspose, viewProj, view, viewInvTranspose, proj, projInvTranspose);

			// 카메라 위치 업데이트
			UpdateCamPos(gCam.GetPosition());

			if (_TechType == e_Basic)
			{
				// 큐브맵 갱신
				if (mShaderMode == e_ShaderSkyBox)
					SetShaderValue(e_ShaderValResource, "gSkyBox", mMapManager->GetCubeMap());
			}

			// 쉐도우맵 갱신
			SetShaderValue(e_ShaderValResource, "gShadowMap"	  , mShadowMap->mDepthMapSRV);
			SetShaderValue(e_ShaderValMtx     , "gShadowTransform", mShadowMap->mShadowTransform);
			SetShaderValue(e_ShaderValMtx     , "gLightViewProj"  , mShadowMap->mLightViewProj); 
		}

		//-----------------------------------------------------------------------------------//
		// 공통
		//-----------------------------------------------------------------------------------//
#ifdef POSTEFFECT_ON
		// HDR 텍스처 갱신
		SetShaderValue(e_ShaderValResource, "gHDRTex", mCoreStorage->mHDRSRV);
#endif
	}

	// '기본' 쉐이더 변수 업데이트
	void SetUIBasicShaderValueIns(TECH_TYPE _TechType)
	{
		//-----------------------------------------------------------------------------------//
		// 일반 쉐이더 전용
		//-----------------------------------------------------------------------------------//
		// 기본 정보 세팅
		XMMATRIX world = XMMatrixIdentity();
		XMMATRIX worldInvTranspose = cMathHelper::InverseTranspose(world);

		// 카메라 값 세팅
		XMMATRIX UIview             = gCam.UIView();
		XMMATRIX UIproj             = gCam.UIProj();
		XMMATRIX UIviewProj         = gCam.UIViewProj();
		XMMATRIX UIviewInvTranspose = cMathHelper::InverseTranspose(gCam.UIView());
		XMMATRIX UIprojInvTranspose = cMathHelper::InverseTranspose(gCam.UIProj());

		// 모델 매트릭스 업데이트
		UpdateUIWorldMtxIns(world, worldInvTranspose, UIviewProj, UIview, UIviewInvTranspose, UIproj, UIprojInvTranspose);
	}

	// '개별' 쉐이더 변수 업데이트
	void SetModelShaderValue(InitMetaData* mNowModel)
	{
		// 모델 매트릭스
		SetShaderValue(e_ShaderValMtx, "gTexTFMtx", mNowModel->mTexMtx);
		SetShaderValue(e_ShaderValMtx, "gLocTMMtx", mNowModel->mLocTMMtx);
		SetShaderValue(e_ShaderValMtx, "gWdTMMtx" , mNowModel->mWdTMMtx);

		// 모델 리소스
		SetShaderValue(e_ShaderValResource, "gDiffuseTex" , mNowModel->mDiffuseSRV);
		SetShaderValue(e_ShaderValResource, "gSpecularTex", mNowModel->mSpecularSRV);
		SetShaderValue(e_ShaderValResource, "gNormalTex"  , mNowModel->mNomalSRV);

		// 애니메이션 쉐이더 업데이트
		if (mNowModel->mShaderMode == e_ShaderPongTexAni)
		{
			// 스킨 텍스처
			SetShaderValue(e_ShaderValResource, "gIdleTex"  , mNowModel->mSkinTex[e_Idle  ]->mTexSRV);
			SetShaderValue(e_ShaderValResource, "gDamageTex", mNowModel->mSkinTex[e_Damage]->mTexSRV);
			SetShaderValue(e_ShaderValResource, "gRunTex"   , mNowModel->mSkinTex[e_Run   ]->mTexSRV);
			SetShaderValue(e_ShaderValResource, "gWalkTex"  , mNowModel->mSkinTex[e_Walk  ]->mTexSRV);
			SetShaderValue(e_ShaderValResource, "gDeathTex" , mNowModel->mSkinTex[e_Death ]->mTexSRV);
			SetShaderValue(e_ShaderValResource, "gAttackTex", mNowModel->mSkinTex[e_Attack]->mTexSRV);

			// 모델 스킨 텍스처
			SetShaderValue(e_ShaderValResource, "gIdleModelTex"  , mNowModel->mSkinModelTex[e_Idle  ]->mTexSRV);
			SetShaderValue(e_ShaderValResource, "gDamageModelTex", mNowModel->mSkinModelTex[e_Damage]->mTexSRV);
			SetShaderValue(e_ShaderValResource, "gRunModelTex"   , mNowModel->mSkinModelTex[e_Run   ]->mTexSRV);
			SetShaderValue(e_ShaderValResource, "gWalkModelTex"  , mNowModel->mSkinModelTex[e_Walk  ]->mTexSRV);
			SetShaderValue(e_ShaderValResource, "gDeathModelTex" , mNowModel->mSkinModelTex[e_Death ]->mTexSRV);
			SetShaderValue(e_ShaderValResource, "gAttackModelTex", mNowModel->mSkinModelTex[e_Attack]->mTexSRV);


			// 테스트 (스킨 잘 되나)
			//vector<XMFLOAT4X4>& _SkinMtx = cAniManager::GetInstance()->mData[mNowModel->mCreateName]["Run"]->GetSkinStorage(12); // 애니키 번호
			//SetShaderMtxArray(e_ShaderValMtxArray, "gBoneTransforms", &_SkinMtx[0], _SkinMtx.size());
		}
		// 맵 업데이트
		else if (mNowModel->mShaderMode == e_ShaderPongTexMap)
		{
			// 기본 맵
			SetShaderValue(e_ShaderValResource, "gMapTex"      , mNowModel->mSkinModelTex[0]->mTexSRV);
			SetShaderValue(e_ShaderValResource, "gHeightMapTex", mNowModel->mSkinModelTex[1]->mTexSRV);
		}

		// 깊이 버퍼 렌더링용
		float CamNear = gCam.GetNearZ();
		float CamFar  = gCam.GetFarZ();
		SetShaderValue(e_ShaderVal, "gNear", CamNear);
		SetShaderValue(e_ShaderVal, "gFar" , CamFar);
	}

	// 플레이어 손전등 업데이트
	void SetPlyerSpotLight()
	{
		mLightManager->mPlayerSpotLight.Position  = gCam.GetThirdPosition();
		mLightManager->mPlayerSpotLight.Direction = gCam.GetLook();
		SetShaderValue(e_ShaderVal, "gSpotLight", mLightManager->mPlayerSpotLight);
	}

	int GetIAStride() const
	{
		// 버퍼 구조는 어떻게 되있는가?
		switch (mShaderMode)
		{
		case e_ShaderColor:
			return sizeof(VertexPC);

		case e_ShaderLight:
			return sizeof(VertexPN);

		case e_ShaderFinHDR:
		case e_ShaderDeferred:
			return sizeof(VertexG);

		case e_ShaderPongTexAni:
			return sizeof(VertexPNTAni);

		case e_ShaderPongTexMap:
			return sizeof(VertexPNTMap);

		case e_ShaderSkyBox:
			return sizeof(VertexSkyBox);

		default:
		case e_ShaderPongTex:
		case e_ShaderCartoonTex:
			return sizeof(VertexPNT);
		}
	}

	// 쉐이더 모드, 렌더링 모델 셋팅
	void SetModelShaderMode(InitMetaData* mNowModel, SHADER_TYPE _ShaderMode)
	{
		mShaderMode = _ShaderMode;

		// 각종 쉐이더 변수 업데이트
		SetModelShaderValue(mNowModel);
	}

	// 현재 레이어 전송
	ID3D11InputLayout* GetInputLayout(TECH_TYPE _TechType)
	{
		return mShader[mShaderMode]->mInputLayout[(int)_TechType];
	}

	// 일반 쉐이더 얻기
	void GetDesc(D3DX11_TECHNIQUE_DESC* techDesc, TECH_TYPE _TechType)
	{
		mShader[mShaderMode]->mTech[int(_TechType)]->GetDesc(techDesc);
	}

	void GetPassByIndex(int idx, TECH_TYPE _TechType)
	{
		mShader[mShaderMode]->mTech[int(_TechType)]->GetPassByIndex(idx)->Apply(0, mCoreStorage->md3dImmediateContext);
	}

	// 라이트 매니저 얻기
	cLightManager* GetLightManager()
	{
		return mLightManager;
	}

	// G버퍼 셋
	void SetGbuffer()
	{
		EffectStorage* tEffectStorage = mShader[e_ShaderDeferred];
		mShaderMode = e_ShaderDeferred;

		// G버퍼 Get
		GetGBufferShaderValue(tEffectStorage, "gGDepthTex");
		GetGBufferShaderValue(tEffectStorage, "gGDiffuseTex");
		GetGBufferShaderValue(tEffectStorage, "gGPositionTex");
		GetGBufferShaderValue(tEffectStorage, "gGSpecularTex");
		GetGBufferShaderValue(tEffectStorage, "gGNormalTex");
		GetGBufferShaderValue(tEffectStorage, "gGShadowTex");

		// G 버퍼 Set
		SetShaderValue(e_ShaderValResource, "gGDepthTex"   , mCoreStorage->mDepthSRV);
		SetShaderValue(e_ShaderValResource, "gGDiffuseTex" , mCoreStorage->mColorSRV);
		SetShaderValue(e_ShaderValResource, "gGPositionTex", mCoreStorage->mPositionSRV);
		SetShaderValue(e_ShaderValResource, "gGSpecularTex", mCoreStorage->mSpecularSRV);
		SetShaderValue(e_ShaderValResource, "gGNormalTex"  , mCoreStorage->mNomalSRV);
		SetShaderValue(e_ShaderValResource, "gGShadowTex"  , mCoreStorage->mShadowSRV);
	}

	// 공통 쉐이더
	void SetCommonShaderValue()
	{
		// 윈도우 크기
		SetCommonShaderValue(e_ShaderVal, "gWinWidth", mCoreStorage->mClientWidth);

		// 나머지 쉐이더 변수 설정
		SetCommonShaderValue(e_ShaderVal, "gDirLight"     , mLightManager->mSunDirLight);
		SetCommonShaderValue(e_ShaderVal, "gPointLight"   , mLightManager->mPointLight);
		SetCommonShaderValue(e_ShaderVal, "gSpotLight"    , mLightManager->mPlayerSpotLight);
	}

	// G 버퍼 클리어
	void ClearGbuffer()
	{
		EffectStorage* tEffectStorage = mShader[e_ShaderDeferred];
		mShaderMode = e_ShaderDeferred;

		// G버퍼
		ClearGBufferShaderValue(tEffectStorage, "gGDepthTex"   );
		ClearGBufferShaderValue(tEffectStorage, "gGDiffuseTex" );
		ClearGBufferShaderValue(tEffectStorage, "gGPositionTex");
		ClearGBufferShaderValue(tEffectStorage, "gGSpecularTex");
		ClearGBufferShaderValue(tEffectStorage, "gGNormalTex"  );
		ClearGBufferShaderValue(tEffectStorage, "gGShadowTex"  );
	}

protected:
	// 쉐이더 변수 Set 하기
	template <class T>
	void SetShaderValue(SHADER_VAL_TYPE _ValueEnum, char* _Name, T& _value)
	{
		switch (_ValueEnum)
		{
			// 매트릭스
		case e_ShaderValMtx:
			mShader[mShaderMode]->mfxMtx[_Name]->SetMatrix(reinterpret_cast<float*>(&_value));
			break;

			// 벡터
		case e_ShaderValVtx:
			mShader[mShaderMode]->mfxVtx[_Name]->SetFloatVector(reinterpret_cast<float*>(&_value));
			break;

			// 일반 변수
		case e_ShaderVal:
			mShader[mShaderMode]->mfxValue[_Name]->SetRawValue(&_value, 0, sizeof(_value));
			break;
		}
	}

	// 쉐이더 변수 Set 하기
	template <class T>
	void SetCommonShaderValue(SHADER_VAL_TYPE _ValueEnum, char* _Name, T& _value)
	{
		switch (_ValueEnum)
		{
			// 매트릭스
		case e_ShaderValMtx:
			mShader[e_ShaderDeferred]->mfxMtx[_Name]->SetMatrix(reinterpret_cast<float*>(&_value));
			break;

			// 벡터
		case e_ShaderValVtx:
			mShader[e_ShaderDeferred]->mfxVtx[_Name]->SetFloatVector(reinterpret_cast<float*>(&_value));
			break;

			// 일반 변수
		case e_ShaderVal:
			mShader[e_ShaderDeferred]->mfxValue[_Name]->SetRawValue(&_value, 0, sizeof(_value));
			break;
		}
	}

	// 쉐이더 변수 Set 하기
	template <class T>
	void SetCommonShaderValue(SHADER_VAL_TYPE _ValueEnum, char* _Name, T& _value, int _size)
	{
		switch (_ValueEnum)
		{
			// 일반 변수
		case e_ShaderVal:
			mShader[e_ShaderDeferred]->mfxValue[_Name]->SetRawValue(&_value, 0, sizeof(_value) * _size);
			break;
		}
	}

	// 쉐이더 변수 Set 하기
	template <class T>
	void SetShaderValue(SHADER_VAL_TYPE _ValueEnum, char* _Name, T& _value, int _size)
	{
		switch (_ValueEnum)
		{
		// 일반 변수
		case e_ShaderVal:
			mShader[mShaderMode]->mfxValue[_Name]->SetRawValue(&_value, 0, sizeof(_value) * _size);
			break;
		}
	}

	// 기본 매트릭스 업데이트
	template <class T>
	void UpdateWorldMtx(T& world, T& worldInvTranspose, T& worldViewProj, T& view, T& viewInvTranspose, T& proj, T& ProjInvTranspose)
	{
		SetShaderValue(e_ShaderValMtx, "gWorld"			       , world);
		SetShaderValue(e_ShaderValMtx, "gWorldInvTranspose"    , worldInvTranspose);
		SetShaderValue(e_ShaderValMtx, "gWorldViewProj"        , worldViewProj);
		SetShaderValue(e_ShaderValMtx, "gView"			       , view);
		SetShaderValue(e_ShaderValMtx, "gViewInvTranspose"     , viewInvTranspose);
		SetShaderValue(e_ShaderValMtx, "gProj"			       , proj);
		SetShaderValue(e_ShaderValMtx, "gProjInvTranspose"     , ProjInvTranspose);
	}

	template <class T>
	void UpdateWorldMtxIns(T& world, T& worldInvTranspose, T& viewProj, T& view, T& viewInvTranspose, T& proj, T& ProjInvTranspose)
	{
		SetShaderValue(e_ShaderValMtx, "gWorld"			       , world);
		SetShaderValue(e_ShaderValMtx, "gWorldInvTranspose"    , worldInvTranspose);
		SetShaderValue(e_ShaderValMtx, "gViewProj"		       , viewProj);
		SetShaderValue(e_ShaderValMtx, "gView"			       , view);
		SetShaderValue(e_ShaderValMtx, "gViewInvTranspose"     , viewInvTranspose);
		SetShaderValue(e_ShaderValMtx, "gProj"			       , proj);
		SetShaderValue(e_ShaderValMtx, "gProjInvTranspose"     , ProjInvTranspose);
	}

	// 기본 매트릭스 업데이트
	template <class T>
	void UpdateUIWorldMtx(T& world, T& worldInvTranspose, T& oview, T& oviewInvTranspose, T& oproj, T& oProjInvTranspose, T& oworldviewProj)
	{
		SetShaderValue(e_ShaderValMtx, "gWorld"					, world);
		SetShaderValue(e_ShaderValMtx, "gWorldInvTranspose"		, worldInvTranspose);
		SetShaderValue(e_ShaderValMtx, "gOrthoView"				, oview);
		SetShaderValue(e_ShaderValMtx, "gOrthoViewInvTranspose" , oviewInvTranspose);
		SetShaderValue(e_ShaderValMtx, "gOrthoProj"				, oproj);
		SetShaderValue(e_ShaderValMtx, "gOrthoProjInvTranspose" , oProjInvTranspose);
		SetShaderValue(e_ShaderValMtx, "gOrthoWorldViewProj"	, oworldviewProj);
	}

	template <class T>
	void UpdateUIWorldMtxIns(T& world, T& worldInvTranspose, T& oview, T& oviewInvTranspose, T& oproj, T& oProjInvTranspose, T& oviewproj)
	{
		SetShaderValue(e_ShaderValMtx, "gWorld"					, world);
		SetShaderValue(e_ShaderValMtx, "gWorldInvTranspose"	    , worldInvTranspose);
		SetShaderValue(e_ShaderValMtx, "gOrthoView"			    , oview);
		SetShaderValue(e_ShaderValMtx, "gOrthoViewInvTranspose" , oviewInvTranspose);
		SetShaderValue(e_ShaderValMtx, "gOrthoProj"				, oproj);
		SetShaderValue(e_ShaderValMtx, "gOrthoProjInvTranspose" , oProjInvTranspose);
		SetShaderValue(e_ShaderValMtx, "gOrthoViewProj"		    , oviewproj);
	}

	template <class T>
	void UpdateCamPos(T& _Pos)
	{
		SetShaderValue(e_ShaderValVtx, "gEyePosW", _Pos);
	}

	template <class T>
	void UpdateMat(T& Mat)
	{
		mShader[mShaderMode]->mfxValue["gMaterial"]->SetRawValue(&Mat, 0, sizeof(Mat));
	}

private:
	// FX 초기화 & 빌드
	void InitFX()
	{
		// 색상
		BuildFX(e_ShaderColor, L"PC.fx", "ColorTech", "ColorTech");

		// 퐁 쉐이더
		BuildFX(e_ShaderLight, L"PL.fx", "LightTech", "LightTech");

		// 퐁쉐이더, 텍스처
		BuildFX(e_ShaderPongTex, L"PNT.fx", "PongTex", "SDPongTex");

		// 카툰쉐이더, 텍스처
		BuildFX(e_ShaderCartoonTex, L"PNT.fx", "CartoonTex", "SDCartoonTex");

		// 퐁쉐이더, 텍스처, 애니
		BuildFX(e_ShaderPongTexAni, L"PNT_ANI.fx", "PongTexAni", "SDPongTexAni");

		// 퐁쉐이더, 텍스처, 맵
		BuildFX(e_ShaderPongTexMap, L"PNT_MAP.fx", "PongTexMap", "SDPongTexMap");

		// 스카이박스
		BuildFX(e_ShaderSkyBox, L"SKY_BOX.fx", "SkyBoxTech", "SkyBoxTech");

#ifdef POSTEFFECT_ON
		// HDR 계산 쉐이더
		BuildFX(e_ShaderHDR_CS, L"HDR_CS.fx", "HDRDownScale", "CombineHDR");

		// 디퍼드 렌더링
		BuildFX(e_ShaderFinHDR, L"HDR.fx", "FinHDR", "FinHDR");
#endif
		// 디퍼드 렌더링
		BuildFX(e_ShaderDeferred, L"Deferred.fx", "Deferred", "Deferred");
	}

	void BuildFX(SHADER_TYPE _ShaderMode, const wchar_t* _FxName, char* _TechniqueName, char* _ShadowTechniqueName)
	{
		// 쉐이더 셋
		mShaderMode = _ShaderMode;

		// 저장공간
		EffectStorage* tEffectStorage = new EffectStorage();

		// FX 파일 로드
		LoadFX(tEffectStorage, _FxName);

		// 테크 셋
		SetTech(tEffectStorage, e_Basic , _TechniqueName);
		SetTech(tEffectStorage, e_Shadow, _ShadowTechniqueName);
		
		// 기본 재질 설정
		// 흰색 (머테리얼 매니저를 불러올 수 없어서, 일단 이렇게 처리)
		Material tMat;
		tMat.Ambient  = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		tMat.Diffuse  = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		tMat.Specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

		// 기본 재질 적용
		tEffectStorage->mfxValue["gMaterial"]->SetRawValue(&tMat, 0, sizeof(tMat));

		// 저장
		mShader[mShaderMode] = tEffectStorage;
	}

	// 테크 셋
	void SetTech(EffectStorage* tEffectStorage, TECH_TYPE _TechType, char* _TechniqueName)
	{
		// 테크닉 사용할 수 있게끔 값 얻기
		GetTech(tEffectStorage, _TechType, _TechniqueName);
		
		// 상수버퍼 수정할 수 있게끔 값 얻기 (Get)
		GetShaderValue(tEffectStorage, "gWorld"			         , e_ShaderValMtx);      // 매트릭스
		GetShaderValue(tEffectStorage, "gWorldInvTranspose"      , e_ShaderValMtx);      
		GetShaderValue(tEffectStorage, "gView"		             , e_ShaderValMtx);      
		GetShaderValue(tEffectStorage, "gViewInvTranspose"       , e_ShaderValMtx);      
		GetShaderValue(tEffectStorage, "gProj"			         , e_ShaderValMtx);      
		GetShaderValue(tEffectStorage, "gProjInvTranspose"       , e_ShaderValMtx);      
		GetShaderValue(tEffectStorage, "gWorldViewProj"	         , e_ShaderValMtx);      
		GetShaderValue(tEffectStorage, "gViewProj"		         , e_ShaderValMtx);   
		GetShaderValue(tEffectStorage, "gOrthoView"		         , e_ShaderValMtx);      
		GetShaderValue(tEffectStorage, "gOrthoViewInvTranspose"  , e_ShaderValMtx);      
		GetShaderValue(tEffectStorage, "gOrthoProj"			     , e_ShaderValMtx);      
		GetShaderValue(tEffectStorage, "gOrthoProjInvTranspose"  , e_ShaderValMtx);      
		GetShaderValue(tEffectStorage, "gOrthoWorldViewProj"	 , e_ShaderValMtx);      
		GetShaderValue(tEffectStorage, "gOrthoViewProj"		     , e_ShaderValMtx);  
		GetShaderValue(tEffectStorage, "gTexTFMtx"			     , e_ShaderValMtx);      
		GetShaderValue(tEffectStorage, "gLocTMMtx"		         , e_ShaderValMtx);      
		GetShaderValue(tEffectStorage, "gWdTMMtx"                , e_ShaderValMtx);      
		GetShaderValue(tEffectStorage, "gShadowTransform"        , e_ShaderValMtx);      
		GetShaderValue(tEffectStorage, "gLightViewProj"          , e_ShaderValMtx);  
																			       
		GetShaderValue(tEffectStorage, "gEyePosW"		   , e_ShaderValVtx);      // 버텍스
																		
		GetShaderValue(tEffectStorage, "gWinWidth"		   , e_ShaderVal);	       // 구조체
		GetShaderValue(tEffectStorage, "gDirLight"		   , e_ShaderVal);	       
		GetShaderValue(tEffectStorage, "gPointLight"	   , e_ShaderVal);
		GetShaderValue(tEffectStorage, "gSpotLight"		   , e_ShaderVal);
		GetShaderValue(tEffectStorage, "gMaterial"		   , e_ShaderVal);
		GetShaderValue(tEffectStorage, "gFar"			   , e_ShaderVal);
		GetShaderValue(tEffectStorage, "gNear"             , e_ShaderVal);
		GetShaderValue(tEffectStorage, "gTDownScaleCB"     , e_ShaderVal);
		GetShaderValue(tEffectStorage, "gTFinalPassCB"     , e_ShaderVal);
		GetShaderValue(tEffectStorage, "gDiffuseTex"	   , e_ShaderValResource); // 리소스
		GetShaderValue(tEffectStorage, "gSpecularTex"	   , e_ShaderValResource);
		GetShaderValue(tEffectStorage, "gNormalTex"		   , e_ShaderValResource);
		GetShaderValue(tEffectStorage, "gHDRTex"           , e_ShaderValResource);
		GetShaderValue(tEffectStorage, "gAvgLum"           , e_ShaderValResource);
		GetShaderValue(tEffectStorage, "gAverageValues1D"  , e_ShaderValResource);

		GetShaderValue(tEffectStorage, "gAverageLum1"       , e_ShaderValUAV);     // UAV
		GetShaderValue(tEffectStorage, "gAverageLum2"       , e_ShaderValUAV);    

		// 테스트
		//GetShaderValue(tEffectStorage, "gBoneTransforms"   , e_ShaderValMtxArray);
		
		// 애니 텍스처
		GetShaderValue(tEffectStorage, "gIdleTex"		   , e_ShaderValResource);
		GetShaderValue(tEffectStorage, "gDamageTex"		   , e_ShaderValResource);
		GetShaderValue(tEffectStorage, "gRunTex"		   , e_ShaderValResource);
		GetShaderValue(tEffectStorage, "gWalkTex"		   , e_ShaderValResource);
		GetShaderValue(tEffectStorage, "gDeathTex"		   , e_ShaderValResource);
		GetShaderValue(tEffectStorage, "gAttackTex"	       , e_ShaderValResource);

		// 애니 모델 텍스처
		GetShaderValue(tEffectStorage, "gIdleModelTex"	   , e_ShaderValResource);
		GetShaderValue(tEffectStorage, "gDamageModelTex"   , e_ShaderValResource);
		GetShaderValue(tEffectStorage, "gRunModelTex"	   , e_ShaderValResource);
		GetShaderValue(tEffectStorage, "gWalkModelTex"	   , e_ShaderValResource);
		GetShaderValue(tEffectStorage, "gDeathModelTex"	   , e_ShaderValResource);
		GetShaderValue(tEffectStorage, "gAttackModelTex"   , e_ShaderValResource);

		// 맵 모델 텍스처
		GetShaderValue(tEffectStorage, "gMapTex"		   , e_ShaderValResource);
		GetShaderValue(tEffectStorage, "gHeightMapTex"     , e_ShaderValResource);

		// 큐브맵 텍스처
		GetShaderValue(tEffectStorage, "gSkyBox"           , e_ShaderValResource);

		// 쉐도우맵 텍스처
		GetShaderValue(tEffectStorage, "gShadowMap"        , e_ShaderValResource);

		// IA 생성
		CreateIA(tEffectStorage, _TechType);
	}

	// 입력 조립기 초기화 (FVF)
	void CreateIA(EffectStorage* tEffectStorage, TECH_TYPE& _TechType)
	{
		// 레이아웃 초기화
		if (mShaderMode == e_ShaderColor)
		{
			// Create the vertex input layout.
			D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT   , 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA  , 0 },
				{ "COLOR"   , 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA  , 0 },
				{ "PEDDING" , 0, DXGI_FORMAT_R32_FLOAT		   , 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA  , 0 },
				{ "WORLD"   , 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0                           , D3D11_INPUT_PER_INSTANCE_DATA, 1 },
				{ "WORLD"   , 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16                          , D3D11_INPUT_PER_INSTANCE_DATA, 1 },
				{ "WORLD"   , 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32                          , D3D11_INPUT_PER_INSTANCE_DATA, 1 },
				{ "WORLD"   , 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48                          , D3D11_INPUT_PER_INSTANCE_DATA, 1 }
			};

			// Create the input layout
			NEW_IA(tEffectStorage, _TechType, vertexDesc, _countof(vertexDesc));
		}
		else if (mShaderMode == e_ShaderLight)
		{
			D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT   , 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA  , 0 },
				{ "NORMAL"  , 0, DXGI_FORMAT_R32G32B32_FLOAT   , 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA  , 0 },
				{ "PEDDING" , 0, DXGI_FORMAT_R32G32_FLOAT      , 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA  , 0 },
				{ "WORLD"   , 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0                           , D3D11_INPUT_PER_INSTANCE_DATA, 1 },
				{ "WORLD"   , 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16                          , D3D11_INPUT_PER_INSTANCE_DATA, 1 },
				{ "WORLD"   , 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32                          , D3D11_INPUT_PER_INSTANCE_DATA, 1 },
				{ "WORLD"   , 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48                          , D3D11_INPUT_PER_INSTANCE_DATA, 1 }	
			};

			// Create the input layout
			NEW_IA(tEffectStorage, _TechType, vertexDesc, _countof(vertexDesc));
		}
		else if (mShaderMode == e_ShaderPongTex || mShaderMode == e_ShaderCartoonTex)
		{
			D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT   , 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA  , 0 },
				{ "NORMAL"  , 0, DXGI_FORMAT_R32G32B32_FLOAT   , 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA  , 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT      , 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA  , 0 },
				{ "TANGENT" , 0, DXGI_FORMAT_R32G32B32_FLOAT   , 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA  , 0 },
				{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT   , 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA  , 0 },
				{ "PEDDING" , 0, DXGI_FORMAT_R32G32_FLOAT	   , 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA  , 0 },
				{ "WORLD"   , 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0                           , D3D11_INPUT_PER_INSTANCE_DATA, 1 },
				{ "WORLD"   , 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16                          , D3D11_INPUT_PER_INSTANCE_DATA, 1 },
				{ "WORLD"   , 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32                          , D3D11_INPUT_PER_INSTANCE_DATA, 1 },
				{ "WORLD"   , 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48                          , D3D11_INPUT_PER_INSTANCE_DATA, 1 }		
			};
			// Create the input layout
			NEW_IA(tEffectStorage, _TechType, vertexDesc, _countof(vertexDesc));
		}
		else if (mShaderMode == e_ShaderDeferred || mShaderMode == e_ShaderFinHDR)
		{
			D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT   , 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA  , 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT      , 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA  , 0 },
				{ "PEDDING" , 0, DXGI_FORMAT_R32G32B32_FLOAT   , 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA  , 0 },
				{ "WORLD"   , 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0                           , D3D11_INPUT_PER_INSTANCE_DATA, 1 },
				{ "WORLD"   , 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16                          , D3D11_INPUT_PER_INSTANCE_DATA, 1 },
				{ "WORLD"   , 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32                          , D3D11_INPUT_PER_INSTANCE_DATA, 1 },
				{ "WORLD"   , 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48                          , D3D11_INPUT_PER_INSTANCE_DATA, 1 }		
			};
			// Create the input layout
			NEW_IA(tEffectStorage, _TechType, vertexDesc, _countof(vertexDesc));
		}
		else if (mShaderMode == e_ShaderPongTexAni)
		{
			D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
			{
				{ "TEXCOORD"   , 0, DXGI_FORMAT_R32G32_FLOAT      , 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA  , 0 },
				{ "WEIGHTS"    , 0, DXGI_FORMAT_R32G32B32_FLOAT   , 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA  , 0 },
				{ "VTXINFO"    , 0, DXGI_FORMAT_R32G32B32_FLOAT   , 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA  , 0 },
				{ "BONEINDICES", 0, DXGI_FORMAT_R32G32B32A32_UINT , 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA  , 0 },
				{ "WORLD"      , 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0                           , D3D11_INPUT_PER_INSTANCE_DATA, 1 },
				{ "WORLD"      , 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16                          , D3D11_INPUT_PER_INSTANCE_DATA, 1 },
				{ "WORLD"      , 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32                          , D3D11_INPUT_PER_INSTANCE_DATA, 1 },
				{ "WORLD"      , 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48                          , D3D11_INPUT_PER_INSTANCE_DATA, 1 },
				{ "ANIDATA"    , 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 64                          , D3D11_INPUT_PER_INSTANCE_DATA, 1 }	
				// 인스턴스데이터에 애니 텍스처 번호, 프레임번호 넘겨야함
			};
			// Create the input layout
			NEW_IA(tEffectStorage, _TechType, vertexDesc, _countof(vertexDesc));
		}
		else if (mShaderMode == e_ShaderPongTexMap)
		{
			D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
			{
				{ "TEXCOORD"   , 0, DXGI_FORMAT_R32G32_FLOAT      , 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA  , 0 },
				{ "VTXINFO"    , 0, DXGI_FORMAT_R32G32_FLOAT      , 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA  , 0 },
				{ "WORLD"      , 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0                           , D3D11_INPUT_PER_INSTANCE_DATA, 1 },
				{ "WORLD"      , 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16                          , D3D11_INPUT_PER_INSTANCE_DATA, 1 },
				{ "WORLD"      , 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32                          , D3D11_INPUT_PER_INSTANCE_DATA, 1 },
				{ "WORLD"      , 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48                          , D3D11_INPUT_PER_INSTANCE_DATA, 1 },
				{ "TEXDATA"    , 0, DXGI_FORMAT_R32G32B32_FLOAT   , 1, 64                          , D3D11_INPUT_PER_INSTANCE_DATA, 1 }	
				// 인스턴스데이터에 애니 텍스처 번호, 프레임번호 넘겨야함
			};
			// Create the input layout
			NEW_IA(tEffectStorage, _TechType, vertexDesc, _countof(vertexDesc));
		}
		else if (mShaderMode == e_ShaderSkyBox)
		{
			D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
			{
				{ "POSITION"   , 0, DXGI_FORMAT_R32G32B32_FLOAT   , 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA  , 0 },
				{ "PEDDING"    , 0, DXGI_FORMAT_R32_FLOAT		  , 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA  , 0 },
				{ "WORLD"      , 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0                           , D3D11_INPUT_PER_INSTANCE_DATA, 1 },
				{ "WORLD"      , 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16                          , D3D11_INPUT_PER_INSTANCE_DATA, 1 },
				{ "WORLD"      , 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32                          , D3D11_INPUT_PER_INSTANCE_DATA, 1 },
				{ "WORLD"      , 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48                          , D3D11_INPUT_PER_INSTANCE_DATA, 1 }
				// 인스턴스데이터에 애니 텍스처 번호, 프레임번호 넘겨야함
			};
			// Create the input layout
			NEW_IA(tEffectStorage, _TechType, vertexDesc, _countof(vertexDesc));
		}
	}

	// IA 생성
	void NEW_IA(EffectStorage* tEffectStorage, TECH_TYPE& _TechType, D3D11_INPUT_ELEMENT_DESC* vertexDesc, int _FVFNum)
	{
		// Create the input layout
		static D3DX11_PASS_DESC passDesc;

		// 쉐이더 정보 가져오기
		tEffectStorage->mTech[(int)_TechType]->GetPassByIndex(0)->GetDesc(&passDesc);

		// IA 생성
		HR(cCoreStorage::GetInstance()->md3dDevice->CreateInputLayout(
			vertexDesc,
			_FVFNum,
			passDesc.pIAInputSignature,
			passDesc.IAInputSignatureSize,
			&tEffectStorage->mInputLayout[(int)_TechType]));
	}

	// FX 파일 로드
	void LoadFX(EffectStorage* tEffectStorage, const wchar_t* _Name)
	{
		DWORD shaderFlags = 0;
#if defined( DEBUG ) || defined( _DEBUG )
		shaderFlags |= D3D10_SHADER_DEBUG;
		shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif

		ID3D10Blob* compiledShader = 0;
		ID3D10Blob* compilationMsgs = 0;

		// 라이트
		HRESULT hr = D3DCompileFromFile(_Name, 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, 0, "fx_5_0", shaderFlags,
			0, &compiledShader, &compilationMsgs);

		// compilationMsgs는 오류나 경고를 저장할 수 있습니다.
		if (compilationMsgs != 0)
		{
			//MessageBoxA(0, (char*)compilationMsgs->GetBufferPointer(), 0, 0);
			ReleaseCOM(compilationMsgs);
		}

		// compilationMsg가없는 경우에도 다른 오류가 없는지 확인하십시오.
		if (FAILED(hr))
		{
			MessageBox(NULL, NULL, L"D3DX11CompileFromFile", MB_OK);
		}

		// 컴파일
		HR(D3DX11CreateEffectFromMemory(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(),
			0, cCoreStorage::GetInstance()->md3dDevice, &tEffectStorage->mFX));

		// Done with compiled shader.
		ReleaseCOM(compiledShader);
	}

	// 쉐이더 변수 참조값 얻기 ( 초기화 )
	void GetShaderValue(EffectStorage* tEffectStorage, char* _Name, int _ValueEnum)
	{
		switch (_ValueEnum)
		{
			// 매트릭스
		case e_ShaderValMtxArray:
		case e_ShaderValMtx:
			tEffectStorage->mfxMtx[_Name] = tEffectStorage->mFX->GetVariableByName(_Name)->AsMatrix();
			break;

			// 벡터
		case e_ShaderValVtx:
			tEffectStorage->mfxVtx[_Name] = tEffectStorage->mFX->GetVariableByName(_Name)->AsVector();
			break;

			// 일반 변수
		case e_ShaderVal:
			tEffectStorage->mfxValue[_Name] = tEffectStorage->mFX->GetVariableByName(_Name);
			break;

			// 리소스
		case e_ShaderValResource:
			tEffectStorage->mfxResource[_Name] = tEffectStorage->mFX->GetVariableByName(_Name)->AsShaderResource();
			break;

			// UAV
		case e_ShaderValUAV:
			tEffectStorage->mfxUAV[_Name] = tEffectStorage->mFX->GetVariableByName(_Name)->AsUnorderedAccessView();
			break;
		}
	}

	// 쉐이더 변수 참조값 얻기 ( 초기화 )
	void GetGBufferShaderValue(EffectStorage* tEffectStorage, char* _Name)
	{
		tEffectStorage->mfxResource[_Name] = tEffectStorage->mFX->GetVariableByName(_Name)->AsShaderResource();
	}

	// 쉐이더 변수 참조값 얻기 ( 초기화 )
	void ClearGBufferShaderValue(EffectStorage* tEffectStorage, char* _Name)
	{
		tEffectStorage->mfxResource[_Name]->AsShaderResource()->SetResource(NULL);
	}

	// 태크닉 초기화
	void GetTech(EffectStorage* tEffectStorage, TECH_TYPE _TechType, char* _Name)
	{
		tEffectStorage->mTech[(int)_TechType] = tEffectStorage->mFX->GetTechniqueByName(_Name);
	}

};
