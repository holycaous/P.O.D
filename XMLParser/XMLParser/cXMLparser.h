#pragma once
#pragma warning(disable:4996)

// 재질 읽기
#define stMtrl        "<Material>"
#define edMtrl        "</Material>" 

// 매쉬 읽기
#define stMesh        "<Object"
#define edMesh        "</Object>"

// 로컬 TM
#define stLocalTM     "<LocalTM>"
#define edLocalTM     "</LocalTM>"

// 월드 TM
#define stWorldTM     "<WorldTM>"
#define edWorldTM     "</WorldTM>"

// 바운딩 박스
#define stBoundBox    "<BoundingBox>"
#define edBoundBox    "</BoundingBox>"

// 애니메이션 읽기
#define stAni         "<Key>"
#define edAni         "</Key>"

// 포지션 읽기
#define stPos         "<Position>"
#define edPos         "</Position>"

// 회전 읽기
#define stRot         "<Quaternion>"
#define edRot         "</Quaternion>"

// 크기 읽기
#define stScale       "<Scale>"
#define edScale       "</Scale>"

// 부모 읽기
#define stParent      "<Parent"
#define edParent      "</Parent>"

// 버텍스 읽기
#define stVtx         "<Vertex>"
#define edVtx         "</Vertex>"

// 노멀 버텍스 
#define stVtxN        "<VertexNormal>"
#define edVtxN        "</VertexNormal>"

// 인덱스 읽기
#define stIdx         "<TriIndex"
#define edIdx         "</TriIndex>"

// 텍스처 버텍스	    
#define stVtxT        "<TexVertex>"
#define edVtxT        "</TexVertex>"

// 텍스처 인덱스      
#define stIdxT        "<TexIndex"
#define edIdxT        "</TexIndex>"

// 정점 가중치
#define stVtxWeight   "<VertexWeight"
#define edVtxWeight   "</VertexWeight>"


