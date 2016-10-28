#pragma once

#include <d3dcompiler.h>

extern cCam gCam;

// ���̴� �Ŵ���
class cShaderManager : public cSingleton<cShaderManager>
{
	cCoreStorage*  mCoreStorage  = cCoreStorage ::GetInstance();
	cLightManager* mLightManager = cLightManager::GetInstance();

	// ������ ���
	SHADER_TYPE mShaderMode;
public:
	// ���̴� ����
	map<int, EffectStorage*> mShader;

	void Init()
	{
		// FX �ʱ�ȭ & ����
		InitFX();
	}

	void ClearClass()
	{
		// ���̴� ����
		for (map<int, EffectStorage*>::iterator itor = mShader.begin(); itor != mShader.end();)
		{
			// ���̴� ���� ���� ��ü ���� ����
			SafeDelete(itor->second);

			// ���� ������
			mShader.erase(itor++);
		}
		// ���̴� ���� ����
		mShader.clear();
	}

public:
	// Get & Set
	// ���̴� ���� Set �ϱ� ( �����ε� )
	void SetShaderValue(SHADER_VAL_TYPE _ValueEnum, char* _Name, ID3D11ShaderResourceView* _value)
	{
		mShader[mShaderMode]->mfxResource[_Name]->SetResource(_value);
	}

	// '�⺻' ���̴� ���� ������Ʈ
	void SetBasicShaderValue(InitMetaData* mNowModel, int _Num)
	{
		// �⺻ ���� ����
		XMMATRIX world             = XMLoadFloat4x4(&mNowModel->mObjData[_Num].mWdMtx);
		XMMATRIX worldInvTranspose = cMathHelper::InverseTranspose(world);
		XMMATRIX viewInvTranspose  = cMathHelper::InverseTranspose(gCam.View());
		XMMATRIX worldViewProj     = world * gCam.ViewProj();
		XMMATRIX WorldView		   = gCam.View() * world;

		// ī�޶� �� ����
		XMMATRIX view      = gCam.View();
		XMMATRIX proj      = gCam.Proj();
		XMMATRIX viewProj  = gCam.ViewProj();
		XMMATRIX projInvTranspose = cMathHelper::InverseTranspose(gCam.Proj());

		// viewInvTranspose
		// -> �̵��� ���� �ϰ�
		// -> ȸ���� �׵��� �츮��
		// -> ��������
		// -> �ٽ� 1��
		// -> ����� ���ؼ�
		// -> �ϴ°ž�

		// �� ��Ʈ���� ������Ʈ
		UpdateWorldMtx(world, worldInvTranspose, worldViewProj, view, viewInvTranspose, proj, projInvTranspose);

		// ī�޶� ��ġ ������Ʈ
		UpdateCamPos(gCam.GetPosition());
	}

	// '�⺻' ���̴� ���� ������Ʈ
	void SetBasicShaderValueIns()
	{
		// �⺻ ���� ����
		XMMATRIX world = XMMatrixIdentity();
		XMMATRIX worldInvTranspose = cMathHelper::InverseTranspose(world);

		// ī�޶� �� ����
		XMMATRIX view			  = gCam.View();
		XMMATRIX proj			  = gCam.Proj();
		XMMATRIX projInvTranspose = cMathHelper::InverseTranspose(gCam.Proj());

		XMMATRIX viewProj		  = gCam.ViewProj();
		XMMATRIX viewInvTranspose = cMathHelper::InverseTranspose(gCam.View());

		//// �̷������� ���� ������, �� ��Ʈ������ ----------
		//XMFLOAT4X4 _world;
		//XMMATRIX modelScale  = XMMatrixScaling(0.5f, 0.5f, 0.5f);
		//XMMATRIX modelOffset = XMMatrixTranslation(0.0f, 1.0f, 0.0f);
		//XMStoreFloat4x4(&_world, XMMatrixMultiply(modelScale, modelOffset));
		////---------------------------------------------------

		// �� ��Ʈ���� ������Ʈ
		UpdateWorldMtxIns(world, worldInvTranspose, viewProj, view, viewInvTranspose, proj, projInvTranspose);

		// ī�޶� ��ġ ������Ʈ
		UpdateCamPos(gCam.GetPosition());
	}

