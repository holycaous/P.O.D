#pragma once
#pragma warning(disable:4996)

// ���� �б�
#define stMtrl        "<Material>"
#define edMtrl        "</Material>" 

// �Ž� �б�
#define stMesh        "<Object"
#define edMesh        "</Object>"

// ���� TM
#define stLocalTM     "<LocalTM>"
#define edLocalTM     "</LocalTM>"

// ���� TM
#define stWorldTM     "<WorldTM>"
#define edWorldTM     "</WorldTM>"

// �ٿ�� �ڽ�
#define stBoundBox    "<BoundingBox>"
#define edBoundBox    "</BoundingBox>"

// �ִϸ��̼� �б�
#define stAni         "<Key>"
#define edAni         "</Key>"

// ������ �б�
#define stPos         "<Position>"
#define edPos         "</Position>"

// ȸ�� �б�
#define stRot         "<Quaternion>"
#define edRot         "</Quaternion>"

// ũ�� �б�
#define stScale       "<Scale>"
#define edScale       "</Scale>"

// �θ� �б�
#define stParent      "<Parent"
#define edParent      "</Parent>"

// ���ؽ� �б�
#define stVtx         "<Vertex>"
#define edVtx         "</Vertex>"

// ��� ���ؽ� 
#define stVtxN        "<VertexNormal>"
#define edVtxN        "</VertexNormal>"

// �ε��� �б�
#define stIdx         "<TriIndex"
#define edIdx         "</TriIndex>"

// �ؽ�ó ���ؽ�	    
#define stVtxT        "<TexVertex>"
#define edVtxT        "</TexVertex>"

// �ؽ�ó �ε���      
#define stIdxT        "<TexIndex"
#define edIdxT        "</TexIndex>"

// ���� ����ġ
#define stVtxWeight   "<VertexWeight"
#define edVtxWeight   "</VertexWeight>"


// enum ��
typedef enum
{
	// �׳� �б�
	e_Read,

	// ��������
	e_Mtrl,
	e_Mesh,
	e_LocalTM,
	e_WorldTM,
	e_BoundBox,
	e_Ani,
	e_Parent,
	e_Vtx,
	e_VtxN,
	e_Idx,
	e_VtxT,
	e_IdxT,
	e_VtxWeight,

}ReadingCurrentData;

class cXMLParser
{
private:
	//------------------------------------------------------------------//
	// �ϼ� �� ������
	//------------------------------------------------------------------//

	// ���� ������� ���� ��ġ����
	vector<CharBuf> mNewModleLoc;

	// ���� ������� ���� ����
	vector<MyMeshData> mMyMeshData;

	// ���θ������ ������ ����
	MyBoneData mMyBoneData;
	

	//------------------------------------------------------------------//
	// XML ������
	//------------------------------------------------------------------//

	// ��Ÿ ������
	vector<InitMetaData> mMeatDatas;

	// �ؽ�ó ���� (���� ����)
	map<int, TextuerInfo> mMtrl;

	// ���� ������
	FILE* mFilePointer = nullptr;

	// ����
	char mReadBuf[BUF_SIZE];
	char mSaveBuf[BUF_SIZE];

	// ���� ���
	char mParsingFileRoute[BUF_SIZE];             // �Ľ� ���� ���
	char mFileName[BUF_SIZE];		              // ���� �̸�
	char mExportFileFormat[BUF_SIZE];             // �ͽ���Ʈ ����
	char mExportFileLoc[BUF_SIZE];                // �ͽ���Ʈ ��ġ
	char mFinExportModelLoc  [BUF_SIZE]; // �Ϸ�� �� ��ġ�� ���� txt
	char mFinExportModelData[BUF_SIZE];  // �Ϸ�� �� ������ ���� txt
	char mFinExportBoneData[BUF_SIZE];   // �Ϸ�� ��(�ִ�) ������ ���� txt

	// �� ��ǥ �̸�
	char mName[BUF_SIZE];

	// �ִ��̸� �������
	char mAnyType[BUF_SIZE];

	// ���� �����Ͱ� �� �ൿ
	ReadingCurrentData mReadCurData = e_Read;

	// ���� ���� ����
	int mMtrlCount;

public:
	// ���� ����
	void ClearClass()
	{
		mMeatDatas.clear();
		mMtrl.clear();
		mNewModleLoc.clear();
		mMyBoneData.ClearClass();
		mMyMeshData.clear();
	}

	// XML �ʱ�ȭ
	void Init(char* _ModelRoute, char* _Name)
	{
		// ��ǥ �̸� ���
		strcpy(mName, _Name);

		// Bone ��ǥ �̸� ���
		strcpy(mMyBoneData.mMainName, mName);

		// ��� ����
		strcpy(mParsingFileRoute, _ModelRoute);

		// �ͽ���Ʈ ���� ���� ���
		strcpy(mExportFileFormat, ".pod");

		// �ͽ���Ʈ ���� ���� ��ġ
		strcpy(mExportFileLoc, "Export/");

		// ���� �̸� ���
		GetFileName();

		// �ͽ���Ʈ ���� �����̼�
		strcpy(mFinExportModelLoc, mExportFileLoc);
		strcat(mFinExportModelLoc, mFileName);
		strcat(mFinExportModelLoc, "Loc");
		strcat(mFinExportModelLoc, mExportFileFormat);

		// �ͽ���Ʈ ���� ��
		strcpy(mFinExportBoneData, mExportFileLoc);
		strcat(mFinExportBoneData, mFileName);
		strcat(mFinExportBoneData, "Bone");
		strcat(mFinExportBoneData, mExportFileFormat);

		// ���� ī��Ʈ �ʱ�ȭ
		mMtrlCount = -1;
	}

	// XML �ʱ�ȭ
	void InitSkinModel(char* _ModelRoute, char* _Name, char* _AniType)
	{
		// ��ǥ �̸� ���
		strcpy(mName, _Name);

		// Bone ��ǥ �̸� ���
		strcpy(mMyBoneData.mMainName, mName);

		// ��� ����
		strcpy(mParsingFileRoute, _ModelRoute);

		// �ͽ���Ʈ ���� ���� ���
		strcpy(mExportFileFormat, ".pod");

		// �ͽ���Ʈ ���� ���� ��ġ
		strcpy(mExportFileLoc, "Export/");

		// ���� �̸� ���
		GetFileName();

		// �ִ� �̸� �ű��
		strcpy(mAnyType, _AniType);

		// �ͽ���Ʈ ���� �����̼�
		strcpy(mFinExportModelLoc, mExportFileLoc);
		strcat(mFinExportModelLoc, mFileName);
		strcat(mFinExportModelLoc, mAnyType);
		strcat(mFinExportModelLoc, "Loc");
		strcat(mFinExportModelLoc, mExportFileFormat);

		// �ͽ���Ʈ ���� ��
		strcpy(mFinExportBoneData, mExportFileLoc);
		strcat(mFinExportBoneData, mFileName);
		strcat(mFinExportBoneData, mAnyType);
		strcat(mFinExportBoneData, "Bone");
		strcat(mFinExportBoneData, mExportFileFormat);

		// ���� ī��Ʈ �ʱ�ȭ
		mMtrlCount = -1;
	}

	// XML �ʱ�ȭ
	void Init(char* _ModelRoute, char* _Name, char* _AniName)
	{
		// ��ǥ �̸� ���
		strcpy(mName, _Name);

		// Bone ��ǥ �̸� ���
		strcpy(mMyBoneData.mMainName, mName);

		// Bone �ִ� �̸� ���
		strcpy(mMyBoneData.mAniName, _AniName);

		// ��� ����
		strcpy(mParsingFileRoute, _ModelRoute);

		// �ͽ���Ʈ ���� ���� ���
		strcpy(mExportFileFormat, ".pod");

		// �ͽ���Ʈ ���� ���� ��ġ
		strcpy(mExportFileLoc, "Export/");

		// ���� �̸� ���
		GetFileName();


		// �ͽ���Ʈ ���� �����̼�
		strcpy(mFinExportModelLoc, mExportFileLoc);
		strcat(mFinExportModelLoc, mFileName);
		strcat(mFinExportModelLoc, "Loc");
		strcat(mFinExportModelLoc, mExportFileFormat);

		// �ͽ���Ʈ ���� ��
		strcpy(mFinExportBoneData, mExportFileLoc);
		strcat(mFinExportBoneData, mFileName);
		strcat(mFinExportBoneData, "Bone");
		strcat(mFinExportBoneData, mMyBoneData.mAniName);
		strcat(mFinExportBoneData, mExportFileFormat);
	}

	// XML �ؽ�ó ���
	void SetTexture(char* _DiffuseMap = "NULL", float _Opacity = 100.0f, char* _NoamleMap = "NULL", char* _SpecularMap = "NULL")
	{
		// �ؽ�ó �ʱ�ȭ
		TextuerInfo tTex;
		strcpy(tTex.mDiffuseMap , _DiffuseMap);
		strcpy(tTex.mNoamleMap  , _NoamleMap);
		strcpy(tTex.mSpecularMap, _SpecularMap);

		// ����
		tTex.mOpacity = _Opacity;

		// �� ���
		mMtrl[++mMtrlCount] = tTex;
	}

	// �Ľ�
	void LoadXMLModel()
	{
		// XML ������ �а�, ����Ѵ�.
		LoadXmlData();

		// ������ ���� ��
		FinalData();

		//--------------------------------------------//
		// �Ž� ������
		//--------------------------------------------//

		// << ���� >>
		// �� �������� �Ž������� ������ �ۼ��Ѵ�.
		WriteLocMyFormat_Model();

		// << �Ž� ������ >>
		// �� �������� �Ž������͸� �ۼ��Ѵ�.
		for (unsigned int i = 0; i < mNewModleLoc.size(); ++i)
			WriteDataMyFormat_Model(i); 

		// �׽�Ʈ�ڵ� (�׽�Ʈ) 
		// �Ž������� ���� ���� �б�
		//mNewModleLoc.clear();
		//ReadLocMyFormat_Model(mFinExportModelLoc);
		//
		// �� �������� ������ Ȯ���Ѵ� (�׽�Ʈ) @@@@@@
		//MyMeshData test;
		//ReadDataMyFormat_Model(mNewModleLoc[0].Data, &test); // 0�� ������ Ȯ��~

		// ������ Ŭ����
		ClearClass();
	}

