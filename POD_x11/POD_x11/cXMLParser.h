#pragma once
class cXMLParser
{
public:
	//------------------------------------------------------------------//
	// �ϼ� �� ������
	//------------------------------------------------------------------//

	// ���� ������� ���� ��ġ����
	vector<CharBuf> mNewModleLoc;

	// ���� ������� ���� ����
	MyMeshData mMyMeshData;

	//------------------------------------------------------------------//	
private:
	cCoreStorage* mCoreStorage = cCoreStorage::GetInstance();

	// ���� ������
	FILE* mFilePointer = nullptr;

	// ����
	char mReadBuf[BUF_SIZE];
	char mSaveBuf[BUF_SIZE];
	
public:
	cXMLParser()
	{
		ClearClass();
		InitClass();
	}

	~cXMLParser()
	{
		ClearClass();
	}

	void InitClass()
	{
		ClearPointer();
		mMyMeshData.InitClass();
	}

	// ���� �ʱ�ȭ
	void ClearClass()
	{
		ClearPointer();
		mNewModleLoc.clear();
	}

	void ClearPointer()
	{
		mMyMeshData.ClearClass();

		memset(&mReadBuf, '\0', sizeof(mReadBuf));
		memset(&mSaveBuf, '\0', sizeof(mSaveBuf));

		mFilePointer  = nullptr;
	}

	// �Ľ�
	void LoadXMLRoute(string& _ModelRoute)
	{
		// �ʱ�ȭ
		ClearPointer();

		// XML ��Ʈ ������ �б�
		ReadLocMyFormat_Model(_ModelRoute);
	}

	// �Ľ�
	void LoadXMLModel(string& _ModelData, InitMetaData* _InitMetaData)
	{
		// XML �� ������ �б�
		ReadDataMyFormat_Model(_ModelData); 

		// ���� ������ ������ �ֱ�
		SaveMyFormat(_InitMetaData);
	}

	// �Ľ�
	void LoadXMLSkinModel(string& _ModelData, InitMetaData* _InitMetaData, InitMetaData* _UseMetaData, FSM_TYPE _fsmType)
	{
		// ������ �а�, �����ϱ� (���ؽ�)
		LoadVtx(_ModelData, _InitMetaData);
		
		// ��Ų �� �ؽ�ó ����� �ű��
		MakeSkinModelTex(_InitMetaData->Vertices, _UseMetaData, _fsmType);
	}

	// ��Ų �� �ؽ�ó ����� �ű��
	void MakeSkinModelTex(vector<Vertex>& _compareVtxArr, InitMetaData* _UseMetaData, FSM_TYPE _fsmType)
	{
		// ������ ���� �𵨿�, ������ ���� ���� ����
		_UseMetaData->mSkinModelTex[(int)_fsmType] = new SkinTexture();

		// ������ ����ϰ� ���� ���
		CalData(_compareVtxArr, _UseMetaData, _fsmType, _compareVtxArr.size());
	}

	// ��Ų �� �ؽ�ó ����� �ű��
	void MakeSkinMapTex(vector<Vertex>& _compareVtxArr, InitMetaData* _UseMetaData, int _mapType, string _AddName)
	{
		// ������ ���� �𵨿�, ������ ���� ���� ���� (�ϴ� 0����, ���ٸ� ���°��� ���־���. ���߿� �ʿ��ϸ� Ȯ���� ����)
		_UseMetaData->mSkinModelTex[_mapType] = new SkinTexture();

		// ������ ����ϰ� ���� ���
		CalMapData(_compareVtxArr, _UseMetaData, _mapType, _compareVtxArr.size(), _AddName);
	}

	// �� ������ ���
	void CalMapData(vector<Vertex>& _compareVtxArr, InitMetaData* _UseMetaData, int _fsmType, int _VtxSize, string _AddName)
	{
		string tTexName;
		
		tTexName += "Export/SkinMapTex/";
		tTexName += _UseMetaData->mObjName;
		tTexName += _AddName;
		tTexName += ".dds";

		//--------------------------------------------------------------------------------------------------//
		// �ִ� ������ ä���
		//--------------------------------------------------------------------------------------------------//
		// �̸� ����
		_UseMetaData->mSkinModelTex[_fsmType]->mName = _UseMetaData->mObjName;

		// �ִ� Ÿ�� ����
		_UseMetaData->mSkinModelTex[_fsmType]->mAniType = (float)_fsmType;

		// �ؽ�ó �ػ�
		_UseMetaData->mSkinModelTex[_fsmType]->mTexWidth  = 4.0f;
		_UseMetaData->mSkinModelTex[_fsmType]->mTexHeight = (float)_VtxSize;

		//--------------------------------------------------------------------------------------------------//
		// ���� ��� ���ڿ�
		//--------------------------------------------------------------------------------------------------//
		wstring _WsTexName;
		StringToWchar_t(tTexName, _WsTexName);

		//--------------------------------------------------------------------------------------------------//
		// �ؽ�ó ����
		//--------------------------------------------------------------------------------------------------//
		RetryLoadSkinModelTex:
		D3DX11_IMAGE_LOAD_INFO _info;
		_info.Width          = (UINT)_UseMetaData->mSkinModelTex[_fsmType]->mTexWidth;
		_info.Height         = (UINT)_UseMetaData->mSkinModelTex[_fsmType]->mTexHeight; 
		_info.Depth			 = 0;
		_info.FirstMipLevel  = 0;
		_info.MipLevels      = 1;
		_info.Usage          = D3D11_USAGE_DEFAULT;
		_info.BindFlags      = D3D11_BIND_SHADER_RESOURCE;
		_info.CpuAccessFlags = 0;
		_info.MiscFlags      = 0;
		_info.Format         = DXGI_FORMAT_R32G32B32A32_FLOAT;
		_info.Filter         = D3DX11_FILTER_POINT;
		_info.MipFilter      = D3DX11_FILTER_POINT;
		_info.pSrcInfo       = NULL;

		HRESULT hr = D3DX11CreateShaderResourceViewFromFile(cCoreStorage::GetInstance()->md3dDevice, _WsTexName.c_str(), &_info, 0, &_UseMetaData->mSkinModelTex[_fsmType]->mTexSRV, 0);

		// �ؽ�ó�� ����
		if (hr == D3D11_ERROR_FILE_NOT_FOUND)
		{
			// ���ٸ� �ؽ�ó ����
			cout << "MapSkinTex ������ ���� �����մϴ�. �ټ� �ð��� �ɸ� �� �ֽ��ϴ�." << endl;

			// �������� ���
			CalNormalVtx(_UseMetaData);
			// ���� �ø���
			CalVtxINCAndSet(_UseMetaData);
			// ź��Ʈ ���� ���
			CalTangentSpace(_UseMetaData);

			// ��Ų �ؽ�ó �����
			MakeSkinTex(_compareVtxArr, _WsTexName, _UseMetaData, _fsmType, _VtxSize);

			// �ؽ�ó �� ��Ų ���ε�
			goto RetryLoadSkinModelTex;
		}
		else if (hr == S_OK)
			cout << "MapSkinTex �ε� ����" << endl;
		else
			cout << ">> ��õ��� ���� ���� �߻�. SkinTex ���� <<" << endl;

		_WsTexName.clear();
	}