	// '����' ���̴� ���� ������Ʈ
	void SetModelShaderValue(InitMetaData* mNowModel)
	{
		// �� ��Ʈ����
		SetShaderValue(e_ShaderValMtx,      "gTexTFMtx"   , mNowModel->mTexMtx);
		SetShaderValue(e_ShaderValMtx,      "gLocTMMtx"   , mNowModel->mLocTMMtx);
		SetShaderValue(e_ShaderValMtx,      "gWdTMMtx"    , mNowModel->mWdTMMtx);

		// �� ���ҽ�
		SetShaderValue(e_ShaderValResource, "gDiffuseTex" , mNowModel->mDiffuseSRV);
		SetShaderValue(e_ShaderValResource, "gSpecularTex", mNowModel->mSpecularSRV);
		SetShaderValue(e_ShaderValResource, "gNormalTex"  , mNowModel->mNomalSRV);

		// ���� ���� ��������
		float CamNear = gCam.GetNearZ();
		float CamFar  = gCam.GetFarZ();
		SetShaderValue(e_ShaderVal, "gNear", CamNear);
		SetShaderValue(e_ShaderVal, "gFar" , CamFar);
	}

	// �÷��̾� ������ ������Ʈ
	void SetPlyerSpotLight()
	{
		mLightManager->mPlayerSpotLight.Position  = gCam.GetPosition();
		mLightManager->mPlayerSpotLight.Direction = gCam.GetLook();
		SetShaderValue(e_ShaderVal, "gSpotLight", mLightManager->mPlayerSpotLight);
	}

	int GetIAStride() const
	{
		// ���� ������ ��� ���ִ°�?
		switch (mShaderMode)
		{
		case e_ShaderColor:
			return sizeof(VertexPC);

		case e_ShaderLight:
			return sizeof(VertexPN);

		case e_ShaderDeferred:
			return sizeof(VertexG);

		default:
		case e_ShaderPongTex:
		case e_ShaderCartoonTex:
			return sizeof(VertexPNT);

		}
	}

	// ���̴� ���, ������ �� ����
	void SetModelShaderMode(InitMetaData* mNowModel, SHADER_TYPE _ShaderMode)
	{
		mShaderMode = _ShaderMode;

		// ���� ���̴� ���� ������Ʈ
		SetModelShaderValue(mNowModel);
	}

	// ���� ���̾� ����
	ID3D11InputLayout* GetInputLayout()
	{
		return mShader[mShaderMode]->mInputLayout;
	}

	// �Ϲ� ���̴� ���
	void GetDesc(D3DX11_TECHNIQUE_DESC* techDesc)
	{
		mShader[mShaderMode]->mTech->GetDesc(techDesc);
	}

	// ���۵� ���̴� ���
	void GetDefeeredDesc(D3DX11_TECHNIQUE_DESC* techDesc)
	{
		mShader[e_ShaderDeferred]->mTech->GetDesc(techDesc);
	}

	void GetPassByIndex(int idx)
	{
		mShader[mShaderMode]->mTech->GetPassByIndex(idx)->Apply(0, mCoreStorage->md3dImmediateContext);
	}

	// ����Ʈ �Ŵ��� ���
	cLightManager* GetLightManager()
	{
		return mLightManager;
	}

