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
	void LoadXMLModel(string& _ModelData, InitMetaData& _InitMetaData)
	{
		// XML �� ������ �б�
		ReadDataMyFormat_Model(_ModelData); 

		// ���� ������ ������ �ֱ�
		SaveMyFormat(_InitMetaData);
	}

	// ���� �ڽ� �����
	// �ڽ� �����
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

		_InitMetaData.TexVertices.assign(&tv[0], &tv[8]);

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


		// �ؽ�ó
		XMFLOAT3 tv[4];

		// uv��ǥ ���� ���� 
		// - ���� ���׸��� �ȵǰ� ����
		tv[0] = XMFLOAT3(0.0f, 0.0f, 0.0f);
		tv[1] = XMFLOAT3(1.0f, 0.0f, 0.0f);
		tv[2] = XMFLOAT3(0.0f, 1.0f, 0.0f);
		tv[3] = XMFLOAT3(1.0f, 1.0f, 0.0f);

		_InitMetaData.TexVertices.assign(&tv[0], &tv[4]);

		// �ؽ�Ʈ �ε��� ��ǥ ���� ����
		UINT ti[6];

		ti[0] = 0; ti[1] = 1; ti[2] = 2;
		ti[3] = 1; ti[4] = 3; ti[5] = 2;

		_InitMetaData.TexIndices.assign(&ti[0], &ti[6]);

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

	// �� �������� �д´�.(������) // <--- �׽�Ʈ
	void ReadDataMyFormat_Model(string _FinExportModelData)
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
			fread(&mMyMeshData.mAniData.Quaternion[i], sizeof(KeyVtx), 1, mFilePointer);

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
			// Ÿ�� �ε���
			fread(&mMyMeshData.weightVtx[i].TagetIdx, sizeof(int), 1, mFilePointer);

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
				fread(&_MyBoneData->mSaveBoneData[i].mAniData.Quaternion[idx], sizeof(KeyVtx), 1, mFilePointer);

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
	void SaveMyFormat(InitMetaData& _InitMetaData)
	{
		// ���ؽ�
		_InitMetaData.Vertices = mMyMeshData.vertices;

		// �ε���
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

		// �ִ� ������
		_InitMetaData.mAniData = mMyMeshData.mAniData;

		// ����ġ ������
		_InitMetaData.weightVtx = mMyMeshData.weightVtx;

		// TM ��Ʈ����
		_InitMetaData.mLocTMMtx = mMyMeshData.mTMLocalMtx;
		_InitMetaData.mWdTMMtx  = mMyMeshData.mTMWorldMtx;

		// ������
		_InitMetaData.mVertexOffset = mMyMeshData.vertexOffset;
		_InitMetaData.mIndexOffset  = mMyMeshData.indexOffset;

		// ī��Ʈ
		_InitMetaData.mIndexCount = mMyMeshData.indexCount;

		// ���ҽ� ��� (���� ����)
		// �׽�Ʈ
		//if (_InitMetaData.mCreateName == "Model4_1")
		//	int i = 0;

		LoadTex(_InitMetaData, mMyMeshData.mMyMat.mDiffuseSRV , e_DiffuseMap);
		LoadTex(_InitMetaData, mMyMeshData.mMyMat.mNomalSRV   , e_NomalMap);
		LoadTex(_InitMetaData, mMyMeshData.mMyMat.mSpecularSRV, e_SpecularMap);


		// ���� (��ǻ�� ��)
		_InitMetaData.mOpacity = mMyMeshData.mMyMat.mOpacity;

		// �ٿ�� �ڽ�
		_InitMetaData.mBoundingBox = mMyMeshData.mBoundingBox;

		//--------------------------------------------------//
		// ������Ʈ �ĺ� ����
		//--------------------------------------------------//

		// �̸�
		strcpy(_InitMetaData.mMainName, mMyMeshData.mMainName);

		// ������Ʈ�̸�
		_InitMetaData.mObjID = mMyMeshData.mObjID;
		strcpy(_InitMetaData.mObjName , mMyMeshData.mObjName);
		strcpy(_InitMetaData.mObjClass, mMyMeshData.mObjClass);

		// ���� Ŭ���� �̸�
		_InitMetaData.mParentID = mMyMeshData.mParentID;
		strcpy(_InitMetaData.mParentName, mMyMeshData.mParentName);

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
	//void LoadTex(const wchar_t* _TexName, TEXTURE_TYPE e_InitTex)
	//{
	//	// �ؽ�ó �ε�
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

	// �ؽ�ó �ε�
	void LoadTex(InitMetaData& _InitMetaData, string _TexName, TEXTURE_TYPE e_InitTex)
	{
		// string --> wString���� ��ȯ
		wstring _WsTexName;
		StringToWchar_t(_TexName.c_str(), _WsTexName);

		// �ؽ�ó �ε�
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

		// Ŭ����
		_WsTexName.clear();
		_TexName.clear();
	}

	// ��ȯ �Լ�
	void StringToWchar_t(string _string, wstring& _wstring)
	{
		// ��ȯ
		for (unsigned int i = 0; i < _string.length(); ++i)
			_wstring += wchar_t(_string[i]);

		// ������
		_wstring += wchar_t('\0');

		_string.clear();
	}
};