	// �Ľ�
	void LoadXMLSkinModel()
	{
		// XML ������ �а�, ����Ѵ�.
		LoadXmlData();

		// ������ ���� ��
		FinalData();

		//--------------------------------------------//
		// �Ž� ������
		//--------------------------------------------//

		// << ���� >>
		// �� �������� �Ž������� ������ �ۼ��Ѵ�.
		WriteLocMyFormat_SkinModel();

		// << �Ž� ������ >>
		// �� �������� �Ž������͸� �ۼ��Ѵ�.
		for (unsigned int i = 0; i < mNewModleLoc.size(); ++i)
			WriteDataMyFormat_SkinModel(i);

		// �׽�Ʈ�ڵ� (�׽�Ʈ) 
		// �Ž������� ���� ���� �б�
		//mNewModleLoc.clear();
		//ReadLocMyFormat_Model(mFinExportModelLoc);
		//
		// �� �������� ������ Ȯ���Ѵ� (�׽�Ʈ) @@@@@@
		//MyMeshData test;
		//ReadDataMyFormat_Model(mNewModleLoc[0].Data, &test); // 0�� ������ Ȯ��~

		// ������ Ŭ����
		ClearClass();
	}

	// �Ľ�
	void LoadXMLBone()
	{
		// XML ������ �а�, ����Ѵ�.
		LoadXmlData();

		// ������ ���� ��
		FinalData();

		//--------------------------------------------//
		// �� ������
		//--------------------------------------------//
		// Ʈ�� ���� ����
		mMyBoneData.SetTree();

		// �� �������� �� �����͸� �ۼ��Ѵ�.
		WriteDataMyFormat_Bone();

		// �� �������� �� �����͸� �д´�. (�׽�Ʈ) @@@@@
		//mMyBoneData.ClearClass();
		//ReadDataMyFormat_Bone();

		// ������ Ŭ����
		ClearClass();
	}

private:
	// ���� �̸� ���
	void GetFileName()
	{
		strcpy(mReadBuf, mParsingFileRoute);
		strcpy(mFileName, ReadString("Import", '.'));
	}

	// xml ������ �Ľ�
	void LoadXmlData()
	{
		// ������ ����
		// - ��� ���� ��, ReadDetaileData���� Ŭ���� ��
		string _ReadLineData;

		// ������ ����
		if (FileOpen(mParsingFileRoute, "rb"))
		{
			//���� ������ Ž���Ѵ�.
			while (!feof(mFilePointer))
			{
				// ���� ������ ó���Ѵ�.
				ReadDetaileData(_ReadLineData);
			}

			// ���ؽ� �ű��
			MoveVtx();

			// �� ���� ( �����ø��� & ź��Ʈ ���� ��� )
			Build_PNT();

			// ������ ���� üũ
			DataErrCheck();

			// �������� �ε��� ����Ʈ�� �����Ѵ�.
			CalSubModel();
		}
	}

	// ���� ���� üũ
	void DataErrCheck()
	{
		// ���� �ݱ�
		FileClose();
		printf("[Succes] ���� ����.\n");

		// ������ ���� �� ��ŭ
		// �ؽ�ó ��� Ȯ��
		int i = 0;
		for (auto itor = mMtrl.begin(); itor != mMtrl.end(); ++itor)
		{
			printf("<< %d ��° �� �ؽ�ó üũ >>\n", i++);
			ConfirmMtrl(itor->second.mDiffuseMap);
			ConfirmMtrl(itor->second.mSpecularMap);
			ConfirmMtrl(itor->second.mNoamleMap);
		}
	}

	// �ؽ�ó Ȯ��
	void ConfirmMtrl(char* _MapName)
	{
		// �ؽ�ó ��� Ȯ��
		if (!strstr(_MapName, "NULL"))
		{
			char mInPutTexFileLoc[BUF_SIZE]; // �ؽ�ó ��ġ
			char mOutPutTexFileLoc[BUF_SIZE]; // �ؽ�ó ��ġ
			strcpy(mInPutTexFileLoc, "Import/");
			strcat(mInPutTexFileLoc, _MapName);

			// ������츸 ������ ��.
			if (FileOpen(mInPutTexFileLoc, "rb"))
			{
				FileClose();
				printf("[Succes] ���� ����.\n");

				strcpy(mOutPutTexFileLoc, "Export/");
				strcat(mOutPutTexFileLoc, _MapName);
				strcpy(_MapName, mOutPutTexFileLoc);

				// �ƿ�ǲ���� ���� �̵�
				wstring _input, _output;
				const wchar_t* Input = CharToWChar(mInPutTexFileLoc, _input);
				const wchar_t* output = CharToWChar(mOutPutTexFileLoc, _output);
				int i = MoveFileEx(Input, output, MOVEFILE_COPY_ALLOWED | MOVEFILE_WRITE_THROUGH | MOVEFILE_REPLACE_EXISTING);
				_input.clear();
				_output.clear();
			}
			// �ȿ������� ���� ó��
			else
				strcpy(_MapName, "NULL");
		}
	}

	// ���� ����
	bool FileOpen(const char* _Route, const char* _ReadType)
	{
		//�ش� ����� ������ ����.
		fopen_s(&mFilePointer, _Route, _ReadType);

		// �ش� ��ο� ������ �ֳ�?
		if (mFilePointer == NULL)
		{
			printf("[Error] [%s] ���� ����. <-- �ش��ο� ������ �������� �ʽ��ϴ�.\n", _Route);
			return false;
		}
		if ((_ReadType == "wb") || (_ReadType == "wt"))
		{
			rewind(mFilePointer);
			printf("[Succes] [%s] ������ ����.\n", _Route);
		}
		else if ((_ReadType == "rb") || (_ReadType == "rt"))
		{
			rewind(mFilePointer);
			printf("[Succes] [%s] ������ ����.\n", _Route);
		}
		return true;
	}

	void FileClose()
	{
		fclose(mFilePointer);
		mFilePointer = nullptr;
	}

	// ���� ���� �б�
	void ReadLineData(string& _ReadLineData)
	{
		// ������ �д´�
		fgets(mReadBuf, BUF_SIZE, mFilePointer);

		// ���ϴ� ������ �ű��
		sscanf_s(mReadBuf, "%s", mSaveBuf, sizeof(mSaveBuf));

		// ��Ʈ�� �ڷ������� �ű��.
		// �� ��������, ���ؽ�, �ε���, tex ��
		_ReadLineData = mSaveBuf;
	}

