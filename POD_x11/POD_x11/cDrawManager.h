#pragma once
extern cCam gCam;

class cDrawManager : public cSingleton<cDrawManager>
{
	// 필요한거
	cCoreStorage*     mCoreStorage     = cCoreStorage    ::GetInstance();
	cShaderManager*   mShaderManager   = cShaderManager  ::GetInstance();
	cMaterialManager* mMaterialManager = cMaterialManager::GetInstance();
	cModelManager*    mModelManager    = cModelManager   ::GetInstance();
	cShadowMap*       mShadowMap       = cShadowMap      ::GetInstance();

	// 어떻게 그릴 것인가
	ID3D11RasterizerState* mWireframeRS  = nullptr;
	ID3D11RasterizerState* mSolidframeRS = nullptr;

public:
	// 모드 설정
	bool mSolidDraw;

	//// 깊이, 스텐실 테스트 설정
	//ID3D11DepthStencilState* m_pNoDepthWriteLessStencilMaskState = nullptr;
	//ID3D11DepthStencilState* mDepthStencilState = nullptr;
public:
	void Init()
	{
		// 레스터라이즈 방법
		InitRes();
		SetRes(e_Solid);
		mSolidDraw = true;

		// 각종 쉐이더 변수 업데이트
		for (map<string, InitMetaData*>::iterator itor = mModelManager->mAllModelData.begin(); itor != mModelManager->mAllModelData.end(); ++itor)
		{
			mShaderManager->SetModelShaderValue(itor->second);
		}

		//// 깊이 버퍼 1
		//D3D11_DEPTH_STENCIL_DESC descDepth;
		//ZeroMemory(&descDepth, sizeof(descDepth));
		//
		//descDepth.DepthEnable                           = TRUE;
		//descDepth.DepthWriteMask                        = D3D11_DEPTH_WRITE_MASK_ZERO;
		//descDepth.DepthFunc                             = D3D11_COMPARISON_LESS;
		//descDepth.StencilEnable                         = TRUE;
		//descDepth.StencilReadMask                       = D3D11_DEFAULT_STENCIL_READ_MASK;
		//descDepth.StencilWriteMask                      = D3D11_DEFAULT_STENCIL_WRITE_MASK;
		//const D3D11_DEPTH_STENCILOP_DESC noSkyStencilOp = { D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_ALWAYS };
		//descDepth.FrontFace                             = noSkyStencilOp;
		//descDepth.BackFace                              = noSkyStencilOp;
		//
		//mCoreStorage->md3dDevice->CreateDepthStencilState(&descDepth, &m_pNoDepthWriteLessStencilMaskState);
		//
		//// 깊이 버퍼 2
		//ZeroMemory(&descDepth, sizeof(descDepth));
		//
		//descDepth.DepthEnable                          = TRUE;
		//descDepth.DepthWriteMask                       = D3D11_DEPTH_WRITE_MASK_ALL;
		//descDepth.DepthFunc                            = D3D11_COMPARISON_LESS;
		//descDepth.StencilEnable                        = TRUE;
		//descDepth.StencilReadMask                      = D3D11_DEFAULT_STENCIL_READ_MASK;
		//descDepth.StencilWriteMask                     = D3D11_DEFAULT_STENCIL_WRITE_MASK;
		//const D3D11_DEPTH_STENCILOP_DESC stencilMarkOp = { D3D11_STENCIL_OP_REPLACE, D3D11_STENCIL_OP_REPLACE, D3D11_STENCIL_OP_REPLACE, D3D11_COMPARISON_ALWAYS };
		//descDepth.FrontFace                            = stencilMarkOp;
		//descDepth.BackFace                             = stencilMarkOp;
		//
		//// Create the depth stencil state.
		//mCoreStorage->md3dDevice->CreateDepthStencilState(&descDepth, &mDepthStencilState);
	}

	void ClearClass()
	{
		// 어떻게 그릴것인가 (와이어)
		ReleaseCOM(mWireframeRS);
		ReleaseCOM(mSolidframeRS);
		//ReleaseCOM(m_pNoDepthWriteLessStencilMaskState);
		//ReleaseCOM(mDepthStencilState);
	}
		
	// 오버로딩 할 예정
	void Draw(DXGI_FORMAT _ForMat)
	{
		// 모델 그리기
		DrawALLModel(_ForMat);
	}

