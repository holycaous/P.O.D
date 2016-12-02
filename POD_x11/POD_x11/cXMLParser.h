#pragma once
class cXMLParser
{
public:
	//------------------------------------------------------------------//
	// 완성 된 데이터
	//------------------------------------------------------------------//

	// 새로 만들어질 모델의 위치정보
	vector<CharBuf> mNewModleLoc;

	// 새로 만들어진 모델의 정보
	MyMeshData mMyMeshData;

	//------------------------------------------------------------------//	
private:
	cCoreStorage* mCoreStorage = cCoreStorage::GetInstance();

	// 파일 포인터
	FILE* mFilePointer = nullptr;

	// 버퍼
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

	// 버퍼 초기화
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

	// 파싱
	void LoadXMLRoute(string& _ModelRoute)
	{
		// 초기화
		ClearPointer();

		// XML 루트 데이터 읽기
		ReadLocMyFormat_Model(_ModelRoute);
	}

	// 파싱
	void LoadXMLModel(string& _ModelData, InitMetaData* _InitMetaData)
	{
		// XML 모델 데이터 읽기
		ReadDataMyFormat_Model(_ModelData); 

		// 얻은 데이터 복사해 넣기
		SaveMyFormat(_InitMetaData);
	}

	// 파싱
	void LoadXMLSkinModel(string& _ModelData, InitMetaData* _InitMetaData, InitMetaData* _UseMetaData, FSM_TYPE _fsmType)
	{
		// 데이터 읽고, 저장하기 (버텍스)
		LoadVtx(_ModelData, _InitMetaData);
		
		// 스킨 모델 텍스처 만들고 옮기기
		MakeSkinModelTex(_InitMetaData->Vertices, _UseMetaData, _fsmType);
	}

	// 스킨 모델 텍스처 만들고 옮기기
	void MakeSkinModelTex(vector<Vertex>& _compareVtxArr, InitMetaData* _UseMetaData, FSM_TYPE _fsmType)
	{
		// 저장할 원본 모델에, 데이터 넣을 공간 생성
		_UseMetaData->mSkinModelTex[(int)_fsmType] = new SkinTexture();

		// 데이터 계산하고 쓰기 등등
		CalData(_compareVtxArr, _UseMetaData, _fsmType, _compareVtxArr.size());
	}

	// 스킨 맵 텍스처 만들고 옮기기
	void MakeSkinMapTex(vector<Vertex>& _compareVtxArr, InitMetaData* _UseMetaData, int _mapType, string _AddName)
	{
		// 저장할 원본 모델에, 데이터 넣을 공간 생성 (일단 0으로, 별다른 상태값은 안주었음. 나중에 필요하면 확장할 예정)
		_UseMetaData->mSkinModelTex[_mapType] = new SkinTexture();

		// 데이터 계산하고 쓰기 등등
		CalMapData(_compareVtxArr, _UseMetaData, _mapType, _compareVtxArr.size(), _AddName);
	}

	// 맵 데이터 계산
	void CalMapData(vector<Vertex>& _compareVtxArr, InitMetaData* _UseMetaData, int _fsmType, int _VtxSize, string _AddName)
	{
		string tTexName;
		
		tTexName += "Export/SkinMapTex/";
		tTexName += _UseMetaData->mObjName;
		tTexName += _AddName;
		tTexName += ".dds";

		//--------------------------------------------------------------------------------------------------//
		// 애니 데이터 채우기
		//--------------------------------------------------------------------------------------------------//
		// 이름 저장
		_UseMetaData->mSkinModelTex[_fsmType]->mName = _UseMetaData->mObjName;

		// 애니 타입 저장
		_UseMetaData->mSkinModelTex[_fsmType]->mAniType = (float)_fsmType;

		// 텍스처 해상도
		_UseMetaData->mSkinModelTex[_fsmType]->mTexWidth  = 4.0f;
		_UseMetaData->mSkinModelTex[_fsmType]->mTexHeight = (float)_VtxSize;

		//--------------------------------------------------------------------------------------------------//
		// 파일 경로 문자열
		//--------------------------------------------------------------------------------------------------//
		wstring _WsTexName;
		StringToWchar_t(tTexName, _WsTexName);

		//--------------------------------------------------------------------------------------------------//
		// 텍스처 연결
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

		// 텍스처가 없다
		if (hr == D3D11_ERROR_FILE_NOT_FOUND)
		{
			// 없다면 텍스처 생성
			cout << "MapSkinTex 파일이 없어 생성합니다. 다소 시간이 걸릴 수 있습니다." << endl;

			// 법선벡터 계산
			CalNormalVtx(_UseMetaData);
			// 정점 늘리기
			CalVtxINCAndSet(_UseMetaData);
			// 탄젠트 공간 계산
			CalTangentSpace(_UseMetaData);

			// 스킨 텍스처 만들기
			MakeSkinTex(_compareVtxArr, _WsTexName, _UseMetaData, _fsmType, _VtxSize);

			// 텍스처 모델 스킨 리로딩
			goto RetryLoadSkinModelTex;
		}
		else if (hr == S_OK)
			cout << "MapSkinTex 로딩 성공" << endl;
		else
			cout << ">> 명시되지 않은 에러 발생. SkinTex 파일 <<" << endl;

		_WsTexName.clear();
	}

	// 데이터 계산
	void CalData(vector<Vertex>& _compareVtxArr, InitMetaData* _UseMetaData, int _fsmType, int _VtxSize)
	{
		string _TexName; SelectName((FSM_TYPE)_fsmType, _TexName);
		string tTexName;
		
		tTexName += "Export/SkinModelTex/";
		tTexName += _UseMetaData->mObjName;
		tTexName += _TexName;
		tTexName += ".dds";

		//--------------------------------------------------------------------------------------------------//
		// 애니 데이터 채우기
		//--------------------------------------------------------------------------------------------------//
		// 이름 저장
		_UseMetaData->mSkinModelTex[_fsmType]->mName = _TexName;

		// 애니 타입 저장
		_UseMetaData->mSkinModelTex[_fsmType]->mAniType = (float)_fsmType;

		// 텍스처 해상도
		_UseMetaData->mSkinModelTex[_fsmType]->mTexWidth  = 4.0f;
		_UseMetaData->mSkinModelTex[_fsmType]->mTexHeight = (float)_VtxSize;

		//--------------------------------------------------------------------------------------------------//
		// 파일 경로 문자열
		//--------------------------------------------------------------------------------------------------//
		wstring _WsTexName;
		StringToWchar_t(tTexName, _WsTexName);

		//--------------------------------------------------------------------------------------------------//
		// 텍스처 연결
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

		// 텍스처가 없다
		if (hr == D3D11_ERROR_FILE_NOT_FOUND)
		{
			// 없다면 텍스처 생성
			cout << "ModelSkinTex 파일이 없어 생성합니다. 다소 시간이 걸릴 수 있습니다." << endl;

			// 스킨 텍스처 만들기
			MakeSkinTex(_compareVtxArr, _WsTexName, _UseMetaData, _fsmType, _VtxSize);

			// 텍스처 모델 스킨 리로딩
			goto RetryLoadSkinModelTex;
		}
		else if (hr == S_OK)
			cout << "ModelSkinTex 로딩 성공" << endl;
		else
			cout << ">> 명시되지 않은 에러 발생. SkinTex 파일 <<" << endl;

		_WsTexName.clear();
	}
	