	// ������ �ڼ��� �б� (�з�, ����)
	void ReadDetaileData(string& _ReadLineData)
	{
		// ���� ���� ������ ��ȣ
		int _CreateMatNum = -1;
		TextuerInfo tTex;

		// ���� �ʱ�ȭ�� �� ��ȣ
		int _ReadParentNum = -1;
		InitMetaData tMetaData;

		// ��ǥ�� ������ ��
		KeyVtx    tKeyVtx;
		XMFLOAT3  tVtx;
		RotKeyVtx tRotKeyVtx;

		// �ؽ�ó ��ǥ�� �����Ұ�
		XMFLOAT2 tTVtx;

		// �� ����ġ ����
		WeightVtx tWeightVtx;
		BoneData  tBoneData;

		// ������ �д´�.
		ReadLineData(_ReadLineData);

		//// ����
		//if (stMtrl == _ReadLineData)
		//{
		//	while (true)
		//	{
		//		// ���� ������ �д´�.
		//		ReadLineData(_ReadLineData);
		//
		//		// ���� �˸��� ���ڰ� ���Դ��� Ȯ��
		//		if (edMtrl == _ReadLineData) { mReadCurData = e_Read; break; }
		//
		//		// ���ϴ� ������ �ű�� ( ���� ������۴� �о����ִ� ���� )
		//		sscanf_s(mReadBuf, "\t\t<Slot Index=\"%d\">\n", &_CreateMatNum);
		//
		//		// �ش� ������ �����Ѵ� �����Ѵ�.
		//		// ��, ������ ������ ������ �����Ѵ�. (��� �� �̸��� NULL�̸�, ���� �ؽ�ó�� �߰� ���ߴٴ� ��)
		//		if (strstr(mMtrl[_CreateMatNum].mDiffuseMap, "NULL") && strstr(mMtrl[_CreateMatNum].mSpecularMap, "NULL") && strstr(mMtrl[_CreateMatNum].mNoamleMap, "NULL"))
		//			mMtrl[_CreateMatNum] = tTex;
		//	}
		//}
		//// �Ž�
		//else 
		if (stMesh == _ReadLineData)
		{
			// �ӽ� ���� �ʱ�ȭ
			memset(&tMetaData, 0, sizeof(tMetaData));

			// ���ϴ� ������ �ű�� ( ���� ������۴� �о����ִ� ���� )
			// ���� ������Ʈ ����
			sscanf_s(mReadBuf, "\t\t<Object ID=\"%d\"", &tMetaData.mObjID);

			// ��Ʈ�� ������ �б�
			strcpy(tMetaData.mObjName , ReadString("Name=" , '\"'));
			strcpy(tMetaData.mObjClass, ReadString("Class=", '\"'));

			// �������ʹ� �� �ű�� ��
			while (true)
			{
				// ���� �˸��� ���ڰ� ���Դ��� Ȯ��
				if (edMesh == _ReadLineData) { mReadCurData = e_Read; break; }
				else if (stParent == _ReadLineData)
				{
					// �θ� ������Ʈ ����
					sscanf_s(mReadBuf, "\t\t\t<Parent ID=\"%d\"", &tMetaData.mParentID);

					// �θ� ������Ʈ �̸� �ű��
					strcpy(tMetaData.mParentName, ReadString("Name=", '\"'));
				}
				else if (stLocalTM == _ReadLineData)
				{
					// ���� ������ �д´�.
					ReadLineData(_ReadLineData);
					sscanf_s(mReadBuf, "\t\t\t\t<Value>[%f,%f,%f,%f]</Value>", &tMetaData.mTMLocalMtx._11, &tMetaData.mTMLocalMtx._12, &tMetaData.mTMLocalMtx._13, &tMetaData.mTMLocalMtx._14);

					// ���� ������ �д´�.
					ReadLineData(_ReadLineData);
					sscanf_s(mReadBuf, "\t\t\t\t<Value>[%f,%f,%f,%f]</Value>", &tMetaData.mTMLocalMtx._21, &tMetaData.mTMLocalMtx._22, &tMetaData.mTMLocalMtx._23, &tMetaData.mTMLocalMtx._24);

					// ���� ������ �д´�.
					ReadLineData(_ReadLineData);
					sscanf_s(mReadBuf, "\t\t\t\t<Value>[%f,%f,%f,%f]</Value>", &tMetaData.mTMLocalMtx._31, &tMetaData.mTMLocalMtx._32, &tMetaData.mTMLocalMtx._33, &tMetaData.mTMLocalMtx._34);

					// ���� ������ �д´�.
					ReadLineData(_ReadLineData);
					sscanf_s(mReadBuf, "\t\t\t\t<Value>[%f,%f,%f,%f]</Value>", &tMetaData.mTMLocalMtx._41, &tMetaData.mTMLocalMtx._42, &tMetaData.mTMLocalMtx._43, &tMetaData.mTMLocalMtx._44);
				}
				else if (stWorldTM == _ReadLineData)
				{
					// ���� ������ �д´�.
					ReadLineData(_ReadLineData);
					sscanf_s(mReadBuf, "\t\t\t\t<Value>[%f,%f,%f,%f]</Value>", &tMetaData.mTMWorldMtx._11, &tMetaData.mTMWorldMtx._12, &tMetaData.mTMWorldMtx._13, &tMetaData.mTMWorldMtx._14);

					// ���� ������ �д´�.
					ReadLineData(_ReadLineData);
					sscanf_s(mReadBuf, "\t\t\t\t<Value>[%f,%f,%f,%f]</Value>", &tMetaData.mTMWorldMtx._21, &tMetaData.mTMWorldMtx._22, &tMetaData.mTMWorldMtx._23, &tMetaData.mTMWorldMtx._24);

					// ���� ������ �д´�.
					ReadLineData(_ReadLineData);
					sscanf_s(mReadBuf, "\t\t\t\t<Value>[%f,%f,%f,%f]</Value>", &tMetaData.mTMWorldMtx._31, &tMetaData.mTMWorldMtx._32, &tMetaData.mTMWorldMtx._33, &tMetaData.mTMWorldMtx._34);

					// ���� ������ �д´�.
					ReadLineData(_ReadLineData);
					sscanf_s(mReadBuf, "\t\t\t\t<Value>[%f,%f,%f,%f]</Value>", &tMetaData.mTMWorldMtx._41, &tMetaData.mTMWorldMtx._42, &tMetaData.mTMWorldMtx._43, &tMetaData.mTMWorldMtx._44);
				}
				else if (stBoundBox == _ReadLineData)
				{
					// ���� ������ �д´�.
					ReadLineData(_ReadLineData);
					sscanf_s(mReadBuf, "\t\t\t\t<Max>[%f,%f,%f]</Max>", &tMetaData.mBoundingBox.Max.x, &tMetaData.mBoundingBox.Max.y, &tMetaData.mBoundingBox.Max.z);

					// ���� ������ �д´�.
					ReadLineData(_ReadLineData);
					sscanf_s(mReadBuf, "\t\t\t\t<Min>[%f,%f,%f]</Min>", &tMetaData.mBoundingBox.Min.x, &tMetaData.mBoundingBox.Min.y, &tMetaData.mBoundingBox.Min.z);

					// ���� ������ �д´�.
					ReadLineData(_ReadLineData);
					sscanf_s(mReadBuf, "\t\t\t\t<Center>[%f,%f,%f]</Center>", &tMetaData.mBoundingBox.Center.x, &tMetaData.mBoundingBox.Center.y, &tMetaData.mBoundingBox.Center.z);
				}
				else if (stVtx == _ReadLineData)
				{
					while (true)
					{
						// ���� ������ �д´�.
						ReadLineData(_ReadLineData);

						// Ż�� ����
						if (edVtx == _ReadLineData) { break; }

						sscanf_s(mReadBuf, "\t\t\t\t<Value Index=\"%*d\">[%f,%f,%f]</Value>", &tVtx.x, &tVtx.y, &tVtx.z);
						tMetaData.Vertices.push_back(tVtx);
					}
				}
				else if (stVtxN == _ReadLineData)
				{
					while (true)
					{
						// ���� ������ �д´�.
						ReadLineData(_ReadLineData);

						// Ż�� ����
						if (edVtxN == _ReadLineData) { break; }

						sscanf_s(mReadBuf, "\t\t\t\t<Value Index=\"%*d\">[%f,%f,%f]</Value>", &tVtx.x, &tVtx.y, &tVtx.z);
						tMetaData.NVertices.push_back(tVtx);
					}
				}
				else if (stIdx == _ReadLineData)
				{
					while (true)
					{
						// ���� ������ �д´�.
						ReadLineData(_ReadLineData);

						// Ż�� ����
						if (edIdx == _ReadLineData) { break; }

						sscanf_s(mReadBuf, "\t\t\t\t<Value MaterialID=\"%d\">[%f,%f,%f]</Value>", &tKeyVtx.Key, &tKeyVtx.Vtx.x, &tKeyVtx.Vtx.y, &tKeyVtx.Vtx.z);

						// ���� ������ -1(�⺻)�̶�� 0���� �ʱ�ȭ ��Ų��.
						tKeyVtx.Key -= 1;

						if (tKeyVtx.Key <= -1)
							tKeyVtx.Key = 0;

						tMetaData.Indices.push_back(tKeyVtx);

						// ����� ���� �߰�
						tMetaData.AddMat(tKeyVtx.Key);
					}
				}
				else if (stVtxT == _ReadLineData)
				{
					while (true)
					{
						// ���� ������ �д´�.
						ReadLineData(_ReadLineData);

						// Ż�� ����
						if (edVtxT == _ReadLineData) { break; }

						sscanf_s(mReadBuf, "\t\t\t\t<Value Index=\"%*d\">[%f,%f]</Value>\n", &tTVtx.x, &tTVtx.y);
						tMetaData.TexVertices.push_back(tTVtx);
					}
				}
				else if (stIdxT == _ReadLineData)
				{
					while (true)
					{
						// ���� ������ �д´�.
						ReadLineData(_ReadLineData);

						// Ż�� ����
						if (edIdxT == _ReadLineData) { break; }

						sscanf_s(mReadBuf, "\t\t\t\t<Value MaterialID=\"%d\">[%f,%f,%f]</Value>", &tKeyVtx.Key, &tKeyVtx.Vtx.x, &tKeyVtx.Vtx.y, &tKeyVtx.Vtx.z);

						// ���� ������ -1(�⺻)�̶�� 0���� �ʱ�ȭ ��Ų��.
						if (tKeyVtx.Key == -1)
							tKeyVtx.Key = 0;

						tMetaData.TexIndices.push_back(tKeyVtx);
					}
				}
				else if (stVtxWeight == _ReadLineData)
				{
					while (true)
					{
						// ���� ������ �д´�.
						ReadLineData(_ReadLineData);

						// Ż�� ����
						if (edVtxWeight == _ReadLineData) { break; }

						sscanf_s(mReadBuf, "\t\t\t\t<Vertex Index=\"%d\">", &tWeightVtx.TagetIdx);

						// �ݺ��Ѵ�.
						while (true)
						{
							ReadLineData(_ReadLineData);

							if (edVtx == _ReadLineData) { break; }

							sscanf_s(mReadBuf, "\t\t\t\t\t<Value BoneID=\"%d", &tBoneData.ID);
							tBoneData.Weight = ReadFloat("\">", '<');
							strcpy(tBoneData.Name, ReadString("Name=", '\"'));

							// �� ������ ����
							tWeightVtx.Bone.push_back(tBoneData);
						}

						// ����
						tMetaData.mWeightVtx.push_back(tWeightVtx);
						tWeightVtx.Bone.clear();
					}
				}
				else if (stAni == _ReadLineData)
				{
					// �ִϸ��̼� ���ᰡ ���ö����� �ݺ�
					if (edAni == _ReadLineData) { break; }

					while (true)
					{
						// ��ġ
						if (stPos == _ReadLineData)
						{
							while (true)
							{
								if (edPos == _ReadLineData) { break; }

								// Ű �� �ʱ�ȭ ��, ����
								tKeyVtx.Key = -1;
								sscanf_s(mReadBuf, "\t\t\t\t\t<Value Frame=\"%df\">[%f,%f,%f]</Value>", &tKeyVtx.Key, &tKeyVtx.Vtx.x, &tKeyVtx.Vtx.y, &tKeyVtx.Vtx.z);

								// ������ Ű ���� ��ȿ�Ҷ�
								if (tKeyVtx.Key >= 0)
									tMetaData.mAniData.Position.push_back(tKeyVtx);

								// ���� ������ �д´�.
								ReadLineData(_ReadLineData);
							}
						}

						// ȸ��
						if (stRot == _ReadLineData)
						{
							while (true)
							{
								if (edRot == _ReadLineData) { break; }

								// Ű �� �ʱ�ȭ ��, ����
								tRotKeyVtx.Key = -1;
								sscanf_s(mReadBuf, "\t\t\t\t\t<Value Frame=\"%df\">[%f,%f,%f,%f]</Value>", &tRotKeyVtx.Key, &tRotKeyVtx.Vtx.x, &tRotKeyVtx.Vtx.y, &tRotKeyVtx.Vtx.z, &tRotKeyVtx.Vtx.w);

								// ������ Ű ���� ��ȿ�Ҷ�
								if (tRotKeyVtx.Key >= 0)
									tMetaData.mAniData.Quaternion.push_back(tRotKeyVtx);

								// ���� ������ �д´�.
								ReadLineData(_ReadLineData);
							}
						}

						// ũ��
						if (stScale == _ReadLineData)
						{
							while (true)
							{
								if (edScale == _ReadLineData) { break; }

								// Ű �� �ʱ�ȭ ��, ����
								tKeyVtx.Key = -1;
								sscanf_s(mReadBuf, "\t\t\t\t\t<Value Frame=\"%df\">[%f,%f,%f]</Value>", &tKeyVtx.Key, &tKeyVtx.Vtx.x, &tKeyVtx.Vtx.y, &tKeyVtx.Vtx.z);

								// ������ Ű ���� ��ȿ�Ҷ�
								if (tKeyVtx.Key >= 0)
									tMetaData.mAniData.Scale.push_back(tKeyVtx);

								// ���� ������ �д´�.
								ReadLineData(_ReadLineData);
							}
						}
						// �ִϸ��̼� ���ᰡ ���ö����� �ݺ�
						if (edAni == _ReadLineData) { break; }

						// ���� ������ �д´�.
						ReadLineData(_ReadLineData);
					}
				}

				// ���� ������ �д´�.
				ReadLineData(_ReadLineData);
			}

			// �����Ѵ�.
			strcpy(tMetaData.mMainName, mName);
			mMeatDatas.push_back(tMetaData);
		}

		// ������ ���� �ʱ�ȭ
		_ReadLineData.clear();
	}