	// G���� ��
	void SetGbuffer()
	{
		EffectStorage* tEffectStorage = mShader[e_ShaderDeferred];

		// G���� Get
		GetGBufferShaderValue(tEffectStorage, "gGDepthTex");
		GetGBufferShaderValue(tEffectStorage, "gGDiffuseTex");
		GetGBufferShaderValue(tEffectStorage, "gGPositionTex");
		GetGBufferShaderValue(tEffectStorage, "gGSpecularTex");
		GetGBufferShaderValue(tEffectStorage, "gGNormalTex");

		// G ���� Set
		SetShaderValue(e_ShaderValResource, "gGDepthTex"   , mCoreStorage->mDepthSRV);
		SetShaderValue(e_ShaderValResource, "gGDiffuseTex" , mCoreStorage->mColorSRV);
		SetShaderValue(e_ShaderValResource, "gGPositionTex", mCoreStorage->mPositionSRV);
		SetShaderValue(e_ShaderValResource, "gGSpecularTex", mCoreStorage->mSpecularSRV);
		SetShaderValue(e_ShaderValResource, "gGNormalTex"  , mCoreStorage->mNomalSRV);
	
	}

	// ���� ���̴�
	void SetCommonShaderValue()
	{
		// ������ ���̴� ���� ����
		SetCommonShaderValue(e_ShaderVal, "gDirLight"     , mLightManager->mSunDirLight);
		SetCommonShaderValue(e_ShaderVal, "gPointLight"   , mLightManager->mPointLight);
		SetCommonShaderValue(e_ShaderVal, "gSpotLight"    , mLightManager->mPlayerSpotLight);
	}

	// G ���� Ŭ����
	void ClearGbuffer()
	{
		EffectStorage* tEffectStorage = mShader[e_ShaderDeferred];

		// G����
		ClearGBufferShaderValue(tEffectStorage, "gGDepthTex"   );
		ClearGBufferShaderValue(tEffectStorage, "gGDiffuseTex" );
		ClearGBufferShaderValue(tEffectStorage, "gGPositionTex");
		ClearGBufferShaderValue(tEffectStorage, "gGSpecularTex");
		ClearGBufferShaderValue(tEffectStorage, "gGNormalTex"  );
	}

protected:
	// ���̴� ���� Set �ϱ�
	template <class T>
	void SetShaderValue(SHADER_VAL_TYPE _ValueEnum, char* _Name, T& _value)
	{
		switch (_ValueEnum)
		{
			// ��Ʈ����
		case e_ShaderValMtx:
			mShader[mShaderMode]->mfxMtx[_Name]->SetMatrix(reinterpret_cast<float*>(&_value));
			break;

			// ����
		case e_ShaderValVtx:
			mShader[mShaderMode]->mfxVtx[_Name]->SetFloatVector(reinterpret_cast<float*>(&_value));
			break;

			// �Ϲ� ����
		case e_ShaderVal:
			mShader[mShaderMode]->mfxValue[_Name]->SetRawValue(&_value, 0, sizeof(_value));
			break;
		}
	}

	// ���̴� ���� Set �ϱ�
	template <class T>
	void SetCommonShaderValue(SHADER_VAL_TYPE _ValueEnum, char* _Name, T& _value)
	{
		switch (_ValueEnum)
		{
			// ��Ʈ����
		case e_ShaderValMtx:
			mShader[e_ShaderDeferred]->mfxMtx[_Name]->SetMatrix(reinterpret_cast<float*>(&_value));
			break;

			// ����
		case e_ShaderValVtx:
			mShader[e_ShaderDeferred]->mfxVtx[_Name]->SetFloatVector(reinterpret_cast<float*>(&_value));
			break;

			// �Ϲ� ����
		case e_ShaderVal:
			mShader[e_ShaderDeferred]->mfxValue[_Name]->SetRawValue(&_value, 0, sizeof(_value));
			break;
		}
	}

	// �⺻ ��Ʈ���� ������Ʈ
	template <class T>
	void UpdateWorldMtx(T& world, T& worldInvTranspose, T& worldViewProj, T& view, T& viewInvTranspose, T& proj, T& ProjInvTranspose)
	{
		SetShaderValue(e_ShaderValMtx, "gWorld", world);
		SetShaderValue(e_ShaderValMtx, "gWorldInvTranspose", worldInvTranspose);
		SetShaderValue(e_ShaderValMtx, "gWorldViewProj", worldViewProj);
		SetShaderValue(e_ShaderValMtx, "gView", view);
		SetShaderValue(e_ShaderValMtx, "gViewInvTranspose", viewInvTranspose);
		SetShaderValue(e_ShaderValMtx, "gProj", proj);
		SetShaderValue(e_ShaderValMtx, "gProjInvTranspose", ProjInvTranspose);
	}

