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

	// �� ������
	InitMetaData* mInitMetaData;

	// ���� ������
	FILE* mFilePointer = nullptr;

	// ����
	char mReadBuf[BUF_SIZE];
	char mSaveBuf[BUF_SIZE];
	
public:
	cXMLParser()
	{
		InitClass();
	}

	~cXMLParser()
	{
		ClearClass();
	}

	void InitClass()
	{
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
		mInitMetaData = nullptr;
		mFilePointer  = nullptr;
	}

	// �Ľ�
	void LoadXMLRoute(string& _ModelRoute)
	{
		// XML ��Ʈ ������ �б�
		ReadLocMyFormat_Model(_ModelRoute);
	}

	// �Ľ�
	void LoadXMLModel(string& _ModelData)
	{
		// XML �� ������ �б�
		ReadDataMyFormat_Model(_ModelData); 

		// ���� ������ ������ �ֱ�
		SaveMyFormat();
	}

	// �� ���� ���
	void SetModelBuf(InitMetaData* _InitMetaData)
	{
		mInitMetaData = _InitMetaData;
	}

	// ���� �ڽ� �����
	// �ڽ� �����
	void LoadBox(float width, float height, float depth)
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

		mInitMetaData->Vertices.assign(&v[0], &v[8]);


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

		mInitMetaData->Indices.assign(&i[0], &i[36]);


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

		mInitMetaData->TexVertices.assign(&tv[0], &tv[8]);

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

		mInitMetaData->TexIndices.assign(&ti[0], &ti[36]);

		// ����� ����
		mInitMetaData = nullptr;

	}

	void LoadScreen(float width, float height)
	{
		Vertex v[8];

		float w2 = width;
		float h2 = height;

		// Fill in the front face vertex data.
		v[0] = Vertex(-w2, +h2, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
		v[1] = Vertex(+w2, +h2, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
		v[2] = Vertex(-w2, -h2, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
		v[3] = Vertex(+w2, -h2, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

		mInitMetaData->Vertices.assign(&v[0], &v[4]);


		// Create the indices.
		UINT i[6];

		i[0] = 0; i[1] = 1; i[2] = 2;
		i[3] = 1; i[4] = 3; i[5] = 2;

		mInitMetaData->Indices.assign(&i[0], &i[6]);


		// �ؽ�ó
		XMFLOAT3 tv[4];

		// uv��ǥ ���� ���� 
		// - ���� ���׸��� �ȵǰ� ����
		tv[0] = XMFLOAT3(0.0f, 0.0f, 0.0f);
		tv[1] = XMFLOAT3(1.0f, 0.0f, 0.0f);
		tv[2] = XMFLOAT3(0.0f, 1.0f, 0.0f);
		tv[3] = XMFLOAT3(1.0f, 1.0f, 0.0f);

		mInitMetaData->TexVertices.assign(&tv[0], &tv[4]);

		// �ؽ�Ʈ �ε��� ��ǥ ���� ����
		UINT ti[6];

		ti[0] = 0; ti[1] = 1; ti[2] = 2;
		ti[3] = 1; ti[4] = 3; ti[5] = 2;

		mInitMetaData->TexIndices.assign(&ti[0], &ti[6]);

		// ����� ����
		mInitMetaData = nullptr;
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
		fclose(mFilePointer);
		printf("[Succes] ���� ����.\n");
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
		mMyMeshData.aniData.Position.resize(len);

		for (int i = 0; i < len; ++i)
			fread(&mMyMeshData.aniData.Position[i], sizeof(KeyVtx), 1, mFilePointer);

		// ȸ��
		fread(&len, sizeof(int), 1, mFilePointer);
		mMyMeshData.aniData.Quaternion.resize(len);

		for (int i = 0; i < len; ++i)
			fread(&mMyMeshData.aniData.Quaternion[i], sizeof(KeyVtx), 1, mFilePointer);

		// ������
		fread(&len, sizeof(int), 1, mFilePointer);
		mMyMeshData.aniData.Scale.resize(len);

		for (int i = 0; i < len; ++i)
			fread(&mMyMeshData.aniData.Scale[i], sizeof(KeyVtx), 1, mFilePointer);


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
		fread(&mMyMeshData.vertexOffset, sizeof(int), 1, mFilePointer);
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
		fclose(mFilePointer);
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
		_MyBoneData->mBoneData.resize(len);

		// �� �����Ϳ� ���� ��ŭ
		for (unsigned int i = 0; i < _MyBoneData->mBoneData.size(); ++i)
		{
			// TM ���
			fread(&_MyBoneData->mBoneData[i].mTMLocalMtx, sizeof(XMFLOAT4X4), 1, mFilePointer);
			fread(&_MyBoneData->mBoneData[i].mTMWorldMtx, sizeof(XMFLOAT4X4), 1, mFilePointer);

			// �ִϵ�����

			// ��ġ
			fread(&len, sizeof(int), 1, mFilePointer);
			_MyBoneData->mBoneData[i].aniData.Position.resize(len);
			for (int idx = 0; idx < len; ++idx)
				fread(&_MyBoneData->mBoneData[i].aniData.Position[idx], sizeof(KeyVtx), 1, mFilePointer);

			// ȸ��
			fread(&len, sizeof(int), 1, mFilePointer);
			_MyBoneData->mBoneData[i].aniData.Quaternion.resize(len);
			for (int idx = 0; idx < len; ++idx)
				fread(&_MyBoneData->mBoneData[i].aniData.Quaternion[idx], sizeof(KeyVtx), 1, mFilePointer);

			// ������
			fread(&len, sizeof(int), 1, mFilePointer);
			_MyBoneData->mBoneData[i].aniData.Scale.resize(len);
			for (int idx = 0; idx < len; ++idx)
				fread(&_MyBoneData->mBoneData[i].aniData.Scale[idx], sizeof(KeyVtx), 1, mFilePointer);


			// �ٿ�� �ڽ�
			fread(&_MyBoneData->mBoneData[i].mBoundingBox, sizeof(BoundBox), 1, mFilePointer);

			//--------------------------------------------------//
			// ������Ʈ �ĺ� ����
			//--------------------------------------------------//

			// ������Ʈ�̸�
			fread(&_MyBoneData->mBoneData[i].mObjID, sizeof(int), 1, mFilePointer);

			fread(&len, sizeof(int), 1, mFilePointer);
			fread(&_MyBoneData->mBoneData[i].mObjName, sizeof(char), len, mFilePointer);

			fread(&len, sizeof(int), 1, mFilePointer);
			fread(&_MyBoneData->mBoneData[i].mObjClass, sizeof(char), len, mFilePointer);

			// ���� Ŭ���� �̸�
			fread(&_MyBoneData->mBoneData[i].mParentID, sizeof(int), 1, mFilePointer);

			fread(&len, sizeof(int), 1, mFilePointer);
			fread(&_MyBoneData->mBoneData[i].mParentName, sizeof(char), len, mFilePointer);
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
		fclose(mFilePointer);
		printf("[Succes] ���� ����.\n");
	}

	// ���� �����ϱ�
	void SaveMyFormat()
	{
		// ���ؽ�
		mInitMetaData->Vertices = mMyMeshData.vertices;

		// �ε���
		for (unsigned int i = 0; i < mMyMeshData.indices.size(); ++i)
		{
			for (int vIdx = 0; vIdx < 3; ++vIdx)
			{
				switch (vIdx)
				{
				case 0:
					mInitMetaData->Indices.push_back((UINT)mMyMeshData.indices[i].x);
					break;
				case 1:
					mInitMetaData->Indices.push_back((UINT)mMyMeshData.indices[i].y);
					break;
				case 2:
					mInitMetaData->Indices.push_back((UINT)mMyMeshData.indices[i].z);
					break;
				default:
					break;
				}
			}
		}

		// �ִ� ������
		mInitMetaData->aniData = mMyMeshData.aniData;

		// ����ġ ������
		mInitMetaData->weightVtx = mMyMeshData.weightVtx;

		// TM ��Ʈ����
		mInitMetaData->mLocTMMtx = mMyMeshData.mTMLocalMtx;
		mInitMetaData->mWdTMMtx  = mMyMeshData.mTMWorldMtx;

		// ������
		mInitMetaData->mVertexOffset = mMyMeshData.vertexOffset;
		mInitMetaData->mIndexOffset  = mMyMeshData.indexOffset;

		// ī��Ʈ
		mInitMetaData->mIndexCount = mMyMeshData.indexCount;

		// ���ҽ� ��� (���� ����)
		// �׽�Ʈ
		//if (mInitMetaData->mCreateName == "Model4_1")
		//	int i = 0;

		LoadTex(mMyMeshData.mMyMat.mDiffuseSRV , e_DiffuseMap);
		LoadTex(mMyMeshData.mMyMat.mNomalSRV   , e_NomalMap);
		LoadTex(mMyMeshData.mMyMat.mSpecularSRV, e_SpecularMap);


		// ���� (��ǻ�� ��)
		mInitMetaData->mOpacity = mMyMeshData.mMyMat.mOpacity;

		// �ٿ�� �ڽ�
		mInitMetaData->mBoundingBox = mMyMeshData.mBoundingBox;

		//--------------------------------------------------//
		// ������Ʈ �ĺ� ����
		//--------------------------------------------------//

		// �̸�
		strcpy(mInitMetaData->mMainName, mMyMeshData.mMainName);

		// ������Ʈ�̸�
		mInitMetaData->mObjID = mMyMeshData.mObjID;
		strcpy(mInitMetaData->mObjName , mMyMeshData.mObjName);
		strcpy(mInitMetaData->mObjClass, mMyMeshData.mObjClass);

		// ���� Ŭ���� �̸�
		mInitMetaData->mParentID = mMyMeshData.mParentID;
		strcpy(mInitMetaData->mParentName, mMyMeshData.mParentName);

		// ����� ����
		mInitMetaData = nullptr;
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
			cout << "[Succes] [" << _Route << "] ������ ����.\n";
		else if ((_ReadType == "rb") || (_ReadType == "rt"))
			cout << "[Succes] [" << _Route << "] ������ ����.\n";
		return true;
	}

	// �ؽ�ó �ε�
	//void LoadTex(const wchar_t* _TexName, TEXTURE_TYPE e_InitTex)
	//{
	//	// �ؽ�ó �ε�
	//	switch (e_InitTex)
	//	{
	//	case e_DiffuseMap:
	//		HR(D3DX11CreateShaderResourceViewFromFile(mCoreStorage->md3dDevice, _TexName, 0, 0, &mInitMetaData->mDiffuseSRV, 0));
	//		break;
	//	case e_SpecularMap:
	//		HR(D3DX11CreateShaderResourceViewFromFile(mCoreStorage->md3dDevice, _TexName, 0, 0, &mInitMetaData->mSpecularSRV, 0));
	//		break;
	//	case e_NomalMap:
	//		HR(D3DX11CreateShaderResourceViewFromFile(mCoreStorage->md3dDevice, _TexName, 0, 0, &mInitMetaData->mNomalSRV, 0));
	//		break;
	//	}
	//}

	// �ؽ�ó �ε�
	void LoadTex(string _TexName, TEXTURE_TYPE e_InitTex)
	{
		// string --> wString���� ��ȯ
		wstring _WsTexName;
		StringToWchar_t(_TexName.c_str(), _WsTexName);

		// �ؽ�ó �ε�
		switch (e_InitTex)
		{
		case e_DiffuseMap:
			HR(D3DX11CreateShaderResourceViewFromFile(mCoreStorage->md3dDevice, _WsTexName.c_str(), 0, 0, &mInitMetaData->mDiffuseSRV, 0));
			break;
		case e_NomalMap:
			HR(D3DX11CreateShaderResourceViewFromFile(mCoreStorage->md3dDevice, _WsTexName.c_str(), 0, 0, &mInitMetaData->mNomalSRV, 0));
			break;
		case e_SpecularMap:
			HR(D3DX11CreateShaderResourceViewFromFile(mCoreStorage->md3dDevice, _WsTexName.c_str(), 0, 0, &mInitMetaData->mSpecularSRV, 0));
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

		_string.clear();
	}
};