	// �������� �ε�������Ʈ�� �й��Ѵ�.
	void CalSubModel()
	{
		// ���� ���� �ε��� ����Ʈ���� �ű��.

		// ��� �Ž��� ��ȸ�Ѵ�.
		for (unsigned int i = 0; i < mMeatDatas.size(); ++i)
		{
			// ������, �ε�����, �ؽ�ó �ε����� ������ ũ�⸦ ������ �ִ�.
			for (unsigned int idx = 0; idx < mMeatDatas[i].Indices.size(); ++idx)
			{
				int _key = mMeatDatas[i].Indices[idx].Key;
				mMtrl[_key].Key = _key;
				mMtrl[_key].mMatIdx.Idx.push_back(mMeatDatas[i].Indices[idx].Vtx);
				mMtrl[_key].mMatIdx.tIdx.push_back(mMeatDatas[i].TexIndices[idx].Vtx);
			}
		}
	}

	// �� �������� �ۼ��Ѵ�.(���)
	void WriteLocMyFormat_Model()
	{
		// ���� ������� ���� �����̼�
		// �ͽ���Ʈ ���� �����̼� ����
		mNewModleLoc.resize(mMyMeshData.size());
		for (unsigned int i = 0; i < mMyMeshData.size(); ++i)
		{
			char _ModelNumBuf[512];
			itoa10(i, _ModelNumBuf);
			strcpy(mNewModleLoc[i].Data, mExportFileLoc);
			strcat(mNewModleLoc[i].Data, mFileName);
			strcat(mNewModleLoc[i].Data, "Data");
			strcat(mNewModleLoc[i].Data, _ModelNumBuf);
			strcat(mNewModleLoc[i].Data, mExportFileFormat);
		}

		// ���� ���� ����
		FileOpen(mFinExportModelLoc, "wb");

		// ������� �� �����
		int _ModelNum = mNewModleLoc.size();
		fwrite(&_ModelNum, sizeof(int), 1, mFilePointer);

		// ������ ����� �Ž� ��� ������ �����ϱ�
		for (int i = 0; i < _ModelNum; ++i)
		{
			int len = strlen(mNewModleLoc[i].Data) + 1;
			fwrite(&len, sizeof(int), 1, mFilePointer);
			fwrite(&mNewModleLoc[i].Data, sizeof(char), len, mFilePointer);
		}

		// ���� ����
		FileClose();
		printf("[Succes] ���� ����.\n");
	}

	// �� �������� �ۼ��Ѵ�.(���)
	void WriteLocMyFormat_SkinModel()
	{
		// ���� ������� ���� �����̼�
		// �ͽ���Ʈ ���� �����̼� ����
		mNewModleLoc.resize(mMyMeshData.size());
		for (unsigned int i = 0; i < mMyMeshData.size(); ++i)
		{
			char _ModelNumBuf[512];
			itoa10(i, _ModelNumBuf);
			strcpy(mNewModleLoc[i].Data, mExportFileLoc);
			strcat(mNewModleLoc[i].Data, mFileName);
			strcat(mNewModleLoc[i].Data, mAnyType);
			strcat(mNewModleLoc[i].Data, "Data");
			strcat(mNewModleLoc[i].Data, _ModelNumBuf);
			strcat(mNewModleLoc[i].Data, mExportFileFormat);
		}

		// ���� ���� ����
		FileOpen(mFinExportModelLoc, "wb");

		// ������� �� �����
		int _ModelNum = mNewModleLoc.size();
		fwrite(&_ModelNum, sizeof(int), 1, mFilePointer);

		// ������ ����� �Ž� ��� ������ �����ϱ�
		for (int i = 0; i < _ModelNum; ++i)
		{
			int len = strlen(mNewModleLoc[i].Data) + 1;
			fwrite(&len, sizeof(int), 1, mFilePointer);
			fwrite(&mNewModleLoc[i].Data, sizeof(char), len, mFilePointer);
		}

		// ���� ����
		FileClose();
		printf("[Succes] ���� ����.\n");
	}

	// ���� �б� �׽�Ʈ (�� ������ ��� �ε�)
	void ReadLocMyFormat_Model(char* _FinModelLoc)
	{
		// �� ���Ͽ����� mFinExportModelLoc �� �κп� �ش��ϴ� ���ڿ���
		// �� ���� �κп� �־� �ش� ������ �־���� �Ѵ�.

		// ���� ���� ����
		FileOpen(_FinModelLoc, "rb");

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
	}

	// �� �������� �ۼ��Ѵ�.(������)
	void WriteDataMyFormat_Model(int _ModelNum)
	{
		// �ͽ���Ʈ ���� �����̼�
		char _ModelNumBuf[512];
		itoa10(_ModelNum, _ModelNumBuf);
		strcpy(mFinExportModelData, mExportFileLoc);
		strcat(mFinExportModelData, mFileName);
		strcat(mFinExportModelData, "Data");
		strcat(mFinExportModelData, _ModelNumBuf);
		strcat(mFinExportModelData, mExportFileFormat);

		// ���� ���� ����
		FileOpen(mFinExportModelData, "wb");

		// ���ؽ�
		int len = mMyMeshData[_ModelNum].vertices.size();
		fwrite(&len, sizeof(int), 1, mFilePointer);

		for (int i = 0; i < len; ++i)
			fwrite(&mMyMeshData[_ModelNum].vertices[i], sizeof(Vertex), 1, mFilePointer);
		
		// �ε��� ����Ʈ
		len = mMyMeshData[_ModelNum].indices.size();
		fwrite(&len, sizeof(int), 1, mFilePointer);

		for (int i = 0; i < len; ++i)
			fwrite(&mMyMeshData[_ModelNum].indices[i], sizeof(XMFLOAT3), 1, mFilePointer);

		// �ִϵ�����
	
		// ��ġ
		len = mMyMeshData[_ModelNum].aniData.Position.size();
		fwrite(&len, sizeof(int), 1, mFilePointer);

		for (int i = 0; i < len; ++i)
			fwrite(&mMyMeshData[_ModelNum].aniData.Position[i], sizeof(KeyVtx), 1, mFilePointer);

		// ȸ��
		len = mMyMeshData[_ModelNum].aniData.Quaternion.size();
		fwrite(&len, sizeof(int), 1, mFilePointer);

		for (int i = 0; i < len; ++i)
			fwrite(&mMyMeshData[_ModelNum].aniData.Quaternion[i], sizeof(RotKeyVtx), 1, mFilePointer);

		// ������
		len = mMyMeshData[_ModelNum].aniData.Scale.size();
		fwrite(&len, sizeof(int), 1, mFilePointer);

		for (int i = 0; i < len; ++i)
			fwrite(&mMyMeshData[_ModelNum].aniData.Scale[i], sizeof(KeyVtx), 1, mFilePointer);
		
		// ����ġ ������

		// ��ü ������
		len = mMyMeshData[_ModelNum].weightVtx.size();
		fwrite(&len, sizeof(int), 1, mFilePointer);

		for (int i = 0; i < len; ++i)
		{
			// Ÿ�� �ε���
			fwrite(&mMyMeshData[_ModelNum].weightVtx[i].TagetIdx, sizeof(int), 1, mFilePointer);

			// �� ������ �� ũ��
			int Bonelen = mMyMeshData[_ModelNum].weightVtx[i].Bone.size();
			fwrite(&Bonelen, sizeof(int), 1, mFilePointer);

			// �� ������
			for (int idx = 0; idx < Bonelen; ++idx)
			{
				// ID
				fwrite(&mMyMeshData[_ModelNum].weightVtx[i].Bone[idx].ID, sizeof(int), 1, mFilePointer);

				// ����ġ
				fwrite(&mMyMeshData[_ModelNum].weightVtx[i].Bone[idx].Weight, sizeof(float), 1, mFilePointer);

				// ����(�̸�)
				int Bonebuflen = strlen(mMyMeshData[_ModelNum].weightVtx[i].Bone[idx].Name) + 1;
				fwrite(&Bonebuflen, sizeof(int), 1, mFilePointer);
				fwrite(&mMyMeshData[_ModelNum].weightVtx[i].Bone[idx].Name, sizeof(char), Bonebuflen, mFilePointer);
			}
		}

		// ������
		fwrite(&mMyMeshData[_ModelNum].vertexOffset, sizeof(UINT), 1, mFilePointer);
		fwrite(&mMyMeshData[_ModelNum].indexOffset, sizeof(UINT), 1, mFilePointer);

		// ī��Ʈ
		fwrite(&mMyMeshData[_ModelNum].indexCount, sizeof(UINT), 1, mFilePointer);

		// ���� ����

		// ����
		fwrite(&mMyMeshData[_ModelNum].mMyMat.mOpacity, sizeof(float), 1, mFilePointer);

		// ��ǻ��
		len = strlen(mMyMeshData[_ModelNum].mMyMat.mDiffuseMap) + 1;
		fwrite(&len, sizeof(int), 1, mFilePointer);
		fwrite(&mMyMeshData[_ModelNum].mMyMat.mDiffuseMap, sizeof(char), len, mFilePointer);

		// ���
		len = strlen(mMyMeshData[_ModelNum].mMyMat.mNoamleMap) + 1;
		fwrite(&len, sizeof(int), 1, mFilePointer);
		fwrite(&mMyMeshData[_ModelNum].mMyMat.mNoamleMap, sizeof(char), len, mFilePointer);

		// ����
		len = strlen(mMyMeshData[_ModelNum].mMyMat.mSpecularMap) + 1;
		fwrite(&len, sizeof(int), 1, mFilePointer);
		fwrite(&mMyMeshData[_ModelNum].mMyMat.mSpecularMap, sizeof(char), len, mFilePointer);

		// TM ���
		fwrite(&mMyMeshData[_ModelNum].mTMLocalMtx, sizeof(XMFLOAT4X4), 1, mFilePointer);
		fwrite(&mMyMeshData[_ModelNum].mTMWorldMtx, sizeof(XMFLOAT4X4), 1, mFilePointer);

		// �ٿ�� �ڽ�
		fwrite(&mMyMeshData[_ModelNum].mBoundingBox, sizeof(BoundBox), 1, mFilePointer);

		//--------------------------------------------------//
		// ������Ʈ �ĺ� ����
		//--------------------------------------------------//

		// �̸�
		len = strlen(mMyMeshData[_ModelNum].mMainName) + 1;
		fwrite(&len, sizeof(int), 1, mFilePointer);
		fwrite(&mMyMeshData[_ModelNum].mMainName, sizeof(char), len, mFilePointer);

		// ������Ʈ�̸�
		fwrite(&mMyMeshData[_ModelNum].mObjID, sizeof(int), 1, mFilePointer);
		
		len = strlen(mMyMeshData[_ModelNum].mObjName) + 1;
		fwrite(&len, sizeof(int), 1, mFilePointer);
		fwrite(&mMyMeshData[_ModelNum].mObjName, sizeof(char), len, mFilePointer);

		len = strlen(mMyMeshData[_ModelNum].mObjClass) + 1;
		fwrite(&len, sizeof(int), 1, mFilePointer);
		fwrite(&mMyMeshData[_ModelNum].mObjClass, sizeof(char), len, mFilePointer);

		// ���� Ŭ���� �̸�
		fwrite(&mMyMeshData[_ModelNum].mParentID, sizeof(int), 1, mFilePointer);

		len = strlen(mMyMeshData[_ModelNum].mParentName) + 1;
		fwrite(&len, sizeof(int), 1, mFilePointer);
		fwrite(&mMyMeshData[_ModelNum].mParentName, sizeof(char), len, mFilePointer);

		// ���� ����
		FileClose();
		printf("[Succes] ���� ����.\n");
	}