	// 텍스처 쓰기
	void WriteTex(vector<Vertex>& _compareVtxArr, InitMetaData* _UseMetaData, int _fsmType)
	{
		D3D11_MAPPED_SUBRESOURCE MappedResource;

		// 버퍼 열기
		HRESULT hr = cCoreStorage::GetInstance()->md3dImmediateContext->Map(_UseMetaData->mSkinModelTex[_fsmType]->mTexture, //매핑할 텍스처
			D3D11CalcSubresource(0, 0, 1),																				     //서브 리소스 번호
			D3D11_MAP_WRITE_DISCARD,																					     //리소스에 쓴다
			0,																											    
			&MappedResource);																							     //데이터를 쓸 포인터

		// 열기 성공했을때만
		if (hr == S_OK)
		{
			// 데이터 맵핑 레퍼런스 얻기
			float* pTexels = (float*)MappedResource.pData;
			
			// 열 반복 (버텍스 숫자)
			for (unsigned int y = 0; y < _compareVtxArr.size(); ++y)
			{
				for (int i = 0; i < 4; ++i)
				{
					// 픽셀 선택
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
						cout << "행렬 범위 초과" << endl;
						break;
					}
				}
			}

			// 버퍼 닫기
			cCoreStorage::GetInstance()->md3dImmediateContext->Unmap(_UseMetaData->mSkinModelTex[_fsmType]->mTexture, D3D11CalcSubresource(0, 0, 1));
		}
		else
			cout << "스킨 모델 텍스처 맵핑 실패" << endl;
	}

	// 텍스처 생성
	void CreateTex(InitMetaData* _UseMetaData, int _fsmType, int _VtxSize)
	{
		D3D11_TEXTURE2D_DESC TextureDesc;
		ZeroMemory(&TextureDesc, sizeof(TextureDesc));

		TextureDesc.Width              = 4;					          // 한 픽셀당 Position, Normal, TangentU, BiNormal
		TextureDesc.Height             = _VtxSize;			          // 버텍스 사이즈 만큼 				
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

	// 텍스처 저장
	void SaveTex(wstring& _TexName, InitMetaData* _UseMetaData, int _fsmType)
	{
		D3DX11SaveTextureToFile(cCoreStorage::GetInstance()->md3dImmediateContext,
			_UseMetaData->mSkinModelTex[_fsmType]->mTexture,      //저장할 텍스처
			D3DX11_IFF_DDS,								      //dds로 저장
			_TexName.c_str());							      //이름

		// 텍스처를 만들었으니, 기존 데이터는 삭제
		_UseMetaData->mSkinModelTex[_fsmType]->ClearTex();
	}

	// 텍스처에 붙을 이름 선택하기
	void SelectName(int _fsmType, string& _TexName)
	{
		// 이름 선택
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

	// 스킨 텍스처 만들기
	void MakeSkinTex(vector<Vertex>& _compareVtxArr, wstring& _TexName, InitMetaData* _UseMetaData, int _fsmType, int _VtxSize)
	{
		// 텍스처 생성
		CreateTex(_UseMetaData, _fsmType, _VtxSize);

		// 텍스처 쓰기
		WriteTex(_compareVtxArr, _UseMetaData, _fsmType);

		// 텍스처 저장
		SaveTex(_TexName, _UseMetaData, _fsmType);
	}

	void LoadVtx(string& _FinExportModelData, InitMetaData* _InitMetaData)
	{
		//-----------------------------------------------------------//
		// 데이터 읽기
		//-----------------------------------------------------------//
		// 이진 파일 열기
		FileOpen(_FinExportModelData.c_str(), "rb");
		int len = 0;

		// 버텍스
		fread(&len, sizeof(int), 1, mFilePointer);
		mMyMeshData.vertices.resize(len);

		for (int i = 0; i < len; ++i)
			fread(&mMyMeshData.vertices[i], sizeof(Vertex), 1, mFilePointer);

		// 파일 종료
		FileClose();
		printf("[Succes] 파일 종료.\n");
		

		//-----------------------------------------------------------//
		// 데이터 저장
		//-----------------------------------------------------------//
		// 버텍스
		_InitMetaData->Vertices = mMyMeshData.vertices;


	}

	// 더미 박스 만들기
	// 박스 만들기
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


		// 텍스처
		XMFLOAT3 tv[8];

		// uv좌표 임의 구성 
		// - 서브 머테리얼 안되게 구성
		tv[0] = XMFLOAT3(0.0f, 0.0f, 0.0f);
		tv[1] = XMFLOAT3(1.0f, 0.0f, 0.0f);
		tv[2] = XMFLOAT3(0.0f, 1.0f, 0.0f);
		tv[3] = XMFLOAT3(1.0f, 1.0f, 0.0f);
		tv[4] = XMFLOAT3(1.0f, 0.0f, 0.0f);
		tv[5] = XMFLOAT3(0.0f, 0.0f, 0.0f);
		tv[6] = XMFLOAT3(1.0f, 1.0f, 0.0f);
		tv[7] = XMFLOAT3(0.0f, 1.0f, 0.0f);

		_InitMetaData->TexVertices.assign(&tv[0], &tv[8]);

		// 텍스트 인덱스 좌표 임의 구성
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

	// 스피어 만들기
	void LoadSphere(InitMetaData* _InitMetaData, float& _radius, UINT& _sliceCount, UINT& _stackCount)
	{
		//
		// 상단 극점에있는 꼭지점을 계산하고 스택 아래로 이동합니다.
		//

		// Poles : 텍스처 좌표 왜곡이있을 것입니다.
		// 매핑 할 때 극에 할당 할 텍스처 맵상의 유일한 점이 아닙니다.
		// 구형의 사각형 텍스처.

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

		// 각 스택 링에 대한 정점을 계산합니다 (폴을 링으로 계산하지 않음).
		for (UINT i = 1; i <= _stackCount - 1; ++i)
		{
			float phi = i * phiStep;

			// 링의 정점.
			for (UINT j = 0; j <= _sliceCount; ++j)
			{
				float theta = j * thetaStep;

				Vertex v;

				// spherical to cartesian
				v.Position.x = _radius * sinf(phi) * cosf(theta);
				v.Position.y = _radius * cosf(phi);
				v.Position.z = _radius * sinf(phi) * sinf(theta);

				// 쎄타에 관한 P의 부분 파생
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
		// 최상위 스택의 인덱스를 계산합니다. 최상위 스택이 처음에 정점 버퍼에 쓰여졌습니다.
		// 상단 극을 첫 번째 링에 연결합니다.
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
		// 내부 스택의 인덱스를 계산합니다 (폴에 연결되지 않음).
		//

		// 첫 번째 정점의 첫 번째 정점 인덱스에 대한 인덱스를 오프셋합니다.
		// 이것은 극점 꼭지점을 건너 뛰고 있습니다.
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
		// 하단 스택의 인덱스를 계산합니다. 맨 아래 스택이 마지막으로 버텍스 버퍼에 쓰여졌습니다.
		// 하단 극을 하단 고리에 연결합니다.
		//

		// 마지막으로 남극 꼭지점이 추가되었습니다.
		UINT southPoleIndex = (UINT)_InitMetaData->Vertices.size() - 1;

		// 마지막 반지에서 첫 번째 정점의 인덱스에 인덱스를 오프셋합니다.
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


		// 텍스처
		XMFLOAT3 tv[4];

		// uv좌표 임의 구성 
		// - 서브 머테리얼 안되게 구성
		tv[0] = XMFLOAT3(0.0f, 0.0f, 0.0f);
		tv[1] = XMFLOAT3(1.0f, 0.0f, 0.0f);
		tv[2] = XMFLOAT3(0.0f, 1.0f, 0.0f);
		tv[3] = XMFLOAT3(1.0f, 1.0f, 0.0f);

		_InitMetaData->TexVertices.assign(&tv[0], &tv[4]);

		// 텍스트 인덱스 좌표 임의 구성
		UINT ti[6];

		ti[0] = 0; ti[1] = 1; ti[2] = 2;
		ti[3] = 1; ti[4] = 3; ti[5] = 2;

		_InitMetaData->TexIndices.assign(&ti[0], &ti[6]);
	}

	// 맵 만들기
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

		// 하이트맵 전송
		for (int i = 0; i < (int)(_Xwidth * _Zdepth); ++i)
			_InitMetaData->Vertices[i].Position.y = _InitMetaData->mHeightmap[i];

		// 스킨 모델 텍스처 만들고 옮기기
		MakeSkinMapTex(_InitMetaData->Vertices, _InitMetaData, 0, "");

		// 하이트맵 로딩
		LoadHeightMap(_InitMetaData, _hightMapPss, _Xwidth, _Zdepth);
	}

	// 하이트맵 로딩
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

	// 정점 증가코드 2
	void CalVtxINCAndSet(InitMetaData* _MetaData)
	{
		// 메타데이터를 변경해서, 정점을 늘려줘야한다.
		// 의도는: 버텍스 좌표와, uv좌표는 불일치 할수 있다는 것에서 출발
		//         그러므로, 버텍스 좌표와 uv 좌표를 각각 뽑아내, 두개가 일치하면 내비두고,
		//         불일치시, 버텍스 좌표는 그대로 사용하고, uv 좌표를 수정하여 정점을 추가로 후미에 배치한다.
		//         이후, 추가한 정점정보를 바탕으로, 모델 인덱스리스트를 다시 작성한다.
		//
		// *핵심: uv 좌표 넣기전, 비어있는 공간인지 확인
		//        1) 비어있는 공간이라면 삽입,
		//        2) 비어 있지 않는 공간이라면, 비교
		//           -> 넣으려는 uv 좌표가 일치한다면, 아무작업 안하고, 다음 좌표로
		//           -> 넣으려는 uv 좌표가 일치하지 않는다면, 새로 생성 ( 버텍스 좌표 복사, uv 좌표 복사 )
		//        3) 정점을 추가하면, 현재 검색하고 있는 모델 인덱스에, 해당 정점 번호를 넣는다.


		// 모델의 인덱스 크기만큼 반복 ( 텍스처 인덱스도 일반적으로는 동일하므로, 비동일한 모델 처리시 예외 처리 필요 )
		for (unsigned int i = 0; i < _MetaData->Indices.size(); ++i)
		{
			// 현재 인덱스 번호를 검색한다.
			UINT _ModelIdx = _MetaData->Indices[i];    //  모델의 현재 인덱스
			UINT _TexIdx = _MetaData->TexIndices[i]; // 텍스처의 현재 인덱스

			// 각각의 인덱스 번호에 해당하는 uv값을 꺼낸다.
			XMFLOAT2& ModelTexUV = _MetaData->Vertices[_ModelIdx].TexUV; //   모델 인덱스의 UV 값
			XMFLOAT3&      TexUV = _MetaData->TexVertices[_TexIdx];      // 텍스처 인덱스의 UV 값

			// 모델의 현재 인덱스가 가르키는 버텍스의 uv가 비어있는가?
			if (ModelTexUV.x == FLT_MAX || ModelTexUV.y == FLT_MAX)
			{
				// 비어있다면,
				// 텍스처 uv 값 바로 삽입
				ModelTexUV.x = TexUV.x;
				ModelTexUV.y = TexUV.y;
			}
			else
			{
				// 비어 있지 않다면
				// 기존 모델의 UV와 현재 텍스처의 UV가 일치하는가?
				if (ModelTexUV.x == TexUV.x && ModelTexUV.y == TexUV.y)
				{
					// 일치한다면, 

				}
				else
				{
					// 일치하지 않는다면, 정점 늘리기
					// 해당 버텍스의 정보를 꺼낸다
					Vertex ModelVtx = _MetaData->Vertices[_ModelIdx];

					// 텍스처 정보 변경
					ModelTexUV.x = TexUV.x;
					ModelTexUV.y = TexUV.y;

					// 복사
					_MetaData->Vertices.push_back(ModelVtx);
				}
			}
		}
	}

	// 법선 계산
	void CalNormalVtx(InitMetaData* _MetaData)
	{
		vector< vector<XMVECTOR> > face_normal;

		//면 노멀
		face_normal.resize(_MetaData->Indices.size() / 3);
		for (unsigned int i = 0; i < face_normal.size(); ++i)
			face_normal[i].resize(2);

		//-----------------------------------//
		//면 노멀법선
		//-----------------------------------//
		//삼각형 갯수만큼 반복
		for (int fi = 0; fi < (int)_MetaData->Indices.size() / 3; ++fi)
		{
			//	삼각형 데이터를 얻는다.
			WORD index1 = _MetaData->Indices[fi * 3 + 0];
			WORD index2 = _MetaData->Indices[fi * 3 + 1];
			WORD index3 = _MetaData->Indices[fi * 3 + 2];

			//	삼각형에 해당하는 정점3개를 구한다.
			XMVECTOR vtx1 = XMLoadFloat3(&_MetaData->Vertices[index1].Position);
			XMVECTOR vtx2 = XMLoadFloat3(&_MetaData->Vertices[index2].Position);
			XMVECTOR vtx3 = XMLoadFloat3(&_MetaData->Vertices[index3].Position);

			// 면벡터의 중간 지점을 저장한다.
			face_normal[fi][0] = vtx1 + vtx2 + vtx3;

			//	한개 정점에서 다른 두개 정점을 향하는 방향벡터 두개를 구한다.
			XMVECTOR vtx_dir1 = vtx2 - vtx1;
			XMVECTOR vtx_dir2 = vtx3 - vtx1;

			//	방향 벡터 2개를 정규화 한다.
			vtx_dir1 = XMVector3Normalize(vtx_dir1);
			vtx_dir2 = XMVector3Normalize(vtx_dir2);

			//	외적하여 법선벡터를 구한다.
			XMVECTOR normal_dir = XMVector3Cross(vtx_dir1, vtx_dir2);

			// 면벡터의 뻣친 지점을 저장한다.
			face_normal[fi][1] = face_normal[fi][0] + normal_dir * 1.0f;

		} //반복

		//-----------------------------------//
		//정점 노멀법선
		//-----------------------------------//
		//정점갯수만큼 반복 
		for (unsigned int vi = 0; vi < _MetaData->Vertices.size(); ++vi) //<- 정점번호 0부터 순차검색
		{
			//	누적벡터 0,0,0 초기화 
			XMVECTOR result_vec = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

			//	누적갯수 0 초기화 
			int result_count = 0;

			//	삼각형 갯수만큼 반복
			for (unsigned int fi = 0; fi < _MetaData->Indices.size() / 3; ++fi)
			{
				//		삼각형 데이터를 얻는다.
				WORD index1 = _MetaData->Indices[fi * 3 + 0];
				WORD index2 = _MetaData->Indices[fi * 3 + 1];
				WORD index3 = _MetaData->Indices[fi * 3 + 2];

				//		삼각형 데이터 중에 정점 인덱스가 잇냐?
				if ((index1 == (WORD)vi) || (index2 == (WORD)vi) || (index3 == (WORD)vi)) //<- vi는 정점번호, index에는 각각의 정점번호가 들어가있다.
				{   //		예
					//			누적벡터 에 면 법선벡터를 더한다.
					XMVECTOR tmp_dir = face_normal[fi][1] - face_normal[fi][0]; //방향벡터를 누적해야함.
					tmp_dir = XMVector3Normalize(tmp_dir);

					// 결과 벡터에 누적
					result_vec += tmp_dir;

					// 누적갯수 1 증가.
					++result_count;
				}
			}//	반복
			//	누적벡터 /= 누적갯수
			result_vec /= (float)result_count;

			//노멀라이즈
			result_vec = XMVector3Normalize(result_vec);

			//정점(법선)에 법선벡터 저장
			XMStoreFloat3(&_MetaData->Vertices[vi].Normal, result_vec);

		}//반복

		// 자원 제거
		// 인덱스
		for (unsigned int i = 0; i < face_normal.size(); ++i)
		{
			face_normal[i].clear();
		}
		face_normal.clear();
	}

	// 탄젠트 공간 계산
	void CalTangentSpace(InitMetaData* _MetaData)
	{
		// 저장 공간
		vector<XMVECTOR> tan1(_MetaData->Vertices.size());
		vector<XMVECTOR> tan2(_MetaData->Vertices.size());

		// 면 갯수 만큼
		for (int fi = 0; fi < (int)_MetaData->Indices.size() / 3; ++fi)
		{
			//	삼각형 데이터를 얻는다.
			WORD index1 = _MetaData->Indices[fi * 3 + 0];
			WORD index2 = _MetaData->Indices[fi * 3 + 1];
			WORD index3 = _MetaData->Indices[fi * 3 + 2];

			//	삼각형에 해당하는 정점3개를 구한다.
			XMVECTOR Vtx1 = XMLoadFloat3(&_MetaData->Vertices[index1].Position);
			XMVECTOR Vtx2 = XMLoadFloat3(&_MetaData->Vertices[index2].Position);
			XMVECTOR Vtx3 = XMLoadFloat3(&_MetaData->Vertices[index3].Position);

			// 삼각형에 해당하는 텍스터 정점3개를 구한다.
			XMVECTOR TexVtx1 = XMLoadFloat2(&_MetaData->Vertices[index1].TexUV);
			XMVECTOR TexVtx2 = XMLoadFloat2(&_MetaData->Vertices[index2].TexUV);
			XMVECTOR TexVtx3 = XMLoadFloat2(&_MetaData->Vertices[index3].TexUV);

			// 탄젠트 xyz 구하기
			float x1 = XMVectorGetX(Vtx2) - XMVectorGetX(Vtx1);
			float x2 = XMVectorGetX(Vtx3) - XMVectorGetX(Vtx1);
			float y1 = XMVectorGetY(Vtx2) - XMVectorGetY(Vtx1);
			float y2 = XMVectorGetY(Vtx3) - XMVectorGetY(Vtx1);
			float z1 = XMVectorGetZ(Vtx2) - XMVectorGetZ(Vtx1);
			float z2 = XMVectorGetZ(Vtx3) - XMVectorGetZ(Vtx1);

			// 탄젠트 w 구하기
			float s1 = XMVectorGetX(TexVtx2) - XMVectorGetX(TexVtx1);
			float s2 = XMVectorGetX(TexVtx3) - XMVectorGetX(TexVtx1);
			float t1 = XMVectorGetY(TexVtx2) - XMVectorGetY(TexVtx1);
			float t2 = XMVectorGetY(TexVtx3) - XMVectorGetY(TexVtx1);

			// 예외 셋팅
			XMVECTOR sdir;
			XMVECTOR tdir;
			if ((s1 * t2 - s2 * t1) == 0.0f)
			{
				sdir = XMLoadFloat3(&XMFLOAT3(1.0f, 0.0f, 0.0f));
				tdir = XMLoadFloat3(&XMFLOAT3(0.0f, 1.0f, 0.0f));
			}
			else
			{
				// 공식 대입
				float r = 1.0F / (s1 * t2 - s2 * t1);
				sdir = XMLoadFloat3(&XMFLOAT3((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r, (t2 * z1 - t1 * z2) * r));
				tdir = XMLoadFloat3(&XMFLOAT3((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r, (s1 * z2 - s2 * z1) * r));
			}

			// 값 누적
			tan1[index1] += sdir;
			tan1[index2] += sdir;
			tan1[index3] += sdir;

			tan2[index1] += tdir;
			tan2[index2] += tdir;
			tan2[index3] += tdir;
		}

		// 값 복사
		for (int v = 0; v < (int)_MetaData->Vertices.size(); ++v)
		{
			const XMVECTOR& n = XMLoadFloat3(&_MetaData->Vertices[v].Normal);
			const XMVECTOR& t = tan1[v];

			// Gram-Schmidt orthogonalize
			// 탄젠트 Vtx 저장하기 (xyz)
			XMStoreFloat3(&_MetaData->Vertices[v].TangentU, XMVector3Normalize((t - n * XMVector3Dot(n, t))));

			// Calculate handedness
			//// 텍스처 좌표 방향벡터 구한다. (w) <-- 일단 안구함 (나중에 구해서 쓸꺼)
			float TangentU = (XMVectorGetX(XMVector3Dot(XMVector3Cross(n, t), tan2[v])) < 0.0F) ? -1.0f : 1.0f;

			// 외적하여 계산한다
			XMVECTOR BiNormal = XMVector3Cross(XMLoadFloat3(&_MetaData->Vertices[v].TangentU), n) * TangentU;

			// 결과 값 저장
			XMStoreFloat3(&_MetaData->Vertices[v].BiNormal, BiNormal);
		}

		tan1.clear();
		tan2.clear();
	}

	// 맵 계산
	void CalMap(InitMetaData* _InitMetaData, float& _Xwidth, float& _Zdepth, float& _CellSize)
	{
		CalVtx(_InitMetaData, _Xwidth, _Zdepth, _CellSize);
		CalIB (_InitMetaData, _Xwidth, _Zdepth);
	}

	// 버텍스 계산
	void CalVtx(InitMetaData* _InitMetaData, float& _Xwidth, float& _Zdepth, float& _CellSize)
	{
		// 중간좌표 저장
		float halfWidth = 0.5f * (_Xwidth * _CellSize);
		float halfDepth = 0.5f * (_Zdepth * _CellSize);

		// 정규화된 텍스처 좌표계로 넘긴다.
		float du = 1.0f / (_Xwidth / _CellSize - 1);
		float dv = 1.0f / (_Zdepth / _CellSize - 1);

		// 맵 중앙에 원점을 두고 맵 생성 
		for (UINT y = 0; y < (UINT)_Zdepth; ++y)
		{
			float zPos = halfDepth - (y * _CellSize);
			for (UINT x = 0; x < (UINT)_Xwidth; ++x)
			{
				float xPos = -halfWidth + (x * _CellSize);

				// 버텍스 넣기
				_InitMetaData->Vertices[y * (UINT)_Xwidth + x].Position = XMFLOAT3(xPos, 0.0f, zPos);

				// 텍스처 버텍스 계산
				_InitMetaData->TexVertices[y * (UINT)_Xwidth + x] = XMFLOAT3(x * du, y * dv, 0.0f);

				// 텍스처 uv 맞추기
				_InitMetaData->Vertices[y * (UINT)_Xwidth + x].TexUV.x = x * du;
				_InitMetaData->Vertices[y * (UINT)_Xwidth + x].TexUV.y = y * dv;
			}
		}
	}

	// 인덱스 계산
	void CalIB(InitMetaData* _InitMetaData, float& _Xwidth, float& _Zdepth)
	{
		// 인덱스 계산
		int baseIdx = 0;
		for (UINT y = 0; y < _Zdepth - 1; ++y)
		{
			for (UINT x = 0; x < _Xwidth - 1; ++x)
			{
				//----------------------------------------------------------------//
				// 인덱스
				//----------------------------------------------------------------//
				// 상단 인덱스
				_InitMetaData->Indices[baseIdx + 0] = (UINT)(y * _Xwidth + x);
				_InitMetaData->Indices[baseIdx + 1] = (UINT)(y * _Xwidth + x + 1);
				_InitMetaData->Indices[baseIdx + 2] = (UINT)((y + 1) * _Xwidth + x);

				// 아래 인덱스
				_InitMetaData->Indices[baseIdx + 3] = (UINT)((y + 1) * _Xwidth + x);
				_InitMetaData->Indices[baseIdx + 4] = (UINT)(y * _Xwidth + x + 1);
				_InitMetaData->Indices[baseIdx + 5] = (UINT)((y + 1) * _Xwidth + x + 1);


				//----------------------------------------------------------------//
				// 텍스처 인덱스
				//----------------------------------------------------------------//
				// 상단 인덱스
				_InitMetaData->TexIndices[baseIdx + 0] = (UINT)(y * _Xwidth + x);
				_InitMetaData->TexIndices[baseIdx + 1] = (UINT)(y * _Xwidth + x + 1);
				_InitMetaData->TexIndices[baseIdx + 2] = (UINT)((y + 1) * _Xwidth + x);

				// 아래 인덱스
				_InitMetaData->TexIndices[baseIdx + 3] = (UINT)((y + 1) * _Xwidth + x);
				_InitMetaData->TexIndices[baseIdx + 4] = (UINT)(y * _Xwidth + x + 1);
				_InitMetaData->TexIndices[baseIdx + 5] = (UINT)((y + 1) * _Xwidth + x + 1);

				// 다음 사각형
				baseIdx += 6;
			}
		}
	}


	// 하이트맵 로딩
	void LoadHeightmap(InitMetaData* _InitMetaData, string& _hightMapPss, float& _Xwidth, float& _Zdepth, float& _HeightScale)
	{
		// 전체 크기
		int tMapSize = (int)(_Xwidth * _Zdepth);

		// 하이트맵 읽어드릴 버퍼
		vector<unsigned char> in(tMapSize);

		// 변환
		wstring _WsTexName;
		StringToWchar_t(_hightMapPss, _WsTexName);

		// 하이트맵 바이너리로 읽기
		std::ifstream inFile;
		inFile.open(_WsTexName.c_str(), std::ios_base::binary);

		// 하이트맵 읽기
		if (inFile)
		{
			// Read the RAW bytes.
			inFile.read((char*)&in[0], (std::streamsize)in.size());

			// Done with file.
			inFile.close();
		}
		
		// 데이터 가공 및 복사
		for (int i = 0; i < tMapSize; ++i)
			_InitMetaData->mHeightmap[i] = (in[i] / 255.0f) * _HeightScale;
	}
	
	// 부드럽게하기 <-- 0~255의 정수값을 실수값으로 가공하는 과정
	void Smooth(InitMetaData* _InitMetaData, float& _Xwidth, float& _Zdepth)
	{
		// 임시버퍼
		std::vector<float> dest(_InitMetaData->mHeightmap.size());

		for (UINT y = 0; y < (UINT)_Zdepth; ++y)
		{
			for (UINT x = 0; x < (UINT)_Xwidth; ++x)
			{
				dest[y * (UINT)_Xwidth + x] = Average(_InitMetaData, _Xwidth, _Zdepth, y, x);
			}
		}
		
		// 복사해서 넣기
		_InitMetaData->mHeightmap = dest;
	}

	// 평균 높이 계산
	float Average(InitMetaData* _InitMetaData, float& _Xwidth, float& _Zdepth, int y, int x)
	{
		// 함수는 ij 요소의 평균 높이를 계산합니다.
		// 그것은 8 개의 이웃 한 픽셀들로 그것 자체를 평균화한다.
		// 픽셀에 이웃이 없다면 포함시키지 말라.
		// 즉, 가장자리 픽셀에는 인접 픽셀이 없습니다.
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

		// 음수를 허용하려면 int를 사용하십시오. 우리가 UINT를 사용한다면, @ i = 0, m = i-1 = UINT_MAX
		// 외부 for 루프의 반복은 발생하지 않습니다.
		for (int m = y - 1; m <= y + 1; ++m)
		{
			for (int n = x - 1; n <= x + 1; ++n)
			{
				// 실제 유효한 값인지 확인
				if (InBounds(_Xwidth, _Zdepth, m, n))
				{
					avg += _InitMetaData->mHeightmap[m * (int)_Xwidth + n];
					num += 1.0f;
				}
			}
		}

		return avg / num;
	}

	// 실제 유효한 값인지 확인
	bool InBounds(float& _Xwidth, float& _Zdepth, int y, int x)
	{
		// ij가 유효한 인덱스 인 경우 참; 그렇지 않은 경우는 false
		return
			y >= 0 && y < (int)_Zdepth &&
			x >= 0 && x < (int)_Xwidth;
	}

	// 파일 읽기 테스트 (모델 데이터 경로 로드)
	void ReadLocMyFormat_Model(string _FinModelLoc)
	{
		// 본 파일에서는 mFinExportModelLoc 이 부분에 해당하는 문자열을
		// 모델 생성 부분에 넣어 해당 변수에 넣어줘야 한다.

		// 이진 파일 열기
		FileOpen(_FinModelLoc.c_str(), "rb");

		// 만들어진 모델 몇개인지
		int _ModelNum = 0;
		fread(&_ModelNum, sizeof(int), 1, mFilePointer);
		mNewModleLoc.resize(_ModelNum);

		// 매쉬 위치 데이터 저장
		for (int i = 0; i < _ModelNum; ++i)
		{
			int len = 0;
			fread(&len, sizeof(int), 1, mFilePointer);
			fread(&mNewModleLoc[i], sizeof(char), len, mFilePointer);
		}

		// 파일 종료
		FileClose();
		printf("[Succes] 파일 종료.\n");
		_FinModelLoc.clear();
	}

	// 내 포맷으로 읽는다.(데이터)
	void ReadDataMyFormat_Model(string& _FinExportModelData)
	{
		// 이진 파일 열기
		FileOpen(_FinExportModelData.c_str(), "rb");
		int len = 0;

		//------------------------------------------------------------------------------//
		// 본 데이터
		//------------------------------------------------------------------------------//

		// 버텍스
		fread(&len, sizeof(int), 1, mFilePointer);
		mMyMeshData.vertices.resize(len);

		for (int i = 0; i < len; ++i)
			fread(&mMyMeshData.vertices[i], sizeof(Vertex), 1, mFilePointer);

		// 인덱스 리스트
		fread(&len, sizeof(int), 1, mFilePointer);
		mMyMeshData.indices.resize(len);

		for (int i = 0; i < len; ++i)
			fread(&mMyMeshData.indices[i], sizeof(XMFLOAT3), 1, mFilePointer);

		// 애니데이터

		// 위치
		fread(&len, sizeof(int), 1, mFilePointer);
		mMyMeshData.mAniData.Position.resize(len);

		for (int i = 0; i < len; ++i)
			fread(&mMyMeshData.mAniData.Position[i], sizeof(KeyVtx), 1, mFilePointer);

		// 회전
		fread(&len, sizeof(int), 1, mFilePointer);
		mMyMeshData.mAniData.Quaternion.resize(len);

		for (int i = 0; i < len; ++i)
			fread(&mMyMeshData.mAniData.Quaternion[i], sizeof(RotKeyVtx), 1, mFilePointer);

		// 스케일
		fread(&len, sizeof(int), 1, mFilePointer);
		mMyMeshData.mAniData.Scale.resize(len);

		for (int i = 0; i < len; ++i)
			fread(&mMyMeshData.mAniData.Scale[i], sizeof(KeyVtx), 1, mFilePointer);


		// 가중치 데이터

		// 전체 사이즈
		fread(&len, sizeof(int), 1, mFilePointer);
		mMyMeshData.weightVtx.resize(len);

		for (int i = 0; i < len; ++i)
		{
			// 타겟 버텍스 인덱스
			fread(&mMyMeshData.weightVtx[i].TgVtxIdx, sizeof(int), 1, mFilePointer);

			// 본 데이터 총 크기
			int Bonelen = 0;
			fread(&Bonelen, sizeof(int), 1, mFilePointer);
			mMyMeshData.weightVtx[i].Bone.resize(Bonelen);

			// 본 데이터
			for (int idx = 0; idx < Bonelen; ++idx)
			{
				// ID
				fread(&mMyMeshData.weightVtx[i].Bone[idx].ID, sizeof(int), 1, mFilePointer);

				// 가중치
				fread(&mMyMeshData.weightVtx[i].Bone[idx].Weight, sizeof(float), 1, mFilePointer);

				// 버퍼(이름)
				int Bonebuflen = 0;
				fread(&Bonebuflen, sizeof(int), 1, mFilePointer);
				fread(&mMyMeshData.weightVtx[i].Bone[idx].Name, sizeof(char), Bonebuflen, mFilePointer);
			}
		}

		// 오프셋
		fread(&mMyMeshData.vertexOffset, sizeof(UINT), 1, mFilePointer);
		fread(&mMyMeshData.indexOffset, sizeof(UINT), 1, mFilePointer);

		// 카운트
		fread(&mMyMeshData.indexCount, sizeof(UINT), 1, mFilePointer);

		// 재질 정보

		// 투명도
		fread(&mMyMeshData.mMyMat.mOpacity, sizeof(float), 1, mFilePointer);

		// 디퓨즈
		fread(&len, sizeof(int), 1, mFilePointer);
		fread(&mMyMeshData.mMyMat.mDiffuseSRV, sizeof(char), len, mFilePointer);

		// 노멀
		fread(&len, sizeof(int), 1, mFilePointer);
		fread(&mMyMeshData.mMyMat.mNomalSRV, sizeof(char), len, mFilePointer);

		// 스팩
		fread(&len, sizeof(int), 1, mFilePointer);
		fread(&mMyMeshData.mMyMat.mSpecularSRV, sizeof(char), len, mFilePointer);


		// TM 행렬
		fread(&mMyMeshData.mTMLocalMtx, sizeof(XMFLOAT4X4), 1, mFilePointer);
		fread(&mMyMeshData.mTMWorldMtx, sizeof(XMFLOAT4X4), 1, mFilePointer);

		// 바운딩 박스
		fread(&mMyMeshData.mBoundingBox, sizeof(BoundBox), 1, mFilePointer);

		//--------------------------------------------------//
		// 오브젝트 식별 정보
		//--------------------------------------------------//

		// 이름
		fread(&len, sizeof(int), 1, mFilePointer);
		fread(&mMyMeshData.mMainName, sizeof(char), len, mFilePointer);

		// 오브젝트이름
		fread(&mMyMeshData.mObjID, sizeof(int), 1, mFilePointer);

		fread(&len, sizeof(int), 1, mFilePointer);
		fread(&mMyMeshData.mObjName, sizeof(char), len, mFilePointer);

		fread(&len, sizeof(int), 1, mFilePointer);
		fread(&mMyMeshData.mObjClass, sizeof(char), len, mFilePointer);

		// 상위 클래스 이름
		fread(&mMyMeshData.mParentID, sizeof(int), 1, mFilePointer);

		fread(&len, sizeof(int), 1, mFilePointer);
		fread(&mMyMeshData.mParentName, sizeof(char), len, mFilePointer);

		// 파일 종료
		FileClose();
		printf("[Succes] 파일 종료.\n");
	}

	// 내 포맷으로 읽는다.(데이터) - 본
	void ReadDataMyFormat_Bone(string& _BoneRoute, MyBoneData* _MyBoneData)
	{
		// 이진 파일 열기
		FileOpen(_BoneRoute.c_str(), "rb");

		// Bone 이름
		int len = 0;
		fread(&len, sizeof(int), 1, mFilePointer);
		fread(&_MyBoneData->mMainName, sizeof(char), len, mFilePointer);

		// Bone 애니 이름
		fread(&len, sizeof(int), 1, mFilePointer);
		fread(&_MyBoneData->mAniName, sizeof(char), len, mFilePointer);

		// 본 데이터 사이즈
		fread(&len, sizeof(int), 1, mFilePointer);
		_MyBoneData->mSaveBoneData.resize(len);

		// 본 데이터에 등룩된 만큼
		for (unsigned int i = 0; i < _MyBoneData->mSaveBoneData.size(); ++i)
		{
			// TM 행렬
			fread(&_MyBoneData->mSaveBoneData[i].mTMLocalMtx, sizeof(XMFLOAT4X4), 1, mFilePointer);
			fread(&_MyBoneData->mSaveBoneData[i].mTMWorldMtx, sizeof(XMFLOAT4X4), 1, mFilePointer);

			// 애니데이터

			// 위치
			fread(&len, sizeof(int), 1, mFilePointer);
			_MyBoneData->mSaveBoneData[i].mAniData.Position.resize(len);
			for (int idx = 0; idx < len; ++idx)
				fread(&_MyBoneData->mSaveBoneData[i].mAniData.Position[idx], sizeof(KeyVtx), 1, mFilePointer);

			// 회전
			fread(&len, sizeof(int), 1, mFilePointer);
			_MyBoneData->mSaveBoneData[i].mAniData.Quaternion.resize(len);
			for (int idx = 0; idx < len; ++idx)
				fread(&_MyBoneData->mSaveBoneData[i].mAniData.Quaternion[idx], sizeof(RotKeyVtx), 1, mFilePointer);

			// 스케일
			fread(&len, sizeof(int), 1, mFilePointer);
			_MyBoneData->mSaveBoneData[i].mAniData.Scale.resize(len);
			for (int idx = 0; idx < len; ++idx)
				fread(&_MyBoneData->mSaveBoneData[i].mAniData.Scale[idx], sizeof(KeyVtx), 1, mFilePointer);


			// 바운딩 박스
			fread(&_MyBoneData->mSaveBoneData[i].mBoundingBox, sizeof(BoundBox), 1, mFilePointer);

			//--------------------------------------------------//
			// 오브젝트 식별 정보
			//--------------------------------------------------//

			// 오브젝트이름
			fread(&_MyBoneData->mSaveBoneData[i].mObjID, sizeof(int), 1, mFilePointer);

			fread(&len, sizeof(int), 1, mFilePointer);
			fread(&_MyBoneData->mSaveBoneData[i].mObjName, sizeof(char), len, mFilePointer);

			fread(&len, sizeof(int), 1, mFilePointer);
			fread(&_MyBoneData->mSaveBoneData[i].mObjClass, sizeof(char), len, mFilePointer);

			// 상위 클래스 이름
			fread(&_MyBoneData->mSaveBoneData[i].mParentID, sizeof(int), 1, mFilePointer);

			fread(&len, sizeof(int), 1, mFilePointer);
			fread(&_MyBoneData->mSaveBoneData[i].mParentName, sizeof(char), len, mFilePointer);
		}

		// 본 하이라이키 사이즈
		fread(&len, sizeof(int), 1, mFilePointer);
		_MyBoneData->mBoneHierarchy.resize(len);

		// 본 하이라이키에 등록된 만큼
		for (int i = 0; i < len; ++i)
		{
			// 이터레이터 사이즈
			int idxlen = 0;
			fread(&idxlen, sizeof(int), 1, mFilePointer);
			_MyBoneData->mBoneHierarchy[i].resize(idxlen);

			// 이터레이터 등록된 만큼
			for (int idx = 0; idx < idxlen; ++idx)
			{
				// 오브젝트 키 
				fread(&_MyBoneData->mBoneHierarchy[i][idx].Key, sizeof(int), 1, mFilePointer);

				// 오브젝트 이름
				int Buflen = 0;
				fread(&Buflen, sizeof(int), 1, mFilePointer);
				fread(&_MyBoneData->mBoneHierarchy[i][idx].Name, sizeof(char), Buflen, mFilePointer);

				// 오브젝트 클래스
				fread(&Buflen, sizeof(int), 1, mFilePointer);
				fread(&_MyBoneData->mBoneHierarchy[i][idx].objClass, sizeof(char), Buflen, mFilePointer);
			}
		}

		// 파일 종료
		FileClose();
		printf("[Succes] 파일 종료.\n");
	}

	// 파일 저장하기
	void SaveMyFormat(InitMetaData* _InitMetaData)
	{
		// 버텍스
		_InitMetaData->Vertices = mMyMeshData.vertices;

		// 인덱스
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

		// 애니 데이터
		_InitMetaData->mAniData = mMyMeshData.mAniData;

		// 가중치 데이터
		_InitMetaData->weightVtx = mMyMeshData.weightVtx;

		// TM 매트릭스
		_InitMetaData->mLocTMMtx = mMyMeshData.mTMLocalMtx;
		_InitMetaData->mWdTMMtx  = mMyMeshData.mTMWorldMtx;

		// 오프셋
		_InitMetaData->mVertexOffset = mMyMeshData.vertexOffset;
		_InitMetaData->mIndexOffset  = mMyMeshData.indexOffset;

		// 카운트
		_InitMetaData->mIndexCount = mMyMeshData.indexCount;

		// 리소스 얻기 (재질 정보)
		// 테스트
		//if (_InitMetaData->mCreateName == "Model4_1")
		//	int i = 0;

		LoadTex(_InitMetaData, mMyMeshData.mMyMat.mDiffuseSRV , e_DiffuseMap);
		LoadTex(_InitMetaData, mMyMeshData.mMyMat.mNomalSRV   , e_NomalMap);
		LoadTex(_InitMetaData, mMyMeshData.mMyMat.mSpecularSRV, e_SpecularMap);


		// 투명 (디퓨즈 맵)
		_InitMetaData->mOpacity = mMyMeshData.mMyMat.mOpacity;

		// 바운딩 박스
		_InitMetaData->mBoundingBox = mMyMeshData.mBoundingBox;

		//--------------------------------------------------//
		// 오브젝트 식별 정보
		//--------------------------------------------------//

		// 이름
		strcpy(_InitMetaData->mMainName, mMyMeshData.mMainName);

		// 오브젝트이름
		_InitMetaData->mObjID = mMyMeshData.mObjID;
		strcpy(_InitMetaData->mObjName , mMyMeshData.mObjName);
		strcpy(_InitMetaData->mObjClass, mMyMeshData.mObjClass);

		// 상위 클래스 이름
		_InitMetaData->mParentID = mMyMeshData.mParentID;
		strcpy(_InitMetaData->mParentName, mMyMeshData.mParentName);

	}
private:
	// 파일 열기
	bool FileOpen(string _Route, string _ReadType)
	{
		//해당 경로의 파일을 연다.
		fopen_s(&mFilePointer, _Route.c_str(), _ReadType.c_str());

		// 해당 경로에 파일이 있나?
		if (mFilePointer == NULL)
		{
			printf("[Error] [%s] 열기 실패. <-- 해당경로에 파일이 존재하지 않습니다.\n", _Route);
			return false;
		}
		if ((_ReadType == "wb") || (_ReadType == "wt"))
		{
			rewind(mFilePointer);
			cout << "[Succes] [" << _Route << "] 쓰기모드 시작.\n";
		}
		else if ((_ReadType == "rb") || (_ReadType == "rt"))
		{
			rewind(mFilePointer);
			cout << "[Succes] [" << _Route << "] 열기모드 시작.\n";
		}
		return true;
	}

	void FileClose()
	{
		fclose(mFilePointer);
		mFilePointer = nullptr;
	}

	// 텍스처 로드
	void LoadTex(InitMetaData* _InitMetaData, string _TexName, TEXTURE_TYPE e_InitTex)
	{
		// string --> wString으로 변환
		wstring _WsTexName;
		StringToWchar_t(_TexName, _WsTexName);

		// 텍스처 로딩
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

	// 변환 함수
	void StringToWchar_t(string& _string, wstring& _wstring)
	{
		// 변환
		for (unsigned int i = 0; i < _string.length(); ++i)
			_wstring += wchar_t(_string[i]);

		// 마무리
		_wstring += wchar_t('\0');
	}
};