	// 오버로딩 할 예정
	void DrawIns(DXGI_FORMAT _ForMat)
	{
		// 변수 셋팅
		updateValue();

		// 쉐도우 맵 그리기
		DrawShadowMap(_ForMat);

		// 첫 패스 렌더링
		onePessRender(_ForMat);
		
		// 두번째 패스 렌더링
		twoPessRender();

		// 클리어
		clearValue();
	}

	// 어떻게 그릴 것인가
	void SetRes(RES_STATE e_Res)
	{
		switch (e_Res)
		{
		default:
		case e_Solid:
			cCoreStorage::GetInstance()->md3dImmediateContext->RSSetState(mSolidframeRS);
			break;

		case e_Wire:
			cCoreStorage::GetInstance()->md3dImmediateContext->RSSetState(mWireframeRS);
			break;
		}
	}

	// 업데이트
	void Update(float& dt)
	{
		//// 이터레이터 내부에 있는 모든 모델을 업데이트 하자
		//for (map<string, InitMetaData*>::iterator itor = mModelManager->mAllModelData.begin(); itor != mModelManager->mAllModelData.end(); ++itor)
		//{
		//	// 모델 내부의 월드 매트릭스 업데이트
		//	for (unsigned int i = 0; i < itor->second->mWdMtx.size(); ++i)
		//	{
		//		itor->second->mWdMtx[i].;
		//	}
		//}
		//
		//// 디렉셔널 라이트
		//mShaderManager->GetLightManager()->mSunDirLight.Direction = gCam.GetLook();
		//
		// 플레이어 손전등
		//mShaderManager->SetPlyerSpotLight();
		//
		//// 모델 매니저 업데이트
		//mModelManager->Update(dt);

		//// 포인트 라이트 (카메라에 붙어있는)
		//mShaderManager->GetLightManager()->mPointLight.Position = gCam.GetPosition();

		// 카메라 업데이트
		UpdateCam(dt);

		// 인스턴스 업데이트
		mModelManager->UpdateIns();
	}

private:
	void InitRes()
	{
		// 어떤식으로 그릴 것인가? 객체 생성
		// 와이어
		static D3D11_RASTERIZER_DESC frameDesc;
		ZeroMemory(&frameDesc, sizeof(D3D11_RASTERIZER_DESC));
		frameDesc.FillMode              = D3D11_FILL_WIREFRAME;
		frameDesc.CullMode              = D3D11_CULL_NONE;
		frameDesc.FrontCounterClockwise = false;
		frameDesc.DepthClipEnable       = true;

		HR(cCoreStorage::GetInstance()->md3dDevice->CreateRasterizerState(&frameDesc, &mWireframeRS));

		// 솔리드
		ZeroMemory(&frameDesc, sizeof(D3D11_RASTERIZER_DESC));
		frameDesc.FillMode              = D3D11_FILL_SOLID;
		frameDesc.CullMode              = D3D11_CULL_BACK;
		frameDesc.FrontCounterClockwise = false;
		frameDesc.DepthClipEnable       = true;

		HR(cCoreStorage::GetInstance()->md3dDevice->CreateRasterizerState(&frameDesc, &mSolidframeRS));

		// 깊이 버퍼 스테이트
		D3D11_DEPTH_STENCIL_DESC;

		// 블랜드 스테이트
		D3D11_BLEND;

	}

	void UpdateCam(float& dt)
	{
		// 카메라 매트릭스 재생성 
		gCam.UpdateViewMatrix();
	}

	// 변수 셋팅
	void updateValue()
	{
		// G버퍼 얻기
		mShaderManager->SetGbuffer();

		// 공용 쉐이더 업
		mShaderManager->SetCommonShaderValue();
	}

	// 클리어
	void clearValue()
	{
		mShaderManager->ClearGbuffer();
		mShaderManager->GetPassByIndex(0, e_Basic);
		mShaderManager->GetPassByIndex(0, e_Shadow);
	}

	// 첫 패스 렌더링
	void onePessRender(DXGI_FORMAT& _ForMat)
	{
		// 렌더 타겟 클리어
		mCoreStorage->PreRender();

		//----------------------------------------------------------------------------//
		// 이 사이에 렌더링 !!
		// 모델 그리기
		DrawInsALLModel(_ForMat);
		//----------------------------------------------------------------------------//

		// MRT 클리어 ( 다음에서 쓰기 위해 )
		mCoreStorage->PostRender();
	}