	// �� �������� �ۼ��Ѵ�.(������)
	void WriteDataMyFormat_SkinModel(int _ModelNum)
	{
		// �ͽ���Ʈ ���� �����̼�
		char _ModelNumBuf[512];
		itoa10(_ModelNum, _ModelNumBuf);
		strcpy(mFinExportModelData, mExportFileLoc);
		strcat(mFinExportModelData, mFileName);
		strcat(mFinExportModelData, mAnyType);
		strcat(mFinExportModelData, "Data");
		strcat(mFinExportModelData, _ModelNumBuf);
		strcat(mFinExportModelData, mExportFileFormat);

		// ���� ���� ����
		FileOpen(mFinExportModelData, "wb");

		// ���ؽ�
		int len = mMyMeshData[_ModelNum].vertices.size();
		fwrite(&len, sizeof(int), 1, mFilePointer);

		for (int i = 0; i < len; ++i)
			fwrite(&mMyMeshData[_ModelNum].vertices[i], sizeof(Vertex), 1, mFilePointer);
		
		// �ε��� ����Ʈ
		len = mMyMeshData[_ModelNum].indices.size();
		fwrite(&len, sizeof(int), 1, mFilePointer);

		for (int i = 0; i < len; ++i)
			fwrite(&mMyMeshData[_ModelNum].indices[i], sizeof(XMFLOAT3), 1, mFilePointer);

		// �ִϵ�����
	
		// ��ġ
		len = mMyMeshData[_ModelNum].aniData.Position.size();
		fwrite(&len, sizeof(int), 1, mFilePointer);

		for (int i = 0; i < len; ++i)
			fwrite(&mMyMeshData[_ModelNum].aniData.Position[i], sizeof(KeyVtx), 1, mFilePointer);

		// ȸ��
		len = mMyMeshData[_ModelNum].aniData.Quaternion.size();
		fwrite(&len, sizeof(int), 1, mFilePointer);

		for (int i = 0; i < len; ++i)
			fwrite(&mMyMeshData[_ModelNum].aniData.Quaternion[i], sizeof(RotKeyVtx), 1, mFilePointer);

		// ������
		len = mMyMeshData[_ModelNum].aniData.Scale.size();
		fwrite(&len, sizeof(int), 1, mFilePointer);

		for (int i = 0; i < len; ++i)
			fwrite(&mMyMeshData[_ModelNum].aniData.Scale[i], sizeof(KeyVtx), 1, mFilePointer);
		
		// ����ġ ������

		// ��ü ������
		len = mMyMeshData[_ModelNum].weightVtx.size();
		fwrite(&len, sizeof(int), 1, mFilePointer);

		for (int i = 0; i < len; ++i)
		{
			// Ÿ�� �ε���
			fwrite(&mMyMeshData[_ModelNum].weightVtx[i].TagetIdx, sizeof(int), 1, mFilePointer);

			// �� ������ �� ũ��
			int Bonelen = mMyMeshData[_ModelNum].weightVtx[i].Bone.size();
			fwrite(&Bonelen, sizeof(int), 1, mFilePointer);

			// �� ������
			for (int idx = 0; idx < Bonelen; ++idx)
			{
				// ID
				fwrite(&mMyMeshData[_ModelNum].weightVtx[i].Bone[idx].ID, sizeof(int), 1, mFilePointer);

				// ����ġ
				fwrite(&mMyMeshData[_ModelNum].weightVtx[i].Bone[idx].Weight, sizeof(float), 1, mFilePointer);

				// ����(�̸�)
				int Bonebuflen = strlen(mMyMeshData[_ModelNum].weightVtx[i].Bone[idx].Name) + 1;
				fwrite(&Bonebuflen, sizeof(int), 1, mFilePointer);
				fwrite(&mMyMeshData[_ModelNum].weightVtx[i].Bone[idx].Name, sizeof(char), Bonebuflen, mFilePointer);
			}
		}

		// ������
		fwrite(&mMyMeshData[_ModelNum].vertexOffset, sizeof(UINT), 1, mFilePointer);
		fwrite(&mMyMeshData[_ModelNum].indexOffset, sizeof(UINT), 1, mFilePointer);

		// ī��Ʈ
		fwrite(&mMyMeshData[_ModelNum].indexCount, sizeof(UINT), 1, mFilePointer);

		// ���� ����

		// ����
		fwrite(&mMyMeshData[_ModelNum].mMyMat.mOpacity, sizeof(float), 1, mFilePointer);

		// ��ǻ��
		len = strlen(mMyMeshData[_ModelNum].mMyMat.mDiffuseMap) + 1;
		fwrite(&len, sizeof(int), 1, mFilePointer);
		fwrite(&mMyMeshData[_ModelNum].mMyMat.mDiffuseMap, sizeof(char), len, mFilePointer);

		// ���
		len = strlen(mMyMeshData[_ModelNum].mMyMat.mNoamleMap) + 1;
		fwrite(&len, sizeof(int), 1, mFilePointer);
		fwrite(&mMyMeshData[_ModelNum].mMyMat.mNoamleMap, sizeof(char), len, mFilePointer);

		// ����
		len = strlen(mMyMeshData[_ModelNum].mMyMat.mSpecularMap) + 1;
		fwrite(&len, sizeof(int), 1, mFilePointer);
		fwrite(&mMyMeshData[_ModelNum].mMyMat.mSpecularMap, sizeof(char), len, mFilePointer);

		// TM ���
		fwrite(&mMyMeshData[_ModelNum].mTMLocalMtx, sizeof(XMFLOAT4X4), 1, mFilePointer);
		fwrite(&mMyMeshData[_ModelNum].mTMWorldMtx, sizeof(XMFLOAT4X4), 1, mFilePointer);

		// �ٿ�� �ڽ�
		fwrite(&mMyMeshData[_ModelNum].mBoundingBox, sizeof(BoundBox), 1, mFilePointer);

		//--------------------------------------------------//
		// ������Ʈ �ĺ� ����
		//--------------------------------------------------//

		// �̸�
		len = strlen(mMyMeshData[_ModelNum].mMainName) + 1;
		fwrite(&len, sizeof(int), 1, mFilePointer);
		fwrite(&mMyMeshData[_ModelNum].mMainName, sizeof(char), len, mFilePointer);

		// ������Ʈ�̸�
		fwrite(&mMyMeshData[_ModelNum].mObjID, sizeof(int), 1, mFilePointer);
		
		len = strlen(mMyMeshData[_ModelNum].mObjName) + 1;
		fwrite(&len, sizeof(int), 1, mFilePointer);
		fwrite(&mMyMeshData[_ModelNum].mObjName, sizeof(char), len, mFilePointer);

		len = strlen(mMyMeshData[_ModelNum].mObjClass) + 1;
		fwrite(&len, sizeof(int), 1, mFilePointer);
		fwrite(&mMyMeshData[_ModelNum].mObjClass, sizeof(char), len, mFilePointer);

		// ���� Ŭ���� �̸�
		fwrite(&mMyMeshData[_ModelNum].mParentID, sizeof(int), 1, mFilePointer);

		len = strlen(mMyMeshData[_ModelNum].mParentName) + 1;
		fwrite(&len, sizeof(int), 1, mFilePointer);
		fwrite(&mMyMeshData[_ModelNum].mParentName, sizeof(char), len, mFilePointer);

		// ���� ����
		FileClose();
		printf("[Succes] ���� ����.\n");
	}

