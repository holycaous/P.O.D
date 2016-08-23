#pragma once
extern cCam gCam;

class cDrawManager : public cSingleton<cDrawManager>
{
	// �ʿ��Ѱ�
	cCoreStorage*     mCoreStorage     = cCoreStorage    ::GetInstance();
	cShaderManager*   mShaderManager   = cShaderManager  ::GetInstance();
	cMaterialManager* mMaterialManager = cMaterialManager::GetInstance();
	cModelManager*    mModelManager    = cModelManager   ::GetInstance();


	// ��� �׸� ���ΰ�
	ID3D11RasterizerState* mWireframeRS  = nullptr;
	ID3D11RasterizerState* mSolidframeRS = nullptr;

	//// ����, ���ٽ� �׽�Ʈ ����
	//ID3D11DepthStencilState* m_pNoDepthWriteLessStencilMaskState = nullptr;
	//ID3D11DepthStencilState* m_DepthStencilState = nullptr;
public:
	void Init()
	{
		// �����Ͷ����� ���
		InitRes();
		SetRes(e_Solid);

		// ���� ���̴� ���� ������Ʈ
		for (map<string, InitMetaData*>::iterator itor = mModelManager->mAllModelData.begin(); itor != mModelManager->mAllModelData.end(); ++itor)
		{
			mShaderManager->SetModelShaderValue(itor->second);
		}

		//// ���� ���� 1
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
		//// ���� ���� 2
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
		// ������� �׸� ���ΰ�? ��ü ����
		// ���̾�
		static D3D11_RASTERIZER_DESC frameDesc;
		ZeroMemory(&frameDesc, sizeof(D3D11_RASTERIZER_DESC));
		frameDesc.FillMode              = D3D11_FILL_WIREFRAME;
		frameDesc.CullMode              = D3D11_CULL_BACK;
		frameDesc.FrontCounterClockwise = false;
		frameDesc.DepthClipEnable       = true;

		HR(cCoreStorage::GetInstance()->md3dDevice->CreateRasterizerState(&frameDesc, &mWireframeRS));

		// �ָ���
		ZeroMemory(&frameDesc, sizeof(D3D11_RASTERIZER_DESC));
		frameDesc.FillMode              = D3D11_FILL_SOLID;
		frameDesc.CullMode              = D3D11_CULL_BACK;
		frameDesc.FrontCounterClockwise = false;
		frameDesc.DepthClipEnable       = true;

		HR(cCoreStorage::GetInstance()->md3dDevice->CreateRasterizerState(&frameDesc, &mSolidframeRS));
	}

	void ClearClass()
	{
		// ��� �׸����ΰ� (���̾�)
		ReleaseCOM(mWireframeRS);
		ReleaseCOM(mSolidframeRS);
		//ReleaseCOM(m_pNoDepthWriteLessStencilMaskState);
		//ReleaseCOM(m_DepthStencilState);
	}
		
	// �����ε� �� ����
	void Draw(DXGI_FORMAT _ForMat)
	{
		// �� �׸���
		DrawALLModel(_ForMat);
	}

	// �����ε� �� ����
	void DrawIns(DXGI_FORMAT _ForMat)
	{
		// G���� ���
		mShaderManager->SetGbuffer();
		
		// ���� ���̴� ��
		mShaderManager->SetCommonShaderValue();

		// ���� Ÿ�� Ŭ����
		mCoreStorage->PreRender();
		
		//----------------------------------------------------------------------------//
		// �� ���̿� ������ !!
		// �� �׸���
		DrawInsALLModel(_ForMat);
		//----------------------------------------------------------------------------//

		// �ؽ�ó Ŭ����
		mCoreStorage->PostRender();

		////----------------------------------------------------------------------------//
		// Ÿ�ٿ� �ű� ���̴�? 
		// �ȼ� ���̴����� �ؽ�ó ���� �б��� �������� �� ( PC, PL, PNT ���) <-- �� �ؽ�ó�� ���� ���̴��� �����ϱ�
		StartDeferredRendering();

		// G���� Ŭ����
		mShaderManager->ClearGbuffer();
		mShaderManager->GetPassByIndex(0);
	}

	// ��� �׸� ���ΰ�
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