	// 디퍼드 렌더링 시작
	void twoPessRender()
	{
		//mCoreStorage->md3dImmediateContext->OMSetDepthStencilState(m_pNoDepthWriteLessStencilMaskState, 1);

		// 렌더 타겟 셋 
		mCoreStorage->SetRenderTaget();

#ifdef DEBUG_MODE
		//-------------------------------------------------------------------//
		// 스크린 그리기
		if (mSolidDraw)
		{
			DrawScreen();
		}
		else
		{
			cCoreStorage::GetInstance()->md3dImmediateContext->RSSetState(mSolidframeRS);
			DrawScreen();
			cCoreStorage::GetInstance()->md3dImmediateContext->RSSetState(mWireframeRS);
		}
#else
		DrawScreen();
#endif
		//-------------------------------------------------------------------//
	}

	// 쉐도우 맵 렌더링
	void DrawShadowMap(DXGI_FORMAT& _ForMat)
	{
		// 쉐도우 맵 셋팅
		mShadowMap->BindDsvAndSetNullRenderTarget();

		// 쉐도우 맵 렌더링
		DrawSDInsALLModel(_ForMat);

	}

	// 스크린 그리기
	void DrawScreen()
	{
		static D3DX11_TECHNIQUE_DESC TechDesc;

		// 기본 쉐이더 업데이트
		mShaderManager->SetBasicShaderValueIns(e_Basic);

		// 쉐이더 모드 갱신
		// 사각형 그리기
		SHADER_TYPE _ShaderMode = e_ShaderDeferred;

		mShaderManager->SetModelShaderMode(mModelManager->mScreen, _ShaderMode);

		// 쉐이더 모드에 셋팅된 값 가져오기
		UINT offset[2] = { 0, 0 };
		UINT stride[2] = { mShaderManager->GetIAStride(), sizeof(XMFLOAT4X4) };

		// 쉐이더 입력조립기 세팅 (Set)
		mCoreStorage->md3dImmediateContext->IASetInputLayout(mShaderManager->GetInputLayout(e_Basic));
		mCoreStorage->md3dImmediateContext->IASetPrimitiveTopology(mModelManager->mScreen->_PRIMITIVE_TOPOLOGY);

		// 버퍼 생성
		ID3D11Buffer* VBs[2] = { mModelManager->mScreenBuffer->mVB, mModelManager->mScreenBuffer->mInstancedBuffer[mModelManager->mScreen->mCreateName] };

		// 입력조립기에 버퍼 할당 <-- 일단 밖으로 뺐는데.. 현재 패스가 0이라 밑에꺼와 차이점이 없음.. 패스를 늘려봐야 알 듯
		mCoreStorage->md3dImmediateContext->IASetVertexBuffers(0, 2, VBs, stride, offset);
		mCoreStorage->md3dImmediateContext->IASetIndexBuffer(mModelManager->mScreenBuffer->mIB, DXGI_FORMAT_R32_UINT, 0);

		// 쉐이더 정보 얻기
		mShaderManager->GetDesc(&TechDesc, e_Basic);

		// 모델 그리기
		// 패스만큼, 반복
		for (UINT p = 0; p < TechDesc.Passes; ++p)
		{
			// 드로우 콜마다 밑에 있는 모델구조 전체를 한번에 다 그림

			// 쉐이더 패스 적용
			mShaderManager->GetPassByIndex(p, e_Basic);

			// 사진 그리기
			mCoreStorage->md3dImmediateContext->DrawIndexedInstanced
				(mModelManager->mScreen->mIndexCount,
				 mModelManager->mScreen->mObjData.size(),
				 mModelManager->mScreen->mIndexOffset,
				 mModelManager->mScreen->mVertexOffset,
				 0);

		}
	}

