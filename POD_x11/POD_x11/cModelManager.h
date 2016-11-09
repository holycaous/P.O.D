#pragma once
#include "cXMLparser.h"

extern cCam gCam;

class cModelManager : public cSingleton<cModelManager>
{
public:
	map<int   , BufferType*>    mBufferType;   // 정점 버퍼 종류마다 생성할 예정 ( PNT, PL 등.. )
	map<string, InitMetaData*>  mAllModelData; // 모든 모델의 데이터
	map<string, vector<string>> mModelChain;   // 모델들을 추가할때 쓸 모델 체인(서브 모델때문에 부득이하게 만들었어야했음 -_-;;)

	vector<SHADER_TYPE> mUseAllShader;		   // 전체 모델에 사용한 쉐이더
	vector<SHADER_TYPE> mUseShader;			   // 사용중 모델에 사용한 쉐이더
	vector<string>		mUseModel;			   // 만들어진 모델

	InitMetaData* mScreen       = nullptr;	   // G버퍼 스크린
	BufferType  * mScreenBuffer = nullptr;     // G버퍼 스크린
	XMFLOAT4X4    mScreenMtx;                  // G버퍼 스크린

	// 파서
	cXMLParser mXMLParser;
	
	// 애니 매니저
	cAniManager* mAniManager = cAniManager::GetInstance();
public:
	void Init()
	{
		// 스크린 만들기
		CreateScreen();

		// 모델 등록 (다른 종류의 모델만 1개씩)
		CreateBoxModel("BOX1", e_ShaderColor, 0.7f, D3D_PRIMITIVE_TOPOLOGY_LINESTRIP);
		
		CreateBoxModel("BOX2", e_ShaderLight);
		
		CreateBoxModel("BOX3", e_ShaderPongTex);
		AddTex	      ("BOX3", "WoodCrate01_diff.dds", e_DiffuseMap);
		AddTex		  ("BOX3", "WoodCrate01_norm.dds", e_NomalMap);
		AddTex	      ("BOX3", "WoodCrate01_spec.dds", e_SpecularMap);

		// 맵 추가
		CreateModel("Map1", "Export/FinTestMapLoc.pod", e_ShaderPongTex);

		// 모델 추가
		CreateModel("Model0", "Export/FinSkinning_TestLoc.pod", e_ShaderPongTex);
		CreateModel("Model1", "Export/FinAman_boyLoc.pod"     , e_ShaderPongTex);
		CreateModel("Model2", "Export/FinCat1Loc.pod"         , e_ShaderPongTex);
		CreateModel("Model3", "Export/FinAnonSoldierLoc.pod"  , e_ShaderPongTex);
		//CreateModel("Model4", "Export/FinCyclopsLoc.pod"      , e_ShaderPongTex);
		CreateModel("Model5", "Export/FinTestSkinLoc.pod"     , e_ShaderPongTex);

		// 애니 추가
		CreateBoneAni("Model0", "Export/FinSkinning_TestBoneIdle.pod", e_ShaderPongTex);
		CreateBoneAni("Model1", "Export/FinAman_boyBoneIdle.pod"     , e_ShaderPongTex);
		CreateBoneAni("Model2", "Export/FinCat1BoneIdle.pod"         , e_ShaderPongTex);
		CreateBoneAni("Model3", "Export/FinAnonSoldierBoneIdle.pod"  , e_ShaderPongTex);
		//CreateBoneAni("Model4", "Export/FinCyclopsBoneIdle.pod"      , e_ShaderPongTex);
		CreateBoneAni("Model5", "Export/FinTestSkinBoneIdle.pod"     , e_ShaderPongTex);

		//CreateModel("Model4", "Export/FinAnonSoldierLoc.pod", "Export/FinAnonSoldierBone.pod", e_ShaderPongTex);
		//CreateModel("Model5", "Export/FinAnonSoldierLoc.pod", "Export/FinAnonSoldierBone.pod", e_ShaderPongTex);
		//CreateModel("Model6", "Export/FinAnonSoldierLoc.pod", "Export/FinAnonSoldierBone.pod", e_ShaderPongTex);
		//CreateModel("Model7", "Export/FinAnonSoldierLoc.pod", "Export/FinAnonSoldierBone.pod", e_ShaderPongTex);
	
		// 만들어진 모델 등록
		ModelRegistration();
	}