	// �� �׸���
	void DrawALLModel(DXGI_FORMAT _ForMat)
	{
		static int _BeforeShaderMode;
		static string _BeforeModelName;
		static D3DX11_TECHNIQUE_DESC TechDesc;			

		// ���ͷ����� ���ο� �ִ� ��� ���� �׸���
		for (map<string, InitMetaData*>::iterator itor = mModelManager->mAllModelData.begin(); itor != mModelManager->mAllModelData.end(); ++itor)
		{
			// ���̴� ��� ����
			// ���̴� ����, �� �̸��� ������ ���۸� ������ �ʿ䰡 ����.
			if (itor->second->mShaderMode != _BeforeShaderMode && itor->second->mCreateName != _BeforeModelName)
			{
				SHADER_TYPE _ShaderMode = itor->second->mShaderMode;

				// ���̴� ��� ������Ʈ
				mShaderManager->SetModelShaderMode(itor->second, _ShaderMode);

				// ���̴� ��忡 ���õ� �� ��������
				UINT offset = 0;
				UINT stride = mShaderManager->GetIAStride();


				// ���̴� �Է������� ���� (Set)
				mCoreStorage->md3dImmediateContext->IASetInputLayout(mShaderManager->GetInputLayout());
				mCoreStorage->md3dImmediateContext->IASetPrimitiveTopology(itor->second->_PRIMITIVE_TOPOLOGY);

				// �Է������⿡ ���� �Ҵ� <-- �ϴ� ������ ���µ�.. ���� �н��� 0�̶� �ؿ����� �������� ����.. �н��� �÷����� �� ��
				mCoreStorage->md3dImmediateContext->IASetVertexBuffers(0, 1, &mModelManager->GetBuffer(_ShaderMode)->mVB, &stride, &offset);
				mCoreStorage->md3dImmediateContext->IASetIndexBuffer(mModelManager->GetBuffer(_ShaderMode)->mIB, _ForMat, 0);

				// ���� ���
				mShaderManager->GetDesc(&TechDesc);

				// ���� ���̴� ��� ����
				_BeforeShaderMode = itor->second->mShaderMode;
				_BeforeModelName  = itor->second->mCreateName;
			}

			// �� �׸���
			// �н���ŭ, �ݺ�
			for (UINT p = 0; p < TechDesc.Passes; ++p)
			{
				// ��ο� �ݸ��� �ؿ� �ִ� �𵨱��� ��ü�� �ѹ��� �� �׸�

				//// �Է������⿡ ���� �Ҵ�
				//mCoreStorage->md3dImmediateContext->IASetVertexBuffers(0, 1, &mModelManager->GetBuffer(_ShaderMode)->mVB, &stride, &offset);
				//mCoreStorage->md3dImmediateContext->IASetIndexBuffer(mModelManager->GetBuffer(_ShaderMode)->mIB, _ForMat, 0);
				//
				// �⺻ ���̴� ���� ������Ʈ 
				//mShaderManager->SetGbuffer();

				// �� ���ο� �ִ� ��Ʈ���� ��ŭ, �ݺ�
				for (unsigned int i = 0; i < itor->second->mObjData.size(); ++i)
				{
					// ���̴� �н� ����
					mShaderManager->SetBasicShaderValue(itor->second, i);
					mShaderManager->GetPassByIndex(p);

					// �� �׸���
					mCoreStorage->md3dImmediateContext->DrawIndexed(itor->second->mIndexCount, itor->second->mIndexOffset, itor->second->mVertexOffset);
				}

				//// G���� ����
				//mShaderManager->ClearGbuffer();
				//mShaderManager->GetPassByIndex(p);
			}
		}
	}