	// ������ ���
	void CalData(vector<Vertex>& _compareVtxArr, InitMetaData* _UseMetaData, int _fsmType, int _VtxSize)
	{
		string _TexName; SelectName((FSM_TYPE)_fsmType, _TexName);
		string tTexName;
		
		tTexName += "Export/SkinModelTex/";
		tTexName += _UseMetaData->mObjName;
		tTexName += _TexName;
		tTexName += ".dds";

		//--------------------------------------------------------------------------------------------------//
		// �ִ� ������ ä���
		//--------------------------------------------------------------------------------------------------//
		// �̸� ����
		_UseMetaData->mSkinModelTex[_fsmType]->mName = _TexName;

		// �ִ� Ÿ�� ����
		_UseMetaData->mSkinModelTex[_fsmType]->mAniType = (float)_fsmType;

		// �ؽ�ó �ػ�
		_UseMetaData->mSkinModelTex[_fsmType]->mTexWidth  = 4.0f;
		_UseMetaData->mSkinModelTex[_fsmType]->mTexHeight = (float)_VtxSize;

		//--------------------------------------------------------------------------------------------------//
		// ���� ��� ���ڿ�
		//--------------------------------------------------------------------------------------------------//
		wstring _WsTexName;
		StringToWchar_t(tTexName, _WsTexName);

		//--------------------------------------------------------------------------------------------------//
		// �ؽ�ó ����
		//--------------------------------------------------------------------------------------------------//
		RetryLoadSkinModelTex:
		D3DX11_IMAGE_LOAD_INFO _info;
		_info.Width          = (UINT)_UseMetaData->mSkinModelTex[_fsmType]->mTexWidth;
		_info.Height         = (UINT)_UseMetaData->mSkinModelTex[_fsmType]->mTexHeight; 
		_info.Depth			 = 0;
		_info.FirstMipLevel  = 0;
		_info.MipLevels      = 1;
		_info.Usage          = D3D11_USAGE_DEFAULT;
		_info.BindFlags      = D3D11_BIND_SHADER_RESOURCE;
		_info.CpuAccessFlags = 0;
		_info.MiscFlags      = 0;
		_info.Format         = DXGI_FORMAT_R32G32B32A32_FLOAT;
		_info.Filter         = D3DX11_FILTER_POINT;
		_info.MipFilter      = D3DX11_FILTER_POINT;
		_info.pSrcInfo       = NULL;

		HRESULT hr = D3DX11CreateShaderResourceViewFromFile(cCoreStorage::GetInstance()->md3dDevice, _WsTexName.c_str(), &_info, 0, &_UseMetaData->mSkinModelTex[_fsmType]->mTexSRV, 0);

		// �ؽ�ó�� ����
		if (hr == D3D11_ERROR_FILE_NOT_FOUND)
		{
			// ���ٸ� �ؽ�ó ����
			cout << "ModelSkinTex ������ ���� �����մϴ�. �ټ� �ð��� �ɸ� �� �ֽ��ϴ�." << endl;

			// ��Ų �ؽ�ó �����
			MakeSkinTex(_compareVtxArr, _WsTexName, _UseMetaData, _fsmType, _VtxSize);

			// �ؽ�ó �� ��Ų ���ε�
			goto RetryLoadSkinModelTex;
		}
		else if (hr == S_OK)
			cout << "ModelSkinTex �ε� ����" << endl;
		else
			cout << ">> ��õ��� ���� ���� �߻�. SkinTex ���� <<" << endl;

		_WsTexName.clear();
	}
	
	// �ؽ�ó ����
	void WriteTex(vector<Vertex>& _compareVtxArr, InitMetaData* _UseMetaData, int _fsmType)
	{
		D3D11_MAPPED_SUBRESOURCE MappedResource;

		// ���� ����
		HRESULT hr = cCoreStorage::GetInstance()->md3dImmediateContext->Map(_UseMetaData->mSkinModelTex[_fsmType]->mTexture, //������ �ؽ�ó
			D3D11CalcSubresource(0, 0, 1),																				     //���� ���ҽ� ��ȣ
			D3D11_MAP_WRITE_DISCARD,																					     //���ҽ��� ����
			0,																											    
			&MappedResource);																							     //�����͸� �� ������

		// ���� ������������
		if (hr == S_OK)
		{
			// ������ ���� ���۷��� ���
			float* pTexels = (float*)MappedResource.pData;
			
			// �� �ݺ� (���ؽ� ����)
			for (unsigned int y = 0; y < _compareVtxArr.size(); ++y)
			{
				for (int i = 0; i < 4; ++i)
				{
					// �ȼ� ����
					int _sTex1 = (y * MappedResource.RowPitch / sizeof(float)) + (i * sizeof(float)) + 0;
					int _sTex2 = (y * MappedResource.RowPitch / sizeof(float)) + (i * sizeof(float)) + 1;
					int _sTex3 = (y * MappedResource.RowPitch / sizeof(float)) + (i * sizeof(float)) + 2;
					int _sTex4 = (y * MappedResource.RowPitch / sizeof(float)) + (i * sizeof(float)) + 3;

					switch (i)
					{
					case 0:
						pTexels[_sTex1] = _compareVtxArr[y].Position.x;     //R (float 1)
						pTexels[_sTex2] = _compareVtxArr[y].Position.y;     //G (float 2)
						pTexels[_sTex3] = _compareVtxArr[y].Position.z;     //B (float 3)
						pTexels[_sTex4] = 1.0f;							    //A (float 4)
						break;
					case 1:
						pTexels[_sTex1] = _compareVtxArr[y].Normal.x;       //R (float 1)
						pTexels[_sTex2] = _compareVtxArr[y].Normal.y;       //G (float 2)
						pTexels[_sTex3] = _compareVtxArr[y].Normal.z;       //B (float 3)
						pTexels[_sTex4] = 1.0f;							    //A (float 4)
						break;
					case 2:
						pTexels[_sTex1] = _compareVtxArr[y].TangentU.x;     //R (float 1)
						pTexels[_sTex2] = _compareVtxArr[y].TangentU.y;     //G (float 2)
						pTexels[_sTex3] = _compareVtxArr[y].TangentU.z;     //B (float 3)
						pTexels[_sTex4] = 1.0f;							    //A (float 4)
						break;
					case 3:
						pTexels[_sTex1] = _compareVtxArr[y].BiNormal.x;     //R (float 1)
						pTexels[_sTex2] = _compareVtxArr[y].BiNormal.y;     //G (float 2)
						pTexels[_sTex3] = _compareVtxArr[y].BiNormal.z;     //B (float 3)
						pTexels[_sTex4] = 1.0f;							    //A (float 4)
						break;
					default:
						cout << "��� ���� �ʰ�" << endl;
						break;
					}
				}
			}

			// ���� �ݱ�
			cCoreStorage::GetInstance()->md3dImmediateContext->Unmap(_UseMetaData->mSkinModelTex[_fsmType]->mTexture, D3D11CalcSubresource(0, 0, 1));
		}
		else
			cout << "��Ų �� �ؽ�ó ���� ����" << endl;
	}

	// �ؽ�ó ����
	void CreateTex(InitMetaData* _UseMetaData, int _fsmType, int _VtxSize)
	{
		D3D11_TEXTURE2D_DESC TextureDesc;
		ZeroMemory(&TextureDesc, sizeof(TextureDesc));

		TextureDesc.Width              = 4;					          // �� �ȼ��� Position, Normal, TangentU, BiNormal
		TextureDesc.Height             = _VtxSize;			          // ���ؽ� ������ ��ŭ 				
		TextureDesc.MipLevels          = 1;                             						
		TextureDesc.ArraySize          = 1;                             						
		TextureDesc.Format             = DXGI_FORMAT_R32G32B32A32_FLOAT;
		TextureDesc.SampleDesc.Count   = 1;  
		TextureDesc.SampleDesc.Quality = 0;
		TextureDesc.Usage              = D3D11_USAGE_DYNAMIC;
		TextureDesc.BindFlags          = D3D11_BIND_SHADER_RESOURCE;
		TextureDesc.CPUAccessFlags     = D3D11_CPU_ACCESS_WRITE;
		TextureDesc.MiscFlags		   = 0;

		HR(cCoreStorage::GetInstance()->md3dDevice->CreateTexture2D(&TextureDesc, 0, &_UseMetaData->mSkinModelTex[_fsmType]->mTexture));
	}

	// �ؽ�ó ����
	void SaveTex(wstring& _TexName, InitMetaData* _UseMetaData, int _fsmType)
	{
		D3DX11SaveTextureToFile(cCoreStorage::GetInstance()->md3dImmediateContext,
			_UseMetaData->mSkinModelTex[_fsmType]->mTexture,      //������ �ؽ�ó
			D3DX11_IFF_DDS,								      //dds�� ����
			_TexName.c_str());							      //�̸�

		// �ؽ�ó�� ���������, ���� �����ʹ� ����
		_UseMetaData->mSkinModelTex[_fsmType]->ClearTex();
	}

	// �ؽ�ó�� ���� �̸� �����ϱ�
	void SelectName(int _fsmType, string& _TexName)
	{
		// �̸� ����
		switch (_fsmType)
		{
		default:
		case e_Idle:
			_TexName = "Idle";
			break;
		case e_Damage:
			_TexName = "Damage";
			break;
		case e_Run:
			_TexName = "Run";
			break;
		case e_Walk:
			_TexName = "Walk";
			break;
		case e_Death:
			_TexName = "Death";
			break;
		case e_Attack:
			_TexName = "Attack";
			break;
		}
	}

	// ��Ų �ؽ�ó �����
	void MakeSkinTex(vector<Vertex>& _compareVtxArr, wstring& _TexName, InitMetaData* _UseMetaData, int _fsmType, int _VtxSize)
	{
		// �ؽ�ó ����
		CreateTex(_UseMetaData, _fsmType, _VtxSize);

		// �ؽ�ó ����
		WriteTex(_compareVtxArr, _UseMetaData, _fsmType);

		// �ؽ�ó ����
		SaveTex(_TexName, _UseMetaData, _fsmType);
	}

	void LoadVtx(string& _FinExportModelData, InitMetaData* _InitMetaData)
	{
		//-----------------------------------------------------------//
		// ������ �б�
		//-----------------------------------------------------------//
		// ���� ���� ����
		FileOpen(_FinExportModelData.c_str(), "rb");
		int len = 0;

		// ���ؽ�
		fread(&len, sizeof(int), 1, mFilePointer);
		mMyMeshData.vertices.resize(len);

		for (int i = 0; i < len; ++i)
			fread(&mMyMeshData.vertices[i], sizeof(Vertex), 1, mFilePointer);

		// ���� ����
		FileClose();
		printf("[Succes] ���� ����.\n");
		

		//-----------------------------------------------------------//
		// ������ ����
		//-----------------------------------------------------------//
		// ���ؽ�
		_InitMetaData->Vertices = mMyMeshData.vertices;


	}

