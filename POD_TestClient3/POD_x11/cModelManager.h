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
	
	// 카메라
	cAniManager* mAniManager = cAniManager::GetInstance();
public:
	void Init()
	{
		// 스크린 만들기
		CreateScreen();

		// 모델 등록 (다른 종류의 모델만 1개씩)
		CreateBoxModel("BOX1", e_ShaderColor, D3D_PRIMITIVE_TOPOLOGY_LINESTRIP);
		
		CreateBoxModel("BOX2", e_ShaderLight);
		
		CreateBoxModel("BOX3", e_ShaderPongTex);
		AddTex	      ("BOX3", L"WoodCrate01_diff.dds", e_DiffuseMap);
		AddTex		  ("BOX3", L"WoodCrate01_norm.dds", e_NomalMap);
		AddTex	      ("BOX3", L"WoodCrate01_spec.dds", e_SpecularMap);
		
		CreateBoxModel("BOX4", e_ShaderPongTex); //e_ShaderCartoonTex
		AddTex	      ("BOX4", L"WoodCrate01_diff.dds", e_DiffuseMap);
		AddTex		  ("BOX4", L"WoodCrate01_norm.dds", e_NomalMap);
		AddTex	      ("BOX4", L"WoodCrate01_spec.dds", e_SpecularMap);
		
		// 모델 추가
		CreateModel("Model0", "Export/FinSkinning_TestLoc.pod", "Export/FinSkinning_TestBone.pod", e_ShaderPongTex);
		CreateModel("Model1", "Export/FinAman_boyLoc.pod"     , "Export/FinAman_boyBone.pod"     , e_ShaderPongTex);
		CreateModel("Model2", "Export/FinCat1Loc.pod"         , "Export/FinCat1Bone.pod"         , e_ShaderPongTex);
		CreateModel("Model3", "Export/FinAnonSoldierLoc.pod"  , "Export/FinAnonSoldierBone.pod"  , e_ShaderPongTex);
		//CreateModel("Model4", "Export/FinCyclopsLoc.pod"      , "Export/FinCyclopsBone.pod"      , e_ShaderPongTex);

		//CreateModel("Model4", "Export/FinAnonSoldierLoc.pod", "Export/FinAnonSoldierBone.pod", e_ShaderPongTex);
		//CreateModel("Model5", "Export/FinAnonSoldierLoc.pod", "Export/FinAnonSoldierBone.pod", e_ShaderPongTex);
		//CreateModel("Model6", "Export/FinAnonSoldierLoc.pod", "Export/FinAnonSoldierBone.pod", e_ShaderPongTex);
		//CreateModel("Model7", "Export/FinAnonSoldierLoc.pod", "Export/FinAnonSoldierBone.pod", e_ShaderPongTex);
	
		// 만들어진 모델 등록
		ModelRegistration();
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
		SafeDelete(mScreen);
		SafeDelete(mScreenBuffer);
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
		for (map<int, BufferType*>::iterator itor = mBufferType.begin(); itor != mBufferType.end(); ++itor)
		{
			for (auto itor2 = itor->second->mInstancedBuffer.begin(); itor2 != itor->second->mInstancedBuffer.end(); ++itor2)
			{
				ReleaseCOM(itor2->second);
			}
		}

		// 모델에 사용되었던 쉐이더 제거
		mUseAllShader.clear();
		mUseShader   .clear();

		// 모델 이름 제거
		mUseModel.clear();
	}

	void ClearClass()
	{
		// 버퍼 데이터 삭제
		for (map<int, BufferType*>::iterator itor = mBufferType.begin(); itor != mBufferType.end();)
		{
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
	void CreateModel(string _Name, string _ModelRoute, string _BoneRoute, SHADER_TYPE _ShaderMode, D3D_PRIMITIVE_TOPOLOGY _D3D_PRIMITIVE_TOPOLOGY = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
	{
		string tSelectModelRoute, tNewName;
		char _ModelNumBuf[128];
		//----------------------------------------------------//
		// 모델 경로 파싱
		//----------------------------------------------------//
		mXMLParser.LoadXMLRoute(_ModelRoute);

		// 읽은 경로만큼 매시데이터가 존재한다는 것이므로,
		// 이에 맞춰 파싱할 데이터를 넣을 공간을 확보해준다.
		int _ReadLocNum = mXMLParser.mNewModleLoc.size();
		mXMLParser.mMyMeshData.resize(_ReadLocNum);

		// 읽은 경로내의 모델 갯수만큼 반복
		for (int i = 0; i < _ReadLocNum; ++i)
		{
			tSelectModelRoute = mXMLParser.mNewModleLoc[i].Data; // 선택된 모델의 루트

			// 첫 모델이라면 자신의 이름으로 짓는다.
			if (i == 0)
				tNewName = _Name;
			// 서브 모델이 있는 모델이라면 '_번호'로 추가해서 짓는다.
			else
			{
				memset(&_ModelNumBuf, '\0', sizeof(_ModelNumBuf));
				itoa10(i, _ModelNumBuf);
				tNewName = _Name + '_' + _ModelNumBuf; 
			}

			// 모델 추가 생성 (기초 정보로 공간 확보)
			mAllModelData[tNewName] = new InitMetaData(_ShaderMode, _D3D_PRIMITIVE_TOPOLOGY);

			// 모델의 종류
			mAllModelData[tNewName]->mModelType = e_ParsingModel;
			mAllModelData[tNewName]->mCreateName = tNewName;

			// 추가한 모델을 파서에 등록해 데이터를 채워 넣을거임
			mXMLParser.SetModelBuf(mAllModelData[tNewName]);

			//----------------------------------------------------//
			// 모델 데이터 파싱
			//----------------------------------------------------//
			mXMLParser.LoadXMLModel(tSelectModelRoute, i);

			// 쉐이더&모델 초기화 전용
			UseAllShaderModel(_ShaderMode);
			UsingModel(tNewName);

			// 모델 체인에 이름 등록
			mModelChain[_Name].push_back(tNewName);

			// string 삭제
			tSelectModelRoute.clear();
			tNewName.clear();
		}
		
		//----------------------------------------------------//
		// 본 데이터 파싱
		//----------------------------------------------------//
		// 본은 데이터당 하나씩이기에 이런식으로 저장해도 됨.
		MyBoneData mMyBoneData;

		// 본 파싱
		mXMLParser.ReadDataMyFormat_Bone(_BoneRoute, &mMyBoneData);

		// 저장
		mAniManager->mData[_Name] = mMyBoneData;

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

		// 파서에 버퍼 등록
		mXMLParser.SetModelBuf(_nowModel);

		// 파싱 시작
		mXMLParser.LoadBox(10.0f, 10.0f, 10.0f);

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

		// 파서에 버퍼 등록
		mXMLParser.SetModelBuf(mScreen);

		// 파싱 시작
		mXMLParser.LoadScreen(WIN_X / 10, WIN_Y / 10);
		
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
	void AddModel(string _Name, float _x, float _y, float _z, OBJ_MOVEABLE _moveAble = e_StaticObj)
	{
		string _SlectModel;
		SHADER_TYPE _UseShader;

		// 모델의 체인만큼 (서브 모델)
		for (unsigned int i = 0; i < mModelChain[_Name].size(); ++i)
		{
			// 체인에 있는 모델을 순차적으로 선택
			_SlectModel = mModelChain[_Name][i];

			// 체인에 있는 모델 모두 월드매트릭스에 추가
			mAllModelData[_SlectModel]->AddModel(_x, _y, _z, _moveAble); // <-- 요 부분을 map<string, vector<InitMetaData*>> mModelChain 로 바꿔서
			_UseShader = mAllModelData[_SlectModel]->mShaderMode;		 // 모델마다 자신의 서브모델들을 모두 등록할 수 있게 해야 한다.
			UsingShaderModel(_UseShader);								 // 그래서 모델 크리에이트 하고, string으로 같은 
			_SlectModel.clear();										 // 문자열을 가진 객체들을 찾아 해쉬로 만들어둘 필요가 잇다.
																		 // 즉, 자기자신의 내부에잇는 모든 서브 모델들의 포문을 돌면서
																		 // 같은위치를 추가를 해 줘야한다는 것.
		}
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
	void AddTex(string _Name, const wchar_t* _TexName, TEXTURE_TYPE _InitTexTpye)
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
		for (unsigned int i = 0; i < mUseShader.size(); ++i)
		{
			mBufferType[mUseShader[i]]->MakeInsBuf();
		}

		// 스크린 인스턴스 버퍼 생성
		mScreenBuffer->MakeScreenInsBuf(mScreen);
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