	// 모델 그리기
	void DrawALLModel(DXGI_FORMAT _ForMat)
	{
		static int _BeforeShaderMode;
		static string _BeforeModelName;
		static D3DX11_TECHNIQUE_DESC TechDesc;

		// 이터레이터 내부에 있는 모든 모델을 그리자
		for (map<string, InitMetaData*>::iterator itor = mModelManager->mAllModelData.begin(); itor != mModelManager->mAllModelData.end(); ++itor)
		{
			// 쉐이더 모드 갱신
			// 쉐이더 모드와, 모델 이름이 같으면 버퍼를 갱신할 필요가 없음.
			if (itor->second->mShaderMode != _BeforeShaderMode && itor->second->mCreateName != _BeforeModelName)
			{
				SHADER_TYPE _ShaderMode = itor->second->mShaderMode;

				// 쉐이더 모드 업데이트
				mShaderManager->SetModelShaderMode(itor->second, _ShaderMode);

				// 쉐이더 모드에 셋팅된 값 가져오기
				UINT offset = 0;
				UINT stride = mShaderManager->GetIAStride();


				// 쉐이더 입력조립기 세팅 (Set)
				mCoreStorage->md3dImmediateContext->IASetInputLayout(mShaderManager->GetInputLayout(e_Basic));
				mCoreStorage->md3dImmediateContext->IASetPrimitiveTopology(itor->second->_PRIMITIVE_TOPOLOGY);

				// 입력조립기에 버퍼 할당 <-- 일단 밖으로 뺐는데.. 현재 패스가 0이라 밑에꺼와 차이점이 없음.. 패스를 늘려봐야 알 듯
				mCoreStorage->md3dImmediateContext->IASetVertexBuffers(0, 1, &mModelManager->GetBuffer(_ShaderMode)->mVB, &stride, &offset);
				mCoreStorage->md3dImmediateContext->IASetIndexBuffer(mModelManager->GetBuffer(_ShaderMode)->mIB, _ForMat, 0);

				// 정보 얻기
				mShaderManager->GetDesc(&TechDesc, e_Basic);

				// 현재 쉐이더 모드 갱신
				_BeforeShaderMode = itor->second->mShaderMode;
				_BeforeModelName  = itor->second->mCreateName;
			}

			// 모델 그리기
			// 패스만큼, 반복
			for (UINT p = 0; p < TechDesc.Passes; ++p)
			{
				// 드로우 콜마다 밑에 있는 모델구조 전체를 한번에 다 그림

				//// 입력조립기에 버퍼 할당
				//mCoreStorage->md3dImmediateContext->IASetVertexBuffers(0, 1, &mModelManager->GetBuffer(_ShaderMode)->mVB, &stride, &offset);
				//mCoreStorage->md3dImmediateContext->IASetIndexBuffer(mModelManager->GetBuffer(_ShaderMode)->mIB, _ForMat, 0);
				//
				// 기본 쉐이더 변수 업데이트 
				//mShaderManager->SetGbuffer();

				// 모델 내부에 있는 매트릭스 만큼, 반복
				map<int, ObjData>& tobjMtx = itor->second->mObjData;
				UINT i = -1;
				for (auto itor2 = tobjMtx.begin(); itor2 != tobjMtx.end(); ++itor2)
				{
					// 쉐이더 패스 적용
					mShaderManager->SetBasicShaderValue(itor->second, ++i);
					mShaderManager->GetPassByIndex(p, e_Basic);

					// 모델 그리기
					mCoreStorage->md3dImmediateContext->DrawIndexed(itor->second->mIndexCount, itor->second->mIndexOffset, itor->second->mVertexOffset);
				}

				//// G버퍼 해제
				//mShaderManager->ClearGbuffer();
				//mShaderManager->GetPassByIndex(p);
			}
		}
	}

