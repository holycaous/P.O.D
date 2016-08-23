#pragma once
extern cCam gCam;

class cDrawManager : public cSingleton<cDrawManager>
{
	// 필요한거
	cCoreStorage*     mCoreStorage     = cCoreStorage    ::GetInstance();
	cShaderManager*   mShaderManager   = cShaderManager  ::GetInstance();
	cMaterialManager* mMaterialManager = cMaterialManager::GetInstance();
	cModelManager*    mModelManager    = cModelManager   ::GetInstance();


	// 어떻게 그릴 것인가
	ID3D11RasterizerState* mWireframeRS  = nullptr;
	ID3D11RasterizerState* mSolidframeRS = nullptr;

	//// 깊이, 스텐실 테스트 설정
	//ID3D11DepthStencilState* m_pNoDepthWriteLessStencilMaskState = nullptr;
	//ID3D11DepthStencilState* m_DepthStencilState = nullptr;
public:
	void Init()
	{
		// 레스터라이즈 방법
		InitRes();
		SetRes(e_Solid);

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
		//mCoreStorage->md3dDevice->CreateDepthStencilState(&descDepth, &m_DepthStencilState);
	}

	void InitRes()
	{
		// 어떤식으로 그릴 것인가? 객체 생성
		// 와이어
		static D3D11_RASTERIZER_DESC frameDesc;
		ZeroMemory(&frameDesc, sizeof(D3D11_RASTERIZER_DESC));
		frameDesc.FillMode              = D3D11_FILL_WIREFRAME;
		frameDesc.CullMode              = D3D11_CULL_BACK;
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
	}