	// 특정 모델의 본 그리기
	void DrawBone(string _modelName, string _aniName)
	{
		auto Stroage = mAniManager->mData[_modelName][_aniName].GetLapStorage();
		for (unsigned int i = 0; i < Stroage.size(); ++i)
		{
			AddModel("BOX1", Stroage[i]);
		}
	}

	// 특정 모델의 본 그리기
	void DrawBone(string _modelName, string _aniName, float _x, float _y, float _z)
	{
		auto Stroage = mAniManager->mData[_modelName][_aniName].GetLapStorage();
		for (unsigned int i = 0; i < Stroage.size(); ++i)
		{
			Stroage[i]._41 += _x;
			Stroage[i]._42 += _y;
			Stroage[i]._43 += _z;

			AddModel("BOX1", Stroage[i]);
		}
	}

	// 스크린 만들기
	void CreateScreen()
	{
		if (mScreen == nullptr)
		{
			// 스크린 모델 생성하기
			CreateScreenModel();

			// 스크린 모델 월드 매트릭스 초기화
			// 매트릭스 초기화
			XMMATRIX I = XMMatrixIdentity();

			// 스크린 월드 매트릭스 초기화
			XMStoreFloat4x4(&mScreenMtx, I);
		}
	}

	// 스크린 삭제
	void ClearScreen()
	{
		mScreen->ClearWdMtx();
		mScreenBuffer->ClearInsBuf();

		SafeDelete(mScreenBuffer);
		SafeDelete(mScreen);
	}

	// 모델 등록
	void ModelRegistration()
	{
		// 계산
		CalModel();

		// 버퍼 등록
		AddBuffer();
	}

	// 모델 이동하기
	void SetPos(int _uniqueCode, string _Name, float _x, float _y, float _z)
	{
		string _SlectModel;

		// 모델의 체인만큼 (서브 모델)
		for (unsigned int i = 0; i < mModelChain[_Name].size(); ++i)
		{
			// 체인에 있는 모델을 순차적으로 선택
			_SlectModel = mModelChain[_Name][i];

			// 체인에 있는 모델 모두 이동
			mAllModelData[_SlectModel]->SetPos(_uniqueCode, _x, _y, _z);
			_SlectModel.clear();
		}
	}	

	// 모델 회전하기
	void SetRotate(int _uniqueCode, string _Name, float _x, float _y, float _z)
	{
		string _SlectModel;

		// 모델의 체인만큼 (서브 모델)
		for (unsigned int i = 0; i < mModelChain[_Name].size(); ++i)
		{
			// 체인에 있는 모델을 순차적으로 선택
			_SlectModel = mModelChain[_Name][i];

			// 체인에 있는 모델 모두 회전
			mAllModelData[_SlectModel]->SetRotate(_uniqueCode, _x, _y, _z);
			_SlectModel.clear();
		}
	}

	// 해당 위치로가는 벡터
	XMFLOAT3 GetPointDir(int _uniqueCode, string _Name, float _x, float _y, float _z)
	{
		// 체인에 있는 모델 하나만 선택해서 방향 보냄
		XMFLOAT3 ptDir;
		XMStoreFloat3(&ptDir, mAllModelData[mModelChain[_Name][0]]->GetPointDir(_uniqueCode, _x, _y, _z));

		return ptDir;
	}