// enum 값
typedef enum
{
	// 그냥 읽기
	e_Read,

	// 시작조건
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
	// 완성 된 데이터
	//------------------------------------------------------------------//

	// 새로 만들어질 모델의 위치정보
	vector<CharBuf> mNewModleLoc;

	// 새로 만들어진 모델의 정보
	vector<MyMeshData> mMyMeshData;

	// 새로만들어진 본들의 정보
	MyBoneData mMyBoneData;
	

	//------------------------------------------------------------------//
	// XML 데이터
	//------------------------------------------------------------------//

	// 메타 데이터
	vector<InitMetaData> mMeatDatas;

	// 텍스처 정보 (재질 정보)
	map<int, TextuerInfo> mMtrl;

	// 파일 포인터
	FILE* mFilePointer = nullptr;

	// 버퍼
	char mReadBuf[BUF_SIZE];
	char mSaveBuf[BUF_SIZE];

	// 파일 경로
	char mParsingFileRoute[BUF_SIZE];             // 파싱 파일 경로
	char mFileName[BUF_SIZE];		              // 파일 이름
	char mExportFileFormat[BUF_SIZE];             // 익스포트 포맷
	char mExportFileLoc[BUF_SIZE];                // 익스포트 위치
	char mFinExportModelLoc  [BUF_SIZE]; // 완료된 모델 위치를 담은 txt
	char mFinExportModelData[BUF_SIZE];  // 완료된 모델 정보를 담은 txt
	char mFinExportBoneData[BUF_SIZE];   // 완료된 본(애니) 정보를 담은 txt

	// 모델 대표 이름
	char mName[BUF_SIZE];

	// 애니이름 저장공간
	char mAnyType[BUF_SIZE];

	// 읽을 데이터가 할 행동
	ReadingCurrentData mReadCurData = e_Read;

	// 읽을 재질 순서
	int mMtrlCount;

public:
	// 버퍼 비우기
	void ClearClass()
	{
		mMeatDatas.clear();
		mMtrl.clear();
		mNewModleLoc.clear();
		mMyBoneData.ClearClass();
		mMyMeshData.clear();
	}

	// XML 초기화
	void Init(char* _ModelRoute, char* _Name)
	{
		// 대표 이름 등록
		strcpy(mName, _Name);

		// Bone 대표 이름 등록
		strcpy(mMyBoneData.mMainName, mName);

		// 경로 저장
		strcpy(mParsingFileRoute, _ModelRoute);

		// 익스포트 파일 포맷 방식
		strcpy(mExportFileFormat, ".pod");

		// 익스포트 파일 저장 위치
		strcpy(mExportFileLoc, "Export/");

		// 파일 이름 얻기
		GetFileName();

		// 익스포트 파일 로케이션
		strcpy(mFinExportModelLoc, mExportFileLoc);
		strcat(mFinExportModelLoc, mFileName);
		strcat(mFinExportModelLoc, "Loc");
		strcat(mFinExportModelLoc, mExportFileFormat);

		// 익스포트 파일 본
		strcpy(mFinExportBoneData, mExportFileLoc);
		strcat(mFinExportBoneData, mFileName);
		strcat(mFinExportBoneData, "Bone");
		strcat(mFinExportBoneData, mExportFileFormat);

		// 재질 카운트 초기화
		mMtrlCount = -1;
	}

	// XML 초기화
	void InitSkinModel(char* _ModelRoute, char* _Name, char* _AniType)
	{
		// 대표 이름 등록
		strcpy(mName, _Name);

		// Bone 대표 이름 등록
		strcpy(mMyBoneData.mMainName, mName);

		// 경로 저장
		strcpy(mParsingFileRoute, _ModelRoute);

		// 익스포트 파일 포맷 방식
		strcpy(mExportFileFormat, ".pod");

		// 익스포트 파일 저장 위치
		strcpy(mExportFileLoc, "Export/");

		// 파일 이름 얻기
		GetFileName();

		// 애니 이름 옮기기
		strcpy(mAnyType, _AniType);

		// 익스포트 파일 로케이션
		strcpy(mFinExportModelLoc, mExportFileLoc);
		strcat(mFinExportModelLoc, mFileName);
		strcat(mFinExportModelLoc, mAnyType);
		strcat(mFinExportModelLoc, "Loc");
		strcat(mFinExportModelLoc, mExportFileFormat);

		// 익스포트 파일 본
		strcpy(mFinExportBoneData, mExportFileLoc);
		strcat(mFinExportBoneData, mFileName);
		strcat(mFinExportBoneData, mAnyType);
		strcat(mFinExportBoneData, "Bone");
		strcat(mFinExportBoneData, mExportFileFormat);

		// 재질 카운트 초기화
		mMtrlCount = -1;
	}

	// XML 초기화
	void Init(char* _ModelRoute, char* _Name, char* _AniName)
	{
		// 대표 이름 등록
		strcpy(mName, _Name);

		// Bone 대표 이름 등록
		strcpy(mMyBoneData.mMainName, mName);

		// Bone 애니 이름 등록
		strcpy(mMyBoneData.mAniName, _AniName);

		// 경로 저장
		strcpy(mParsingFileRoute, _ModelRoute);

		// 익스포트 파일 포맷 방식
		strcpy(mExportFileFormat, ".pod");

		// 익스포트 파일 저장 위치
		strcpy(mExportFileLoc, "Export/");

		// 파일 이름 얻기
		GetFileName();


		// 익스포트 파일 로케이션
		strcpy(mFinExportModelLoc, mExportFileLoc);
		strcat(mFinExportModelLoc, mFileName);
		strcat(mFinExportModelLoc, "Loc");
		strcat(mFinExportModelLoc, mExportFileFormat);

		// 익스포트 파일 본
		strcpy(mFinExportBoneData, mExportFileLoc);
		strcat(mFinExportBoneData, mFileName);
		strcat(mFinExportBoneData, "Bone");
		strcat(mFinExportBoneData, mMyBoneData.mAniName);
		strcat(mFinExportBoneData, mExportFileFormat);
	}

	// XML 텍스처 등록
	void SetTexture(char* _DiffuseMap = "NULL", float _Opacity = 100.0f, char* _NoamleMap = "NULL", char* _SpecularMap = "NULL")
	{
		// 텍스처 초기화
		TextuerInfo tTex;
		strcpy(tTex.mDiffuseMap , _DiffuseMap);
		strcpy(tTex.mNoamleMap  , _NoamleMap);
		strcpy(tTex.mSpecularMap, _SpecularMap);

		// 투명도
		tTex.mOpacity = _Opacity;

		// 모델 등록
		mMtrl[++mMtrlCount] = tTex;
	}

	// 파싱
	void LoadXMLModel()
	{
		// XML 데이터 읽고, 계산한다.
		LoadXmlData();

		// 데이터 가공 끝
		FinalData();

		//--------------------------------------------//
		// 매쉬 데이터
		//--------------------------------------------//

		// << 로컬 >>
		// 내 포맷으로 매쉬데이터 로컬을 작성한다.
		WriteLocMyFormat_Model();

		// << 매쉬 데이터 >>
		// 내 포맷으로 매쉬데이터를 작성한다.
		for (unsigned int i = 0; i < mNewModleLoc.size(); ++i)
			WriteDataMyFormat_Model(i); 

		// 테스트코드 (테스트) 
		// 매쉬데이터 로컬 파일 읽기
		//mNewModleLoc.clear();
		//ReadLocMyFormat_Model(mFinExportModelLoc);
		//
		// 내 포맷으로 읽은걸 확인한다 (테스트) @@@@@@
		//MyMeshData test;
		//ReadDataMyFormat_Model(mNewModleLoc[0].Data, &test); // 0번 데이터 확인~

		// 데이터 클리어
		ClearClass();
	}

	// 파싱
	void LoadXMLSkinModel()
	{
		// XML 데이터 읽고, 계산한다.
		LoadXmlData();

		// 데이터 가공 끝
		FinalData();

		//--------------------------------------------//
		// 매쉬 데이터
		//--------------------------------------------//

		// << 로컬 >>
		// 내 포맷으로 매쉬데이터 로컬을 작성한다.
		WriteLocMyFormat_SkinModel();

		// << 매쉬 데이터 >>
		// 내 포맷으로 매쉬데이터를 작성한다.
		for (unsigned int i = 0; i < mNewModleLoc.size(); ++i)
			WriteDataMyFormat_SkinModel(i);

		// 테스트코드 (테스트) 
		// 매쉬데이터 로컬 파일 읽기
		//mNewModleLoc.clear();
		//ReadLocMyFormat_Model(mFinExportModelLoc);
		//
		// 내 포맷으로 읽은걸 확인한다 (테스트) @@@@@@
		//MyMeshData test;
		//ReadDataMyFormat_Model(mNewModleLoc[0].Data, &test); // 0번 데이터 확인~

		// 데이터 클리어
		ClearClass();
	}

	// 파싱
	void LoadXMLBone()
	{
		// XML 데이터 읽고, 계산한다.
		LoadXmlData();

		// 데이터 가공 끝
		FinalData();

		//--------------------------------------------//
		// 본 데이터
		//--------------------------------------------//
		// 트리 구조 구성
		mMyBoneData.SetTree();

		// 내 포맷으로 본 데이터를 작성한다.
		WriteDataMyFormat_Bone();

		// 내 포맷으로 본 데이터를 읽는다. (테스트) @@@@@
		//mMyBoneData.ClearClass();
		//ReadDataMyFormat_Bone();

		// 데이터 클리어
		ClearClass();
	}

private:
	// 파일 이름 얻기
	void GetFileName()
	{
		strcpy(mReadBuf, mParsingFileRoute);
		strcpy(mFileName, ReadString("Import", '.'));
	}

	// xml 파일을 파싱
	void LoadXmlData()
	{
		// 데이터 변수
		// - 사용 종료 후, ReadDetaileData에서 클리어 중
		string _ReadLineData;

		// 파일을 연다
		if (FileOpen(mParsingFileRoute, "rb"))
		{
			//파일 끝까지 탐색한다.
			while (!feof(mFilePointer))
			{
				// 읽은 파일을 처리한다.
				ReadDetaileData(_ReadLineData);
			}

			// 버텍스 옮기기
			MoveVtx();

			// 모델 빌드 ( 정점늘리기 & 탄젠트 공간 계산 )
			Build_PNT();

			// 종료전 오류 체크
			DataErrCheck();

			// 재질별로 인덱스 리스트를 구성한다.
			CalSubModel();
		}
	}

	// 파일 에러 체크
	void DataErrCheck()
	{
		// 파일 닫기
		FileClose();
		printf("[Succes] 파일 종료.\n");

		// 생성된 서브 모델 만큼
		// 텍스처 경로 확인
		int i = 0;
		for (auto itor = mMtrl.begin(); itor != mMtrl.end(); ++itor)
		{
			printf("<< %d 번째 모델 텍스처 체크 >>\n", i++);
			ConfirmMtrl(itor->second.mDiffuseMap);
			ConfirmMtrl(itor->second.mSpecularMap);
			ConfirmMtrl(itor->second.mNoamleMap);
		}
	}

	// 텍스처 확인
	void ConfirmMtrl(char* _MapName)
	{
		// 텍스처 경로 확인
		if (!strstr(_MapName, "NULL"))
		{
			char mInPutTexFileLoc[BUF_SIZE]; // 텍스처 위치
			char mOutPutTexFileLoc[BUF_SIZE]; // 텍스처 위치
			strcpy(mInPutTexFileLoc, "Import/");
			strcat(mInPutTexFileLoc, _MapName);

			// 열린경우만 닫으면 됨.
			if (FileOpen(mInPutTexFileLoc, "rb"))
			{
				FileClose();
				printf("[Succes] 파일 종료.\n");

				strcpy(mOutPutTexFileLoc, "Export/");
				strcat(mOutPutTexFileLoc, _MapName);
				strcpy(_MapName, mOutPutTexFileLoc);

				// 아웃풋으로 파일 이동
				wstring _input, _output;
				const wchar_t* Input = CharToWChar(mInPutTexFileLoc, _input);
				const wchar_t* output = CharToWChar(mOutPutTexFileLoc, _output);
				int i = MoveFileEx(Input, output, MOVEFILE_COPY_ALLOWED | MOVEFILE_WRITE_THROUGH | MOVEFILE_REPLACE_EXISTING);
				_input.clear();
				_output.clear();
			}
			// 안열렸으면 없음 처리
			else
				strcpy(_MapName, "NULL");
		}
	}

	// 파일 열기
	bool FileOpen(const char* _Route, const char* _ReadType)
	{
		//해당 경로의 파일을 연다.
		fopen_s(&mFilePointer, _Route, _ReadType);

		// 해당 경로에 파일이 있나?
		if (mFilePointer == NULL)
		{
			printf("[Error] [%s] 열기 실패. <-- 해당경로에 파일이 존재하지 않습니다.\n", _Route);
			return false;
		}
		if ((_ReadType == "wb") || (_ReadType == "wt"))
		{
			rewind(mFilePointer);
			printf("[Succes] [%s] 쓰기모드 시작.\n", _Route);
		}
		else if ((_ReadType == "rb") || (_ReadType == "rt"))
		{
			rewind(mFilePointer);
			printf("[Succes] [%s] 열기모드 시작.\n", _Route);
		}
		return true;
	}

	void FileClose()
	{
		fclose(mFilePointer);
		mFilePointer = nullptr;
	}

	// 한줄 내용 읽기
	void ReadLineData(string& _ReadLineData)
	{
		// 내용을 읽는다
		fgets(mReadBuf, BUF_SIZE, mFilePointer);

		// 원하는 내용을 옮긴다
		sscanf_s(mReadBuf, "%s", mSaveBuf, sizeof(mSaveBuf));

		// 스트링 자료형으로 옮긴다.
		// 뭘 읽을건지, 버텍스, 인덱스, tex 등
		_ReadLineData = mSaveBuf;
	}

	// 데이터 자세히 읽기 (분류, 저장)
	void ReadDetaileData(string& _ReadLineData)
	{
		// 새로 생길 재질의 번호
		int _CreateMatNum = -1;
		TextuerInfo tTex;

		// 현재 초기화할 모델 번호
		int _ReadParentNum = -1;
		InitMetaData tMetaData;

		// 좌표를 저장할 것
		KeyVtx    tKeyVtx;
		XMFLOAT3  tVtx;
		RotKeyVtx tRotKeyVtx;

		// 텍스처 좌표를 저장할것
		XMFLOAT2 tTVtx;

		// 본 가중치 저장
		WeightVtx tWeightVtx;
		BoneData  tBoneData;

		// 파일을 읽는다.
		ReadLineData(_ReadLineData);

		//// 재질
		//if (stMtrl == _ReadLineData)
		//{
		//	while (true)
		//	{
		//		// 다음 파일을 읽는다.
		//		ReadLineData(_ReadLineData);
		//
		//		// 끝을 알리는 문자가 나왔는지 확인
		//		if (edMtrl == _ReadLineData) { mReadCurData = e_Read; break; }
		//
		//		// 원하는 내용을 옮긴다 ( 현재 리드버퍼는 읽어져있는 상태 )
		//		sscanf_s(mReadBuf, "\t\t<Slot Index=\"%d\">\n", &_CreateMatNum);
		//
		//		// 해당 재질을 생성한다 저장한다.
		//		// 단, 기존에 재질이 없을때 저장한다. (모든 맵 이름이 NULL이면, 따로 텍스처를 추가 안했다는 뜻)
		//		if (strstr(mMtrl[_CreateMatNum].mDiffuseMap, "NULL") && strstr(mMtrl[_CreateMatNum].mSpecularMap, "NULL") && strstr(mMtrl[_CreateMatNum].mNoamleMap, "NULL"))
		//			mMtrl[_CreateMatNum] = tTex;
		//	}
		//}
		//// 매쉬
		//else 
		if (stMesh == _ReadLineData)
		{
			// 임시 버퍼 초기화
			memset(&tMetaData, 0, sizeof(tMetaData));

			// 원하는 내용을 옮긴다 ( 현재 리드버퍼는 읽어져있는 상태 )
			// 현재 오브젝트 정보
			sscanf_s(mReadBuf, "\t\t<Object ID=\"%d\"", &tMetaData.mObjID);

			// 스트링 데이터 읽기
			strcpy(tMetaData.mObjName , ReadString("Name=" , '\"'));
			strcpy(tMetaData.mObjClass, ReadString("Class=", '\"'));

			// 이제부터는 다 옮기고 봄
			while (true)
			{
				// 끝을 알리는 문자가 나왔는지 확인
				if (edMesh == _ReadLineData) { mReadCurData = e_Read; break; }
				else if (stParent == _ReadLineData)
				{
					// 부모 오브젝트 정보
					sscanf_s(mReadBuf, "\t\t\t<Parent ID=\"%d\"", &tMetaData.mParentID);

					// 부모 오브젝트 이름 옮기기
					strcpy(tMetaData.mParentName, ReadString("Name=", '\"'));
				}
				else if (stLocalTM == _ReadLineData)
				{
					// 다음 파일을 읽는다.
					ReadLineData(_ReadLineData);
					sscanf_s(mReadBuf, "\t\t\t\t<Value>[%f,%f,%f,%f]</Value>", &tMetaData.mTMLocalMtx._11, &tMetaData.mTMLocalMtx._12, &tMetaData.mTMLocalMtx._13, &tMetaData.mTMLocalMtx._14);

					// 다음 파일을 읽는다.
					ReadLineData(_ReadLineData);
					sscanf_s(mReadBuf, "\t\t\t\t<Value>[%f,%f,%f,%f]</Value>", &tMetaData.mTMLocalMtx._21, &tMetaData.mTMLocalMtx._22, &tMetaData.mTMLocalMtx._23, &tMetaData.mTMLocalMtx._24);

					// 다음 파일을 읽는다.
					ReadLineData(_ReadLineData);
					sscanf_s(mReadBuf, "\t\t\t\t<Value>[%f,%f,%f,%f]</Value>", &tMetaData.mTMLocalMtx._31, &tMetaData.mTMLocalMtx._32, &tMetaData.mTMLocalMtx._33, &tMetaData.mTMLocalMtx._34);

					// 다음 파일을 읽는다.
					ReadLineData(_ReadLineData);
					sscanf_s(mReadBuf, "\t\t\t\t<Value>[%f,%f,%f,%f]</Value>", &tMetaData.mTMLocalMtx._41, &tMetaData.mTMLocalMtx._42, &tMetaData.mTMLocalMtx._43, &tMetaData.mTMLocalMtx._44);
				}
				else if (stWorldTM == _ReadLineData)
				{
					// 다음 파일을 읽는다.
					ReadLineData(_ReadLineData);
					sscanf_s(mReadBuf, "\t\t\t\t<Value>[%f,%f,%f,%f]</Value>", &tMetaData.mTMWorldMtx._11, &tMetaData.mTMWorldMtx._12, &tMetaData.mTMWorldMtx._13, &tMetaData.mTMWorldMtx._14);

					// 다음 파일을 읽는다.
					ReadLineData(_ReadLineData);
					sscanf_s(mReadBuf, "\t\t\t\t<Value>[%f,%f,%f,%f]</Value>", &tMetaData.mTMWorldMtx._21, &tMetaData.mTMWorldMtx._22, &tMetaData.mTMWorldMtx._23, &tMetaData.mTMWorldMtx._24);

					// 다음 파일을 읽는다.
					ReadLineData(_ReadLineData);
					sscanf_s(mReadBuf, "\t\t\t\t<Value>[%f,%f,%f,%f]</Value>", &tMetaData.mTMWorldMtx._31, &tMetaData.mTMWorldMtx._32, &tMetaData.mTMWorldMtx._33, &tMetaData.mTMWorldMtx._34);

					// 다음 파일을 읽는다.
					ReadLineData(_ReadLineData);
					sscanf_s(mReadBuf, "\t\t\t\t<Value>[%f,%f,%f,%f]</Value>", &tMetaData.mTMWorldMtx._41, &tMetaData.mTMWorldMtx._42, &tMetaData.mTMWorldMtx._43, &tMetaData.mTMWorldMtx._44);
				}
				else if (stBoundBox == _ReadLineData)
				{
					// 다음 파일을 읽는다.
					ReadLineData(_ReadLineData);
					sscanf_s(mReadBuf, "\t\t\t\t<Max>[%f,%f,%f]</Max>", &tMetaData.mBoundingBox.Max.x, &tMetaData.mBoundingBox.Max.y, &tMetaData.mBoundingBox.Max.z);

					// 다음 파일을 읽는다.
					ReadLineData(_ReadLineData);
					sscanf_s(mReadBuf, "\t\t\t\t<Min>[%f,%f,%f]</Min>", &tMetaData.mBoundingBox.Min.x, &tMetaData.mBoundingBox.Min.y, &tMetaData.mBoundingBox.Min.z);

					// 다음 파일을 읽는다.
					ReadLineData(_ReadLineData);
					sscanf_s(mReadBuf, "\t\t\t\t<Center>[%f,%f,%f]</Center>", &tMetaData.mBoundingBox.Center.x, &tMetaData.mBoundingBox.Center.y, &tMetaData.mBoundingBox.Center.z);
				}
				else if (stVtx == _ReadLineData)
				{
					while (true)
					{
						// 다음 파일을 읽는다.
						ReadLineData(_ReadLineData);

						// 탈출 조건
						if (edVtx == _ReadLineData) { break; }

						sscanf_s(mReadBuf, "\t\t\t\t<Value Index=\"%*d\">[%f,%f,%f]</Value>", &tVtx.x, &tVtx.y, &tVtx.z);
						tMetaData.Vertices.push_back(tVtx);
					}
				}
				else if (stVtxN == _ReadLineData)
				{
					while (true)
					{
						// 다음 파일을 읽는다.
						ReadLineData(_ReadLineData);

						// 탈출 조건
						if (edVtxN == _ReadLineData) { break; }

						sscanf_s(mReadBuf, "\t\t\t\t<Value Index=\"%*d\">[%f,%f,%f]</Value>", &tVtx.x, &tVtx.y, &tVtx.z);
						tMetaData.NVertices.push_back(tVtx);
					}
				}
				else if (stIdx == _ReadLineData)
				{
					while (true)
					{
						// 다음 파일을 읽는다.
						ReadLineData(_ReadLineData);

						// 탈출 조건
						if (edIdx == _ReadLineData) { break; }

						sscanf_s(mReadBuf, "\t\t\t\t<Value MaterialID=\"%d\">[%f,%f,%f]</Value>", &tKeyVtx.Key, &tKeyVtx.Vtx.x, &tKeyVtx.Vtx.y, &tKeyVtx.Vtx.z);

						// 만약 재질이 -1(기본)이라면 0으로 초기화 시킨다.
						tKeyVtx.Key -= 1;

						if (tKeyVtx.Key <= -1)
							tKeyVtx.Key = 0;

						tMetaData.Indices.push_back(tKeyVtx);

						// 사용한 재질 추가
						tMetaData.AddMat(tKeyVtx.Key);
					}
				}
				else if (stVtxT == _ReadLineData)
				{
					while (true)
					{
						// 다음 파일을 읽는다.
						ReadLineData(_ReadLineData);

						// 탈출 조건
						if (edVtxT == _ReadLineData) { break; }

						sscanf_s(mReadBuf, "\t\t\t\t<Value Index=\"%*d\">[%f,%f]</Value>\n", &tTVtx.x, &tTVtx.y);
						tMetaData.TexVertices.push_back(tTVtx);
					}
				}
				else if (stIdxT == _ReadLineData)
				{
					while (true)
					{
						// 다음 파일을 읽는다.
						ReadLineData(_ReadLineData);

						// 탈출 조건
						if (edIdxT == _ReadLineData) { break; }

						sscanf_s(mReadBuf, "\t\t\t\t<Value MaterialID=\"%d\">[%f,%f,%f]</Value>", &tKeyVtx.Key, &tKeyVtx.Vtx.x, &tKeyVtx.Vtx.y, &tKeyVtx.Vtx.z);

						// 만약 재질이 -1(기본)이라면 0으로 초기화 시킨다.
						if (tKeyVtx.Key == -1)
							tKeyVtx.Key = 0;

						tMetaData.TexIndices.push_back(tKeyVtx);
					}
				}
				else if (stVtxWeight == _ReadLineData)
				{
					while (true)
					{
						// 다음 파일을 읽는다.
						ReadLineData(_ReadLineData);

						// 탈출 조건
						if (edVtxWeight == _ReadLineData) { break; }

						sscanf_s(mReadBuf, "\t\t\t\t<Vertex Index=\"%d\">", &tWeightVtx.TagetIdx);

						// 반복한다.
						while (true)
						{
							ReadLineData(_ReadLineData);

							if (edVtx == _ReadLineData) { break; }

							sscanf_s(mReadBuf, "\t\t\t\t\t<Value BoneID=\"%d", &tBoneData.ID);
							tBoneData.Weight = ReadFloat("\">", '<');
							strcpy(tBoneData.Name, ReadString("Name=", '\"'));

							// 본 데이터 저장
							tWeightVtx.Bone.push_back(tBoneData);
						}

						// 저장
						tMetaData.mWeightVtx.push_back(tWeightVtx);
						tWeightVtx.Bone.clear();
					}
				}
				else if (stAni == _ReadLineData)
				{
					// 애니메이션 종료가 나올때까지 반복
					if (edAni == _ReadLineData) { break; }

					while (true)
					{
						// 위치
						if (stPos == _ReadLineData)
						{
							while (true)
							{
								if (edPos == _ReadLineData) { break; }

								// 키 값 초기화 및, 추출
								tKeyVtx.Key = -1;
								sscanf_s(mReadBuf, "\t\t\t\t\t<Value Frame=\"%df\">[%f,%f,%f]</Value>", &tKeyVtx.Key, &tKeyVtx.Vtx.x, &tKeyVtx.Vtx.y, &tKeyVtx.Vtx.z);

								// 추출한 키 값이 유효할때
								if (tKeyVtx.Key >= 0)
									tMetaData.mAniData.Position.push_back(tKeyVtx);

								// 다음 파일을 읽는다.
								ReadLineData(_ReadLineData);
							}
						}

						// 회전
						if (stRot == _ReadLineData)
						{
							while (true)
							{
								if (edRot == _ReadLineData) { break; }

								// 키 값 초기화 및, 추출
								tRotKeyVtx.Key = -1;
								sscanf_s(mReadBuf, "\t\t\t\t\t<Value Frame=\"%df\">[%f,%f,%f,%f]</Value>", &tRotKeyVtx.Key, &tRotKeyVtx.Vtx.x, &tRotKeyVtx.Vtx.y, &tRotKeyVtx.Vtx.z, &tRotKeyVtx.Vtx.w);

								// 추출한 키 값이 유효할때
								if (tRotKeyVtx.Key >= 0)
									tMetaData.mAniData.Quaternion.push_back(tRotKeyVtx);

								// 다음 파일을 읽는다.
								ReadLineData(_ReadLineData);
							}
						}

						// 크기
						if (stScale == _ReadLineData)
						{
							while (true)
							{
								if (edScale == _ReadLineData) { break; }

								// 키 값 초기화 및, 추출
								tKeyVtx.Key = -1;
								sscanf_s(mReadBuf, "\t\t\t\t\t<Value Frame=\"%df\">[%f,%f,%f]</Value>", &tKeyVtx.Key, &tKeyVtx.Vtx.x, &tKeyVtx.Vtx.y, &tKeyVtx.Vtx.z);

								// 추출한 키 값이 유효할때
								if (tKeyVtx.Key >= 0)
									tMetaData.mAniData.Scale.push_back(tKeyVtx);

								// 다음 파일을 읽는다.
								ReadLineData(_ReadLineData);
							}
						}
						// 애니메이션 종료가 나올때까지 반복
						if (edAni == _ReadLineData) { break; }

						// 다음 파일을 읽는다.
						ReadLineData(_ReadLineData);
					}
				}

				// 다음 파일을 읽는다.
				ReadLineData(_ReadLineData);
			}

			// 저장한다.
			strcpy(tMetaData.mMainName, mName);
			mMeatDatas.push_back(tMetaData);
		}

		// 데이터 변수 초기화
		_ReadLineData.clear();
	}

	// 재질별로 인덱스리스트를 분배한다.
	void CalSubModel()
	{
		// 재질 별로 인덱스 리스트들을 옮긴다.

		// 모든 매쉬를 순회한다.
		for (unsigned int i = 0; i < mMeatDatas.size(); ++i)
		{
			// 어차피, 인덱스와, 텍스처 인덱스는 동일한 크기를 가지고 있다.
			for (unsigned int idx = 0; idx < mMeatDatas[i].Indices.size(); ++idx)
			{
				int _key = mMeatDatas[i].Indices[idx].Key;
				mMtrl[_key].Key = _key;
				mMtrl[_key].mMatIdx.Idx.push_back(mMeatDatas[i].Indices[idx].Vtx);
				mMtrl[_key].mMatIdx.tIdx.push_back(mMeatDatas[i].TexIndices[idx].Vtx);
			}
		}
	}

	// 내 포맷으로 작성한다.(경로)
	void WriteLocMyFormat_Model()
	{
		// 새로 만들어진 모델의 로케이션
		// 익스포트 파일 로케이션 저장
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

		// 이진 파일 열기
		FileOpen(mFinExportModelLoc, "wb");

		// 만들어진 모델 몇개인지
		int _ModelNum = mNewModleLoc.size();
		fwrite(&_ModelNum, sizeof(int), 1, mFilePointer);

		// 위에서 계산한 매쉬 경로 데이터 복사하기
		for (int i = 0; i < _ModelNum; ++i)
		{
			int len = strlen(mNewModleLoc[i].Data) + 1;
			fwrite(&len, sizeof(int), 1, mFilePointer);
			fwrite(&mNewModleLoc[i].Data, sizeof(char), len, mFilePointer);
		}

		// 파일 종료
		FileClose();
		printf("[Succes] 파일 종료.\n");
	}

	// 내 포맷으로 작성한다.(경로)
	void WriteLocMyFormat_SkinModel()
	{
		// 새로 만들어진 모델의 로케이션
		// 익스포트 파일 로케이션 저장
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

		// 이진 파일 열기
		FileOpen(mFinExportModelLoc, "wb");

		// 만들어진 모델 몇개인지
		int _ModelNum = mNewModleLoc.size();
		fwrite(&_ModelNum, sizeof(int), 1, mFilePointer);

		// 위에서 계산한 매쉬 경로 데이터 복사하기
		for (int i = 0; i < _ModelNum; ++i)
		{
			int len = strlen(mNewModleLoc[i].Data) + 1;
			fwrite(&len, sizeof(int), 1, mFilePointer);
			fwrite(&mNewModleLoc[i].Data, sizeof(char), len, mFilePointer);
		}

		// 파일 종료
		FileClose();
		printf("[Succes] 파일 종료.\n");
	}

	// 파일 읽기 테스트 (모델 데이터 경로 로드)
	void ReadLocMyFormat_Model(char* _FinModelLoc)
	{
		// 본 파일에서는 mFinExportModelLoc 이 부분에 해당하는 문자열을
		// 모델 생성 부분에 넣어 해당 변수에 넣어줘야 한다.

		// 이진 파일 열기
		FileOpen(_FinModelLoc, "rb");

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
	}

	// 내 포맷으로 작성한다.(데이터)
	void WriteDataMyFormat_Model(int _ModelNum)
	{
		// 익스포트 파일 로케이션
		char _ModelNumBuf[512];
		itoa10(_ModelNum, _ModelNumBuf);
		strcpy(mFinExportModelData, mExportFileLoc);
		strcat(mFinExportModelData, mFileName);
		strcat(mFinExportModelData, "Data");
		strcat(mFinExportModelData, _ModelNumBuf);
		strcat(mFinExportModelData, mExportFileFormat);

		// 이진 파일 열기
		FileOpen(mFinExportModelData, "wb");

		// 버텍스
		int len = mMyMeshData[_ModelNum].vertices.size();
		fwrite(&len, sizeof(int), 1, mFilePointer);

		for (int i = 0; i < len; ++i)
			fwrite(&mMyMeshData[_ModelNum].vertices[i], sizeof(Vertex), 1, mFilePointer);
		
		// 인덱스 리스트
		len = mMyMeshData[_ModelNum].indices.size();
		fwrite(&len, sizeof(int), 1, mFilePointer);

		for (int i = 0; i < len; ++i)
			fwrite(&mMyMeshData[_ModelNum].indices[i], sizeof(XMFLOAT3), 1, mFilePointer);

		// 애니데이터
	
		// 위치
		len = mMyMeshData[_ModelNum].aniData.Position.size();
		fwrite(&len, sizeof(int), 1, mFilePointer);

		for (int i = 0; i < len; ++i)
			fwrite(&mMyMeshData[_ModelNum].aniData.Position[i], sizeof(KeyVtx), 1, mFilePointer);

		// 회전
		len = mMyMeshData[_ModelNum].aniData.Quaternion.size();
		fwrite(&len, sizeof(int), 1, mFilePointer);

		for (int i = 0; i < len; ++i)
			fwrite(&mMyMeshData[_ModelNum].aniData.Quaternion[i], sizeof(RotKeyVtx), 1, mFilePointer);

		// 스케일
		len = mMyMeshData[_ModelNum].aniData.Scale.size();
		fwrite(&len, sizeof(int), 1, mFilePointer);

		for (int i = 0; i < len; ++i)
			fwrite(&mMyMeshData[_ModelNum].aniData.Scale[i], sizeof(KeyVtx), 1, mFilePointer);
		
		// 가중치 데이터

		// 전체 사이즈
		len = mMyMeshData[_ModelNum].weightVtx.size();
		fwrite(&len, sizeof(int), 1, mFilePointer);

		for (int i = 0; i < len; ++i)
		{
			// 타겟 인덱스
			fwrite(&mMyMeshData[_ModelNum].weightVtx[i].TagetIdx, sizeof(int), 1, mFilePointer);

			// 본 데이터 총 크기
			int Bonelen = mMyMeshData[_ModelNum].weightVtx[i].Bone.size();
			fwrite(&Bonelen, sizeof(int), 1, mFilePointer);

			// 본 데이터
			for (int idx = 0; idx < Bonelen; ++idx)
			{
				// ID
				fwrite(&mMyMeshData[_ModelNum].weightVtx[i].Bone[idx].ID, sizeof(int), 1, mFilePointer);

				// 가중치
				fwrite(&mMyMeshData[_ModelNum].weightVtx[i].Bone[idx].Weight, sizeof(float), 1, mFilePointer);

				// 버퍼(이름)
				int Bonebuflen = strlen(mMyMeshData[_ModelNum].weightVtx[i].Bone[idx].Name) + 1;
				fwrite(&Bonebuflen, sizeof(int), 1, mFilePointer);
				fwrite(&mMyMeshData[_ModelNum].weightVtx[i].Bone[idx].Name, sizeof(char), Bonebuflen, mFilePointer);
			}
		}

		// 오프셋
		fwrite(&mMyMeshData[_ModelNum].vertexOffset, sizeof(UINT), 1, mFilePointer);
		fwrite(&mMyMeshData[_ModelNum].indexOffset, sizeof(UINT), 1, mFilePointer);

		// 카운트
		fwrite(&mMyMeshData[_ModelNum].indexCount, sizeof(UINT), 1, mFilePointer);

		// 재질 정보

		// 투명도
		fwrite(&mMyMeshData[_ModelNum].mMyMat.mOpacity, sizeof(float), 1, mFilePointer);

		// 디퓨즈
		len = strlen(mMyMeshData[_ModelNum].mMyMat.mDiffuseMap) + 1;
		fwrite(&len, sizeof(int), 1, mFilePointer);
		fwrite(&mMyMeshData[_ModelNum].mMyMat.mDiffuseMap, sizeof(char), len, mFilePointer);

		// 노멀
		len = strlen(mMyMeshData[_ModelNum].mMyMat.mNoamleMap) + 1;
		fwrite(&len, sizeof(int), 1, mFilePointer);
		fwrite(&mMyMeshData[_ModelNum].mMyMat.mNoamleMap, sizeof(char), len, mFilePointer);

		// 스팩
		len = strlen(mMyMeshData[_ModelNum].mMyMat.mSpecularMap) + 1;
		fwrite(&len, sizeof(int), 1, mFilePointer);
		fwrite(&mMyMeshData[_ModelNum].mMyMat.mSpecularMap, sizeof(char), len, mFilePointer);

		// TM 행렬
		fwrite(&mMyMeshData[_ModelNum].mTMLocalMtx, sizeof(XMFLOAT4X4), 1, mFilePointer);
		fwrite(&mMyMeshData[_ModelNum].mTMWorldMtx, sizeof(XMFLOAT4X4), 1, mFilePointer);

		// 바운딩 박스
		fwrite(&mMyMeshData[_ModelNum].mBoundingBox, sizeof(BoundBox), 1, mFilePointer);

		//--------------------------------------------------//
		// 오브젝트 식별 정보
		//--------------------------------------------------//

		// 이름
		len = strlen(mMyMeshData[_ModelNum].mMainName) + 1;
		fwrite(&len, sizeof(int), 1, mFilePointer);
		fwrite(&mMyMeshData[_ModelNum].mMainName, sizeof(char), len, mFilePointer);

		// 오브젝트이름
		fwrite(&mMyMeshData[_ModelNum].mObjID, sizeof(int), 1, mFilePointer);
		
		len = strlen(mMyMeshData[_ModelNum].mObjName) + 1;
		fwrite(&len, sizeof(int), 1, mFilePointer);
		fwrite(&mMyMeshData[_ModelNum].mObjName, sizeof(char), len, mFilePointer);

		len = strlen(mMyMeshData[_ModelNum].mObjClass) + 1;
		fwrite(&len, sizeof(int), 1, mFilePointer);
		fwrite(&mMyMeshData[_ModelNum].mObjClass, sizeof(char), len, mFilePointer);

		// 상위 클래스 이름
		fwrite(&mMyMeshData[_ModelNum].mParentID, sizeof(int), 1, mFilePointer);

		len = strlen(mMyMeshData[_ModelNum].mParentName) + 1;
		fwrite(&len, sizeof(int), 1, mFilePointer);
		fwrite(&mMyMeshData[_ModelNum].mParentName, sizeof(char), len, mFilePointer);

		// 파일 종료
		FileClose();
		printf("[Succes] 파일 종료.\n");
	}

	// 내 포맷으로 작성한다.(데이터)
	void WriteDataMyFormat_SkinModel(int _ModelNum)
	{
		// 익스포트 파일 로케이션
		char _ModelNumBuf[512];
		itoa10(_ModelNum, _ModelNumBuf);
		strcpy(mFinExportModelData, mExportFileLoc);
		strcat(mFinExportModelData, mFileName);
		strcat(mFinExportModelData, mAnyType);
		strcat(mFinExportModelData, "Data");
		strcat(mFinExportModelData, _ModelNumBuf);
		strcat(mFinExportModelData, mExportFileFormat);

		// 이진 파일 열기
		FileOpen(mFinExportModelData, "wb");

		// 버텍스
		int len = mMyMeshData[_ModelNum].vertices.size();
		fwrite(&len, sizeof(int), 1, mFilePointer);

		for (int i = 0; i < len; ++i)
			fwrite(&mMyMeshData[_ModelNum].vertices[i], sizeof(Vertex), 1, mFilePointer);
		
		// 인덱스 리스트
		len = mMyMeshData[_ModelNum].indices.size();
		fwrite(&len, sizeof(int), 1, mFilePointer);

		for (int i = 0; i < len; ++i)
			fwrite(&mMyMeshData[_ModelNum].indices[i], sizeof(XMFLOAT3), 1, mFilePointer);

		// 애니데이터
	
		// 위치
		len = mMyMeshData[_ModelNum].aniData.Position.size();
		fwrite(&len, sizeof(int), 1, mFilePointer);

		for (int i = 0; i < len; ++i)
			fwrite(&mMyMeshData[_ModelNum].aniData.Position[i], sizeof(KeyVtx), 1, mFilePointer);

		// 회전
		len = mMyMeshData[_ModelNum].aniData.Quaternion.size();
		fwrite(&len, sizeof(int), 1, mFilePointer);

		for (int i = 0; i < len; ++i)
			fwrite(&mMyMeshData[_ModelNum].aniData.Quaternion[i], sizeof(RotKeyVtx), 1, mFilePointer);

		// 스케일
		len = mMyMeshData[_ModelNum].aniData.Scale.size();
		fwrite(&len, sizeof(int), 1, mFilePointer);

		for (int i = 0; i < len; ++i)
			fwrite(&mMyMeshData[_ModelNum].aniData.Scale[i], sizeof(KeyVtx), 1, mFilePointer);
		
		// 가중치 데이터

		// 전체 사이즈
		len = mMyMeshData[_ModelNum].weightVtx.size();
		fwrite(&len, sizeof(int), 1, mFilePointer);

		for (int i = 0; i < len; ++i)
		{
			// 타겟 인덱스
			fwrite(&mMyMeshData[_ModelNum].weightVtx[i].TagetIdx, sizeof(int), 1, mFilePointer);

			// 본 데이터 총 크기
			int Bonelen = mMyMeshData[_ModelNum].weightVtx[i].Bone.size();
			fwrite(&Bonelen, sizeof(int), 1, mFilePointer);

			// 본 데이터
			for (int idx = 0; idx < Bonelen; ++idx)
			{
				// ID
				fwrite(&mMyMeshData[_ModelNum].weightVtx[i].Bone[idx].ID, sizeof(int), 1, mFilePointer);

				// 가중치
				fwrite(&mMyMeshData[_ModelNum].weightVtx[i].Bone[idx].Weight, sizeof(float), 1, mFilePointer);

				// 버퍼(이름)
				int Bonebuflen = strlen(mMyMeshData[_ModelNum].weightVtx[i].Bone[idx].Name) + 1;
				fwrite(&Bonebuflen, sizeof(int), 1, mFilePointer);
				fwrite(&mMyMeshData[_ModelNum].weightVtx[i].Bone[idx].Name, sizeof(char), Bonebuflen, mFilePointer);
			}
		}

		// 오프셋
		fwrite(&mMyMeshData[_ModelNum].vertexOffset, sizeof(UINT), 1, mFilePointer);
		fwrite(&mMyMeshData[_ModelNum].indexOffset, sizeof(UINT), 1, mFilePointer);

		// 카운트
		fwrite(&mMyMeshData[_ModelNum].indexCount, sizeof(UINT), 1, mFilePointer);

		// 재질 정보

		// 투명도
		fwrite(&mMyMeshData[_ModelNum].mMyMat.mOpacity, sizeof(float), 1, mFilePointer);

		// 디퓨즈
		len = strlen(mMyMeshData[_ModelNum].mMyMat.mDiffuseMap) + 1;
		fwrite(&len, sizeof(int), 1, mFilePointer);
		fwrite(&mMyMeshData[_ModelNum].mMyMat.mDiffuseMap, sizeof(char), len, mFilePointer);

		// 노멀
		len = strlen(mMyMeshData[_ModelNum].mMyMat.mNoamleMap) + 1;
		fwrite(&len, sizeof(int), 1, mFilePointer);
		fwrite(&mMyMeshData[_ModelNum].mMyMat.mNoamleMap, sizeof(char), len, mFilePointer);

		// 스팩
		len = strlen(mMyMeshData[_ModelNum].mMyMat.mSpecularMap) + 1;
		fwrite(&len, sizeof(int), 1, mFilePointer);
		fwrite(&mMyMeshData[_ModelNum].mMyMat.mSpecularMap, sizeof(char), len, mFilePointer);

		// TM 행렬
		fwrite(&mMyMeshData[_ModelNum].mTMLocalMtx, sizeof(XMFLOAT4X4), 1, mFilePointer);
		fwrite(&mMyMeshData[_ModelNum].mTMWorldMtx, sizeof(XMFLOAT4X4), 1, mFilePointer);

		// 바운딩 박스
		fwrite(&mMyMeshData[_ModelNum].mBoundingBox, sizeof(BoundBox), 1, mFilePointer);

		//--------------------------------------------------//
		// 오브젝트 식별 정보
		//--------------------------------------------------//

		// 이름
		len = strlen(mMyMeshData[_ModelNum].mMainName) + 1;
		fwrite(&len, sizeof(int), 1, mFilePointer);
		fwrite(&mMyMeshData[_ModelNum].mMainName, sizeof(char), len, mFilePointer);

		// 오브젝트이름
		fwrite(&mMyMeshData[_ModelNum].mObjID, sizeof(int), 1, mFilePointer);
		
		len = strlen(mMyMeshData[_ModelNum].mObjName) + 1;
		fwrite(&len, sizeof(int), 1, mFilePointer);
		fwrite(&mMyMeshData[_ModelNum].mObjName, sizeof(char), len, mFilePointer);

		len = strlen(mMyMeshData[_ModelNum].mObjClass) + 1;
		fwrite(&len, sizeof(int), 1, mFilePointer);
		fwrite(&mMyMeshData[_ModelNum].mObjClass, sizeof(char), len, mFilePointer);

		// 상위 클래스 이름
		fwrite(&mMyMeshData[_ModelNum].mParentID, sizeof(int), 1, mFilePointer);

		len = strlen(mMyMeshData[_ModelNum].mParentName) + 1;
		fwrite(&len, sizeof(int), 1, mFilePointer);
		fwrite(&mMyMeshData[_ModelNum].mParentName, sizeof(char), len, mFilePointer);

		// 파일 종료
		FileClose();
		printf("[Succes] 파일 종료.\n");
	}

	// 내 포맷으로 읽는다.(데이터) // <--- 테스트
	void ReadDataMyFormat_Model(char* _FinExportModelData, MyMeshData* _MyMeshData)
	{
		// 이진 파일 열기
		FileOpen(_FinExportModelData, "rb");
		int len = 0;

		//------------------------------------------------------------------------------//
		// 본 데이터
		//------------------------------------------------------------------------------//

		// 버텍스
		fread(&len, sizeof(int), 1, mFilePointer);
		_MyMeshData->vertices.resize(len);

		for (int i = 0; i < len; ++i)
			fread(&_MyMeshData->vertices[i], sizeof(Vertex), 1, mFilePointer);

		// 인덱스 리스트
		fread(&len, sizeof(int), 1, mFilePointer);
		_MyMeshData->indices.resize(len);

		for (int i = 0; i < len; ++i)
			fread(&_MyMeshData->indices[i], sizeof(XMFLOAT3), 1, mFilePointer);

		// 애니데이터

		// 위치
		fread(&len, sizeof(int), 1, mFilePointer);
		_MyMeshData->aniData.Position.resize(len);

		for (int i = 0; i < len; ++i)
			fread(&_MyMeshData->aniData.Position[i], sizeof(KeyVtx), 1, mFilePointer);

		// 회전
		fread(&len, sizeof(int), 1, mFilePointer);
		_MyMeshData->aniData.Quaternion.resize(len);

		for (int i = 0; i < len; ++i)
			fread(&_MyMeshData->aniData.Quaternion[i], sizeof(RotKeyVtx), 1, mFilePointer);

		// 스케일
		fread(&len, sizeof(int), 1, mFilePointer);
		_MyMeshData->aniData.Scale.resize(len);

		for (int i = 0; i < len; ++i)
			fread(&_MyMeshData->aniData.Scale[i], sizeof(KeyVtx), 1, mFilePointer);


		// 가중치 데이터

		// 전체 사이즈
		fread(&len, sizeof(int), 1, mFilePointer);
		_MyMeshData->weightVtx.resize(len);

		for (int i = 0; i < len; ++i)
		{
			// 타겟 인덱스
			fread(&_MyMeshData->weightVtx[i].TagetIdx, sizeof(int), 1, mFilePointer);

			// 본 데이터 총 크기
			int Bonelen;
			fread(&Bonelen, sizeof(int), 1, mFilePointer);
			_MyMeshData->weightVtx[i].Bone.resize(Bonelen);

			// 본 데이터
			for (int widx = 0; widx < Bonelen; ++widx)
			{
				// ID
				fread(&_MyMeshData->weightVtx[i].Bone[widx].ID, sizeof(int), 1, mFilePointer);

				// 가중치
				fread(&_MyMeshData->weightVtx[i].Bone[widx].Weight, sizeof(float), 1, mFilePointer);

				// 버퍼(이름)
				int Bonebuflen = 0;
				fread(&Bonebuflen, sizeof(int), 1, mFilePointer);
				fread(&_MyMeshData->weightVtx[i].Bone[widx].Name, sizeof(char), Bonebuflen, mFilePointer);
			}
		}

		// 오프셋
		fread(&_MyMeshData->vertexOffset, sizeof(UINT), 1, mFilePointer);
		fread(&_MyMeshData->indexOffset, sizeof(UINT), 1, mFilePointer);

		// 카운트
		fread(&_MyMeshData->indexCount, sizeof(UINT), 1, mFilePointer);

		// 재질 정보

		// 투명도
		fread(&_MyMeshData->mMyMat.mOpacity, sizeof(float), 1, mFilePointer);

		// 디퓨즈
		fread(&len, sizeof(int), 1, mFilePointer);
		fread(&_MyMeshData->mMyMat.mDiffuseMap, sizeof(char), len, mFilePointer);

		// 노멀
		fread(&len, sizeof(int), 1, mFilePointer);
		fread(&_MyMeshData->mMyMat.mNoamleMap, sizeof(char), len, mFilePointer);

		// 스팩
		fread(&len, sizeof(int), 1, mFilePointer);
		fread(&_MyMeshData->mMyMat.mSpecularMap, sizeof(char), len, mFilePointer);


		// TM 행렬
		fread(&_MyMeshData->mTMLocalMtx, sizeof(XMFLOAT4X4), 1, mFilePointer);
		fread(&_MyMeshData->mTMWorldMtx, sizeof(XMFLOAT4X4), 1, mFilePointer);

		// 바운딩 박스
		fread(&_MyMeshData->mBoundingBox, sizeof(BoundBox), 1, mFilePointer);

		//--------------------------------------------------//
		// 오브젝트 식별 정보
		//--------------------------------------------------//

		// 이름
		fread(&len, sizeof(int), 1, mFilePointer);
		fread(&_MyMeshData->mMainName, sizeof(char), len, mFilePointer);

		// 오브젝트이름
		fread(&_MyMeshData->mObjID, sizeof(int), 1, mFilePointer);

		fread(&len, sizeof(int), 1, mFilePointer);
		fread(&_MyMeshData->mObjName, sizeof(char), len, mFilePointer);

		fread(&len, sizeof(int), 1, mFilePointer);
		fread(&_MyMeshData->mObjClass, sizeof(char), len, mFilePointer);

		// 상위 클래스 이름
		fread(&_MyMeshData->mParentID, sizeof(int), 1, mFilePointer);

		fread(&len, sizeof(int), 1, mFilePointer);
		fread(&_MyMeshData->mParentName, sizeof(char), len, mFilePointer);

		// 파일 종료
		FileClose();
		printf("[Succes] 파일 종료.\n");
	}

	// 내 포맷으로 작성한다.(데이터) - 본
	void WriteDataMyFormat_Bone()
	{
		// 이진 파일 열기
		FileOpen(mFinExportBoneData, "wb");

		// Bone 이름
		int len = strlen(mMyBoneData.mMainName) + 1;
		fwrite(&len, sizeof(int), 1, mFilePointer);
		fwrite(&mMyBoneData.mMainName, sizeof(char), len, mFilePointer);

		// Bone 애니 이름
		len = strlen(mMyBoneData.mAniName) + 1;
		fwrite(&len, sizeof(int), 1, mFilePointer);
		fwrite(&mMyBoneData.mAniName, sizeof(char), len, mFilePointer);

		// 본 데이터 사이즈
		len = mMyBoneData.mBoneData.size();
		fwrite(&len, sizeof(int), 1, mFilePointer);

		// 본 데이터 등룩된 만큼
		for (unsigned int i = 0; i < mMyBoneData.mBoneData.size(); ++i)
		{
			// TM 행렬
			fwrite(&mMyBoneData.mBoneData[i].mTMLocalMtx, sizeof(XMFLOAT4X4), 1, mFilePointer);
			fwrite(&mMyBoneData.mBoneData[i].mTMWorldMtx, sizeof(XMFLOAT4X4), 1, mFilePointer);

			// 애니데이터

			// 위치
			len = mMyBoneData.mBoneData[i].aniData.Position.size();
			fwrite(&len, sizeof(int), 1, mFilePointer);
			for (int idx = 0; idx < len; ++idx)
				fwrite(&mMyBoneData.mBoneData[i].aniData.Position[idx], sizeof(KeyVtx), 1, mFilePointer);

			// 회전
			len = mMyBoneData.mBoneData[i].aniData.Quaternion.size();
			fwrite(&len, sizeof(int), 1, mFilePointer);
			for (int idx = 0; idx < len; ++idx)
				fwrite(&mMyBoneData.mBoneData[i].aniData.Quaternion[idx], sizeof(RotKeyVtx), 1, mFilePointer);

			// 스케일
			len = mMyBoneData.mBoneData[i].aniData.Scale.size();
			fwrite(&len, sizeof(int), 1, mFilePointer);
			for (int idx = 0; idx < len; ++idx)
				fwrite(&mMyBoneData.mBoneData[i].aniData.Scale[idx], sizeof(KeyVtx), 1, mFilePointer);


			// 바운딩 박스
			fwrite(&mMyBoneData.mBoneData[i].mBoundingBox, sizeof(BoundBox), 1, mFilePointer);

			//--------------------------------------------------//
			// 오브젝트 식별 정보
			//--------------------------------------------------//

			// 오브젝트이름
			fwrite(&mMyBoneData.mBoneData[i].mObjID, sizeof(int), 1, mFilePointer);

			len = strlen(mMyBoneData.mBoneData[i].mObjName) + 1;
			fwrite(&len, sizeof(int), 1, mFilePointer);
			fwrite(&mMyBoneData.mBoneData[i].mObjName, sizeof(char), len, mFilePointer);

			len = strlen(mMyBoneData.mBoneData[i].mObjClass) + 1;
			fwrite(&len, sizeof(int), 1, mFilePointer);
			fwrite(&mMyBoneData.mBoneData[i].mObjClass, sizeof(char), len, mFilePointer);

			// 상위 클래스 이름
			fwrite(&mMyBoneData.mBoneData[i].mParentID, sizeof(int), 1, mFilePointer);

			len = strlen(mMyBoneData.mBoneData[i].mParentName) + 1;
			fwrite(&len, sizeof(int), 1, mFilePointer);
			fwrite(&mMyBoneData.mBoneData[i].mParentName, sizeof(char), len, mFilePointer);
		}

		// 본 하이라이키 사이즈
		len = mMyBoneData.mBoneHierarchy.size();
		fwrite(&len, sizeof(int), 1, mFilePointer);

		// 본 하이라이키에 등록된 만큼
		for (int i = 0; i < len; ++i)
		{
			// 이터레이터 사이즈
			int idxlen = mMyBoneData.mBoneHierarchy[i].size();
			fwrite(&idxlen, sizeof(int), 1, mFilePointer);

			// 이터레이터 등록된 만큼
			for (int idx = 0; idx < idxlen; ++idx)
			{
				// 오브젝트 키 
				fwrite(&mMyBoneData.mBoneHierarchy[i][idx].Key, sizeof(int), 1, mFilePointer);

				// 오브젝트 이름
				int Buflen = strlen(mMyBoneData.mBoneHierarchy[i][idx].Name) + 1;
				fwrite(&Buflen, sizeof(int), 1, mFilePointer);
				fwrite(&mMyBoneData.mBoneHierarchy[i][idx].Name, sizeof(char), Buflen, mFilePointer);

				// 오브젝트 클래스
				Buflen = strlen(mMyBoneData.mBoneHierarchy[i][idx].objClass) + 1;
				fwrite(&Buflen, sizeof(int), 1, mFilePointer);
				fwrite(&mMyBoneData.mBoneHierarchy[i][idx].objClass, sizeof(char), Buflen, mFilePointer);
			}
		}

		// 파일 종료
		FileClose();
		printf("[Succes] 파일 종료.\n");
	}

	// 내 포맷으로 읽는다.(데이터) - 본
	void ReadDataMyFormat_Bone()
	{
		// 이진 파일 열기
		FileOpen(mFinExportBoneData, "rb");

		// Bone 이름
		int len = 0;
		fread(&len, sizeof(int), 1, mFilePointer);
		fread(&mMyBoneData.mMainName, sizeof(char), len, mFilePointer);

		// Bone 애니 이름
		fread(&len, sizeof(int), 1, mFilePointer);
		fread(&mMyBoneData.mAniName, sizeof(char), len, mFilePointer);
		
		// 본 데이터 사이즈
		fread(&len, sizeof(int), 1, mFilePointer);
		mMyBoneData.mBoneData.resize(len);

		// 본 데이터에 등룩된 만큼
		for (unsigned int i = 0; i < mMyBoneData.mBoneData.size(); ++i)
		{
			// TM 행렬
			fread(&mMyBoneData.mBoneData[i].mTMLocalMtx, sizeof(XMFLOAT4X4), 1, mFilePointer);
			fread(&mMyBoneData.mBoneData[i].mTMWorldMtx, sizeof(XMFLOAT4X4), 1, mFilePointer);

			// 애니데이터

			// 위치
			fread(&len, sizeof(int), 1, mFilePointer);
			mMyBoneData.mBoneData[i].aniData.Position.resize(len);
			for (int idx = 0; idx < len; ++idx)
				fread(&mMyBoneData.mBoneData[i].aniData.Position[idx], sizeof(KeyVtx), 1, mFilePointer);

			// 회전
			fread(&len, sizeof(int), 1, mFilePointer);
			mMyBoneData.mBoneData[i].aniData.Quaternion.resize(len);
			for (int idx = 0; idx < len; ++idx)
				fread(&mMyBoneData.mBoneData[i].aniData.Quaternion[idx], sizeof(RotKeyVtx), 1, mFilePointer);

			// 스케일
			fread(&len, sizeof(int), 1, mFilePointer);
			mMyBoneData.mBoneData[i].aniData.Scale.resize(len);
			for (int idx = 0; idx < len; ++idx)
				fread(&mMyBoneData.mBoneData[i].aniData.Scale[idx], sizeof(KeyVtx), 1, mFilePointer);


			// 바운딩 박스
			fread(&mMyBoneData.mBoneData[i].mBoundingBox, sizeof(BoundBox), 1, mFilePointer);

			//--------------------------------------------------//
			// 오브젝트 식별 정보
			//--------------------------------------------------//

			// 오브젝트이름
			fread(&mMyBoneData.mBoneData[i].mObjID, sizeof(int), 1, mFilePointer);

			fread(&len, sizeof(int), 1, mFilePointer);
			fread(&mMyBoneData.mBoneData[i].mObjName, sizeof(char), len, mFilePointer);

			fread(&len, sizeof(int), 1, mFilePointer);
			fread(&mMyBoneData.mBoneData[i].mObjClass, sizeof(char), len, mFilePointer);

			// 상위 클래스 이름
			fread(&mMyBoneData.mBoneData[i].mParentID, sizeof(int), 1, mFilePointer);

			fread(&len, sizeof(int), 1, mFilePointer);
			fread(&mMyBoneData.mBoneData[i].mParentName, sizeof(char), len, mFilePointer);
		}

		// 본 하이라이키 사이즈
		fread(&len, sizeof(int), 1, mFilePointer);
		mMyBoneData.mBoneHierarchy.resize(len);
		
		// 본 하이라이키에 등록된 만큼
		for (int i = 0; i < len; ++i)
		{
			// 이터레이터 사이즈
			int idxlen = 0;
			fread(&idxlen, sizeof(int), 1, mFilePointer);
			mMyBoneData.mBoneHierarchy[i].resize(idxlen);

			// 이터레이터 등록된 만큼
			for (int idx = 0; idx < idxlen; ++idx)
			{
				// 오브젝트 키 
				fread(&mMyBoneData.mBoneHierarchy[i][idx].Key, sizeof(int), 1, mFilePointer);

				// 오브젝트 이름
				int Buflen = 0;
				fread(&Buflen, sizeof(int), 1, mFilePointer);
				fread(&mMyBoneData.mBoneHierarchy[i][idx].Name, sizeof(char), Buflen, mFilePointer);

				// 오브젝트 클래스
				fread(&Buflen, sizeof(int), 1, mFilePointer);
				fread(&mMyBoneData.mBoneHierarchy[i][idx].objClass, sizeof(char), Buflen, mFilePointer);
			}
		}

		// 파일 종료
		FileClose();
		printf("[Succes] 파일 종료.\n");
	}


	// 계산 마친 데이터
	void FinalData()
	{
		// 매쉬 수 만큼 반복한다
		for (unsigned int i = 0; i < mMeatDatas.size(); ++i)
		{
			// 서브 재질 수만큼 모델과 비교한다.
			for (auto itor = mMtrl.begin(); itor != mMtrl.end(); ++itor)
			{
				// 더미 & 본 데이터 만들기
				if (strstr(mMeatDatas[i].mObjClass, "Dummy") || (strstr(mMeatDatas[i].mObjClass, "BoneGeometry")))
				{
					// 임시 저장버퍼 (더미 매쉬 생성)
					NodeBone tBoneData;
					tBoneData.init(mMeatDatas[i]);

					// 저장하기
					mMyBoneData.mBoneData.push_back(tBoneData);

					// 해당 데이터들은, 재질을 더 돌 필요가없음.
					// 아예 다른 곳에 저장하기에
					break;
				}
				
				// 모델에 사용됬던 재질과 비교한다.
				for (unsigned int x = 0; x < mMeatDatas[i].mMatNum.size(); ++x)
				{
					// 선택된 재질의 키가, 선택된 모델에서도 사용됬느냐? 
					// 사용됬으면, 모델을 분리해 생성한다.
					if (mMeatDatas[i].mMatNum[x] == itor->second.Key)
					{
						// 임시 저장버퍼 (매쉬 생성)
						MyMeshData tMyMeshData;
						tMyMeshData.init(mMeatDatas[i], itor->second);

						// 저장하기
						mMyMeshData.push_back(tMyMeshData);
					}
				}
			}
		}
	}

	// 문자열만 읽는다.
	char* ReadString(char* _findStr, char _eof)
	{
		// 임시 버퍼
		static char tBuff[BUF_SIZE];
		memset(tBuff, 0, sizeof(char)* BUF_SIZE);
		int  idx = 0;

		// 오브젝트 이름, 오브젝트 클래스 옮기기
		char* addr = strstr(mReadBuf, _findStr);
		if (addr)
		{
			// 찾은 문자열 + 1만큼 더 뒤로
			addr += strlen(_findStr) + 1;

			// eof 문자
			while ((*addr) != _eof)
			{
				// 값을 버퍼에 넣는다.
				tBuff[idx++] = (*addr++);
			}
		}
		return tBuff;
	}

	// 실수만 읽는다.
	float ReadFloat(char* _findStr, char _eof)
	{
		// 임시 버퍼
		static char tBuff[BUF_SIZE];
		memset(tBuff, 0, sizeof(char)* BUF_SIZE);
		int  idx = 0;

		// 오브젝트 이름, 오브젝트 클래스 옮기기
		char* addr = strstr(mReadBuf, _findStr);
		if (addr)
		{
			// 찾은 문자열 + 1만큼 더 뒤로
			addr += strlen(_findStr);

			// eof 문자
			while ((*addr) != _eof)
			{
				// 값을 버퍼에 넣는다.
				tBuff[idx++] = (*addr++);
			}
		}
		return (float)(atof(tBuff));
	}

	// 정수만 읽는다.
	int ReadInt(char* _findStr, char _eof)
	{
		// 임시 버퍼
		static char tBuff[BUF_SIZE];
		memset(tBuff, 0, sizeof(char)* BUF_SIZE);
		int  idx = 0;

		// 오브젝트 이름, 오브젝트 클래스 옮기기
		char* addr = strstr(mReadBuf, _findStr);
		if (addr)
		{
			// 찾은 문자열 + 1만큼 더 뒤로
			addr += strlen(_findStr);

			// eof 문자
			while ((*addr) != _eof)
			{
				// 값을 버퍼에 넣는다.
				tBuff[idx++] = (*addr++);
			}
		}
		return atoi(tBuff);
	}

	// 버텍스 옮기기
	void MoveVtx()
	{
		// 버텍스 옮기기 작업
		// 버텍스 크기만큼 늘린다
		for (unsigned int i = 0; i < mMeatDatas.size(); ++i)
			mMeatDatas[i].AllVertices.resize(mMeatDatas[i].Vertices.size());

		// 버텍스를 옮긴다.
		for (unsigned int i = 0; i < mMeatDatas.size(); ++i)
		{
			for (unsigned int vtx = 0; vtx < mMeatDatas[i].Vertices.size(); ++vtx)
			{
				mMeatDatas[i].AllVertices[vtx].Position = mMeatDatas[i].Vertices[vtx];
				mMeatDatas[i].AllVertices[vtx].Normal   = mMeatDatas[i].NVertices[vtx];
			}
		}
	}

	// 빌드
	void Build_PNT()
	{
		// 생성된 서브 모델 만큼 계산한다
		for (unsigned int i = 0; i < mMeatDatas.size(); ++i)
		{
			// 정점 늘리기 & 버텍스 버퍼로 송신
			CalVtxINCAndSet(&mMeatDatas[i]);

			// 노멀 법선 계산하기
			CalNormalVtx(&mMeatDatas[i]);

			// 탄젠트 공간 계산
			CalTangentSpace(&mMeatDatas[i]);
		}
	}

	// 부동소수점 잡기
	bool FLOAT_ZERO(float _f)
	{
		return (_f < 0.000001 && _f > -0.000001);
	}

	// 탄젠트 공간 계산 
	void CalTangentSpace(InitMetaData* _MetaData)
	{
		// 저장 공간
		vector<XMVECTOR> tan1(_MetaData->AllVertices.size());
		vector<XMVECTOR> tan2(_MetaData->AllVertices.size());

		// 면 갯수 만큼
		for (int fi = 0; fi < (int)_MetaData->Indices.size(); ++fi)
		{
			//	삼각형 데이터를 얻는다.
			WORD index1 = (WORD)_MetaData->Indices[fi].Vtx.x; // 0번
			WORD index2 = (WORD)_MetaData->Indices[fi].Vtx.y; // 1번
			WORD index3 = (WORD)_MetaData->Indices[fi].Vtx.z; // 2번

			//	삼각형에 해당하는 정점3개를 구한다.
			XMVECTOR Vtx1 = XMLoadFloat3(&_MetaData->AllVertices[index1].Position);
			XMVECTOR Vtx2 = XMLoadFloat3(&_MetaData->AllVertices[index2].Position);
			XMVECTOR Vtx3 = XMLoadFloat3(&_MetaData->AllVertices[index3].Position);

			// 삼각형에 해당하는 텍스터 정점3개를 구한다.
			XMVECTOR TexVtx1 = XMLoadFloat2(&_MetaData->AllVertices[index1].TexUV);
			XMVECTOR TexVtx2 = XMLoadFloat2(&_MetaData->AllVertices[index2].TexUV);
			XMVECTOR TexVtx3 = XMLoadFloat2(&_MetaData->AllVertices[index3].TexUV);

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
			if (FLOAT_ZERO(s1 * t2 - s2 * t1))
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


			//if (FLOAT_ZERO(s1 * t2 - s2 * t1) || _isnan(s1 * t2 - s2 * t1))
			//{
			//	sdir = XMLoadFloat3(&XMFLOAT3(1.0f, 0.0f, 0.0f));
			//	tdir = XMLoadFloat3(&XMFLOAT3(0.0f, 1.0f, 0.0f));
			//}
			//else
			//{
			//	// 숫자인지 인피니트(에러)인지 체크한다
			//
			//	//// 결과값이 인피니트라면
			//	//float r = 1.0f / (s1 * t2 - s2 * t1);
			//	//if (_isnan(r))
			//	//{
			//	//	sdir = XMLoadFloat3(&XMFLOAT3(1.0f, 0.0f, 0.0f));
			//	//	tdir = XMLoadFloat3(&XMFLOAT3(0.0f, 1.0f, 0.0f));
			//	//}
			//	//// 인피니트가 아니라면
			//	//else
			//	//{
			//	//	// 공식 대입
			//	//	sdir = XMLoadFloat3(&XMFLOAT3((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r, (t2 * z1 - t1 * z2) * r));
			//	//	tdir = XMLoadFloat3(&XMFLOAT3((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r, (s1 * z2 - s2 * z1) * r));
			//	//}
			//}

			// 값 누적
			tan1[index1] += sdir;
			tan1[index2] += sdir;
			tan1[index3] += sdir;

			tan2[index1] += tdir;
			tan2[index2] += tdir;
			tan2[index3] += tdir;
		}

		// 값 복사
		for (int v = 0; v < (int)_MetaData->AllVertices.size(); ++v)
		{
			const XMVECTOR& n = XMLoadFloat3(&_MetaData->AllVertices[v].Normal);
			const XMVECTOR& t = tan1[v];

			// Gram-Schmidt orthogonalize
			// 탄젠트 Vtx 저장하기 (xyz)
			XMStoreFloat3(&_MetaData->AllVertices[v].TangentU, XMVector3Normalize((t - n * XMVector3Dot(n, t))));


			// Calculate handedness
			//// 텍스처 좌표 방향벡터 구한다. (w) 
			float TangentU = (XMVectorGetX(XMVector3Dot(XMVector3Cross(n, t), tan2[v])) < 0.0F) ? -1.0f : 1.0f;

			// 외적하여 계산한다
			XMVECTOR BiNormal = XMVector3Cross(XMLoadFloat3(&_MetaData->AllVertices[v].TangentU), n) * TangentU;

			// 결과 값 저장
			XMStoreFloat3(&_MetaData->AllVertices[v].BiNormal, BiNormal);
		}

		tan1.clear();
		tan2.clear();
	}

	// 법선 계산
	void CalNormalVtx(InitMetaData* _MetaData)
	{
		vector< vector<XMVECTOR> > face_normal;

		//면 노멀
		face_normal.resize(_MetaData->Indices.size());
		for (unsigned int i = 0; i < face_normal.size(); ++i)
			face_normal[i].resize(2);

		//-----------------------------------//
		//면 노멀법선
		//-----------------------------------//
		//삼각형 갯수만큼 반복
		for (int fi = 0; fi < (int)_MetaData->Indices.size(); ++fi)
		{
			//	삼각형 데이터를 얻는다.
			WORD index1 = (WORD)_MetaData->Indices[fi].Vtx.x;
			WORD index2 = (WORD)_MetaData->Indices[fi].Vtx.y;
			WORD index3 = (WORD)_MetaData->Indices[fi].Vtx.z;

			//	삼각형에 해당하는 정점3개를 구한다.
			XMVECTOR vtx1 = XMLoadFloat3(&_MetaData->Vertices[index1]);
			XMVECTOR vtx2 = XMLoadFloat3(&_MetaData->Vertices[index2]);
			XMVECTOR vtx3 = XMLoadFloat3(&_MetaData->Vertices[index3]);

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
			for (unsigned int fi = 0; fi < _MetaData->Indices.size(); ++fi)
			{
				//		삼각형 데이터를 얻는다.
				WORD index1 = (WORD)_MetaData->Indices[fi].Vtx.x;
				WORD index2 = (WORD)_MetaData->Indices[fi].Vtx.y;
				WORD index3 = (WORD)_MetaData->Indices[fi].Vtx.z;

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
			XMStoreFloat3(&_MetaData->NVertices[vi], result_vec);

		}//반복

		// 자원 제거
		// 인덱스
		for (unsigned int i = 0; i < face_normal.size(); ++i)
		{
			face_normal[i].clear();
		}
		face_normal.clear();
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
			UINT _ModelIdx; // 모델의 현재 인덱스
			UINT _TexIdx;   // 텍스처의 현재 인덱스

			for (int idx = 0; idx < 3; ++idx)
			{
				// 현재 인덱스 번호를 검색한다.
				switch (idx)
				{
				case 0:
					_ModelIdx = (UINT)_MetaData->Indices   [i].Vtx.x; //  모델의 현재 인덱스
					_TexIdx   = (UINT)_MetaData->TexIndices[i].Vtx.x; // 텍스처의 현재 인덱스
					break;

				case 1:
					_ModelIdx = (UINT)_MetaData->Indices   [i].Vtx.y; //  모델의 현재 인덱스
					_TexIdx   = (UINT)_MetaData->TexIndices[i].Vtx.y; // 텍스처의 현재 인덱스
					break;

				case 2:
					_ModelIdx = (UINT)_MetaData->Indices   [i].Vtx.z; //  모델의 현재 인덱스
					_TexIdx   = (UINT)_MetaData->TexIndices[i].Vtx.z; // 텍스처의 현재 인덱스
					break;
				}

				// 각각의 인덱스 번호에 해당하는 uv값을 꺼낸다.
				XMFLOAT2& ModelTexUV = _MetaData->AllVertices[_ModelIdx].TexUV; //   모델 인덱스의 UV 값
				XMFLOAT2&      TexUV = _MetaData->TexVertices[_TexIdx];         // 텍스처 인덱스의 UV 값

				// 모델의 현재 인덱스가 가르키는 버텍스의 uv가 비어있는가? (초기 값)
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
						// 일치한다면, 아무작업도 하지 않는다.

					}
					else
					{
						// 일치하지 않는다면, 정점 늘리기
						// 해당 버텍스의 정보를 꺼낸다
						Vertex ModelVtx = _MetaData->AllVertices[_ModelIdx];

						// 텍스처 정보 변경
						ModelTexUV.x = TexUV.x;
						ModelTexUV.y = TexUV.y;
						
						// 복사
						_MetaData->AllVertices.push_back(ModelVtx);

						// 가중치 추가
						WeightVtx tWeightVtx = _MetaData->mWeightVtx[_ModelIdx];
						_MetaData->mWeightVtx.push_back(tWeightVtx);
					}
				}
			}
		}
	}

	// 10 진수 itoa
	void itoa10(int n, char *buf) 
	{
		char temp[10];                // 최대 10 진수
		int  rem, i = 0;

		if (n == 0)
			temp[i++] = '0';
		while (n != 0) {
			rem = n % 10;             // 나머지 구하기 
			temp[i++] = rem + '0';
			n = n / 10;               // 몫 나누기
		}						      
								      
		while (--i >= 0)              // 결과 반전
			*buf++ = temp[i];	      
		*buf = '\0';                  // eof 문자
	}

	// 컨버터
	const wchar_t* CharToWChar(char* pstrSrc, wstring& _wstring)
	{
		int length = strlen(pstrSrc) + 1;

		// 변환
		for (int i = 0; i < length; ++i)
			_wstring += wchar_t(pstrSrc[i]);

		_wstring += wchar_t('\0');

		// 결과
		return _wstring.c_str();
	}
};