	// �� �������� �д´�.(������) // <--- �׽�Ʈ
	void ReadDataMyFormat_Model(char* _FinExportModelData, MyMeshData* _MyMeshData)
	{
		// ���� ���� ����
		FileOpen(_FinExportModelData, "rb");
		int len = 0;

		//------------------------------------------------------------------------------//
		// �� ������
		//------------------------------------------------------------------------------//

		// ���ؽ�
		fread(&len, sizeof(int), 1, mFilePointer);
		_MyMeshData->vertices.resize(len);

		for (int i = 0; i < len; ++i)
			fread(&_MyMeshData->vertices[i], sizeof(Vertex), 1, mFilePointer);

		// �ε��� ����Ʈ
		fread(&len, sizeof(int), 1, mFilePointer);
		_MyMeshData->indices.resize(len);

		for (int i = 0; i < len; ++i)
			fread(&_MyMeshData->indices[i], sizeof(XMFLOAT3), 1, mFilePointer);

		// �ִϵ�����

		// ��ġ
		fread(&len, sizeof(int), 1, mFilePointer);
		_MyMeshData->aniData.Position.resize(len);

		for (int i = 0; i < len; ++i)
			fread(&_MyMeshData->aniData.Position[i], sizeof(KeyVtx), 1, mFilePointer);

		// ȸ��
		fread(&len, sizeof(int), 1, mFilePointer);
		_MyMeshData->aniData.Quaternion.resize(len);

		for (int i = 0; i < len; ++i)
			fread(&_MyMeshData->aniData.Quaternion[i], sizeof(RotKeyVtx), 1, mFilePointer);

		// ������
		fread(&len, sizeof(int), 1, mFilePointer);
		_MyMeshData->aniData.Scale.resize(len);

		for (int i = 0; i < len; ++i)
			fread(&_MyMeshData->aniData.Scale[i], sizeof(KeyVtx), 1, mFilePointer);


		// ����ġ ������

		// ��ü ������
		fread(&len, sizeof(int), 1, mFilePointer);
		_MyMeshData->weightVtx.resize(len);

		for (int i = 0; i < len; ++i)
		{
			// Ÿ�� �ε���
			fread(&_MyMeshData->weightVtx[i].TagetIdx, sizeof(int), 1, mFilePointer);

			// �� ������ �� ũ��
			int Bonelen;
			fread(&Bonelen, sizeof(int), 1, mFilePointer);
			_MyMeshData->weightVtx[i].Bone.resize(Bonelen);

			// �� ������
			for (int widx = 0; widx < Bonelen; ++widx)
			{
				// ID
				fread(&_MyMeshData->weightVtx[i].Bone[widx].ID, sizeof(int), 1, mFilePointer);

				// ����ġ
				fread(&_MyMeshData->weightVtx[i].Bone[widx].Weight, sizeof(float), 1, mFilePointer);

				// ����(�̸�)
				int Bonebuflen = 0;
				fread(&Bonebuflen, sizeof(int), 1, mFilePointer);
				fread(&_MyMeshData->weightVtx[i].Bone[widx].Name, sizeof(char), Bonebuflen, mFilePointer);
			}
		}

		// ������
		fread(&_MyMeshData->vertexOffset, sizeof(UINT), 1, mFilePointer);
		fread(&_MyMeshData->indexOffset, sizeof(UINT), 1, mFilePointer);

		// ī��Ʈ
		fread(&_MyMeshData->indexCount, sizeof(UINT), 1, mFilePointer);

		// ���� ����

		// ����
		fread(&_MyMeshData->mMyMat.mOpacity, sizeof(float), 1, mFilePointer);

		// ��ǻ��
		fread(&len, sizeof(int), 1, mFilePointer);
		fread(&_MyMeshData->mMyMat.mDiffuseMap, sizeof(char), len, mFilePointer);

		// ���
		fread(&len, sizeof(int), 1, mFilePointer);
		fread(&_MyMeshData->mMyMat.mNoamleMap, sizeof(char), len, mFilePointer);

		// ����
		fread(&len, sizeof(int), 1, mFilePointer);
		fread(&_MyMeshData->mMyMat.mSpecularMap, sizeof(char), len, mFilePointer);


		// TM ���
		fread(&_MyMeshData->mTMLocalMtx, sizeof(XMFLOAT4X4), 1, mFilePointer);
		fread(&_MyMeshData->mTMWorldMtx, sizeof(XMFLOAT4X4), 1, mFilePointer);

		// �ٿ�� �ڽ�
		fread(&_MyMeshData->mBoundingBox, sizeof(BoundBox), 1, mFilePointer);

		//--------------------------------------------------//
		// ������Ʈ �ĺ� ����
		//--------------------------------------------------//

		// �̸�
		fread(&len, sizeof(int), 1, mFilePointer);
		fread(&_MyMeshData->mMainName, sizeof(char), len, mFilePointer);

		// ������Ʈ�̸�
		fread(&_MyMeshData->mObjID, sizeof(int), 1, mFilePointer);

		fread(&len, sizeof(int), 1, mFilePointer);
		fread(&_MyMeshData->mObjName, sizeof(char), len, mFilePointer);

		fread(&len, sizeof(int), 1, mFilePointer);
		fread(&_MyMeshData->mObjClass, sizeof(char), len, mFilePointer);

		// ���� Ŭ���� �̸�
		fread(&_MyMeshData->mParentID, sizeof(int), 1, mFilePointer);

		fread(&len, sizeof(int), 1, mFilePointer);
		fread(&_MyMeshData->mParentName, sizeof(char), len, mFilePointer);

		// ���� ����
		FileClose();
		printf("[Succes] ���� ����.\n");
	}

	// �� �������� �ۼ��Ѵ�.(������) - ��
	void WriteDataMyFormat_Bone()
	{
		// ���� ���� ����
		FileOpen(mFinExportBoneData, "wb");

		// Bone �̸�
		int len = strlen(mMyBoneData.mMainName) + 1;
		fwrite(&len, sizeof(int), 1, mFilePointer);
		fwrite(&mMyBoneData.mMainName, sizeof(char), len, mFilePointer);

		// Bone �ִ� �̸�
		len = strlen(mMyBoneData.mAniName) + 1;
		fwrite(&len, sizeof(int), 1, mFilePointer);
		fwrite(&mMyBoneData.mAniName, sizeof(char), len, mFilePointer);

		// �� ������ ������
		len = mMyBoneData.mBoneData.size();
		fwrite(&len, sizeof(int), 1, mFilePointer);

		// �� ������ ���� ��ŭ
		for (unsigned int i = 0; i < mMyBoneData.mBoneData.size(); ++i)
		{
			// TM ���
			fwrite(&mMyBoneData.mBoneData[i].mTMLocalMtx, sizeof(XMFLOAT4X4), 1, mFilePointer);
			fwrite(&mMyBoneData.mBoneData[i].mTMWorldMtx, sizeof(XMFLOAT4X4), 1, mFilePointer);

			// �ִϵ�����

			// ��ġ
			len = mMyBoneData.mBoneData[i].aniData.Position.size();
			fwrite(&len, sizeof(int), 1, mFilePointer);
			for (int idx = 0; idx < len; ++idx)
				fwrite(&mMyBoneData.mBoneData[i].aniData.Position[idx], sizeof(KeyVtx), 1, mFilePointer);

			// ȸ��
			len = mMyBoneData.mBoneData[i].aniData.Quaternion.size();
			fwrite(&len, sizeof(int), 1, mFilePointer);
			for (int idx = 0; idx < len; ++idx)
				fwrite(&mMyBoneData.mBoneData[i].aniData.Quaternion[idx], sizeof(RotKeyVtx), 1, mFilePointer);

			// ������
			len = mMyBoneData.mBoneData[i].aniData.Scale.size();
			fwrite(&len, sizeof(int), 1, mFilePointer);
			for (int idx = 0; idx < len; ++idx)
				fwrite(&mMyBoneData.mBoneData[i].aniData.Scale[idx], sizeof(KeyVtx), 1, mFilePointer);


			// �ٿ�� �ڽ�
			fwrite(&mMyBoneData.mBoneData[i].mBoundingBox, sizeof(BoundBox), 1, mFilePointer);

			//--------------------------------------------------//
			// ������Ʈ �ĺ� ����
			//--------------------------------------------------//

			// ������Ʈ�̸�
			fwrite(&mMyBoneData.mBoneData[i].mObjID, sizeof(int), 1, mFilePointer);

			len = strlen(mMyBoneData.mBoneData[i].mObjName) + 1;
			fwrite(&len, sizeof(int), 1, mFilePointer);
			fwrite(&mMyBoneData.mBoneData[i].mObjName, sizeof(char), len, mFilePointer);

			len = strlen(mMyBoneData.mBoneData[i].mObjClass) + 1;
			fwrite(&len, sizeof(int), 1, mFilePointer);
			fwrite(&mMyBoneData.mBoneData[i].mObjClass, sizeof(char), len, mFilePointer);

			// ���� Ŭ���� �̸�
			fwrite(&mMyBoneData.mBoneData[i].mParentID, sizeof(int), 1, mFilePointer);

			len = strlen(mMyBoneData.mBoneData[i].mParentName) + 1;
			fwrite(&len, sizeof(int), 1, mFilePointer);
			fwrite(&mMyBoneData.mBoneData[i].mParentName, sizeof(char), len, mFilePointer);
		}

		// �� ���̶���Ű ������
		len = mMyBoneData.mBoneHierarchy.size();
		fwrite(&len, sizeof(int), 1, mFilePointer);

		// �� ���̶���Ű�� ��ϵ� ��ŭ
		for (int i = 0; i < len; ++i)
		{
			// ���ͷ����� ������
			int idxlen = mMyBoneData.mBoneHierarchy[i].size();
			fwrite(&idxlen, sizeof(int), 1, mFilePointer);

			// ���ͷ����� ��ϵ� ��ŭ
			for (int idx = 0; idx < idxlen; ++idx)
			{
				// ������Ʈ Ű 
				fwrite(&mMyBoneData.mBoneHierarchy[i][idx].Key, sizeof(int), 1, mFilePointer);

				// ������Ʈ �̸�
				int Buflen = strlen(mMyBoneData.mBoneHierarchy[i][idx].Name) + 1;
				fwrite(&Buflen, sizeof(int), 1, mFilePointer);
				fwrite(&mMyBoneData.mBoneHierarchy[i][idx].Name, sizeof(char), Buflen, mFilePointer);

				// ������Ʈ Ŭ����
				Buflen = strlen(mMyBoneData.mBoneHierarchy[i][idx].objClass) + 1;
				fwrite(&Buflen, sizeof(int), 1, mFilePointer);
				fwrite(&mMyBoneData.mBoneHierarchy[i][idx].objClass, sizeof(char), Buflen, mFilePointer);
			}
		}

		// ���� ����
		FileClose();
		printf("[Succes] ���� ����.\n");
	}