	// 모델 크기 키우기
	void SetScale(int _uniqueCode, string _Name, float _x, float _y, float _z)
	{
		string _SlectModel;

		// 모델의 체인만큼 (서브 모델)
		for (unsigned int i = 0; i < mModelChain[_Name].size(); ++i)
		{
			// 체인에 있는 모델을 순차적으로 선택
			_SlectModel = mModelChain[_Name][i];

			// 체인에 있는 모델 모두 이동
			mAllModelData[_SlectModel]->SetScale(_uniqueCode, _x, _y, _z);
			_SlectModel.clear();
		}
	}

	// 모델 데이터 가져오기 (_outData에 모두 다 가져감. _outData는 밖에서 생성하고 별도로 삭제해줘야함) // 주의
	void getModelData(int _uniqueCode, string _Name, vector<ObjData>& _outData)
	{
		string _SlectModel;

		// 모델의 체인만큼 (서브 모델)
		for (unsigned int i = 0; i < mModelChain[_Name].size(); ++i)
		{
			// 체인에 있는 모델을 순차적으로 선택
			_SlectModel = mModelChain[_Name][i];

			// 체인에 있는 모델 모두 반환
			_outData.push_back(mAllModelData[_SlectModel]->getObj(_uniqueCode));
			_SlectModel.clear();
		}
	}

	// 모델 데이터 가져오기 (_outData에 모두 다 가져감. _outData는 밖에서 생성하고 별도로 삭제해줘야함) // 주의
	void getAllModelData(string _Name, vector<ObjData>& _outData)
	{
		string _SlectModel;

		// 모델의 체인만큼 (서브 모델)
		for (unsigned int i = 0; i < mModelChain[_Name].size(); ++i)
		{
			// 체인에 있는 모델을 순차적으로 선택
			_SlectModel = mModelChain[_Name][i];

			// 오브젝트 데이터 모두 가져오기
			vector<ObjData> t_ObjDataStorage = mAllModelData[_SlectModel]->getAllObj();
			for (unsigned int x = 0; x < t_ObjDataStorage.size(); ++x)
			{
				// 체인에 있는 모델 모두 반환
				_outData.push_back(t_ObjDataStorage[x]);
			}

			_SlectModel.clear();
			t_ObjDataStorage.clear();
		}
	}

	// 모델 월드 매트릭스 삭제
	void ClearModel()
	{
		// 모델 데이터 삭제
		for (map<string, InitMetaData*>::iterator itor = mAllModelData.begin(); itor != mAllModelData.end(); ++itor)
		{
			// 모델 내부 객체 삭제
			itor->second->ClearWdMtx();
		}

		// 모델 인스턴스 버퍼 삭제
		ClearModelInsBuf();

		// 모델에 사용되었던 쉐이더 제거
		mUseAllShader.clear();
		mUseShader.clear();

		// 모델 이름 제거
		mUseModel.clear();

		// 스크린 클리어
		mScreen->ClearWdMtx();
		mScreenBuffer->ClearInsBuf();
	}

	// 모델 인스턴스 버퍼만 삭제
	void ClearModelInsBuf()
	{
		// 모델 인스턴스 버퍼 삭제
		for (map<int, BufferType*>::iterator itor = mBufferType.begin(); itor != mBufferType.end(); ++itor)
		{
			itor->second->ClearInsBuf();
		}
	}