	// 모델 그리기(인스턴스)
	void DrawInsALLModel(DXGI_FORMAT _ForMat)
	{
		static int _BeforeShaderMode;
		static string _BeforeModelName;
		static D3DX11_TECHNIQUE_DESC TechDesc;
		static SHADER_TYPE _ShaderMode;


		// 이터레이터 내부에 있는 모든 모델을 그리자
		for (map<string, InitMetaData*>::iterator itor = mModelManager->mAllModelData.begin(); itor != mModelManager->mAllModelData.end(); ++itor)
		{
			// 현재 쉐이더 모드를 가져온다.
			InitMetaData* _CurrentModel = itor->second;
			_ShaderMode = _CurrentModel->mShaderMode;

			// 해당 모델의 인스턴싱 버퍼가 활성화 되어있으면, 그린다.
			if (mModelManager->GetBuffer(_ShaderMode)->mInstancedBuffer[_CurrentModel->mCreateName] != nullptr)
			{
				// 쉐이더 모드 갱신
				// 쉐이더 모드나, 모델 이름이 같으면 버퍼를 갱신할 필요가 없음.
				if (_CurrentModel->mShaderMode != _BeforeShaderMode || _CurrentModel->mCreateName != _BeforeModelName)
				{
					// 기본 쉐이더 업데이트
					mShaderManager->SetBasicShaderValueIns(e_Basic);

					// 개별 쉐이더 업데이트
					mShaderManager->SetModelShaderMode(_CurrentModel, _ShaderMode);

					// 쉐이더 모드에 셋팅된 값 가져오기
					UINT offset[2] = { 0, 0 };
					UINT stride[2];

					// 애니가 들어간 텍스처는 별도 처리
					if (_ShaderMode == e_ShaderPongTexAni)
					{
						stride[0] = mShaderManager->GetIAStride();
						stride[1] = sizeof(InsAni);
					}
					// 맵 버퍼
					else if (_ShaderMode == e_ShaderPongTexMap)
					{
						stride[0] = mShaderManager->GetIAStride();
						stride[1] = sizeof(InsMap);

					}
					// 그 외 인스턴스 텍스처
					else
					{
						stride[0] = mShaderManager->GetIAStride();
						stride[1] = sizeof(XMFLOAT4X4);
					}

					// 쉐이더 입력조립기 세팅 (Set)
					mCoreStorage->md3dImmediateContext->IASetInputLayout(mShaderManager->GetInputLayout(e_Basic));
					mCoreStorage->md3dImmediateContext->IASetPrimitiveTopology(_CurrentModel->_PRIMITIVE_TOPOLOGY);

					// 버퍼 생성
					ID3D11Buffer* VBs[2] = { mModelManager->GetBuffer(_ShaderMode)->mVB, mModelManager->GetBuffer(_ShaderMode)->mInstancedBuffer[_CurrentModel->mCreateName] };

					// 입력조립기에 버퍼 할당 <-- 일단 밖으로 뺐는데.. 현재 패스가 0이라 밑에꺼와 차이점이 없음.. 패스를 늘려봐야 알 듯
					mCoreStorage->md3dImmediateContext->IASetVertexBuffers(0, 2, VBs, stride, offset);
					mCoreStorage->md3dImmediateContext->IASetIndexBuffer(mModelManager->GetBuffer(_ShaderMode)->mIB, _ForMat, 0);

					// 현재 쉐이더 모드 갱신
					_BeforeShaderMode = _CurrentModel->mShaderMode;
					_BeforeModelName  = _CurrentModel->mCreateName;

					// 쉐이더 정보 얻기 (어떤 쉐이더를 사용할지 결정하는 곳)
					mShaderManager->GetDesc(&TechDesc, e_Basic);
				}

				// 모델 그리기
				// 패스만큼, 반복
				for (UINT p = 0; p < TechDesc.Passes; ++p)
				{
					// 드로우 콜마다 밑에 있는 모델구조 전체를 한번에 다 그림

					// 쉐이더 패스 적용
					mShaderManager->GetPassByIndex(p, e_Basic);

					if (_ShaderMode == e_ShaderPongTexMap)
					{
						// 모델 그리기
						mCoreStorage->md3dImmediateContext->DrawIndexedInstanced
							(_CurrentModel->mIndexCount - 1,
							_CurrentModel->mObjData.size(),
							_CurrentModel->mIndexOffset,
							_CurrentModel->mVertexOffset,
							0);
					}
					else
					{
						// 모델 그리기
						mCoreStorage->md3dImmediateContext->DrawIndexedInstanced
							(_CurrentModel->mIndexCount,
							_CurrentModel->mObjData.size(),
							_CurrentModel->mIndexOffset,
							_CurrentModel->mVertexOffset,
							0);
					}					
				}
			}
		}
	}