	// �� �������� �д´�.(������) - ��
	void ReadDataMyFormat_Bone()
	{
		// ���� ���� ����
		FileOpen(mFinExportBoneData, "rb");

		// Bone �̸�
		int len = 0;
		fread(&len, sizeof(int), 1, mFilePointer);
		fread(&mMyBoneData.mMainName, sizeof(char), len, mFilePointer);

		// Bone �ִ� �̸�
		fread(&len, sizeof(int), 1, mFilePointer);
		fread(&mMyBoneData.mAniName, sizeof(char), len, mFilePointer);
		
		// �� ������ ������
		fread(&len, sizeof(int), 1, mFilePointer);
		mMyBoneData.mBoneData.resize(len);

		// �� �����Ϳ� ���� ��ŭ
		for (unsigned int i = 0; i < mMyBoneData.mBoneData.size(); ++i)
		{
			// TM ���
			fread(&mMyBoneData.mBoneData[i].mTMLocalMtx, sizeof(XMFLOAT4X4), 1, mFilePointer);
			fread(&mMyBoneData.mBoneData[i].mTMWorldMtx, sizeof(XMFLOAT4X4), 1, mFilePointer);

			// �ִϵ�����

			// ��ġ
			fread(&len, sizeof(int), 1, mFilePointer);
			mMyBoneData.mBoneData[i].aniData.Position.resize(len);
			for (int idx = 0; idx < len; ++idx)
				fread(&mMyBoneData.mBoneData[i].aniData.Position[idx], sizeof(KeyVtx), 1, mFilePointer);

			// ȸ��
			fread(&len, sizeof(int), 1, mFilePointer);
			mMyBoneData.mBoneData[i].aniData.Quaternion.resize(len);
			for (int idx = 0; idx < len; ++idx)
				fread(&mMyBoneData.mBoneData[i].aniData.Quaternion[idx], sizeof(RotKeyVtx), 1, mFilePointer);

			// ������
			fread(&len, sizeof(int), 1, mFilePointer);
			mMyBoneData.mBoneData[i].aniData.Scale.resize(len);
			for (int idx = 0; idx < len; ++idx)
				fread(&mMyBoneData.mBoneData[i].aniData.Scale[idx], sizeof(KeyVtx), 1, mFilePointer);


			// �ٿ�� �ڽ�
			fread(&mMyBoneData.mBoneData[i].mBoundingBox, sizeof(BoundBox), 1, mFilePointer);

			//--------------------------------------------------//
			// ������Ʈ �ĺ� ����
			//--------------------------------------------------//

			// ������Ʈ�̸�
			fread(&mMyBoneData.mBoneData[i].mObjID, sizeof(int), 1, mFilePointer);

			fread(&len, sizeof(int), 1, mFilePointer);
			fread(&mMyBoneData.mBoneData[i].mObjName, sizeof(char), len, mFilePointer);

			fread(&len, sizeof(int), 1, mFilePointer);
			fread(&mMyBoneData.mBoneData[i].mObjClass, sizeof(char), len, mFilePointer);

			// ���� Ŭ���� �̸�
			fread(&mMyBoneData.mBoneData[i].mParentID, sizeof(int), 1, mFilePointer);

			fread(&len, sizeof(int), 1, mFilePointer);
			fread(&mMyBoneData.mBoneData[i].mParentName, sizeof(char), len, mFilePointer);
		}

		// �� ���̶���Ű ������
		fread(&len, sizeof(int), 1, mFilePointer);
		mMyBoneData.mBoneHierarchy.resize(len);
		
		// �� ���̶���Ű�� ��ϵ� ��ŭ
		for (int i = 0; i < len; ++i)
		{
			// ���ͷ����� ������
			int idxlen = 0;
			fread(&idxlen, sizeof(int), 1, mFilePointer);
			mMyBoneData.mBoneHierarchy[i].resize(idxlen);

			// ���ͷ����� ��ϵ� ��ŭ
			for (int idx = 0; idx < idxlen; ++idx)
			{
				// ������Ʈ Ű 
				fread(&mMyBoneData.mBoneHierarchy[i][idx].Key, sizeof(int), 1, mFilePointer);

				// ������Ʈ �̸�
				int Buflen = 0;
				fread(&Buflen, sizeof(int), 1, mFilePointer);
				fread(&mMyBoneData.mBoneHierarchy[i][idx].Name, sizeof(char), Buflen, mFilePointer);

				// ������Ʈ Ŭ����
				fread(&Buflen, sizeof(int), 1, mFilePointer);
				fread(&mMyBoneData.mBoneHierarchy[i][idx].objClass, sizeof(char), Buflen, mFilePointer);
			}
		}

		// ���� ����
		FileClose();
		printf("[Succes] ���� ����.\n");
	}


	// ��� ��ģ ������
	void FinalData()
	{
		// �Ž� �� ��ŭ �ݺ��Ѵ�
		for (unsigned int i = 0; i < mMeatDatas.size(); ++i)
		{
			// ���� ���� ����ŭ �𵨰� ���Ѵ�.
			for (auto itor = mMtrl.begin(); itor != mMtrl.end(); ++itor)
			{
				// ���� & �� ������ �����
				if (strstr(mMeatDatas[i].mObjClass, "Dummy") || (strstr(mMeatDatas[i].mObjClass, "BoneGeometry")))
				{
					// �ӽ� ������� (���� �Ž� ����)
					NodeBone tBoneData;
					tBoneData.init(mMeatDatas[i]);

					// �����ϱ�
					mMyBoneData.mBoneData.push_back(tBoneData);

					// �ش� �����͵���, ������ �� �� �ʿ䰡����.
					// �ƿ� �ٸ� ���� �����ϱ⿡
					break;
				}
				
				// �𵨿� ����� ������ ���Ѵ�.
				for (unsigned int x = 0; x < mMeatDatas[i].mMatNum.size(); ++x)
				{
					// ���õ� ������ Ű��, ���õ� �𵨿����� �������? 
					// ��������, ���� �и��� �����Ѵ�.
					if (mMeatDatas[i].mMatNum[x] == itor->second.Key)
					{
						// �ӽ� ������� (�Ž� ����)
						MyMeshData tMyMeshData;
						tMyMeshData.init(mMeatDatas[i], itor->second);

						// �����ϱ�
						mMyMeshData.push_back(tMyMeshData);
					}
				}
			}
		}
	}

	// ���ڿ��� �д´�.
	char* ReadString(char* _findStr, char _eof)
	{
		// �ӽ� ����
		static char tBuff[BUF_SIZE];
		memset(tBuff, 0, sizeof(char)* BUF_SIZE);
		int  idx = 0;

		// ������Ʈ �̸�, ������Ʈ Ŭ���� �ű��
		char* addr = strstr(mReadBuf, _findStr);
		if (addr)
		{
			// ã�� ���ڿ� + 1��ŭ �� �ڷ�
			addr += strlen(_findStr) + 1;

			// eof ����
			while ((*addr) != _eof)
			{
				// ���� ���ۿ� �ִ´�.
				tBuff[idx++] = (*addr++);
			}
		}
		return tBuff;
	}

	// �Ǽ��� �д´�.
	float ReadFloat(char* _findStr, char _eof)
	{
		// �ӽ� ����
		static char tBuff[BUF_SIZE];
		memset(tBuff, 0, sizeof(char)* BUF_SIZE);
		int  idx = 0;

		// ������Ʈ �̸�, ������Ʈ Ŭ���� �ű��
		char* addr = strstr(mReadBuf, _findStr);
		if (addr)
		{
			// ã�� ���ڿ� + 1��ŭ �� �ڷ�
			addr += strlen(_findStr);

			// eof ����
			while ((*addr) != _eof)
			{
				// ���� ���ۿ� �ִ´�.
				tBuff[idx++] = (*addr++);
			}
		}
		return (float)(atof(tBuff));
	}

	// ������ �д´�.
	int ReadInt(char* _findStr, char _eof)
	{
		// �ӽ� ����
		static char tBuff[BUF_SIZE];
		memset(tBuff, 0, sizeof(char)* BUF_SIZE);
		int  idx = 0;

		// ������Ʈ �̸�, ������Ʈ Ŭ���� �ű��
		char* addr = strstr(mReadBuf, _findStr);
		if (addr)
		{
			// ã�� ���ڿ� + 1��ŭ �� �ڷ�
			addr += strlen(_findStr);

			// eof ����
			while ((*addr) != _eof)
			{
				// ���� ���ۿ� �ִ´�.
				tBuff[idx++] = (*addr++);
			}
		}
		return atoi(tBuff);
	}

	// ���ؽ� �ű��
	void MoveVtx()
	{
		// ���ؽ� �ű�� �۾�
		// ���ؽ� ũ�⸸ŭ �ø���
		for (unsigned int i = 0; i < mMeatDatas.size(); ++i)
			mMeatDatas[i].AllVertices.resize(mMeatDatas[i].Vertices.size());

		// ���ؽ��� �ű��.
		for (unsigned int i = 0; i < mMeatDatas.size(); ++i)
		{
			for (unsigned int vtx = 0; vtx < mMeatDatas[i].Vertices.size(); ++vtx)
			{
				mMeatDatas[i].AllVertices[vtx].Position = mMeatDatas[i].Vertices[vtx];
				mMeatDatas[i].AllVertices[vtx].Normal   = mMeatDatas[i].NVertices[vtx];
			}
		}
	}