	void ClearClass()
	{
		// 버퍼 데이터 삭제
		for (map<int, BufferType*>::iterator itor = mBufferType.begin(); itor != mBufferType.end();)
		{
			// 모델 인스턴스 버퍼 삭제
			for (auto itor2 = itor->second->mInstancedBuffer.begin(); itor2 != itor->second->mInstancedBuffer.end(); ++itor2)
			{
				ReleaseCOM(itor2->second);
			}

			// 버퍼 내부 객체 삭제
			SafeDelete(itor->second);

			// 다음 변수로
			mBufferType.erase(itor++);
		}
		mBufferType.clear();

		// 모델 데이터 삭제
		for (map<string, InitMetaData*>::iterator itor = mAllModelData.begin(); itor != mAllModelData.end();)
		{
			// 모델 내부 객체 삭제
			itor->second->ClearWdMtx();

			// 모델 내부 객체 삭제
			SafeDelete(itor->second);

			// 다음 변수로
			mAllModelData.erase(itor++);
		}
		mAllModelData.clear();

		// 모델 체인 클리어
		for (auto itor = mModelChain.begin(); itor != mModelChain.end(); ++itor)
		{
			// Second 배열에 접근한다.
			// 각각의 원소에 접근하여 삭제한다.
			for (unsigned int i = 0; i < itor->second.size(); ++i)
				itor->second[i].clear();

			// Second 배열 전체를 삭제한다.
			itor->second.clear();
		}
		mModelChain.clear();

		// 모델에 사용되었던 쉐이더 제거
		mUseAllShader.clear();
		mUseShader.clear();

		// 모델 이름 제거
		mUseModel.clear();

		// 스크린 제거
		ClearScreen();
	}