	// 쉐도우 모델 그리기(인스턴스)
	void DrawSDInsALLModel(DXGI_FORMAT _ForMat)
	{
		static int _BeforeShaderMode;
		static string _BeforeModelName;
		static D3DX11_TECHNIQUE_DESC TechDesc;
		static SHADER_TYPE _ShaderMode;


		// 이터레이터 내부에 있는 모든 모델을 그리자
		for (map<string, InitMetaData*>::iterator itor = mModelManager->mAllModelData.begin(); itor != mModelManager->mAllModelData.end(); ++itor)
		{
			// 현재 쉐이더 모드를 가져온다.
			InitMetaData* _CurrentModel = itor->second;

			// 칼라, 라이트, 스카이 박스는 포함되지 않음
			if (_CurrentModel->mShaderMode != e_ShaderColor && _CurrentModel->mShaderMode != e_ShaderLight && _CurrentModel->mShaderMode != e_ShaderSkyBox && _CurrentModel->mShaderMode != e_ShaderPongTexMap)
			{
				// 쉐이더 모드 저장
				_ShaderMode = _CurrentModel->mShaderMode;

				// 해당 모델의 인스턴싱 버퍼가 활성화 되어있으면, 그린다.
				if (mModelManager->GetBuffer(_ShaderMode)->mInstancedBuffer[_CurrentModel->mCreateName] != nullptr)
				{
					// 쉐이더 모드 갱신
					// 쉐이더 모드나, 모델 이름이 같으면 버퍼를 갱신할 필요가 없음.
					if (_CurrentModel->mShaderMode != _BeforeShaderMode || _CurrentModel->mCreateName != _BeforeModelName)
					{
						// 기본 쉐이더 업데이트
						mShaderManager->SetBasicShaderValueIns(e_Shadow);

						// 개별 쉐이더 업데이트
						mShaderManager->SetModelShaderMode(_CurrentModel, _ShaderMode);

						// 쉐이더 모드에 셋팅된 값 가져오기
						UINT offset[2] = { 0, 0 };
						UINT stride[2];

						// 애니가 들어간 텍스처는 별도 처리
						if (_ShaderMode == e_ShaderPongTexAni)
						{
							stride[0] = mShaderManager->GetIAStride();
							stride[1] = sizeof(InsAni);
						}
						// 맵 버퍼
						else if (_ShaderMode == e_ShaderPongTexMap)
						{
							stride[0] = mShaderManager->GetIAStride();
							stride[1] = sizeof(InsMap);

						}
						// 그 외 인스턴스 텍스처
						else
						{
							stride[0] = mShaderManager->GetIAStride();
							stride[1] = sizeof(XMFLOAT4X4);
						}

						// 쉐이더 입력조립기 세팅 (Set)
						mCoreStorage->md3dImmediateContext->IASetInputLayout(mShaderManager->GetInputLayout(e_Shadow));
						mCoreStorage->md3dImmediateContext->IASetPrimitiveTopology(_CurrentModel->_PRIMITIVE_TOPOLOGY);

						// 버퍼 생성
						ID3D11Buffer* VBs[2] = { mModelManager->GetBuffer(_ShaderMode)->mVB, mModelManager->GetBuffer(_ShaderMode)->mInstancedBuffer[_CurrentModel->mCreateName] };

						// 입력조립기에 버퍼 할당 <-- 일단 밖으로 뺐는데.. 현재 패스가 0이라 밑에꺼와 차이점이 없음.. 패스를 늘려봐야 알 듯
						mCoreStorage->md3dImmediateContext->IASetVertexBuffers(0, 2, VBs, stride, offset);
						mCoreStorage->md3dImmediateContext->IASetIndexBuffer(mModelManager->GetBuffer(_ShaderMode)->mIB, _ForMat, 0);

						// 현재 쉐이더 모드 갱신
						_BeforeShaderMode = _CurrentModel->mShaderMode;
						_BeforeModelName  = _CurrentModel->mCreateName;

						// 쉐이더 정보 얻기 (어떤 쉐이더를 사용할지 결정하는 곳)
						mShaderManager->GetDesc(&TechDesc, e_Shadow);
					}

					// 모델 그리기
					// 패스만큼, 반복
					for (UINT p = 0; p < TechDesc.Passes; ++p)
					{
						// 드로우 콜마다 밑에 있는 모델구조 전체를 한번에 다 그림

						// 쉐이더 패스 적용
						mShaderManager->GetPassByIndex(p, e_Shadow);

						if (_ShaderMode == e_ShaderPongTexMap)
						{
							// 모델 그리기
							mCoreStorage->md3dImmediateContext->DrawIndexedInstanced
								(_CurrentModel->mIndexCount - 1,
								_CurrentModel->mObjData.size(),
								_CurrentModel->mIndexOffset,
								_CurrentModel->mVertexOffset,
								0);
						}
						else
						{
							// 모델 그리기
							mCoreStorage->md3dImmediateContext->DrawIndexedInstanced
								(_CurrentModel->mIndexCount,
								_CurrentModel->mObjData.size(),
								_CurrentModel->mIndexOffset,
								_CurrentModel->mVertexOffset,
								0);
						}
					}
				}
			}
		}
	}
};