	template <class T>
	void UpdateWorldMtxIns(T& world, T& worldInvTranspose, T& viewProj, T& view, T& viewInvTranspose, T& proj, T& ProjInvTranspose)
	{
		SetShaderValue(e_ShaderValMtx, "gWorld", world);
		SetShaderValue(e_ShaderValMtx, "gWorldInvTranspose", worldInvTranspose);
		SetShaderValue(e_ShaderValMtx, "gViewProj", viewProj);
		SetShaderValue(e_ShaderValMtx, "gView", view);
		SetShaderValue(e_ShaderValMtx, "gViewInvTranspose", viewInvTranspose);
		SetShaderValue(e_ShaderValMtx, "gProj", proj);
		SetShaderValue(e_ShaderValMtx, "gProjInvTranspose", ProjInvTranspose);
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
	// FX �ʱ�ȭ & ����
	void InitFX()
	{
		// ����
		BuildFX(e_ShaderColor, L"PC.fx", "ColorTech");

		// �� ���̴�
		BuildFX(e_ShaderLight, L"PL.fx", "LightTech");

		// �����̴�, �ؽ�ó
		BuildFX(e_ShaderPongTex, L"PNT.fx", "PongTex");

		// ī�����̴�, �ؽ�ó
		BuildFX(e_ShaderCartoonTex, L"PNT.fx", "CartoonTex");

		// ���۵� ������
		BuildFX(e_ShaderDeferred, L"Deferred.fx", "Deferred");
	}

	void BuildFX(SHADER_TYPE _ShaderMode, const wchar_t* _FxName, char* _TechniqueName)
	{
		// ���̴� ��
		mShaderMode = _ShaderMode;

		// �������
		EffectStorage* tEffectStorage = new EffectStorage();

		// FX ���� �ε�
		LoadFX(tEffectStorage, _FxName);

		// ��ũ�� ����� �� �ְԲ� �� ���
		GetTech(tEffectStorage, _TechniqueName);

		// ������� ������ �� �ְԲ� �� ��� (Get)
		GetShaderValue(tEffectStorage, "gWorld"			   , e_ShaderValMtx);
		GetShaderValue(tEffectStorage, "gWorldInvTranspose", e_ShaderValMtx);
		GetShaderValue(tEffectStorage, "gView"		       , e_ShaderValMtx);
		GetShaderValue(tEffectStorage, "gViewInvTranspose" , e_ShaderValMtx);
		GetShaderValue(tEffectStorage, "gProj"			   , e_ShaderValMtx);
		GetShaderValue(tEffectStorage, "gProjInvTranspose" , e_ShaderValMtx);
		GetShaderValue(tEffectStorage, "gWorldViewProj"	   , e_ShaderValMtx);
		GetShaderValue(tEffectStorage, "gViewProj"		   , e_ShaderValMtx);
		GetShaderValue(tEffectStorage, "gTexTFMtx"		   , e_ShaderValMtx);
		GetShaderValue(tEffectStorage, "gLocTMMtx"		   , e_ShaderValMtx);
		GetShaderValue(tEffectStorage, "gWdTMMtx"          , e_ShaderValMtx);
		GetShaderValue(tEffectStorage, "gEyePosW"		   , e_ShaderValVtx);
		GetShaderValue(tEffectStorage, "gDirLight"		   , e_ShaderVal);
		GetShaderValue(tEffectStorage, "gPointLight"	   , e_ShaderVal);
		GetShaderValue(tEffectStorage, "gSpotLight"		   , e_ShaderVal);
		GetShaderValue(tEffectStorage, "gMaterial"		   , e_ShaderVal);
		GetShaderValue(tEffectStorage, "gFar"			   , e_ShaderVal);
		GetShaderValue(tEffectStorage, "gNear"             , e_ShaderVal);
		GetShaderValue(tEffectStorage, "gDiffuseTex"	   , e_ShaderValResource);
		GetShaderValue(tEffectStorage, "gSpecularTex"	   , e_ShaderValResource);
		GetShaderValue(tEffectStorage, "gNormalTex"		   , e_ShaderValResource);

		// IA ����
		CreateIA(tEffectStorage);

		// �⺻ ���� ����
		// ��� (���׸��� �Ŵ����� �ҷ��� �� ���, �ϴ� �̷��� ó��)
		Material tMat;
		tMat.Ambient  = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		tMat.Diffuse  = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		tMat.Specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

		// �⺻ ���� ����
		tEffectStorage->mfxValue["gMaterial"]->SetRawValue(&tMat, 0, sizeof(tMat));

		// ����
		mShader[mShaderMode] = tEffectStorage;
	}

	// FXO �ε� ���� �ڵ�
	void SampleFXO(SHADER_TYPE _ShaderMode)
	{
		// ���̴� ��
		mShaderMode = _ShaderMode;

		// �������
		EffectStorage* tEffectStorage = new EffectStorage(); // <- ClearClass() ���� ������.

		// fxo ������
		std::ifstream fin("Lighting.fxo", std::ios::binary);

		fin.seekg(0, std::ios_base::end);
		int size = (int)fin.tellg();
		fin.seekg(0, std::ios_base::beg);
		std::vector<char> compiledShader(size);

		fin.read(&compiledShader[0], size);
		fin.close();

		// ����Ʈ ����
		HR(D3DX11CreateEffectFromMemory(&compiledShader[0], size,
			0, cCoreStorage::GetInstance()->md3dDevice, &tEffectStorage->mFX));

		// ��ũ�� ����� �� �ְԲ� �� ���
		GetTech(tEffectStorage, "LightTech");

		// ������� ������ �� �ְԲ� �� ��� (Get)
		// ��Ʈ����
		GetShaderValue(tEffectStorage, "gWorldViewProj"    , e_ShaderValMtx);
		GetShaderValue(tEffectStorage, "gViewProj"         , e_ShaderValMtx);
		GetShaderValue(tEffectStorage, "gWorld"            , e_ShaderValMtx);
		GetShaderValue(tEffectStorage, "gWorldInvTranspose", e_ShaderValMtx);

		// ����
		GetShaderValue(tEffectStorage, "gEyePosW", e_ShaderValVtx);

		// ����
		GetShaderValue(tEffectStorage, "gDirLight"     , e_ShaderVal);
		GetShaderValue(tEffectStorage, "gPointLight"   , e_ShaderVal);
		GetShaderValue(tEffectStorage, "gSpotLight"    , e_ShaderVal);
		GetShaderValue(tEffectStorage, "gMaterial"	   , e_ShaderVal);

		// IA ����
		CreateIA(tEffectStorage);

		// �⺻ ���� ����
		// ���
		Material tMat;
		tMat.Ambient  = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		tMat.Diffuse  = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		tMat.Specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		tEffectStorage->mfxValue["gMaterial"]->SetRawValue(&tMat, 0, sizeof(tMat));

		// ����
		mShader[mShaderMode] = tEffectStorage;
	}

	// FX �ε� ���� �ڵ�
	void SampleFX(SHADER_TYPE _ShaderMode)
	{
		// ���̴� ��
		mShaderMode = _ShaderMode;

		// �������
		EffectStorage* tEffectStorage = new EffectStorage();

		// FX ���� �ε�
		LoadFX(tEffectStorage, L"Lighting.fx");

		// ��ũ�� ����� �� �ְԲ� �� ���
		GetTech(tEffectStorage, "LightTech");

		// ������� ������ �� �ְԲ� �� ��� (Get)
		// ��Ʈ����
		GetShaderValue(tEffectStorage, "gWorldViewProj"    , e_ShaderValMtx);
		GetShaderValue(tEffectStorage, "gViewProj"		   , e_ShaderValMtx);
		GetShaderValue(tEffectStorage, "gWorld"			   , e_ShaderValMtx);
		GetShaderValue(tEffectStorage, "gWorldInvTranspose", e_ShaderValMtx);

		// ����
		GetShaderValue(tEffectStorage, "gEyePosW", e_ShaderValVtx);

		// ����
		GetShaderValue(tEffectStorage, "gDirLight"  , e_ShaderVal);
		GetShaderValue(tEffectStorage, "gPointLight", e_ShaderVal);
		GetShaderValue(tEffectStorage, "gSpotLight" , e_ShaderVal);
		GetShaderValue(tEffectStorage, "gMaterial"  , e_ShaderVal);

		// IA ����
		CreateIA(tEffectStorage);

		// �⺻ ���� ����
		// ���
		Material tMat;
		tMat.Ambient  = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		tMat.Diffuse  = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		tMat.Specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		tEffectStorage->mfxValue["gMaterial"]->SetRawValue(&tMat, 0, sizeof(tMat));

		// ����
		mShader[mShaderMode] = tEffectStorage;
	}

	// �Է� ������ �ʱ�ȭ (FVF)
	void CreateIA(EffectStorage* tEffectStorage)
	{
		// ���̾ƿ� �ʱ�ȭ
		if (mShaderMode == e_ShaderColor)
		{
			// Create the vertex input layout.
			D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT   , 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA  , 0 },
				{ "COLOR"   , 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA  , 0 },
				{ "WORLD"   , 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0                           , D3D11_INPUT_PER_INSTANCE_DATA, 1 },
				{ "WORLD"   , 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16                          , D3D11_INPUT_PER_INSTANCE_DATA, 1 },
				{ "WORLD"   , 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32                          , D3D11_INPUT_PER_INSTANCE_DATA, 1 },
				{ "WORLD"   , 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48                          , D3D11_INPUT_PER_INSTANCE_DATA, 1 }
			};

			// Create the input layout
			NEW_IA(tEffectStorage, vertexDesc, _countof(vertexDesc));
		}
		else if (mShaderMode == e_ShaderLight)
		{
			D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT   , 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA  , 0 },
				{ "NORMAL"  , 0, DXGI_FORMAT_R32G32B32_FLOAT   , 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA  , 0 },
				{ "WORLD"   , 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0                           , D3D11_INPUT_PER_INSTANCE_DATA, 1 },
				{ "WORLD"   , 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16                          , D3D11_INPUT_PER_INSTANCE_DATA, 1 },
				{ "WORLD"   , 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32                          , D3D11_INPUT_PER_INSTANCE_DATA, 1 },
				{ "WORLD"   , 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48                          , D3D11_INPUT_PER_INSTANCE_DATA, 1 }	
			};

			// Create the input layout
			NEW_IA(tEffectStorage, vertexDesc, _countof(vertexDesc));
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
				{ "WORLD"   , 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0                           , D3D11_INPUT_PER_INSTANCE_DATA, 1 },
				{ "WORLD"   , 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16                          , D3D11_INPUT_PER_INSTANCE_DATA, 1 },
				{ "WORLD"   , 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32                          , D3D11_INPUT_PER_INSTANCE_DATA, 1 },
				{ "WORLD"   , 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48                          , D3D11_INPUT_PER_INSTANCE_DATA, 1 }		
			};
			// Create the input layout
			NEW_IA(tEffectStorage, vertexDesc, _countof(vertexDesc));
		}
		else if (mShaderMode == e_ShaderDeferred)
		{
			D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT   , 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA  , 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT      , 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA  , 0 },
				{ "WORLD"   , 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0                           , D3D11_INPUT_PER_INSTANCE_DATA, 1 },
				{ "WORLD"   , 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16                          , D3D11_INPUT_PER_INSTANCE_DATA, 1 },
				{ "WORLD"   , 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32                          , D3D11_INPUT_PER_INSTANCE_DATA, 1 },
				{ "WORLD"   , 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48                          , D3D11_INPUT_PER_INSTANCE_DATA, 1 }		
			};
			// Create the input layout
			NEW_IA(tEffectStorage, vertexDesc, _countof(vertexDesc));
		}
	}

	// IA ����
	void NEW_IA(EffectStorage* tEffectStorage, D3D11_INPUT_ELEMENT_DESC* vertexDesc, int _FVFNum)
	{
		// Create the input layout
		static D3DX11_PASS_DESC passDesc;

		// ���̴� ���� ��������
		tEffectStorage->mTech->GetPassByIndex(0)->GetDesc(&passDesc);

		// IA ����
		HR(cCoreStorage::GetInstance()->md3dDevice->CreateInputLayout(
			vertexDesc,
			_FVFNum,
			passDesc.pIAInputSignature,
			passDesc.IAInputSignatureSize,
			&tEffectStorage->mInputLayout));
	}

	// FX ���� �ε�
	void LoadFX(EffectStorage* tEffectStorage, const wchar_t* _Name)
	{
		DWORD shaderFlags = 0;
#if defined( DEBUG ) || defined( _DEBUG )
		shaderFlags |= D3D10_SHADER_DEBUG;
		shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif

		ID3D10Blob* compiledShader = 0;
		ID3D10Blob* compilationMsgs = 0;

		// ����Ʈ
		HRESULT hr = D3DCompileFromFile(_Name, 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, 0, "fx_5_0", shaderFlags,
			0, &compiledShader, &compilationMsgs);

		// compilationMsgs can store errors or warnings.
		if (compilationMsgs != 0)
		{
			//MessageBoxA(0, (char*)compilationMsgs->GetBufferPointer(), 0, 0);
			ReleaseCOM(compilationMsgs);
		}

		// Even if there are no compilationMsgs, check to make sure there were no other errors.
		if (FAILED(hr))
		{
			MessageBox(NULL, NULL, L"D3DX11CompileFromFile", MB_OK);
		}

		// ������
		HR(D3DX11CreateEffectFromMemory(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(),
			0, cCoreStorage::GetInstance()->md3dDevice, &tEffectStorage->mFX));

		// Done with compiled shader.
		ReleaseCOM(compiledShader);
	}

	// ���̴� ���� ������ ��� ( �ʱ�ȭ )
	void GetShaderValue(EffectStorage* tEffectStorage, char* _Name, int _ValueEnum)
	{
		switch (_ValueEnum)
		{
			// ��Ʈ����
		case e_ShaderValMtx:
			tEffectStorage->mfxMtx[_Name] = tEffectStorage->mFX->GetVariableByName(_Name)->AsMatrix();
			break;

			// ����
		case e_ShaderValVtx:
			tEffectStorage->mfxVtx[_Name] = tEffectStorage->mFX->GetVariableByName(_Name)->AsVector();
			break;

			// �Ϲ� ����
		case e_ShaderVal:
			tEffectStorage->mfxValue[_Name] = tEffectStorage->mFX->GetVariableByName(_Name);
			break;

			// ���ҽ�
		case e_ShaderValResource:
			tEffectStorage->mfxResource[_Name] = tEffectStorage->mFX->GetVariableByName(_Name)->AsShaderResource();
			break;
		}
	}

	// ���̴� ���� ������ ��� ( �ʱ�ȭ )
	void GetGBufferShaderValue(EffectStorage* tEffectStorage, char* _Name)
	{
		tEffectStorage->mfxResource[_Name] = tEffectStorage->mFX->GetVariableByName(_Name)->AsShaderResource();
	}

	// ���̴� ���� ������ ��� ( �ʱ�ȭ )
	void ClearGBufferShaderValue(EffectStorage* tEffectStorage, char* _Name)
	{
		tEffectStorage->mfxResource[_Name]->AsShaderResource()->SetResource(NULL);
	}

	// ��ũ�� �ʱ�ȭ
	void GetTech(EffectStorage* tEffectStorage, char* _Name)
	{
		tEffectStorage->mTech = tEffectStorage->mFX->GetTechniqueByName(_Name);
	}

};