	// 모델 생성하기
	void CreateModel(string _Name, string _ModelRoute, SHADER_TYPE _ShaderMode, D3D_PRIMITIVE_TOPOLOGY _D3D_PRIMITIVE_TOPOLOGY = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
	{
		string tSelectModelRoute, tNewName;
		char _ModelNumBuf[128];
		//----------------------------------------------------//
		// 모델 경로 파싱
		//----------------------------------------------------//
		// 초기화
		mXMLParser.LoadXMLRoute(_ModelRoute);

		// 읽은 경로만큼 매시데이터가 존재한다는 것이므로,
		// 이에 맞춰 파싱할 데이터를 넣을 공간을 확보해준다.
		int _ReadLocNum = mXMLParser.mNewModleLoc.size();

		// 읽은 경로내의 모델 갯수만큼 반복
		for (int i = 0; i < _ReadLocNum; ++i)
		{
			mXMLParser.InitClass();
			tSelectModelRoute = mXMLParser.mNewModleLoc[i].Data; // 선택된 모델의 루트

			// 첫 모델이라면 자신의 이름으로 짓는다.
			tNewName = _Name;
			
			// 서브 모델이 있는 모델이라면 '_번호'로 추가해서 짓는다.
			if (i > 0)
			{
				memset(&_ModelNumBuf, '\0', sizeof(_ModelNumBuf));
				itoa10(i, _ModelNumBuf);
				tNewName = tNewName + '_' + _ModelNumBuf + '\0';
			}

			// 모델 추가 생성 (생성할 이름으로 공간 확보)
			mAllModelData[tNewName] = new InitMetaData(_ShaderMode, _D3D_PRIMITIVE_TOPOLOGY);

			// 모델의 종류
			mAllModelData[tNewName]->mModelType = e_ParsingModel;
			mAllModelData[tNewName]->mCreateName = tNewName;
			
			//----------------------------------------------------//
			// 모델 데이터 파싱
			//----------------------------------------------------//
			mXMLParser.LoadXMLModel(tSelectModelRoute, *mAllModelData[tNewName]);

			// 쉐이더&모델 초기화 전용
			UseAllShaderModel(_ShaderMode);
			UsingModel(tNewName);

			// 모델 체인에 이름 등록
			mModelChain[_Name].push_back(tNewName);

			// 포인터 삭제
			mXMLParser.ClearPointer();

			// string 삭제
			tSelectModelRoute.clear();
			tNewName.clear();
		}

		// 파서 버퍼 초기화 ( 다음 모델을 받기 위해 ) 
		mXMLParser.ClearClass();
		_Name.clear();
	}

	// 모델 생성하기
	void CreateBoneAni(string _Name, string _BoneRoute, SHADER_TYPE _ShaderMode, D3D_PRIMITIVE_TOPOLOGY _D3D_PRIMITIVE_TOPOLOGY = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
	{
		//----------------------------------------------------//
		// 본 데이터 파싱
		//----------------------------------------------------//
		// 본은 데이터당 하나씩이기에 이런식으로 저장해도 됨.
		MyBoneData mMyBoneData;

		// 1개 모델의 본 파싱
		mXMLParser.ReadDataMyFormat_Bone(_BoneRoute, &mMyBoneData);

		// 데이터 계산
		mMyBoneData.CalData();

		// 체인에 연결된 모델에 저장
		for (unsigned int i = 0; i < mModelChain[_Name].size(); ++i)
		{
			string SelectName = mModelChain[_Name][i];
			mAllModelData[SelectName]->mAniName = "Idle";
			mAniManager->mData[SelectName][mMyBoneData.mAniName] = mMyBoneData;
			SelectName.clear();
		}

		// 파서 버퍼 초기화 ( 다음 모델을 받기 위해 ) 
		mXMLParser.ClearClass();
	}

	// 박스 모델 생성하기
	void CreateBoxModel(string _Name, SHADER_TYPE _ShaderMode, D3D_PRIMITIVE_TOPOLOGY _D3D_PRIMITIVE_TOPOLOGY = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
	{
		// 모델 체인에 이름 등록
		mModelChain[_Name].push_back(_Name);

		// 모델 생성
		mAllModelData[_Name] = new InitMetaData(_Name.c_str(), _ShaderMode, _D3D_PRIMITIVE_TOPOLOGY);

		// 현재 모델 정보 얻기
		InitMetaData* _nowModel = mAllModelData[_Name];

		// 모델의 종류
		_nowModel->mModelType = e_BasicModel;
		_nowModel->mCreateName = _Name;

		// 파싱 시작
		mXMLParser.LoadBox(*_nowModel, 10.0f, 10.0f, 10.0f);

		// 쉐이더, 모델 등록 (필터링 용)
		UseAllShaderModel(_ShaderMode);
		UsingModel(_Name);
	}

	void CreateBoxModel(string _Name, SHADER_TYPE _ShaderMode, float _Size, D3D_PRIMITIVE_TOPOLOGY _D3D_PRIMITIVE_TOPOLOGY = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
	{
		// 모델 체인에 이름 등록
		mModelChain[_Name].push_back(_Name);

		// 모델 생성
		mAllModelData[_Name] = new InitMetaData(_Name.c_str(), _ShaderMode, _D3D_PRIMITIVE_TOPOLOGY);

		// 현재 모델 정보 얻기
		InitMetaData* _nowModel = mAllModelData[_Name];

		// 모델의 종류
		_nowModel->mModelType = e_BasicModel;
		_nowModel->mCreateName = _Name;

		// 파싱 시작
		mXMLParser.LoadBox(*_nowModel, _Size, _Size, _Size);

		// 쉐이더, 모델 등록 (필터링 용)
		UseAllShaderModel(_ShaderMode);
		UsingModel(_Name);
	}

	// 스크린 모델 생성하기
	void CreateScreenModel(SHADER_TYPE _ShaderMode = e_ShaderDeferred, D3D_PRIMITIVE_TOPOLOGY _D3D_PRIMITIVE_TOPOLOGY = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
	{
		// 현재 모델 정보 얻기
		mScreen = new InitMetaData("GSreen", _ShaderMode, _D3D_PRIMITIVE_TOPOLOGY);

		// 모델의 종류
		mScreen->mModelType  = e_BasicModel;
		mScreen->mCreateName = "GSreen";

		// 파싱 시작
		mXMLParser.LoadScreen(*mScreen, 1.0f, 1.0f); // 풀 스크린쿼드

		// 변수 계산
		mScreen->CalValue();

		// 버퍼 생성
		mScreenBuffer = new BufferType();

		// 버퍼 빌드
		mScreenBuffer->Build_GScreen(mScreen);
	}

	// 모델에 사용한 모든 쉐이더 추가
	void UseAllShaderModel(SHADER_TYPE _ShaderMode)
	{
		// 처음부터 끝까지 검색
		for (unsigned int i = 0; i < mUseAllShader.size(); ++i)
		{
			// 안에 있는 내용이 같다면, 이미 등록 된 내용이므로 종료
			if (mUseAllShader[i] == _ShaderMode)
				return;
		}
		mUseAllShader.push_back(_ShaderMode);
	}

	// 사용중인 모델에 사용한 쉐이더 추가
	void UsingShaderModel(SHADER_TYPE _ShaderMode)
	{
		// 처음부터 끝까지 검색
		for (unsigned int i = 0; i < mUseShader.size(); ++i)
		{
			// 안에 있는 내용이 같다면, 이미 등록 된 내용이므로 종료
			if (mUseShader[i] == _ShaderMode)
				return;
		}
		mUseShader.push_back(_ShaderMode);
	}

	// 만들어진 모델
	void UsingModel(string _ModelName)
	{
		// 처음부터 끝까지 검색
		for (unsigned int i = 0; i < mUseModel.size(); ++i)
		{
			// 안에 있는 내용이 같다면, 이미 등록 된 내용이므로 종료
			if (mUseModel[i] == _ModelName)
				return;
		}
		mUseModel.push_back(_ModelName);
	}

	// 모델 추가하기
	void AddUpdateModel(string _Name, float _x, float _y, float _z, OBJ_MOVEABLE _moveAble = e_StaticObj)
	{
		// 기존 인스턴스 버퍼 삭제
		ClearModelInsBuf();

		// 모델 파일에 있는 서브 모델 수 만큼
		for (unsigned int i = 0; i < mModelChain[_Name].size(); ++i)
		{
			// 서브 모델 추가
			addSubModel(mModelChain[_Name][i], _x, _y, _z, _moveAble);
		}

		_Name.clear();

		// 다시 만들기
		MakeModelInsBuf();
	}	

	// 모델 추가하기
	void AddUpdateModel(string _Name, XMFLOAT4X4 _Mtx, OBJ_MOVEABLE _moveAble = e_StaticObj)
	{
		// 기존 인스턴스 버퍼 삭제
		ClearModelInsBuf();

		// 모델 파일에 있는 서브 모델 수 만큼
		for (unsigned int i = 0; i < mModelChain[_Name].size(); ++i)
		{
			// 서브 모델 추가
			addSubModel(mModelChain[_Name][i], _Mtx, _moveAble);
		}

		_Name.clear();

		// 다시 만들기
		MakeModelInsBuf();
	}

	// 모델 추가하기
	void AddModel(string _Name, float _x, float _y, float _z, OBJ_MOVEABLE _moveAble = e_StaticObj)
	{
		// 모델 파일에 있는 서브 모델 수 만큼
		for (unsigned int i = 0; i < mModelChain[_Name].size(); ++i)
		{
			// 서브 모델 추가
			addSubModel(mModelChain[_Name][i], _x, _y, _z, _moveAble);
		}

		_Name.clear();
	}

	// 모델 추가하기
	void AddModel(string _Name, XMFLOAT4X4 _Mtx, OBJ_MOVEABLE _moveAble = e_StaticObj)
	{
		// 모델 파일에 있는 서브 모델 수 만큼
		for (unsigned int i = 0; i < mModelChain[_Name].size(); ++i)
		{
			// 서브 모델 추가
			addSubModel(mModelChain[_Name][i], _Mtx, _moveAble);
		}

		_Name.clear();
	}

	// 서브 모델들 모두 추가
	void addSubModel(string& _SlectModel, float& _x, float& _y, float& _z, OBJ_MOVEABLE& _moveAble)
	{
		// 체인에 있는 모델 모두 월드매트릭스에 추가
		mAllModelData[_SlectModel]->AddModel(_x, _y, _z, _moveAble);

		// 쉐이더 추가
		addUseShader(_SlectModel);
	}

	// 서브 모델들 모두 추가
	void addSubModel(string& _SlectModel, XMFLOAT4X4 _Mtx, OBJ_MOVEABLE& _moveAble)
	{
		// 체인에 있는 모델 모두 월드매트릭스에 추가
		mAllModelData[_SlectModel]->AddModel(_Mtx, _moveAble);

		// 쉐이더 추가
		addUseShader(_SlectModel);
	}

	// 사용한 쉐이더 추가
	void addUseShader(string& _SlectModel)
	{
		SHADER_TYPE _UseShader;
		_UseShader = mAllModelData[_SlectModel]->mShaderMode;
		UsingShaderModel(_UseShader);
	}

	// 모델 추가하기
	void AddScreen(float _x, float _y, float _z)
	{
		mScreen->AddModel(_x, _y, _z, e_StaticObj);
	}	

	// 모델 계산하기
	void CalModel()
	{
		for (unsigned int i = 0; i < mUseModel.size(); ++i)
		{
			if (mAllModelData[mUseModel[i]]->mModelType == e_ParsingModel)
				continue;
			mAllModelData[mUseModel[i]]->CalValue();
		}
	}

	// 텍스처 추가하기
	void AddTex(string _Name, string _TexName, TEXTURE_TYPE _InitTexTpye)
	{
		mAllModelData[_Name]->LoadTex(_TexName, _InitTexTpye);
	}

	// 버퍼 추가하기
	void AddBuffer()
	{
		for (unsigned int i = 0; i < mUseAllShader.size(); ++i)
		{
			// 버퍼 생성
			mBufferType[mUseAllShader[i]] = new BufferType(mUseAllShader[i]);

			// 버퍼에 모델 등록
			AddModelInBuffer(mUseAllShader[i]);

			// 버퍼 빌드
			mBufferType[mUseAllShader[i]]->Build();
		}
	}

	// 인스턴스 버퍼 만들기
	void MakeInsbuf()
	{
		// 일반 모델 인스턴스 버퍼 생성
		MakeModelInsBuf();

		// 스크린 인스턴스 버퍼 생성
		mScreenBuffer->MakeScreenInsBuf(mScreen);
	}

	// 모델 인스턴스 버퍼 만들기
	void MakeModelInsBuf()
	{
		// 일반 모델 인스턴스 버퍼 생성
		for (unsigned int i = 0; i < mUseShader.size(); ++i)
		{
			mBufferType[mUseShader[i]]->MakeInsBuf();
		}
	}

	// 인스턴스 버퍼 업데이트
	void UpdateIns()
	{
		for (unsigned int i = 0; i < mUseShader.size(); ++i)
		{
			mBufferType[mUseShader[i]]->UpdateIns();
		}

		// 스크린 인스턴스 버퍼 업데이트
		mScreenBuffer->UpdateScreenIns(mScreen);
	}

	// 버퍼에 모델 추가하기
	void AddModelInBuffer(SHADER_TYPE _ShaderMode)
	{
		for (map<string, InitMetaData*>::iterator itor = mAllModelData.begin(); itor != mAllModelData.end(); ++itor)
		{
			// 쉐이더 모드가 같다면, 추가한다.
			if (_ShaderMode == itor->second->mShaderMode)
				mBufferType[_ShaderMode]->AddModel(itor->second->mCreateName, itor->second);
		}
	}

	// 버퍼 얻기
	BufferType* GetBuffer(SHADER_TYPE _ShaderMode)
	{
		return mBufferType[_ShaderMode];
	}
	
	// 버퍼내부 모델 불러오기
	InitMetaData* GetBufferInModel(SHADER_TYPE _ShaderMode, string _ModelName)
	{
		return mBufferType[_ShaderMode]->mModelList[_ModelName];
	}

	// 업데이트
	void Update(float& dt)
	{
		// 모든 모델들에게 일괄적인 명령을 내린다.
		for (map<string, InitMetaData*>::iterator itor = mAllModelData.begin(); itor != mAllModelData.end(); ++itor)
		{
			

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
};

