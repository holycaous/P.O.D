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
	vector<MyMeshData> mMyMeshData;

	//------------------------------------------------------------------//	
private:
	cCoreStorage* mCoreStorage = cCoreStorage::GetInstance();

	// ���� ������
	FILE* mFilePointer = nullptr;

	// ����
	char mReadBuf[BUF_SIZE];
	char mSaveBuf[BUF_SIZE];
	
	// �ε���
	int mIdx;
public:
<<<<<<< HEAD:POD_x11/POD_x11/cXMLParser.h
	cXMLParser()
	{
		ClearClass();
		InitClass();
	}

=======
>>>>>>> 051cc9b971ed9121d0d4d86d04233c72f1433687:POD_TestClient3/POD_x11/cXMLParser.h
	~cXMLParser()
	{
		ClearClass();
	}

<<<<<<< HEAD:POD_x11/POD_x11/cXMLParser.h
	void InitClass()
	{
		ClearPointer();
		mMyMeshData.InitClass();
	}

=======
>>>>>>> 051cc9b971ed9121d0d4d86d04233c72f1433687:POD_TestClient3/POD_x11/cXMLParser.h
	// ���� �ʱ�ȭ
	void ClearClass()
	{
		mNewModleLoc.clear();
<<<<<<< HEAD:POD_x11/POD_x11/cXMLParser.h
	}

	void ClearPointer()
	{
		mMyMeshData.ClearClass();

		memset(&mReadBuf, '\0', sizeof(mReadBuf));
		memset(&mSaveBuf, '\0', sizeof(mSaveBuf));

		mFilePointer  = nullptr;
=======
		mMyMeshData.clear();
>>>>>>> 051cc9b971ed9121d0d4d86d04233c72f1433687:POD_TestClient3/POD_x11/cXMLParser.h
	}

	// �Ľ�
	void LoadXMLRoute(string _ModelRoute)
	{
		// �ʱ�ȭ
		ClearPointer();

		// XML ��Ʈ ������ �б�
		ReadLocMyFormat_Model(_ModelRoute);
	}

	// �Ľ�
<<<<<<< HEAD:POD_x11/POD_x11/cXMLParser.h
	void LoadXMLModel(string& _ModelData, InitMetaData& _InitMetaData)
=======
	void LoadXMLModel(string _ModelData, int idx)