	// �� �׸���(�ν��Ͻ�)
	void DrawInsALLModel(DXGI_FORMAT _ForMat)
	{
		static int _BeforeShaderMode;
		static string _BeforeModelName;
		static D3DX11_TECHNIQUE_DESC TechDesc;
		static SHADER_TYPE _ShaderMode;

		// ���ͷ����� ���ο� �ִ� ��� ���� �׸���
		for (map<string, InitMetaData*>::iterator itor = mModelManager->mAllModelData.begin(); itor != mModelManager->mAllModelData.end(); ++itor)
		{
			// ���� ���̴� ��带 �����´�.
			_ShaderMode = itor->second->mShaderMode;

			// �ش� ���� �ν��Ͻ� ���۰� Ȱ��ȭ �Ǿ�������, �׸���.
			if (mModelManager->GetBuffer(_ShaderMode)->mInstancedBuffer[itor->second->mCreateName] != nullptr)
			{
				// ���̴� ��� ����
				// ���̴� ��峪, �� �̸��� ������ ���۸� ������ �ʿ䰡 ����.
				if ( itor->second->mShaderMode != _BeforeShaderMode || itor->second->mCreateName != _BeforeModelName)
				{
					// �⺻ ���̴� ������Ʈ
					mShaderManager->SetBasicShaderValueIns();

					// ���� ���̴� ������Ʈ
					mShaderManager->SetModelShaderMode(itor->second, _ShaderMode);

					// ���̴� ��忡 ���õ� �� ��������
					UINT offset[2] = { 0, 0 };
					UINT stride[2] = { mShaderManager->GetIAStride(), sizeof(XMFLOAT4X4) };

					// ���̴� �Է������� ���� (Set)
					mCoreStorage->md3dImmediateContext->IASetInputLayout(mShaderManager->GetInputLayout());
					mCoreStorage->md3dImmediateContext->IASetPrimitiveTopology(itor->second->_PRIMITIVE_TOPOLOGY);

					// ���� ����
					ID3D11Buffer* VBs[2] = { mModelManager->GetBuffer(_ShaderMode)->mVB, mModelManager->GetBuffer(_ShaderMode)->mInstancedBuffer[itor->second->mCreateName] };

					// �Է������⿡ ���� �Ҵ� <-- �ϴ� ������ ���µ�.. ���� �н��� 0�̶� �ؿ����� �������� ����.. �н��� �÷����� �� ��
					mCoreStorage->md3dImmediateContext->IASetVertexBuffers(0, 2, VBs, stride, offset);
					mCoreStorage->md3dImmediateContext->IASetIndexBuffer(mModelManager->GetBuffer(_ShaderMode)->mIB, _ForMat, 0);

					// ���� ���̴� ��� ����
					_BeforeShaderMode = itor->second->mShaderMode;
					_BeforeModelName  = itor->second->mCreateName;

					// ���̴� ���� ���
					mShaderManager->GetDesc(&TechDesc);
				}

				// �� �׸���
				// �н���ŭ, �ݺ�
				for (UINT p = 0; p < TechDesc.Passes; ++p)
				{
					// ��ο� �ݸ��� �ؿ� �ִ� �𵨱��� ��ü�� �ѹ��� �� �׸�

					// ���̴� �н� ����
					mShaderManager->GetPassByIndex(p);

					// �� �׸���
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

	// ������Ʈ
	void Update(float& dt)
	{
		//// ���ͷ����� ���ο� �ִ� ��� ���� ������Ʈ ����
		//for (map<string, InitMetaData*>::iterator itor = mModelManager->mAllModelData.begin(); itor != mModelManager->mAllModelData.end(); ++itor)
		//{
		//	// �� ������ ���� ��Ʈ���� ������Ʈ
		//	for (unsigned int i = 0; i < itor->second->mWdMtx.size(); ++i)
		//	{
		//		itor->second->mWdMtx[i].;
		//	}
		//}
		//
		//// �𷺼ų� ����Ʈ
		//mShaderManager->GetLightManager()->mSunDirLight.Direction = gCam.GetLook();
		
		//// ����Ʈ ����Ʈ
		mShaderManager->GetLightManager()->mPointLight.Position = gCam.GetPosition();
		//

		// �÷��̾� ������
		//mShaderManager->SetPlyerSpotLight();
		//
		//// �� �Ŵ��� ������Ʈ
		//mModelManager->Update(dt);

		// ī�޶� ������Ʈ
		UpdateCam(dt);

		// �ν��Ͻ� ������Ʈ
		mModelManager->UpdateIns();
	}

	void UpdateCam(float& dt)
	{
		// ī�޶� ��Ʈ��
		if (GetAsyncKeyState('W') & 0x8000)
		{
			//----------------------------------//
			// �� Ű �������� ������ ������
			//----------------------------------//


			//----------------------------------//
			gCam.Walk(100.0f*dt);
		}

		if (GetAsyncKeyState('S') & 0x8000)
		{
			//----------------------------------//
			// �� Ű �������� ������ ������
			//----------------------------------//


			//----------------------------------//
			gCam.Walk(-100.0f*dt);
		}

		if (GetAsyncKeyState('A') & 0x8000)
		{
			//----------------------------------//
			// �� Ű �������� ������ ������
			//----------------------------------//

			//----------------------------------//
			gCam.Strafe(-100.0f*dt);
		}

		if (GetAsyncKeyState('D') & 0x8000)
		{
			//----------------------------------//
			// �� Ű �������� ������ ������
			//----------------------------------//


			// �������� ��ġ�� �޾�, �÷��̾� ��ġ�� �����ϴ� �Լ�.
			// �ش� �κ� �̰��� �ƴ�, �ٸ� ������ �����ؾ���.
			// �������� ������ �޴� ��
			//mModelManager->SetPlayerPos(_x, _y, _z);
			//----------------------------------//
			gCam.Strafe(100.0f*dt);
		}
		if (GetAsyncKeyState('E') & 0x8000)
			gCam.Change3PersonCam();

		if (GetAsyncKeyState('R') & 0x8000)
			gCam.Change1PersonCam();

		// ī�޶� ��Ʈ���� ����� 
		gCam.UpdateViewMatrix();
	}

	// ���۵� ������ ����
	void StartDeferredRendering()
	{
		//mCoreStorage->md3dImmediateContext->OMSetDepthStencilState(m_pNoDepthWriteLessStencilMaskState, 1);

		// ���� Ÿ�� �� 
		mCoreStorage->SetRenderTaget();

		//-------------------------------------------------------------------//
		// ��ũ�� �׸���
		DrawScreen();
		//-------------------------------------------------------------------//
	}

	// ��ũ�� �׸���
	void DrawScreen()
	{
		static D3DX11_TECHNIQUE_DESC TechDesc;

		// �⺻ ���̴� ������Ʈ
		mShaderManager->SetBasicShaderValueIns();

		// ���̴� ��� ����
		// �簢�� �׸���
		SHADER_TYPE _ShaderMode = e_ShaderDeferred;

		mShaderManager->SetModelShaderMode(mModelManager->mScreen, _ShaderMode);

		// ���̴� ��忡 ���õ� �� ��������
		UINT offset[2] = { 0, 0 };
		UINT stride[2] = { mShaderManager->GetIAStride(), sizeof(XMFLOAT4X4) };

		// ���̴� �Է������� ���� (Set)
		mCoreStorage->md3dImmediateContext->IASetInputLayout(mShaderManager->GetInputLayout());
		mCoreStorage->md3dImmediateContext->IASetPrimitiveTopology(mModelManager->mScreen->_PRIMITIVE_TOPOLOGY);

		// ���� ����
		ID3D11Buffer* VBs[2] = { mModelManager->mScreenBuffer->mVB, mModelManager->mScreenBuffer->mInstancedBuffer[mModelManager->mScreen->mCreateName] };

		// �Է������⿡ ���� �Ҵ� <-- �ϴ� ������ ���µ�.. ���� �н��� 0�̶� �ؿ����� �������� ����.. �н��� �÷����� �� ��
		mCoreStorage->md3dImmediateContext->IASetVertexBuffers(0, 2, VBs, stride, offset);
		mCoreStorage->md3dImmediateContext->IASetIndexBuffer(mModelManager->mScreenBuffer->mIB, DXGI_FORMAT_R32_UINT, 0);

		// ���̴� ���� ���
		mShaderManager->GetDesc(&TechDesc);

		// �� �׸���
		// �н���ŭ, �ݺ�
		for (UINT p = 0; p < TechDesc.Passes; ++p)
		{
			// ��ο� �ݸ��� �ؿ� �ִ� �𵨱��� ��ü�� �ѹ��� �� �׸�

			// ���̴� �н� ����
			mShaderManager->GetPassByIndex(p);

			// ���� �׸���
			mCoreStorage->md3dImmediateContext->DrawIndexedInstanced
				(mModelManager->mScreen->mIndexCount,
				 mModelManager->mScreen->mObjData.size(),
				 mModelManager->mScreen->mIndexOffset,
				 mModelManager->mScreen->mVertexOffset,
				 0);

		}
	}
};