	void ClearClass()
	{
		// 어떻게 그릴것인가 (와이어)
		ReleaseCOM(mWireframeRS);
		ReleaseCOM(mSolidframeRS);
		//ReleaseCOM(m_pNoDepthWriteLessStencilMaskState);
		//ReleaseCOM(m_DepthStencilState);
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
		// G버퍼 얻기
		mShaderManager->SetGbuffer();
		
		// 공용 쉐이더 업
		mShaderManager->SetCommonShaderValue();

		// 렌더 타겟 클리어
		mCoreStorage->PreRender();
		
		//----------------------------------------------------------------------------//
		// 이 사이에 렌더링 !!
		// 모델 그리기
		DrawInsALLModel(_ForMat);
		//----------------------------------------------------------------------------//

		// 텍스처 클리어
		mCoreStorage->PostRender();

		////----------------------------------------------------------------------------//
		// 타겟에 옮길 쉐이더? 
		// 픽셀 쉐이더에서 텍스처 유무 분기점 나눠야할 듯 ( PC, PL, PNT 등등) <-- 각 텍스처가 없는 쉐이더가 있으니까
		StartDeferredRendering();

		// G버퍼 클리어
		mShaderManager->ClearGbuffer();
		mShaderManager->GetPassByIndex(0);
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
				mCoreStorage->md3dImmediateContext->IASetInputLayout(mShaderManager->GetInputLayout());
				mCoreStorage->md3dImmediateContext->IASetPrimitiveTopology(itor->second->_PRIMITIVE_TOPOLOGY);

				// 입력조립기에 버퍼 할당 <-- 일단 밖으로 뺐는데.. 현재 패스가 0이라 밑에꺼와 차이점이 없음.. 패스를 늘려봐야 알 듯
				mCoreStorage->md3dImmediateContext->IASetVertexBuffers(0, 1, &mModelManager->GetBuffer(_ShaderMode)->mVB, &stride, &offset);
				mCoreStorage->md3dImmediateContext->IASetIndexBuffer(mModelManager->GetBuffer(_ShaderMode)->mIB, _ForMat, 0);

				// 정보 얻기
				mShaderManager->GetDesc(&TechDesc);

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
				for (unsigned int i = 0; i < itor->second->mObjData.size(); ++i)
				{
					// 쉐이더 패스 적용
					mShaderManager->SetBasicShaderValue(itor->second, i);
					mShaderManager->GetPassByIndex(p);

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
			_ShaderMode = itor->second->mShaderMode;

			// 해당 모델의 인스턴싱 버퍼가 활성화 되어있으면, 그린다.
			if (mModelManager->GetBuffer(_ShaderMode)->mInstancedBuffer[itor->second->mCreateName] != nullptr)
			{
				// 쉐이더 모드 갱신
				// 쉐이더 모드나, 모델 이름이 같으면 버퍼를 갱신할 필요가 없음.
				if ( itor->second->mShaderMode != _BeforeShaderMode || itor->second->mCreateName != _BeforeModelName)
				{
					// 기본 쉐이더 업데이트
					mShaderManager->SetBasicShaderValueIns();

					// 개별 쉐이더 업데이트
					mShaderManager->SetModelShaderMode(itor->second, _ShaderMode);

					// 쉐이더 모드에 셋팅된 값 가져오기
					UINT offset[2] = { 0, 0 };
					UINT stride[2] = { mShaderManager->GetIAStride(), sizeof(XMFLOAT4X4) };

					// 쉐이더 입력조립기 세팅 (Set)
					mCoreStorage->md3dImmediateContext->IASetInputLayout(mShaderManager->GetInputLayout());
					mCoreStorage->md3dImmediateContext->IASetPrimitiveTopology(itor->second->_PRIMITIVE_TOPOLOGY);

					// 버퍼 생성
					ID3D11Buffer* VBs[2] = { mModelManager->GetBuffer(_ShaderMode)->mVB, mModelManager->GetBuffer(_ShaderMode)->mInstancedBuffer[itor->second->mCreateName] };

					// 입력조립기에 버퍼 할당 <-- 일단 밖으로 뺐는데.. 현재 패스가 0이라 밑에꺼와 차이점이 없음.. 패스를 늘려봐야 알 듯
					mCoreStorage->md3dImmediateContext->IASetVertexBuffers(0, 2, VBs, stride, offset);
					mCoreStorage->md3dImmediateContext->IASetIndexBuffer(mModelManager->GetBuffer(_ShaderMode)->mIB, _ForMat, 0);

					// 현재 쉐이더 모드 갱신
					_BeforeShaderMode = itor->second->mShaderMode;
					_BeforeModelName  = itor->second->mCreateName;

					// 쉐이더 정보 얻기
					mShaderManager->GetDesc(&TechDesc);
				}

				// 모델 그리기
				// 패스만큼, 반복
				for (UINT p = 0; p < TechDesc.Passes; ++p)
				{
					// 드로우 콜마다 밑에 있는 모델구조 전체를 한번에 다 그림

					// 쉐이더 패스 적용
					mShaderManager->GetPassByIndex(p);

					// 모델 그리기
					mCoreStorage->md3dImmediateContext->DrawIndexedInstanced
						(itor->second->mIndexCount,
						 itor->second->mObjData.size(),
						 itor->second->mIndexOffset,
						 itor->second->mVertexOffset,
						 0);
				}
			}
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
		
		//// 포인트 라이트
		mShaderManager->GetLightManager()->mPointLight.Position = gCam.GetPosition();
		//

		// 플레이어 손전등
		//mShaderManager->SetPlyerSpotLight();
		//
		//// 모델 매니저 업데이트
		//mModelManager->Update(dt);

		// 카메라 업데이트
		UpdateCam(dt);

		// 인스턴스 업데이트
		mModelManager->UpdateIns();
	}

	void UpdateCam(float& dt)
	{
		// 카메라 컨트롤
		if (GetAsyncKeyState('W') & 0x8000)
		{
			//----------------------------------//
			// 상 키 눌렀음을 서버에 보내기
			//----------------------------------//


			//----------------------------------//
			gCam.Walk(100.0f*dt);
		}

		if (GetAsyncKeyState('S') & 0x8000)
		{
			//----------------------------------//
			// 히 키 눌렀음을 서버에 보내기
			//----------------------------------//


			//----------------------------------//
			gCam.Walk(-100.0f*dt);
		}

		if (GetAsyncKeyState('A') & 0x8000)
		{
			//----------------------------------//
			// 좌 키 눌렀음을 서버에 보내기
			//----------------------------------//

			//----------------------------------//
			gCam.Strafe(-100.0f*dt);
		}

		if (GetAsyncKeyState('D') & 0x8000)
		{
			//----------------------------------//
			// 우 키 눌렀음을 서버에 보내기
			//----------------------------------//


			// 서버에서 위치를 받아, 플레이어 위치를 갱신하는 함수.
			// 해당 부분 이곳이 아닌, 다른 곳에서 구현해야함.
			// 서버에서 데이터 받는 곳
			//mModelManager->SetPlayerPos(_x, _y, _z);
			//----------------------------------//
			gCam.Strafe(100.0f*dt);
		}
		if (GetAsyncKeyState('E') & 0x8000)
			gCam.Change3PersonCam();

		if (GetAsyncKeyState('R') & 0x8000)
			gCam.Change1PersonCam();

		// 카메라 매트릭스 재생성 
		gCam.UpdateViewMatrix();
	}

	// 디퍼드 렌더링 시작
	void StartDeferredRendering()
	{
		//mCoreStorage->md3dImmediateContext->OMSetDepthStencilState(m_pNoDepthWriteLessStencilMaskState, 1);

		// 렌더 타겟 셋 
		mCoreStorage->SetRenderTaget();

		//-------------------------------------------------------------------//
		// 스크린 그리기
		DrawScreen();
		//-------------------------------------------------------------------//
	}

	// 스크린 그리기
	void DrawScreen()
	{
		static D3DX11_TECHNIQUE_DESC TechDesc;

		// 기본 쉐이더 업데이트
		mShaderManager->SetBasicShaderValueIns();

		// 쉐이더 모드 갱신
		// 사각형 그리기
		SHADER_TYPE _ShaderMode = e_ShaderDeferred;

		mShaderManager->SetModelShaderMode(mModelManager->mScreen, _ShaderMode);

		// 쉐이더 모드에 셋팅된 값 가져오기
		UINT offset[2] = { 0, 0 };
		UINT stride[2] = { mShaderManager->GetIAStride(), sizeof(XMFLOAT4X4) };

		// 쉐이더 입력조립기 세팅 (Set)
		mCoreStorage->md3dImmediateContext->IASetInputLayout(mShaderManager->GetInputLayout());
		mCoreStorage->md3dImmediateContext->IASetPrimitiveTopology(mModelManager->mScreen->_PRIMITIVE_TOPOLOGY);

		// 버퍼 생성
		ID3D11Buffer* VBs[2] = { mModelManager->mScreenBuffer->mVB, mModelManager->mScreenBuffer->mInstancedBuffer[mModelManager->mScreen->mCreateName] };

		// 입력조립기에 버퍼 할당 <-- 일단 밖으로 뺐는데.. 현재 패스가 0이라 밑에꺼와 차이점이 없음.. 패스를 늘려봐야 알 듯
		mCoreStorage->md3dImmediateContext->IASetVertexBuffers(0, 2, VBs, stride, offset);
		mCoreStorage->md3dImmediateContext->IASetIndexBuffer(mModelManager->mScreenBuffer->mIB, DXGI_FORMAT_R32_UINT, 0);

		// 쉐이더 정보 얻기
		mShaderManager->GetDesc(&TechDesc);

		// 모델 그리기
		// 패스만큼, 반복
		for (UINT p = 0; p < TechDesc.Passes; ++p)
		{
			// 드로우 콜마다 밑에 있는 모델구조 전체를 한번에 다 그림

			// 쉐이더 패스 적용
			mShaderManager->GetPassByIndex(p);

			// 사진 그리기
			mCoreStorage->md3dImmediateContext->DrawIndexedInstanced
				(mModelManager->mScreen->mIndexCount,
				 mModelManager->mScreen->mObjData.size(),
				 mModelManager->mScreen->mIndexOffset,
				 mModelManager->mScreen->mVertexOffset,
				 0);

		}
	}
};