>>>>>>> 051cc9b971ed9121d0d4d86d04233c72f1433687:POD_TestClient3/POD_x11/cXMLParser.h
	{
		// ���� ���õ� ��ȣ
		mIdx = idx;

		// XML �� ������ �б�
		ReadDataMyFormat_Model(_ModelData); 

		// ���� ������ ������ �ֱ�
<<<<<<< HEAD:POD_x11/POD_x11/cXMLParser.h
		SaveMyFormat(_InitMetaData);
=======
		SaveMyFormat();

		// ������� ����
		mInitMetaData = nullptr;
	}

	// �� ���� ���
	void SetModelBuf(InitMetaData* _InitMetaData)
	{
		mInitMetaData = _InitMetaData;
>>>>>>> 051cc9b971ed9121d0d4d86d04233c72f1433687:POD_TestClient3/POD_x11/cXMLParser.h
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
		mMyMeshData[mIdx].vertices.resize(len);

		for (int i = 0; i < len; ++i)
			fread(&mMyMeshData[mIdx].vertices[i], sizeof(Vertex), 1, mFilePointer);

		// �ε��� ����Ʈ
		fread(&len, sizeof(int), 1, mFilePointer);
		mMyMeshData[mIdx].indices.resize(len);

		for (int i = 0; i < len; ++i)
			fread(&mMyMeshData[mIdx].indices[i], sizeof(XMFLOAT3), 1, mFilePointer);

		// �ִϵ�����

		// ��ġ
		fread(&len, sizeof(int), 1, mFilePointer);
		mMyMeshData[mIdx].aniData.Position.resize(len);

		for (int i = 0; i < len; ++i)
			fread(&mMyMeshData[mIdx].aniData.Position[i], sizeof(KeyVtx), 1, mFilePointer);

		// ȸ��
		fread(&len, sizeof(int), 1, mFilePointer);
		mMyMeshData[mIdx].aniData.Quaternion.resize(len);

		for (int i = 0; i < len; ++i)
			fread(&mMyMeshData[mIdx].aniData.Quaternion[i], sizeof(KeyVtx), 1, mFilePointer);

		// ������
		fread(&len, sizeof(int), 1, mFilePointer);
		mMyMeshData[mIdx].aniData.Scale.resize(len);

		for (int i = 0; i < len; ++i)
			fread(&mMyMeshData[mIdx].aniData.Scale[i], sizeof(KeyVtx), 1, mFilePointer);


		// ����ġ ������

		// ��ü ������
		fread(&len, sizeof(int), 1, mFilePointer);
		mMyMeshData[mIdx].weightVtx.resize(len);

		for (int i = 0; i < len; ++i)
		{
			// Ÿ�� �ε���
			fread(&mMyMeshData[mIdx].weightVtx[i].TagetIdx, sizeof(int), 1, mFilePointer);

			// �� ������ �� ũ��
			int Bonelen = 0;
			fread(&Bonelen, sizeof(int), 1, mFilePointer);
			mMyMeshData[mIdx].weightVtx[i].Bone.resize(Bonelen);

			// �� ������
			for (int idx = 0; idx < Bonelen; ++idx)
			{
				// ID
				fread(&mMyMeshData[mIdx].weightVtx[i].Bone[idx].ID, sizeof(int), 1, mFilePointer);

				// ����ġ
				fread(&mMyMeshData[mIdx].weightVtx[i].Bone[idx].Weight, sizeof(float), 1, mFilePointer);

				// ����(�̸�)
				int Bonebuflen = 0;
				fread(&Bonebuflen, sizeof(int), 1, mFilePointer);
				fread(&mMyMeshData[mIdx].weightVtx[i].Bone[idx].Name, sizeof(char), Bonebuflen, mFilePointer);
			}
		}

		// ������
<<<<<<< HEAD:POD_x11/POD_x11/cXMLParser.h
		fread(&mMyMeshData.vertexOffset, sizeof(UINT), 1, mFilePointer);
		fread(&mMyMeshData.indexOffset, sizeof(UINT), 1, mFilePointer);
=======
		fread(&mMyMeshData[mIdx].vertexOffset, sizeof(int), 1, mFilePointer);
		fread(&mMyMeshData[mIdx].indexOffset, sizeof(UINT), 1, mFilePointer);
>>>>>>> 051cc9b971ed9121d0d4d86d04233c72f1433687:POD_TestClient3/POD_x11/cXMLParser.h

		// ī��Ʈ
		fread(&mMyMeshData[mIdx].indexCount, sizeof(UINT), 1, mFilePointer);

		// ���� ����

		// ������
		fread(&mMyMeshData[mIdx].mMyMat.mOpacity, sizeof(float), 1, mFilePointer);

		// ��ǻ��
		fread(&len, sizeof(int), 1, mFilePointer);
		fread(&mMyMeshData[mIdx].mMyMat.mDiffuseSRV, sizeof(char), len, mFilePointer);

		// ���
		fread(&len, sizeof(int), 1, mFilePointer);
		fread(&mMyMeshData[mIdx].mMyMat.mNomalSRV, sizeof(char), len, mFilePointer);

		// ����
		fread(&len, sizeof(int), 1, mFilePointer);
		fread(&mMyMeshData[mIdx].mMyMat.mSpecularSRV, sizeof(char), len, mFilePointer);


		// TM ���
		fread(&mMyMeshData[mIdx].mTMLocalMtx, sizeof(XMFLOAT4X4), 1, mFilePointer);
		fread(&mMyMeshData[mIdx].mTMWorldMtx, sizeof(XMFLOAT4X4), 1, mFilePointer);

		// �ٿ�� �ڽ�
		fread(&mMyMeshData[mIdx].mBoundingBox, sizeof(BoundBox), 1, mFilePointer);

		//--------------------------------------------------//
		// ������Ʈ �ĺ� ����
		//--------------------------------------------------//

		// �̸�
		fread(&len, sizeof(int), 1, mFilePointer);
		fread(&mMyMeshData[mIdx].mMainName, sizeof(char), len, mFilePointer);

		// ������Ʈ�̸�
		fread(&mMyMeshData[mIdx].mObjID, sizeof(int), 1, mFilePointer);

		fread(&len, sizeof(int), 1, mFilePointer);
		fread(&mMyMeshData[mIdx].mObjName, sizeof(char), len, mFilePointer);

		fread(&len, sizeof(int), 1, mFilePointer);
		fread(&mMyMeshData[mIdx].mObjClass, sizeof(char), len, mFilePointer);

		// ���� Ŭ���� �̸�
		fread(&mMyMeshData[mIdx].mParentID, sizeof(int), 1, mFilePointer);

		fread(&len, sizeof(int), 1, mFilePointer);
		fread(&mMyMeshData[mIdx].mParentName, sizeof(char), len, mFilePointer);

		// ���� ����
		FileClose();
		printf("[Succes] ���� ����.\n");
	}

	// �� �������� �д´�.(������) - ��
	void ReadDataMyFormat_Bone(string _BoneRoute, MyBoneData* _MyBoneData)
	{
		// ���� ���� ����
		FileOpen(_BoneRoute.c_str(), "rb");

		// Bone �̸�
		int len = 0;
		fread(&len, sizeof(int), 1, mFilePointer);
		fread(&_MyBoneData->mMainName, sizeof(char), len, mFilePointer);

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
		FileClose();
		printf("[Succes] ���� ����.\n");
	}

	// ���� �����ϱ�
	void SaveMyFormat(InitMetaData& _InitMetaData)
	{
		// ���ؽ�
<<<<<<< HEAD:POD_x11/POD_x11/cXMLParser.h
		_InitMetaData.Vertices = mMyMeshData.vertices;
=======
		mInitMetaData->Vertices = mMyMeshData[mIdx].vertices;
>>>>>>> 051cc9b971ed9121d0d4d86d04233c72f1433687:POD_TestClient3/POD_x11/cXMLParser.h

		// �ε���
		for (unsigned int i = 0; i < mMyMeshData[mIdx].indices.size(); ++i)
		{
			for (int vIdx = 0; vIdx < 3; ++vIdx)
			{
				switch (vIdx)
				{
				case 0:
<<<<<<< HEAD:POD_x11/POD_x11/cXMLParser.h
					_InitMetaData.Indices.push_back((UINT)mMyMeshData.indices[i].x);
					break;
				case 1:
					_InitMetaData.Indices.push_back((UINT)mMyMeshData.indices[i].y);
					break;
				case 2:
					_InitMetaData.Indices.push_back((UINT)mMyMeshData.indices[i].z);
=======
					mInitMetaData->Indices.push_back((UINT)mMyMeshData[mIdx].indices[i].x);
					break;
				case 1:
					mInitMetaData->Indices.push_back((UINT)mMyMeshData[mIdx].indices[i].y);
					break;
				case 2:
					mInitMetaData->Indices.push_back((UINT)mMyMeshData[mIdx].indices[i].z);
>>>>>>> 051cc9b971ed9121d0d4d86d04233c72f1433687:POD_TestClient3/POD_x11/cXMLParser.h
					break;
				default:
					break;
				}
			}
		}

		// �ִ� ������
<<<<<<< HEAD:POD_x11/POD_x11/cXMLParser.h
		_InitMetaData.aniData = mMyMeshData.aniData;

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
=======
		mInitMetaData->aniData = mMyMeshData[mIdx].aniData;

		// ����ġ ������
		mInitMetaData->weightVtx = mMyMeshData[mIdx].weightVtx;

		// TM ��Ʈ����
		mInitMetaData->mLocTMMtx = mMyMeshData[mIdx].mTMLocalMtx;
		mInitMetaData->mWdTMMtx  = mMyMeshData[mIdx].mTMWorldMtx;

		// ������
		mInitMetaData->mVertexOffset = mMyMeshData[mIdx].vertexOffset;
		mInitMetaData->mIndexOffset  = mMyMeshData[mIdx].indexOffset;

		// ī��Ʈ
		mInitMetaData->mIndexCount = mMyMeshData[mIdx].indexCount;

		// ���ҽ� ��� (���� ����)
		LoadTex(mMyMeshData[mIdx].mMyMat.mDiffuseSRV , e_DiffuseMap);
		LoadTex(mMyMeshData[mIdx].mMyMat.mNomalSRV   , e_NomalMap);
		LoadTex(mMyMeshData[mIdx].mMyMat.mSpecularSRV, e_SpecularMap);


		// ���� (��ǻ�� ��)
		mInitMetaData->mOpacity = mMyMeshData[mIdx].mMyMat.mOpacity;

		// �ٿ�� �ڽ�
		mInitMetaData->mBoundingBox = mMyMeshData[mIdx].mBoundingBox;
>>>>>>> 051cc9b971ed9121d0d4d86d04233c72f1433687:POD_TestClient3/POD_x11/cXMLParser.h

		//--------------------------------------------------//
		// ������Ʈ �ĺ� ����
		//--------------------------------------------------//

		// �̸�
<<<<<<< HEAD:POD_x11/POD_x11/cXMLParser.h
		strcpy(_InitMetaData.mMainName, mMyMeshData.mMainName);

		// ������Ʈ�̸�
		_InitMetaData.mObjID = mMyMeshData.mObjID;
		strcpy(_InitMetaData.mObjName , mMyMeshData.mObjName);
		strcpy(_InitMetaData.mObjClass, mMyMeshData.mObjClass);

		// ���� Ŭ���� �̸�
		_InitMetaData.mParentID = mMyMeshData.mParentID;
		strcpy(_InitMetaData.mParentName, mMyMeshData.mParentName);

=======
		strcpy(mInitMetaData->mMainName, mMyMeshData[mIdx].mMainName);

		// ������Ʈ�̸�
		mInitMetaData->mObjID = mMyMeshData[mIdx].mObjID;
		strcpy(mInitMetaData->mObjName , mMyMeshData[mIdx].mObjName);
		strcpy(mInitMetaData->mObjClass, mMyMeshData[mIdx].mObjClass);

		// ���� Ŭ���� �̸�
		mInitMetaData->mParentID = mMyMeshData[mIdx].mParentID;
		strcpy(mInitMetaData->mParentName, mMyMeshData[mIdx].mParentName);
>>>>>>> 051cc9b971ed9121d0d4d86d04233c72f1433687:POD_TestClient3/POD_x11/cXMLParser.h
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
<<<<<<< HEAD:POD_x11/POD_x11/cXMLParser.h
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
=======
	void LoadTex(const wchar_t* _TexName, TEXTURE_TYPE e_InitTex)
	{
		// �ؽ�ó �ε�
		switch (e_InitTex)
		{
		case e_DiffuseMap:
			HR(D3DX11CreateShaderResourceViewFromFile(mCoreStorage->md3dDevice, _TexName, 0, 0, &mInitMetaData->mDiffuseSRV, 0));
			break;
		case e_SpecularMap:
			HR(D3DX11CreateShaderResourceViewFromFile(mCoreStorage->md3dDevice, _TexName, 0, 0, &mInitMetaData->mSpecularSRV, 0));
			break;
		case e_NomalMap:
			HR(D3DX11CreateShaderResourceViewFromFile(mCoreStorage->md3dDevice, _TexName, 0, 0, &mInitMetaData->mNomalSRV, 0));
			break;
		}
	}
>>>>>>> 051cc9b971ed9121d0d4d86d04233c72f1433687:POD_TestClient3/POD_x11/cXMLParser.h

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
	}

	// ��ȯ �Լ�
	void StringToWchar_t(string _string, wstring& _wstring)
	{
		// ��ȯ
		for (unsigned int i = 0; i < _string.length(); ++i)
			_wstring += wchar_t(_string[i]);
<<<<<<< HEAD:POD_x11/POD_x11/cXMLParser.h

		// ������
		_wstring += wchar_t('\0');

		_string.clear();
=======
>>>>>>> 051cc9b971ed9121d0d4d86d04233c72f1433687:POD_TestClient3/POD_x11/cXMLParser.h
	}
};