	// ����
	void Build_PNT()
	{
		// ������ ���� �� ��ŭ ����Ѵ�
		for (unsigned int i = 0; i < mMeatDatas.size(); ++i)
		{
			// ���� �ø��� & ���ؽ� ���۷� �۽�
			CalVtxINCAndSet(&mMeatDatas[i]);

			// ��� ���� ����ϱ�
			CalNormalVtx(&mMeatDatas[i]);

			// ź��Ʈ ���� ���
			CalTangentSpace(&mMeatDatas[i]);
		}
	}

	// �ε��Ҽ��� ���
	bool FLOAT_ZERO(float _f)
	{
		return (_f < 0.000001 && _f > -0.000001);
	}

	// ź��Ʈ ���� ��� 
	void CalTangentSpace(InitMetaData* _MetaData)
	{
		// ���� ����
		vector<XMVECTOR> tan1(_MetaData->AllVertices.size());
		vector<XMVECTOR> tan2(_MetaData->AllVertices.size());

		// �� ���� ��ŭ
		for (int fi = 0; fi < (int)_MetaData->Indices.size(); ++fi)
		{
			//	�ﰢ�� �����͸� ��´�.
			WORD index1 = (WORD)_MetaData->Indices[fi].Vtx.x; // 0��
			WORD index2 = (WORD)_MetaData->Indices[fi].Vtx.y; // 1��
			WORD index3 = (WORD)_MetaData->Indices[fi].Vtx.z; // 2��

			//	�ﰢ���� �ش��ϴ� ����3���� ���Ѵ�.
			XMVECTOR Vtx1 = XMLoadFloat3(&_MetaData->AllVertices[index1].Position);
			XMVECTOR Vtx2 = XMLoadFloat3(&_MetaData->AllVertices[index2].Position);
			XMVECTOR Vtx3 = XMLoadFloat3(&_MetaData->AllVertices[index3].Position);

			// �ﰢ���� �ش��ϴ� �ؽ��� ����3���� ���Ѵ�.
			XMVECTOR TexVtx1 = XMLoadFloat2(&_MetaData->AllVertices[index1].TexUV);
			XMVECTOR TexVtx2 = XMLoadFloat2(&_MetaData->AllVertices[index2].TexUV);
			XMVECTOR TexVtx3 = XMLoadFloat2(&_MetaData->AllVertices[index3].TexUV);

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
			if (FLOAT_ZERO(s1 * t2 - s2 * t1))
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


			//if (FLOAT_ZERO(s1 * t2 - s2 * t1) || _isnan(s1 * t2 - s2 * t1))
			//{
			//	sdir = XMLoadFloat3(&XMFLOAT3(1.0f, 0.0f, 0.0f));
			//	tdir = XMLoadFloat3(&XMFLOAT3(0.0f, 1.0f, 0.0f));
			//}
			//else
			//{
			//	// �������� ���Ǵ�Ʈ(����)���� üũ�Ѵ�
			//
			//	//// ������� ���Ǵ�Ʈ���
			//	//float r = 1.0f / (s1 * t2 - s2 * t1);
			//	//if (_isnan(r))
			//	//{
			//	//	sdir = XMLoadFloat3(&XMFLOAT3(1.0f, 0.0f, 0.0f));
			//	//	tdir = XMLoadFloat3(&XMFLOAT3(0.0f, 1.0f, 0.0f));
			//	//}
			//	//// ���Ǵ�Ʈ�� �ƴ϶��
			//	//else
			//	//{
			//	//	// ���� ����
			//	//	sdir = XMLoadFloat3(&XMFLOAT3((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r, (t2 * z1 - t1 * z2) * r));
			//	//	tdir = XMLoadFloat3(&XMFLOAT3((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r, (s1 * z2 - s2 * z1) * r));
			//	//}
			//}

			// �� ����
			tan1[index1] += sdir;
			tan1[index2] += sdir;
			tan1[index3] += sdir;

			tan2[index1] += tdir;
			tan2[index2] += tdir;
			tan2[index3] += tdir;
		}

		// �� ����
		for (int v = 0; v < (int)_MetaData->AllVertices.size(); ++v)
		{
			const XMVECTOR& n = XMLoadFloat3(&_MetaData->AllVertices[v].Normal);
			const XMVECTOR& t = tan1[v];

			// Gram-Schmidt orthogonalize
			// ź��Ʈ Vtx �����ϱ� (xyz)
			XMStoreFloat3(&_MetaData->AllVertices[v].TangentU, XMVector3Normalize((t - n * XMVector3Dot(n, t))));


			// Calculate handedness
			//// �ؽ�ó ��ǥ ���⺤�� ���Ѵ�. (w) 
			float TangentU = (XMVectorGetX(XMVector3Dot(XMVector3Cross(n, t), tan2[v])) < 0.0F) ? -1.0f : 1.0f;

			// �����Ͽ� ����Ѵ�
			XMVECTOR BiNormal = XMVector3Cross(XMLoadFloat3(&_MetaData->AllVertices[v].TangentU), n) * TangentU;

			// ��� �� ����
			XMStoreFloat3(&_MetaData->AllVertices[v].BiNormal, BiNormal);
		}

		tan1.clear();
		tan2.clear();
	}

	// ���� ���
	void CalNormalVtx(InitMetaData* _MetaData)
	{
		vector< vector<XMVECTOR> > face_normal;

		//�� ���
		face_normal.resize(_MetaData->Indices.size());
		for (unsigned int i = 0; i < face_normal.size(); ++i)
			face_normal[i].resize(2);

		//-----------------------------------//
		//�� ��ֹ���
		//-----------------------------------//
		//�ﰢ�� ������ŭ �ݺ�
		for (int fi = 0; fi < (int)_MetaData->Indices.size(); ++fi)
		{
			//	�ﰢ�� �����͸� ��´�.
			WORD index1 = (WORD)_MetaData->Indices[fi].Vtx.x;
			WORD index2 = (WORD)_MetaData->Indices[fi].Vtx.y;
			WORD index3 = (WORD)_MetaData->Indices[fi].Vtx.z;

			//	�ﰢ���� �ش��ϴ� ����3���� ���Ѵ�.
			XMVECTOR vtx1 = XMLoadFloat3(&_MetaData->Vertices[index1]);
			XMVECTOR vtx2 = XMLoadFloat3(&_MetaData->Vertices[index2]);
			XMVECTOR vtx3 = XMLoadFloat3(&_MetaData->Vertices[index3]);

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
			for (unsigned int fi = 0; fi < _MetaData->Indices.size(); ++fi)
			{
				//		�ﰢ�� �����͸� ��´�.
				WORD index1 = (WORD)_MetaData->Indices[fi].Vtx.x;
				WORD index2 = (WORD)_MetaData->Indices[fi].Vtx.y;
				WORD index3 = (WORD)_MetaData->Indices[fi].Vtx.z;

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
			XMStoreFloat3(&_MetaData->NVertices[vi], result_vec);

		}//�ݺ�

		// �ڿ� ����
		// �ε���
		for (unsigned int i = 0; i < face_normal.size(); ++i)
		{
			face_normal[i].clear();
		}
		face_normal.clear();
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
			UINT _ModelIdx; // ���� ���� �ε���
			UINT _TexIdx;   // �ؽ�ó�� ���� �ε���

			for (int idx = 0; idx < 3; ++idx)
			{
				// ���� �ε��� ��ȣ�� �˻��Ѵ�.
				switch (idx)
				{
				case 0:
					_ModelIdx = (UINT)_MetaData->Indices   [i].Vtx.x; //  ���� ���� �ε���
					_TexIdx   = (UINT)_MetaData->TexIndices[i].Vtx.x; // �ؽ�ó�� ���� �ε���
					break;

				case 1:
					_ModelIdx = (UINT)_MetaData->Indices   [i].Vtx.y; //  ���� ���� �ε���
					_TexIdx   = (UINT)_MetaData->TexIndices[i].Vtx.y; // �ؽ�ó�� ���� �ε���
					break;

				case 2:
					_ModelIdx = (UINT)_MetaData->Indices   [i].Vtx.z; //  ���� ���� �ε���
					_TexIdx   = (UINT)_MetaData->TexIndices[i].Vtx.z; // �ؽ�ó�� ���� �ε���
					break;
				}

				// ������ �ε��� ��ȣ�� �ش��ϴ� uv���� ������.
				XMFLOAT2& ModelTexUV = _MetaData->AllVertices[_ModelIdx].TexUV; //   �� �ε����� UV ��
				XMFLOAT2&      TexUV = _MetaData->TexVertices[_TexIdx];         // �ؽ�ó �ε����� UV ��

				// ���� ���� �ε����� ����Ű�� ���ؽ��� uv�� ����ִ°�? (�ʱ� ��)
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
						// ��ġ�Ѵٸ�, �ƹ��۾��� ���� �ʴ´�.

					}
					else
					{
						// ��ġ���� �ʴ´ٸ�, ���� �ø���
						// �ش� ���ؽ��� ������ ������
						Vertex ModelVtx = _MetaData->AllVertices[_ModelIdx];

						// �ؽ�ó ���� ����
						ModelTexUV.x = TexUV.x;
						ModelTexUV.y = TexUV.y;
						
						// ����
						_MetaData->AllVertices.push_back(ModelVtx);

						// ����ġ �߰�
						WeightVtx tWeightVtx = _MetaData->mWeightVtx[_ModelIdx];
						_MetaData->mWeightVtx.push_back(tWeightVtx);
					}
				}
			}
		}
	}

	// 10 ���� itoa
	void itoa10(int n, char *buf) 
	{
		char temp[10];                // �ִ� 10 ����
		int  rem, i = 0;

		if (n == 0)
			temp[i++] = '0';
		while (n != 0) {
			rem = n % 10;             // ������ ���ϱ� 
			temp[i++] = rem + '0';
			n = n / 10;               // �� ������
		}						      
								      
		while (--i >= 0)              // ��� ����
			*buf++ = temp[i];	      
		*buf = '\0';                  // eof ����
	}

	// ������
	const wchar_t* CharToWChar(char* pstrSrc, wstring& _wstring)
	{
		int length = strlen(pstrSrc) + 1;

		// ��ȯ
		for (int i = 0; i < length; ++i)
			_wstring += wchar_t(pstrSrc[i]);

		_wstring += wchar_t('\0');

		// ���
		return _wstring.c_str();
	}
};