	// ���� �ڽ� �����
	// �ڽ� �����
	void LoadBox(InitMetaData* _InitMetaData, float width, float height, float depth)
	{
		Vertex v[8];

		float w2 = 0.5f * width;
		float h2 = 0.5f * height;
		float d2 = 0.5f * depth;

		// Fill in the front face vertex data.
		v[0] = Vertex(-w2, +h2, -d2, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
		v[1] = Vertex(+w2, +h2, -d2, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
		v[2] = Vertex(-w2, -h2, -d2, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
		v[3] = Vertex(+w2, -h2, -d2, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

		// Fill in the back face vertex data.
		v[4] = Vertex(-w2, +h2, +d2, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
		v[5] = Vertex(+w2, +h2, +d2, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
		v[6] = Vertex(-w2, -h2, +d2, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
		v[7] = Vertex(+w2, -h2, +d2, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

		_InitMetaData->Vertices.assign(&v[0], &v[8]);


		// Create the indices.
		UINT i[36];

		i[0] = 0; i[1] = 1; i[2] = 2;
		i[3] = 1; i[4] = 3; i[5] = 2;

		i[6] = 1; i[7] = 5; i[8] = 3;
		i[9] = 5; i[10] = 7; i[11] = 3;

		i[12] = 5; i[13] = 4; i[14] = 7;
		i[15] = 4; i[16] = 6; i[17] = 7;

		i[18] = 4; i[19] = 0; i[20] = 6;
		i[21] = 0; i[22] = 2; i[23] = 6;

		i[24] = 4; i[25] = 5; i[26] = 0;
		i[27] = 5; i[28] = 1; i[29] = 0;

		i[30] = 2; i[31] = 3; i[32] = 6;
		i[33] = 3; i[34] = 7; i[35] = 6;

		_InitMetaData->Indices.assign(&i[0], &i[36]);


		// �ؽ�ó
		XMFLOAT3 tv[8];

		// uv��ǥ ���� ���� 
		// - ���� ���׸��� �ȵǰ� ����
		tv[0] = XMFLOAT3(0.0f, 0.0f, 0.0f);
		tv[1] = XMFLOAT3(1.0f, 0.0f, 0.0f);
		tv[2] = XMFLOAT3(0.0f, 1.0f, 0.0f);
		tv[3] = XMFLOAT3(1.0f, 1.0f, 0.0f);
		tv[4] = XMFLOAT3(1.0f, 0.0f, 0.0f);
		tv[5] = XMFLOAT3(0.0f, 0.0f, 0.0f);
		tv[6] = XMFLOAT3(1.0f, 1.0f, 0.0f);
		tv[7] = XMFLOAT3(0.0f, 1.0f, 0.0f);

		_InitMetaData->TexVertices.assign(&tv[0], &tv[8]);

		// �ؽ�Ʈ �ε��� ��ǥ ���� ����
		UINT ti[36];

		ti[0] = 0; ti[1] = 1; ti[2] = 2;
		ti[3] = 1; ti[4] = 3; ti[5] = 2;

		ti[6] = 1; ti[7] = 5; ti[8] = 3;
		ti[9] = 5; ti[10] = 7; ti[11] = 3;

		ti[12] = 5; ti[13] = 4; ti[14] = 7;
		ti[15] = 4; ti[16] = 6; ti[17] = 7;

		ti[18] = 4; ti[19] = 0; ti[20] = 6;
		ti[21] = 0; ti[22] = 2; ti[23] = 6;

		ti[24] = 4; ti[25] = 5; ti[26] = 0;
		ti[27] = 5; ti[28] = 1; ti[29] = 0;

		ti[30] = 2; ti[31] = 3; ti[32] = 6;
		ti[33] = 3; ti[34] = 7; ti[35] = 6;

		_InitMetaData->TexIndices.assign(&ti[0], &ti[36]);
	}

	// ���Ǿ� �����
	void LoadSphere(InitMetaData* _InitMetaData, float& _radius, UINT& _sliceCount, UINT& _stackCount)
	{
		//
		// ��� �������ִ� �������� ����ϰ� ���� �Ʒ��� �̵��մϴ�.
		//

		// Poles : �ؽ�ó ��ǥ �ְ������� ���Դϴ�.
		// ���� �� �� �ؿ� �Ҵ� �� �ؽ�ó �ʻ��� ������ ���� �ƴմϴ�.
		// ������ �簢�� �ؽ�ó.

		Vertex topVertex   (0.0f, +_radius, 0.0f, 0.0f, +1.0f, 0.0f, 1.0f, 0.0f, 0.0f);
		Vertex bottomVertex(0.0f, -_radius, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f);
		
		topVertex.TexUV.x    = 0.0f;
		topVertex.TexUV.y    = 0.0f;
		bottomVertex.TexUV.x = 0.0f;
		bottomVertex.TexUV.y = 1.0f;

		_InitMetaData->Vertices   .push_back(topVertex);
		_InitMetaData->TexVertices.push_back(XMFLOAT3(topVertex.TexUV.x, topVertex.TexUV.y, 0.0f));


		float phiStep   = XM_PI / _stackCount;
		float thetaStep = 2.0f * XM_PI / _sliceCount;

		// �� ���� ���� ���� ������ ����մϴ� (���� ������ ������� ����).
		for (UINT i = 1; i <= _stackCount - 1; ++i)
		{
			float phi = i * phiStep;

			// ���� ����.
			for (UINT j = 0; j <= _sliceCount; ++j)
			{
				float theta = j * thetaStep;

				Vertex v;

				// spherical to cartesian
				v.Position.x = _radius * sinf(phi) * cosf(theta);
				v.Position.y = _radius * cosf(phi);
				v.Position.z = _radius * sinf(phi) * sinf(theta);

				// ��Ÿ�� ���� P�� �κ� �Ļ�
				v.TangentU.x = -_radius * sinf(phi) * sinf(theta);
				v.TangentU.y = 0.0f;
				v.TangentU.z = +_radius * sinf(phi) * cosf(theta);

				XMVECTOR T = XMLoadFloat3(&v.TangentU);
				XMStoreFloat3(&v.TangentU, XMVector3Normalize(T));

				XMVECTOR p = XMLoadFloat3(&v.Position);
				XMStoreFloat3(&v.Normal, XMVector3Normalize(p));

				v.TexUV.x = theta / XM_2PI;
				v.TexUV.y = phi   / XM_PI;

				_InitMetaData->Vertices   .push_back(v);
				_InitMetaData->TexVertices.push_back(XMFLOAT3(v.TexUV.x, v.TexUV.y, 0.0f));
			}
		}

		_InitMetaData->Vertices   .push_back(bottomVertex);
		_InitMetaData->TexVertices.push_back(XMFLOAT3(bottomVertex.TexUV.x, bottomVertex.TexUV.y, 0.0f));

		//
		// �ֻ��� ������ �ε����� ����մϴ�. �ֻ��� ������ ó���� ���� ���ۿ� ���������ϴ�.
		// ��� ���� ù ��° ���� �����մϴ�.
		//

		for (UINT i = 1; i <= _sliceCount; ++i)
		{
			_InitMetaData->Indices.push_back(0);
			_InitMetaData->Indices.push_back(i + 1);
			_InitMetaData->Indices.push_back(i);

			_InitMetaData->TexIndices.push_back(0);
			_InitMetaData->TexIndices.push_back(i + 1);
			_InitMetaData->TexIndices.push_back(i);
		}

		//
		// ���� ������ �ε����� ����մϴ� (���� ������� ����).
		//

		// ù ��° ������ ù ��° ���� �ε����� ���� �ε����� �������մϴ�.
		// �̰��� ���� �������� �ǳ� �ٰ� �ֽ��ϴ�.
		UINT baseIndex = 1;
		UINT ringVertexCount = _sliceCount + 1;
		for (UINT i = 0; i < _stackCount - 2; ++i)
		{
			for (UINT j = 0; j < _sliceCount; ++j)
			{
				_InitMetaData->Indices.push_back(baseIndex + i*ringVertexCount + j);
				_InitMetaData->Indices.push_back(baseIndex + i*ringVertexCount + j + 1);
				_InitMetaData->Indices.push_back(baseIndex + (i + 1)*ringVertexCount + j);

				_InitMetaData->Indices.push_back(baseIndex + (i + 1)*ringVertexCount + j);
				_InitMetaData->Indices.push_back(baseIndex + i*ringVertexCount + j + 1);
				_InitMetaData->Indices.push_back(baseIndex + (i + 1)*ringVertexCount + j + 1);

				_InitMetaData->TexIndices.push_back(baseIndex + i*ringVertexCount + j);
				_InitMetaData->TexIndices.push_back(baseIndex + i*ringVertexCount + j + 1);
				_InitMetaData->TexIndices.push_back(baseIndex + (i + 1)*ringVertexCount + j);
				
				_InitMetaData->TexIndices.push_back(baseIndex + (i + 1)*ringVertexCount + j);
				_InitMetaData->TexIndices.push_back(baseIndex + i*ringVertexCount + j + 1);
				_InitMetaData->TexIndices.push_back(baseIndex + (i + 1)*ringVertexCount + j + 1);
			}
		}

		//
		// �ϴ� ������ �ε����� ����մϴ�. �� �Ʒ� ������ ���������� ���ؽ� ���ۿ� ���������ϴ�.
		// �ϴ� ���� �ϴ� ���� �����մϴ�.
		//

		// ���������� ���� �������� �߰��Ǿ����ϴ�.
		UINT southPoleIndex = (UINT)_InitMetaData->Vertices.size() - 1;

		// ������ �������� ù ��° ������ �ε����� �ε����� �������մϴ�.
		baseIndex = southPoleIndex - ringVertexCount;

		for (UINT i = 0; i < _sliceCount; ++i)
		{
			_InitMetaData->Indices.push_back(southPoleIndex);
			_InitMetaData->Indices.push_back(baseIndex + i);
			_InitMetaData->Indices.push_back(baseIndex + i + 1);

			_InitMetaData->TexIndices.push_back(southPoleIndex);
			_InitMetaData->TexIndices.push_back(baseIndex + i);
			_InitMetaData->TexIndices.push_back(baseIndex + i + 1);
		}
	}

	void LoadScreen(InitMetaData* _InitMetaData, float width, float height)
	{
		Vertex v[8];

		float w2 = width;
		float h2 = height;

		// Fill in the front face vertex data.
		v[0] = Vertex(-w2, +h2, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
		v[1] = Vertex(+w2, +h2, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
		v[2] = Vertex(-w2, -h2, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
		v[3] = Vertex(+w2, -h2, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

		_InitMetaData->Vertices.assign(&v[0], &v[4]);


		// Create the indices.
		UINT i[6];

		i[0] = 0; i[1] = 1; i[2] = 2;
		i[3] = 1; i[4] = 3; i[5] = 2;

		_InitMetaData->Indices.assign(&i[0], &i[6]);


		// �ؽ�ó
		XMFLOAT3 tv[4];

		// uv��ǥ ���� ���� 
		// - ���� ���׸��� �ȵǰ� ����
		tv[0] = XMFLOAT3(0.0f, 0.0f, 0.0f);
		tv[1] = XMFLOAT3(1.0f, 0.0f, 0.0f);
		tv[2] = XMFLOAT3(0.0f, 1.0f, 0.0f);
		tv[3] = XMFLOAT3(1.0f, 1.0f, 0.0f);

		_InitMetaData->TexVertices.assign(&tv[0], &tv[4]);

		// �ؽ�Ʈ �ε��� ��ǥ ���� ����
		UINT ti[6];

		ti[0] = 0; ti[1] = 1; ti[2] = 2;
		ti[3] = 1; ti[4] = 3; ti[5] = 2;

		_InitMetaData->TexIndices.assign(&ti[0], &ti[6]);
	}

	// �� �����
	void LoadMap(InitMetaData* _InitMetaData, string& _hightMapPss, float& _Xwidth, float& _Zdepth, float& _CellSize, float& _HeightScale)
	{
		_InitMetaData->mHeightmap.resize ((int)(_Xwidth * _Zdepth), 0);
		_InitMetaData->Vertices.resize   ((int)(_Xwidth * _Zdepth));
		_InitMetaData->TexVertices.resize((int)(_Xwidth * _Zdepth));
		_InitMetaData->Indices   .resize((int)(((_Xwidth - 1) * (_Zdepth - 1)) * 6)); 
		_InitMetaData->TexIndices.resize((int)(((_Xwidth - 1) * (_Zdepth - 1)) * 6));


		LoadHeightmap(_InitMetaData, _hightMapPss, _Xwidth, _Zdepth, _HeightScale);
		Smooth       (_InitMetaData, _Xwidth, _Zdepth);
		CalMap       (_InitMetaData, _Xwidth, _Zdepth, _CellSize);

		// ����Ʈ�� ����
		for (int i = 0; i < (int)(_Xwidth * _Zdepth); ++i)
			_InitMetaData->Vertices[i].Position.y = _InitMetaData->mHeightmap[i];

		// ��Ų �� �ؽ�ó ����� �ű��
		MakeSkinMapTex(_InitMetaData->Vertices, _InitMetaData, 0, "");

		// ����Ʈ�� �ε�
		LoadHeightMap(_InitMetaData, _hightMapPss, _Xwidth, _Zdepth);
	}

	// ����Ʈ�� �ε�
	void LoadHeightMap(InitMetaData* _InitMetaData, string& _hightMapPss, float& _Xwidth, float& _Zdepth)
	{
		_InitMetaData->mSkinModelTex[1] = new SkinTexture();

		D3D11_TEXTURE2D_DESC texDesc;
		texDesc.Width              = (int)_Xwidth;
		texDesc.Height             = (int)_Zdepth;
		texDesc.MipLevels          = 1;
		texDesc.ArraySize          = 1;
		texDesc.Format             = DXGI_FORMAT_R16_FLOAT;
		texDesc.SampleDesc.Count   = 1;
		texDesc.SampleDesc.Quality = 0;
		texDesc.Usage              = D3D11_USAGE_DEFAULT;
		texDesc.BindFlags          = D3D11_BIND_SHADER_RESOURCE;
		texDesc.CPUAccessFlags     = 0;
		texDesc.MiscFlags          = 0;

		// HALF is defined in xnamath.h, for storing 16-bit float.
		std::vector<HALF> hmap((int)(_Xwidth * _Zdepth));
		std::transform(_InitMetaData->mHeightmap.begin(), _InitMetaData->mHeightmap.end(), hmap.begin(), XMConvertFloatToHalf);

		D3D11_SUBRESOURCE_DATA data;
		data.pSysMem             = &hmap[0];
		data.SysMemPitch         = (int)_Xwidth * sizeof(HALF);
		data.SysMemSlicePitch    = 0;

		ID3D11Texture2D* hmapTex = 0;
		HR(cCoreStorage::GetInstance()->md3dDevice->CreateTexture2D(&texDesc, &data, &_InitMetaData->mSkinModelTex[1]->mTexture));

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format                    = texDesc.Format;
		srvDesc.ViewDimension             = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels       = -1;
		HR(cCoreStorage::GetInstance()->md3dDevice->CreateShaderResourceView(_InitMetaData->mSkinModelTex[1]->mTexture, &srvDesc, &_InitMetaData->mSkinModelTex[1]->mTexSRV));

		// SRV saves reference.
		ReleaseCOM(_InitMetaData->mSkinModelTex[1]->mTexture);
	}

	// ���� �����ڵ� 2
	void CalVtxINCAndSet(InitMetaData* _MetaData)
	{
		// ��Ÿ�����͸� �����ؼ�, ������ �÷�����Ѵ�.
		// �ǵ���: ���ؽ� ��ǥ��, uv��ǥ�� ����ġ �Ҽ� �ִٴ� �Ϳ��� ���
		//         �׷��Ƿ�, ���ؽ� ��ǥ�� uv ��ǥ�� ���� �̾Ƴ�, �ΰ��� ��ġ�ϸ� ����ΰ�,
		//         ����ġ��, ���ؽ� ��ǥ�� �״�� ����ϰ�, uv ��ǥ�� �����Ͽ� ������ �߰��� �Ĺ̿� ��ġ�Ѵ�.
		//         ����, �߰��� ���������� ��������, �� �ε�������Ʈ�� �ٽ� �ۼ��Ѵ�.
		//
		// *�ٽ�: uv ��ǥ �ֱ���, ����ִ� �������� Ȯ��
		//        1) ����ִ� �����̶�� ����,
		//        2) ��� ���� �ʴ� �����̶��, ��
		//           -> �������� uv ��ǥ�� ��ġ�Ѵٸ�, �ƹ��۾� ���ϰ�, ���� ��ǥ��
		//           -> �������� uv ��ǥ�� ��ġ���� �ʴ´ٸ�, ���� ���� ( ���ؽ� ��ǥ ����, uv ��ǥ ���� )
		//        3) ������ �߰��ϸ�, ���� �˻��ϰ� �ִ� �� �ε�����, �ش� ���� ��ȣ�� �ִ´�.


		// ���� �ε��� ũ�⸸ŭ �ݺ� ( �ؽ�ó �ε����� �Ϲ������δ� �����ϹǷ�, ������ �� ó���� ���� ó�� �ʿ� )
		for (unsigned int i = 0; i < _MetaData->Indices.size(); ++i)
		{
			// ���� �ε��� ��ȣ�� �˻��Ѵ�.
			UINT _ModelIdx = _MetaData->Indices[i];    //  ���� ���� �ε���
			UINT _TexIdx = _MetaData->TexIndices[i]; // �ؽ�ó�� ���� �ε���

			// ������ �ε��� ��ȣ�� �ش��ϴ� uv���� ������.
			XMFLOAT2& ModelTexUV = _MetaData->Vertices[_ModelIdx].TexUV; //   �� �ε����� UV ��
			XMFLOAT3&      TexUV = _MetaData->TexVertices[_TexIdx];      // �ؽ�ó �ε����� UV ��

			// ���� ���� �ε����� ����Ű�� ���ؽ��� uv�� ����ִ°�?
			if (ModelTexUV.x == FLT_MAX || ModelTexUV.y == FLT_MAX)
			{
				// ����ִٸ�,
				// �ؽ�ó uv �� �ٷ� ����
				ModelTexUV.x = TexUV.x;
				ModelTexUV.y = TexUV.y;
			}
			else
			{
				// ��� ���� �ʴٸ�
				// ���� ���� UV�� ���� �ؽ�ó�� UV�� ��ġ�ϴ°�?
				if (ModelTexUV.x == TexUV.x && ModelTexUV.y == TexUV.y)
				{
					// ��ġ�Ѵٸ�, 

				}
				else
				{
					// ��ġ���� �ʴ´ٸ�, ���� �ø���
					// �ش� ���ؽ��� ������ ������
					Vertex ModelVtx = _MetaData->Vertices[_ModelIdx];

					// �ؽ�ó ���� ����
					ModelTexUV.x = TexUV.x;
					ModelTexUV.y = TexUV.y;

					// ����
					_MetaData->Vertices.push_back(ModelVtx);
				}
			}
		}
	}

	// ���� ���
	void CalNormalVtx(InitMetaData* _MetaData)
	{
		vector< vector<XMVECTOR> > face_normal;

		//�� ���
		face_normal.resize(_MetaData->Indices.size() / 3);
		for (unsigned int i = 0; i < face_normal.size(); ++i)
			face_normal[i].resize(2);

		//-----------------------------------//
		//�� ��ֹ���
		//-----------------------------------//
		//�ﰢ�� ������ŭ �ݺ�
		for (int fi = 0; fi < (int)_MetaData->Indices.size() / 3; ++fi)
		{
			//	�ﰢ�� �����͸� ��´�.
			WORD index1 = _MetaData->Indices[fi * 3 + 0];
			WORD index2 = _MetaData->Indices[fi * 3 + 1];
			WORD index3 = _MetaData->Indices[fi * 3 + 2];

			//	�ﰢ���� �ش��ϴ� ����3���� ���Ѵ�.
			XMVECTOR vtx1 = XMLoadFloat3(&_MetaData->Vertices[index1].Position);
			XMVECTOR vtx2 = XMLoadFloat3(&_MetaData->Vertices[index2].Position);
			XMVECTOR vtx3 = XMLoadFloat3(&_MetaData->Vertices[index3].Position);

			// �麤���� �߰� ������ �����Ѵ�.
			face_normal[fi][0] = vtx1 + vtx2 + vtx3;

			//	�Ѱ� �������� �ٸ� �ΰ� ������ ���ϴ� ���⺤�� �ΰ��� ���Ѵ�.
			XMVECTOR vtx_dir1 = vtx2 - vtx1;
			XMVECTOR vtx_dir2 = vtx3 - vtx1;

			//	���� ���� 2���� ����ȭ �Ѵ�.
			vtx_dir1 = XMVector3Normalize(vtx_dir1);
			vtx_dir2 = XMVector3Normalize(vtx_dir2);

			//	�����Ͽ� �������͸� ���Ѵ�.
			XMVECTOR normal_dir = XMVector3Cross(vtx_dir1, vtx_dir2);

			// �麤���� ��ģ ������ �����Ѵ�.
			face_normal[fi][1] = face_normal[fi][0] + normal_dir * 1.0f;

		} //�ݺ�

		//-----------------------------------//
		//���� ��ֹ���
		//-----------------------------------//
		//����������ŭ �ݺ� 
		for (unsigned int vi = 0; vi < _MetaData->Vertices.size(); ++vi) //<- ������ȣ 0���� �����˻�
		{
			//	�������� 0,0,0 �ʱ�ȭ 
			XMVECTOR result_vec = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

			//	�������� 0 �ʱ�ȭ 
			int result_count = 0;

			//	�ﰢ�� ������ŭ �ݺ�
			for (unsigned int fi = 0; fi < _MetaData->Indices.size() / 3; ++fi)
			{
				//		�ﰢ�� �����͸� ��´�.
				WORD index1 = _MetaData->Indices[fi * 3 + 0];
				WORD index2 = _MetaData->Indices[fi * 3 + 1];
				WORD index3 = _MetaData->Indices[fi * 3 + 2];

				//		�ﰢ�� ������ �߿� ���� �ε����� �ճ�?
				if ((index1 == (WORD)vi) || (index2 == (WORD)vi) || (index3 == (WORD)vi)) //<- vi�� ������ȣ, index���� ������ ������ȣ�� ���ִ�.
				{   //		��
					//			�������� �� �� �������͸� ���Ѵ�.
					XMVECTOR tmp_dir = face_normal[fi][1] - face_normal[fi][0]; //���⺤�͸� �����ؾ���.
					tmp_dir = XMVector3Normalize(tmp_dir);

					// ��� ���Ϳ� ����
					result_vec += tmp_dir;

					// �������� 1 ����.
					++result_count;
				}
			}//	�ݺ�
			//	�������� /= ��������
			result_vec /= (float)result_count;

			//��ֶ�����
			result_vec = XMVector3Normalize(result_vec);

			//����(����)�� �������� ����
			XMStoreFloat3(&_MetaData->Vertices[vi].Normal, result_vec);

		}//�ݺ�

		// �ڿ� ����
		// �ε���
		for (unsigned int i = 0; i < face_normal.size(); ++i)
		{
			face_normal[i].clear();
		}
		face_normal.clear();
	}

	// ź��Ʈ ���� ���
	void CalTangentSpace(InitMetaData* _MetaData)
	{
		// ���� ����
		vector<XMVECTOR> tan1(_MetaData->Vertices.size());
		vector<XMVECTOR> tan2(_MetaData->Vertices.size());

		// �� ���� ��ŭ
		for (int fi = 0; fi < (int)_MetaData->Indices.size() / 3; ++fi)
		{
			//	�ﰢ�� �����͸� ��´�.
			WORD index1 = _MetaData->Indices[fi * 3 + 0];
			WORD index2 = _MetaData->Indices[fi * 3 + 1];
			WORD index3 = _MetaData->Indices[fi * 3 + 2];

			//	�ﰢ���� �ش��ϴ� ����3���� ���Ѵ�.
			XMVECTOR Vtx1 = XMLoadFloat3(&_MetaData->Vertices[index1].Position);
			XMVECTOR Vtx2 = XMLoadFloat3(&_MetaData->Vertices[index2].Position);
			XMVECTOR Vtx3 = XMLoadFloat3(&_MetaData->Vertices[index3].Position);

			// �ﰢ���� �ش��ϴ� �ؽ��� ����3���� ���Ѵ�.
			XMVECTOR TexVtx1 = XMLoadFloat2(&_MetaData->Vertices[index1].TexUV);
			XMVECTOR TexVtx2 = XMLoadFloat2(&_MetaData->Vertices[index2].TexUV);
			XMVECTOR TexVtx3 = XMLoadFloat2(&_MetaData->Vertices[index3].TexUV);

			// ź��Ʈ xyz ���ϱ�
			float x1 = XMVectorGetX(Vtx2) - XMVectorGetX(Vtx1);
			float x2 = XMVectorGetX(Vtx3) - XMVectorGetX(Vtx1);
			float y1 = XMVectorGetY(Vtx2) - XMVectorGetY(Vtx1);
			float y2 = XMVectorGetY(Vtx3) - XMVectorGetY(Vtx1);
			float z1 = XMVectorGetZ(Vtx2) - XMVectorGetZ(Vtx1);
			float z2 = XMVectorGetZ(Vtx3) - XMVectorGetZ(Vtx1);

			// ź��Ʈ w ���ϱ�
			float s1 = XMVectorGetX(TexVtx2) - XMVectorGetX(TexVtx1);
			float s2 = XMVectorGetX(TexVtx3) - XMVectorGetX(TexVtx1);
			float t1 = XMVectorGetY(TexVtx2) - XMVectorGetY(TexVtx1);
			float t2 = XMVectorGetY(TexVtx3) - XMVectorGetY(TexVtx1);

			// ���� ����
			XMVECTOR sdir;
			XMVECTOR tdir;
			if ((s1 * t2 - s2 * t1) == 0.0f)
			{
				sdir = XMLoadFloat3(&XMFLOAT3(1.0f, 0.0f, 0.0f));
				tdir = XMLoadFloat3(&XMFLOAT3(0.0f, 1.0f, 0.0f));
			}
			else
			{
				// ���� ����
				float r = 1.0F / (s1 * t2 - s2 * t1);
				sdir = XMLoadFloat3(&XMFLOAT3((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r, (t2 * z1 - t1 * z2) * r));
				tdir = XMLoadFloat3(&XMFLOAT3((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r, (s1 * z2 - s2 * z1) * r));
			}

			// �� ����
			tan1[index1] += sdir;
			tan1[index2] += sdir;
			tan1[index3] += sdir;

			tan2[index1] += tdir;
			tan2[index2] += tdir;
			tan2[index3] += tdir;
		}

		// �� ����
		for (int v = 0; v < (int)_MetaData->Vertices.size(); ++v)
		{
			const XMVECTOR& n = XMLoadFloat3(&_MetaData->Vertices[v].Normal);
			const XMVECTOR& t = tan1[v];

			// Gram-Schmidt orthogonalize
			// ź��Ʈ Vtx �����ϱ� (xyz)
			XMStoreFloat3(&_MetaData->Vertices[v].TangentU, XMVector3Normalize((t - n * XMVector3Dot(n, t))));

			// Calculate handedness
			//// �ؽ�ó ��ǥ ���⺤�� ���Ѵ�. (w) <-- �ϴ� �ȱ��� (���߿� ���ؼ� ����)
			float TangentU = (XMVectorGetX(XMVector3Dot(XMVector3Cross(n, t), tan2[v])) < 0.0F) ? -1.0f : 1.0f;

			// �����Ͽ� ����Ѵ�
			XMVECTOR BiNormal = XMVector3Cross(XMLoadFloat3(&_MetaData->Vertices[v].TangentU), n) * TangentU;

			// ��� �� ����
			XMStoreFloat3(&_MetaData->Vertices[v].BiNormal, BiNormal);
		}

		tan1.clear();
		tan2.clear();
	}

	// �� ���
	void CalMap(InitMetaData* _InitMetaData, float& _Xwidth, float& _Zdepth, float& _CellSize)
	{
		CalVtx(_InitMetaData, _Xwidth, _Zdepth, _CellSize);
		CalIB (_InitMetaData, _Xwidth, _Zdepth);
	}

	// ���ؽ� ���
	void CalVtx(InitMetaData* _InitMetaData, float& _Xwidth, float& _Zdepth, float& _CellSize)
	{
		// �߰���ǥ ����
		float halfWidth = 0.5f * (_Xwidth * _CellSize);
		float halfDepth = 0.5f * (_Zdepth * _CellSize);

		// ����ȭ�� �ؽ�ó ��ǥ��� �ѱ��.
		float du = 1.0f / (_Xwidth / _CellSize - 1);
		float dv = 1.0f / (_Zdepth / _CellSize - 1);

		// �� �߾ӿ� ������ �ΰ� �� ���� 
		for (UINT y = 0; y < (UINT)_Zdepth; ++y)
		{
			float zPos = halfDepth - (y * _CellSize);
			for (UINT x = 0; x < (UINT)_Xwidth; ++x)
			{
				float xPos = -halfWidth + (x * _CellSize);

				// ���ؽ� �ֱ�
				_InitMetaData->Vertices[y * (UINT)_Xwidth + x].Position = XMFLOAT3(xPos, 0.0f, zPos);

				// �ؽ�ó ���ؽ� ���
				_InitMetaData->TexVertices[y * (UINT)_Xwidth + x] = XMFLOAT3(x * du, y * dv, 0.0f);

				// �ؽ�ó uv ���߱�
				_InitMetaData->Vertices[y * (UINT)_Xwidth + x].TexUV.x = x * du;
				_InitMetaData->Vertices[y * (UINT)_Xwidth + x].TexUV.y = y * dv;
			}
		}
	}

	// �ε��� ���
	void CalIB(InitMetaData* _InitMetaData, float& _Xwidth, float& _Zdepth)
	{
		// �ε��� ���
		int baseIdx = 0;
		for (UINT y = 0; y < _Zdepth - 1; ++y)
		{
			for (UINT x = 0; x < _Xwidth - 1; ++x)
			{
				//----------------------------------------------------------------//
				// �ε���
				//----------------------------------------------------------------//
				// ��� �ε���
				_InitMetaData->Indices[baseIdx + 0] = (UINT)(y * _Xwidth + x);
				_InitMetaData->Indices[baseIdx + 1] = (UINT)(y * _Xwidth + x + 1);
				_InitMetaData->Indices[baseIdx + 2] = (UINT)((y + 1) * _Xwidth + x);

				// �Ʒ� �ε���
				_InitMetaData->Indices[baseIdx + 3] = (UINT)((y + 1) * _Xwidth + x);
				_InitMetaData->Indices[baseIdx + 4] = (UINT)(y * _Xwidth + x + 1);
				_InitMetaData->Indices[baseIdx + 5] = (UINT)((y + 1) * _Xwidth + x + 1);


				//----------------------------------------------------------------//
				// �ؽ�ó �ε���
				//----------------------------------------------------------------//
				// ��� �ε���
				_InitMetaData->TexIndices[baseIdx + 0] = (UINT)(y * _Xwidth + x);
				_InitMetaData->TexIndices[baseIdx + 1] = (UINT)(y * _Xwidth + x + 1);
				_InitMetaData->TexIndices[baseIdx + 2] = (UINT)((y + 1) * _Xwidth + x);

				// �Ʒ� �ε���
				_InitMetaData->TexIndices[baseIdx + 3] = (UINT)((y + 1) * _Xwidth + x);
				_InitMetaData->TexIndices[baseIdx + 4] = (UINT)(y * _Xwidth + x + 1);
				_InitMetaData->TexIndices[baseIdx + 5] = (UINT)((y + 1) * _Xwidth + x + 1);

				// ���� �簢��
				baseIdx += 6;
			}
		}
	}


	// ����Ʈ�� �ε�
	void LoadHeightmap(InitMetaData* _InitMetaData, string& _hightMapPss, float& _Xwidth, float& _Zdepth, float& _HeightScale)
	{
		// ��ü ũ��
		int tMapSize = (int)(_Xwidth * _Zdepth);

		// ����Ʈ�� �о�帱 ����
		vector<unsigned char> in(tMapSize);

		// ��ȯ
		wstring _WsTexName;
		StringToWchar_t(_hightMapPss, _WsTexName);

		// ����Ʈ�� ���̳ʸ��� �б�
		std::ifstream inFile;
		inFile.open(_WsTexName.c_str(), std::ios_base::binary);

		// ����Ʈ�� �б�
		if (inFile)
		{
			// Read the RAW bytes.
			inFile.read((char*)&in[0], (std::streamsize)in.size());

			// Done with file.
			inFile.close();
		}
		
		// ������ ���� �� ����
		for (int i = 0; i < tMapSize; ++i)
			_InitMetaData->mHeightmap[i] = (in[i] / 255.0f) * _HeightScale;
	}
	
	// �ε巴���ϱ� <-- 0~255�� �������� �Ǽ������� �����ϴ� ����
	void Smooth(InitMetaData* _InitMetaData, float& _Xwidth, float& _Zdepth)
	{
		// �ӽù���
		std::vector<float> dest(_InitMetaData->mHeightmap.size());

		for (UINT y = 0; y < (UINT)_Zdepth; ++y)
		{
			for (UINT x = 0; x < (UINT)_Xwidth; ++x)
			{
				dest[y * (UINT)_Xwidth + x] = Average(_InitMetaData, _Xwidth, _Zdepth, y, x);
			}
		}
		
		// �����ؼ� �ֱ�
		_InitMetaData->mHeightmap = dest;
	}

	// ��� ���� ���
	float Average(InitMetaData* _InitMetaData, float& _Xwidth, float& _Zdepth, int y, int x)
	{
		// �Լ��� ij ����� ��� ���̸� ����մϴ�.
		// �װ��� 8 ���� �̿� �� �ȼ���� �װ� ��ü�� ���ȭ�Ѵ�.
		// �ȼ��� �̿��� ���ٸ� ���Խ�Ű�� ����.
		// ��, �����ڸ� �ȼ����� ���� �ȼ��� �����ϴ�.
		//
		// ----------
		// | 1| 2| 3|
		// ----------
		// |4 |ij| 6|
		// ----------
		// | 7| 8| 9|
		// ----------

		float avg = 0.0f;
		float num = 0.0f;

		// ������ ����Ϸ��� int�� ����Ͻʽÿ�. �츮�� UINT�� ����Ѵٸ�, @ i = 0, m = i-1 = UINT_MAX
		// �ܺ� for ������ �ݺ��� �߻����� �ʽ��ϴ�.
		for (int m = y - 1; m <= y + 1; ++m)
		{
			for (int n = x - 1; n <= x + 1; ++n)
			{
				// ���� ��ȿ�� ������ Ȯ��
				if (InBounds(_Xwidth, _Zdepth, m, n))
				{
					avg += _InitMetaData->mHeightmap[m * (int)_Xwidth + n];
					num += 1.0f;
				}
			}
		}

		return avg / num;
	}

	// ���� ��ȿ�� ������ Ȯ��
	bool InBounds(float& _Xwidth, float& _Zdepth, int y, int x)
	{
		// ij�� ��ȿ�� �ε��� �� ��� ��; �׷��� ���� ���� false
		return
			y >= 0 && y < (int)_Zdepth &&
			x >= 0 && x < (int)_Xwidth;
	}

	// ���� �б� �׽�Ʈ (�� ������ ��� �ε�)
	void ReadLocMyFormat_Model(string _FinModelLoc)
	{
		// �� ���Ͽ����� mFinExportModelLoc �� �κп� �ش��ϴ� ���ڿ���
		// �� ���� �κп� �־� �ش� ������ �־���� �Ѵ�.

		// ���� ���� ����
		FileOpen(_FinModelLoc.c_str(), "rb");

		// ������� �� �����
		int _ModelNum = 0;
		fread(&_ModelNum, sizeof(int), 1, mFilePointer);
		mNewModleLoc.resize(_ModelNum);

		// �Ž� ��ġ ������ ����
		for (int i = 0; i < _ModelNum; ++i)
		{
			int len = 0;
			fread(&len, sizeof(int), 1, mFilePointer);
			fread(&mNewModleLoc[i], sizeof(char), len, mFilePointer);
		}

		// ���� ����
		FileClose();
		printf("[Succes] ���� ����.\n");
		_FinModelLoc.clear();
	}

	// �� �������� �д´�.(������)
	void ReadDataMyFormat_Model(string& _FinExportModelData)
	{
		// ���� ���� ����
		FileOpen(_FinExportModelData.c_str(), "rb");
		int len = 0;

		//------------------------------------------------------------------------------//
		// �� ������
		//------------------------------------------------------------------------------//

		// ���ؽ�
		fread(&len, sizeof(int), 1, mFilePointer);
		mMyMeshData.vertices.resize(len);

		for (int i = 0; i < len; ++i)
			fread(&mMyMeshData.vertices[i], sizeof(Vertex), 1, mFilePointer);

		// �ε��� ����Ʈ
		fread(&len, sizeof(int), 1, mFilePointer);
		mMyMeshData.indices.resize(len);

		for (int i = 0; i < len; ++i)
			fread(&mMyMeshData.indices[i], sizeof(XMFLOAT3), 1, mFilePointer);

		// �ִϵ�����

		// ��ġ
		fread(&len, sizeof(int), 1, mFilePointer);
		mMyMeshData.mAniData.Position.resize(len);

		for (int i = 0; i < len; ++i)
			fread(&mMyMeshData.mAniData.Position[i], sizeof(KeyVtx), 1, mFilePointer);

		// ȸ��
		fread(&len, sizeof(int), 1, mFilePointer);
		mMyMeshData.mAniData.Quaternion.resize(len);

		for (int i = 0; i < len; ++i)
			fread(&mMyMeshData.mAniData.Quaternion[i], sizeof(RotKeyVtx), 1, mFilePointer);

		// ������
		fread(&len, sizeof(int), 1, mFilePointer);
		mMyMeshData.mAniData.Scale.resize(len);

		for (int i = 0; i < len; ++i)
			fread(&mMyMeshData.mAniData.Scale[i], sizeof(KeyVtx), 1, mFilePointer);


		// ����ġ ������

		// ��ü ������
		fread(&len, sizeof(int), 1, mFilePointer);
		mMyMeshData.weightVtx.resize(len);

		for (int i = 0; i < len; ++i)
		{
			// Ÿ�� ���ؽ� �ε���
			fread(&mMyMeshData.weightVtx[i].TgVtxIdx, sizeof(int), 1, mFilePointer);

			// �� ������ �� ũ��
			int Bonelen = 0;
			fread(&Bonelen, sizeof(int), 1, mFilePointer);
			mMyMeshData.weightVtx[i].Bone.resize(Bonelen);

			// �� ������
			for (int idx = 0; idx < Bonelen; ++idx)
			{
				// ID
				fread(&mMyMeshData.weightVtx[i].Bone[idx].ID, sizeof(int), 1, mFilePointer);

				// ����ġ
				fread(&mMyMeshData.weightVtx[i].Bone[idx].Weight, sizeof(float), 1, mFilePointer);

				// ����(�̸�)
				int Bonebuflen = 0;
				fread(&Bonebuflen, sizeof(int), 1, mFilePointer);
				fread(&mMyMeshData.weightVtx[i].Bone[idx].Name, sizeof(char), Bonebuflen, mFilePointer);
			}
		}

		// ������
		fread(&mMyMeshData.vertexOffset, sizeof(UINT), 1, mFilePointer);
		fread(&mMyMeshData.indexOffset, sizeof(UINT), 1, mFilePointer);

		// ī��Ʈ
		fread(&mMyMeshData.indexCount, sizeof(UINT), 1, mFilePointer);

		// ���� ����

		// ����
		fread(&mMyMeshData.mMyMat.mOpacity, sizeof(float), 1, mFilePointer);

		// ��ǻ��
		fread(&len, sizeof(int), 1, mFilePointer);
		fread(&mMyMeshData.mMyMat.mDiffuseSRV, sizeof(char), len, mFilePointer);

		// ���
		fread(&len, sizeof(int), 1, mFilePointer);
		fread(&mMyMeshData.mMyMat.mNomalSRV, sizeof(char), len, mFilePointer);

		// ����
		fread(&len, sizeof(int), 1, mFilePointer);
		fread(&mMyMeshData.mMyMat.mSpecularSRV, sizeof(char), len, mFilePointer);


		// TM ���
		fread(&mMyMeshData.mTMLocalMtx, sizeof(XMFLOAT4X4), 1, mFilePointer);
		fread(&mMyMeshData.mTMWorldMtx, sizeof(XMFLOAT4X4), 1, mFilePointer);

		// �ٿ�� �ڽ�
		fread(&mMyMeshData.mBoundingBox, sizeof(BoundBox), 1, mFilePointer);

		//--------------------------------------------------//
		// ������Ʈ �ĺ� ����
		//--------------------------------------------------//

		// �̸�
		fread(&len, sizeof(int), 1, mFilePointer);
		fread(&mMyMeshData.mMainName, sizeof(char), len, mFilePointer);

		// ������Ʈ�̸�
		fread(&mMyMeshData.mObjID, sizeof(int), 1, mFilePointer);

		fread(&len, sizeof(int), 1, mFilePointer);
		fread(&mMyMeshData.mObjName, sizeof(char), len, mFilePointer);

		fread(&len, sizeof(int), 1, mFilePointer);
		fread(&mMyMeshData.mObjClass, sizeof(char), len, mFilePointer);

		// ���� Ŭ���� �̸�
		fread(&mMyMeshData.mParentID, sizeof(int), 1, mFilePointer);

		fread(&len, sizeof(int), 1, mFilePointer);
		fread(&mMyMeshData.mParentName, sizeof(char), len, mFilePointer);

		// ���� ����
		FileClose();
		printf("[Succes] ���� ����.\n");
	}

	// �� �������� �д´�.(������) - ��
	void ReadDataMyFormat_Bone(string& _BoneRoute, MyBoneData* _MyBoneData)
	{
		// ���� ���� ����
		FileOpen(_BoneRoute.c_str(), "rb");

		// Bone �̸�
		int len = 0;
		fread(&len, sizeof(int), 1, mFilePointer);
		fread(&_MyBoneData->mMainName, sizeof(char), len, mFilePointer);

		// Bone �ִ� �̸�
		fread(&len, sizeof(int), 1, mFilePointer);
		fread(&_MyBoneData->mAniName, sizeof(char), len, mFilePointer);

		// �� ������ ������
		fread(&len, sizeof(int), 1, mFilePointer);
		_MyBoneData->mSaveBoneData.resize(len);

		// �� �����Ϳ� ���� ��ŭ
		for (unsigned int i = 0; i < _MyBoneData->mSaveBoneData.size(); ++i)
		{
			// TM ���
			fread(&_MyBoneData->mSaveBoneData[i].mTMLocalMtx, sizeof(XMFLOAT4X4), 1, mFilePointer);
			fread(&_MyBoneData->mSaveBoneData[i].mTMWorldMtx, sizeof(XMFLOAT4X4), 1, mFilePointer);

			// �ִϵ�����

			// ��ġ
			fread(&len, sizeof(int), 1, mFilePointer);
			_MyBoneData->mSaveBoneData[i].mAniData.Position.resize(len);
			for (int idx = 0; idx < len; ++idx)
				fread(&_MyBoneData->mSaveBoneData[i].mAniData.Position[idx], sizeof(KeyVtx), 1, mFilePointer);

			// ȸ��
			fread(&len, sizeof(int), 1, mFilePointer);
			_MyBoneData->mSaveBoneData[i].mAniData.Quaternion.resize(len);
			for (int idx = 0; idx < len; ++idx)
				fread(&_MyBoneData->mSaveBoneData[i].mAniData.Quaternion[idx], sizeof(RotKeyVtx), 1, mFilePointer);

			// ������
			fread(&len, sizeof(int), 1, mFilePointer);
			_MyBoneData->mSaveBoneData[i].mAniData.Scale.resize(len);
			for (int idx = 0; idx < len; ++idx)
				fread(&_MyBoneData->mSaveBoneData[i].mAniData.Scale[idx], sizeof(KeyVtx), 1, mFilePointer);


			// �ٿ�� �ڽ�
			fread(&_MyBoneData->mSaveBoneData[i].mBoundingBox, sizeof(BoundBox), 1, mFilePointer);

			//--------------------------------------------------//
			// ������Ʈ �ĺ� ����
			//--------------------------------------------------//

			// ������Ʈ�̸�
			fread(&_MyBoneData->mSaveBoneData[i].mObjID, sizeof(int), 1, mFilePointer);

			fread(&len, sizeof(int), 1, mFilePointer);
			fread(&_MyBoneData->mSaveBoneData[i].mObjName, sizeof(char), len, mFilePointer);

			fread(&len, sizeof(int), 1, mFilePointer);
			fread(&_MyBoneData->mSaveBoneData[i].mObjClass, sizeof(char), len, mFilePointer);

			// ���� Ŭ���� �̸�
			fread(&_MyBoneData->mSaveBoneData[i].mParentID, sizeof(int), 1, mFilePointer);

			fread(&len, sizeof(int), 1, mFilePointer);
			fread(&_MyBoneData->mSaveBoneData[i].mParentName, sizeof(char), len, mFilePointer);
		}

		// �� ���̶���Ű ������
		fread(&len, sizeof(int), 1, mFilePointer);
		_MyBoneData->mBoneHierarchy.resize(len);

		// �� ���̶���Ű�� ��ϵ� ��ŭ
		for (int i = 0; i < len; ++i)
		{
			// ���ͷ����� ������
			int idxlen = 0;
			fread(&idxlen, sizeof(int), 1, mFilePointer);
			_MyBoneData->mBoneHierarchy[i].resize(idxlen);

			// ���ͷ����� ��ϵ� ��ŭ
			for (int idx = 0; idx < idxlen; ++idx)
			{
				// ������Ʈ Ű 
				fread(&_MyBoneData->mBoneHierarchy[i][idx].Key, sizeof(int), 1, mFilePointer);

				// ������Ʈ �̸�
				int Buflen = 0;
				fread(&Buflen, sizeof(int), 1, mFilePointer);
				fread(&_MyBoneData->mBoneHierarchy[i][idx].Name, sizeof(char), Buflen, mFilePointer);

				// ������Ʈ Ŭ����
				fread(&Buflen, sizeof(int), 1, mFilePointer);
				fread(&_MyBoneData->mBoneHierarchy[i][idx].objClass, sizeof(char), Buflen, mFilePointer);
			}
		}

		// ���� ����
		FileClose();
		printf("[Succes] ���� ����.\n");
	}

	// ���� �����ϱ�
	void SaveMyFormat(InitMetaData* _InitMetaData)
	{
		// ���ؽ�
		_InitMetaData->Vertices = mMyMeshData.vertices;

		// �ε���
		for (unsigned int i = 0; i < mMyMeshData.indices.size(); ++i)
		{
			for (int vIdx = 0; vIdx < 3; ++vIdx)
			{
				switch (vIdx)
				{
				case 0:
					_InitMetaData->Indices.push_back((UINT)mMyMeshData.indices[i].x);
					break;
				case 1:
					_InitMetaData->Indices.push_back((UINT)mMyMeshData.indices[i].y);
					break;
				case 2:
					_InitMetaData->Indices.push_back((UINT)mMyMeshData.indices[i].z);
					break;
				default:
					break;
				}
			}
		}

		// �ִ� ������
		_InitMetaData->mAniData = mMyMeshData.mAniData;

		// ����ġ ������
		_InitMetaData->weightVtx = mMyMeshData.weightVtx;

		// TM ��Ʈ����
		_InitMetaData->mLocTMMtx = mMyMeshData.mTMLocalMtx;
		_InitMetaData->mWdTMMtx  = mMyMeshData.mTMWorldMtx;

		// ������
		_InitMetaData->mVertexOffset = mMyMeshData.vertexOffset;
		_InitMetaData->mIndexOffset  = mMyMeshData.indexOffset;

		// ī��Ʈ
		_InitMetaData->mIndexCount = mMyMeshData.indexCount;

		// ���ҽ� ��� (���� ����)
		// �׽�Ʈ
		//if (_InitMetaData->mCreateName == "Model4_1")
		//	int i = 0;

		LoadTex(_InitMetaData, mMyMeshData.mMyMat.mDiffuseSRV , e_DiffuseMap);
		LoadTex(_InitMetaData, mMyMeshData.mMyMat.mNomalSRV   , e_NomalMap);
		LoadTex(_InitMetaData, mMyMeshData.mMyMat.mSpecularSRV, e_SpecularMap);


		// ���� (��ǻ�� ��)
		_InitMetaData->mOpacity = mMyMeshData.mMyMat.mOpacity;

		// �ٿ�� �ڽ�
		_InitMetaData->mBoundingBox = mMyMeshData.mBoundingBox;

		//--------------------------------------------------//
		// ������Ʈ �ĺ� ����
		//--------------------------------------------------//

		// �̸�
		strcpy(_InitMetaData->mMainName, mMyMeshData.mMainName);

		// ������Ʈ�̸�
		_InitMetaData->mObjID = mMyMeshData.mObjID;
		strcpy(_InitMetaData->mObjName , mMyMeshData.mObjName);
		strcpy(_InitMetaData->mObjClass, mMyMeshData.mObjClass);

		// ���� Ŭ���� �̸�
		_InitMetaData->mParentID = mMyMeshData.mParentID;
		strcpy(_InitMetaData->mParentName, mMyMeshData.mParentName);

	}
private:
	// ���� ����
	bool FileOpen(string _Route, string _ReadType)
	{
		//�ش� ����� ������ ����.
		fopen_s(&mFilePointer, _Route.c_str(), _ReadType.c_str());

		// �ش� ��ο� ������ �ֳ�?
		if (mFilePointer == NULL)
		{
			printf("[Error] [%s] ���� ����. <-- �ش��ο� ������ �������� �ʽ��ϴ�.\n", _Route);
			return false;
		}
		if ((_ReadType == "wb") || (_ReadType == "wt"))
		{
			rewind(mFilePointer);
			cout << "[Succes] [" << _Route << "] ������ ����.\n";
		}
		else if ((_ReadType == "rb") || (_ReadType == "rt"))
		{
			rewind(mFilePointer);
			cout << "[Succes] [" << _Route << "] ������ ����.\n";
		}
		return true;
	}

	void FileClose()
	{
		fclose(mFilePointer);
		mFilePointer = nullptr;
	}

	// �ؽ�ó �ε�
	void LoadTex(InitMetaData* _InitMetaData, string _TexName, TEXTURE_TYPE e_InitTex)
	{
		// string --> wString���� ��ȯ
		wstring _WsTexName;
		StringToWchar_t(_TexName, _WsTexName);

		// �ؽ�ó �ε�
		switch (e_InitTex)
		{
		case e_DiffuseMap:
			HR(D3DX11CreateShaderResourceViewFromFile(mCoreStorage->md3dDevice, _WsTexName.c_str(), 0, 0, &_InitMetaData->mDiffuseSRV, 0));
			break;
		case e_NomalMap:
			HR(D3DX11CreateShaderResourceViewFromFile(mCoreStorage->md3dDevice, _WsTexName.c_str(), 0, 0, &_InitMetaData->mNomalSRV, 0));
			break;
		case e_SpecularMap:
			HR(D3DX11CreateShaderResourceViewFromFile(mCoreStorage->md3dDevice, _WsTexName.c_str(), 0, 0, &_InitMetaData->mSpecularSRV, 0));
			break;
		}
	}

	// ��ȯ �Լ�
	void StringToWchar_t(string& _string, wstring& _wstring)
	{
		// ��ȯ
		for (unsigned int i = 0; i < _string.length(); ++i)
			_wstring += wchar_t(_string[i]);

		// ������
		_wstring += wchar_t('\0');
	}
};

