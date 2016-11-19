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
	void LoadXMLModel(string& _ModelData, InitMetaData& _InitMetaData)
	{
		// XML 모델 데이터 읽기
		ReadDataMyFormat_Model(_ModelData); 

		// 얻은 데이터 복사해 넣기
		SaveMyFormat(_InitMetaData);
	}

	// 더미 박스 만들기
	// 박스 만들기
	void LoadBox(InitMetaData& _InitMetaData, float width, float height, float depth)
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

		_InitMetaData.Vertices.assign(&v[0], &v[8]);


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

		_InitMetaData.Indices.assign(&i[0], &i[36]);


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

		_InitMetaData.TexVertices.assign(&tv[0], &tv[8]);

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

		_InitMetaData.TexIndices.assign(&ti[0], &ti[36]);
	}

	void LoadScreen(InitMetaData& _InitMetaData, float width, float height)
	{
		Vertex v[8];

		float w2 = width;
		float h2 = height;

		// Fill in the front face vertex data.
		v[0] = Vertex(-w2, +h2, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
		v[1] = Vertex(+w2, +h2, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
		v[2] = Vertex(-w2, -h2, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
		v[3] = Vertex(+w2, -h2, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

		_InitMetaData.Vertices.assign(&v[0], &v[4]);


		// Create the indices.
		UINT i[6];

		i[0] = 0; i[1] = 1; i[2] = 2;
		i[3] = 1; i[4] = 3; i[5] = 2;

		_InitMetaData.Indices.assign(&i[0], &i[6]);


		// 텍스처
		XMFLOAT3 tv[4];

		// uv좌표 임의 구성 
		// - 서브 머테리얼 안되게 구성
		tv[0] = XMFLOAT3(0.0f, 0.0f, 0.0f);
		tv[1] = XMFLOAT3(1.0f, 0.0f, 0.0f);
		tv[2] = XMFLOAT3(0.0f, 1.0f, 0.0f);
		tv[3] = XMFLOAT3(1.0f, 1.0f, 0.0f);

		_InitMetaData.TexVertices.assign(&tv[0], &tv[4]);

		// 텍스트 인덱스 좌표 임의 구성
		UINT ti[6];

		ti[0] = 0; ti[1] = 1; ti[2] = 2;
		ti[3] = 1; ti[4] = 3; ti[5] = 2;

		_InitMetaData.TexIndices.assign(&ti[0], &ti[6]);

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

	// 내 포맷으로 읽는다.(데이터) // <--- 테스트
	void ReadDataMyFormat_Model(string _FinExportModelData)
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
			fread(&mMyMeshData.mAniData.Quaternion[i], sizeof(KeyVtx), 1, mFilePointer);

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
			// 타겟 인덱스
			fread(&mMyMeshData.weightVtx[i].TagetIdx, sizeof(int), 1, mFilePointer);

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
				fread(&_MyBoneData->mSaveBoneData[i].mAniData.Quaternion[idx], sizeof(KeyVtx), 1, mFilePointer);

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
	void SaveMyFormat(InitMetaData& _InitMetaData)
	{
		// 버텍스
		_InitMetaData.Vertices = mMyMeshData.vertices;

		// 인덱스
		for (unsigned int i = 0; i < mMyMeshData.indices.size(); ++i)
		{
			for (int vIdx = 0; vIdx < 3; ++vIdx)
			{
				switch (vIdx)
				{
				case 0:
					_InitMetaData.Indices.push_back((UINT)mMyMeshData.indices[i].x);
					break;
				case 1:
					_InitMetaData.Indices.push_back((UINT)mMyMeshData.indices[i].y);
					break;
				case 2:
					_InitMetaData.Indices.push_back((UINT)mMyMeshData.indices[i].z);
					break;
				default:
					break;
				}
			}
		}

		// 애니 데이터
		_InitMetaData.mAniData = mMyMeshData.mAniData;

		// 가중치 데이터
		_InitMetaData.weightVtx = mMyMeshData.weightVtx;

		// TM 매트릭스
		_InitMetaData.mLocTMMtx = mMyMeshData.mTMLocalMtx;
		_InitMetaData.mWdTMMtx  = mMyMeshData.mTMWorldMtx;

		// 오프셋
		_InitMetaData.mVertexOffset = mMyMeshData.vertexOffset;
		_InitMetaData.mIndexOffset  = mMyMeshData.indexOffset;

		// 카운트
		_InitMetaData.mIndexCount = mMyMeshData.indexCount;

		// 리소스 얻기 (재질 정보)
		// 테스트
		//if (_InitMetaData.mCreateName == "Model4_1")
		//	int i = 0;

		LoadTex(_InitMetaData, mMyMeshData.mMyMat.mDiffuseSRV , e_DiffuseMap);
		LoadTex(_InitMetaData, mMyMeshData.mMyMat.mNomalSRV   , e_NomalMap);
		LoadTex(_InitMetaData, mMyMeshData.mMyMat.mSpecularSRV, e_SpecularMap);


		// 투명 (디퓨즈 맵)
		_InitMetaData.mOpacity = mMyMeshData.mMyMat.mOpacity;

		// 바운딩 박스
		_InitMetaData.mBoundingBox = mMyMeshData.mBoundingBox;

		//--------------------------------------------------//
		// 오브젝트 식별 정보
		//--------------------------------------------------//

		// 이름
		strcpy(_InitMetaData.mMainName, mMyMeshData.mMainName);

		// 오브젝트이름
		_InitMetaData.mObjID = mMyMeshData.mObjID;
		strcpy(_InitMetaData.mObjName , mMyMeshData.mObjName);
		strcpy(_InitMetaData.mObjClass, mMyMeshData.mObjClass);

		// 상위 클래스 이름
		_InitMetaData.mParentID = mMyMeshData.mParentID;
		strcpy(_InitMetaData.mParentName, mMyMeshData.mParentName);

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
	//void LoadTex(const wchar_t* _TexName, TEXTURE_TYPE e_InitTex)
	//{
	//	// 텍스처 로딩
	//	switch (e_InitTex)
	//	{
	//	case e_DiffuseMap:
	//		HR(D3DX11CreateShaderResourceViewFromFile(mCoreStorage->md3dDevice, _TexName, 0, 0, &_InitMetaData.mDiffuseSRV, 0));
	//		break;
	//	case e_SpecularMap:
	//		HR(D3DX11CreateShaderResourceViewFromFile(mCoreStorage->md3dDevice, _TexName, 0, 0, &_InitMetaData.mSpecularSRV, 0));
	//		break;
	//	case e_NomalMap:
	//		HR(D3DX11CreateShaderResourceViewFromFile(mCoreStorage->md3dDevice, _TexName, 0, 0, &_InitMetaData.mNomalSRV, 0));
	//		break;
	//	}
	//}

	// 텍스처 로드
	void LoadTex(InitMetaData& _InitMetaData, string _TexName, TEXTURE_TYPE e_InitTex)
	{
		// string --> wString으로 변환
		wstring _WsTexName;
		StringToWchar_t(_TexName.c_str(), _WsTexName);

		// 텍스처 로딩
		switch (e_InitTex)
		{
		case e_DiffuseMap:
			HR(D3DX11CreateShaderResourceViewFromFile(mCoreStorage->md3dDevice, _WsTexName.c_str(), 0, 0, &_InitMetaData.mDiffuseSRV, 0));
			break;
		case e_NomalMap:
			HR(D3DX11CreateShaderResourceViewFromFile(mCoreStorage->md3dDevice, _WsTexName.c_str(), 0, 0, &_InitMetaData.mNomalSRV, 0));
			break;
		case e_SpecularMap:
			HR(D3DX11CreateShaderResourceViewFromFile(mCoreStorage->md3dDevice, _WsTexName.c_str(), 0, 0, &_InitMetaData.mSpecularSRV, 0));
			break;
		}

		//HRESULT WINAPI
		//	D3DX11CreateShaderResourceViewFromFileW(
		//	ID3D11Device*               pDevice,
		//	LPCWSTR                     pSrcFile,
		//	D3DX11_IMAGE_LOAD_INFO      *pLoadInfo,
		//	ID3DX11ThreadPump*          pPump,
		//	ID3D11ShaderResourceView**  ppShaderResourceView,
		//	HRESULT*                    pHResult);

		// 클리어
		_WsTexName.clear();
		_TexName.clear();
	}

	// 변환 함수
	void StringToWchar_t(string _string, wstring& _wstring)
	{
		// 변환
		for (unsigned int i = 0; i < _string.length(); ++i)
			_wstring += wchar_t(_string[i]);

		// 마무리
		_wstring += wchar_t('\0');

		_string.clear();
	}
};

