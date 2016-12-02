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

	// 맵 매니저
	cMapManager* mMapManager = cMapManager::GetInstance();

	// 플레이어
	PlayerInfo mPlayer;
	
	bool _modelUpdate = false;
public:
	void Init()
	{
		// 스크린 만들기
		CreateScreen();

		// 모델 등록 (다른 종류의 모델만 1개씩)
		CreateBoxModel("BOX1", e_ShaderColor, 0.7f, D3D_PRIMITIVE_TOPOLOGY_LINESTRIP);
		
		CreateBoxModel("BOX2", e_ShaderLight);
		
		CreateBoxModel("BOX3", e_ShaderPongTex);
		AddTex        ("BOX3", "Export/WoodCrate01_diff.dds", e_DiffuseMap);
		AddTex        ("BOX3", "Export/WoodCrate01_norm.dds", e_NomalMap);
		AddTex        ("BOX3", "Export/WoodCrate01_spec.dds", e_SpecularMap);

		// 맵 추가
		CreateMap     ("Map1", "Export/HeightMap/Map1.raw"  , 100.0f, 100.0f, 50.0f, 1000.0f, e_ShaderPongTexMap);
		AddTex        ("Map1", "Export/ground_diff.dds"     , e_DiffuseMap);
		AddTex        ("Map1", "Export/ground_norm.dds"     , e_NomalMap);
		AddTex        ("Map1", "Export/ground_spec.dds"     , e_SpecularMap);

		// 큐브맵 추가
		CreateSkyBox("CubeMap1", "Export/CubeMap/snowcube1024.dds", 0.5f, 20, 20, e_ShaderSkyBox);


		//-------------------------------------------------------------------------------//
		//
		//  ★★ 생성 모델명과 경로이름 반드시 바꿔줄것 ★★
		//
		//-------------------------------------------------------------------------------//
		// 모델 한세트 추가
		//-------------------------------------------------------------------------------//
		CreateModel  ("Model1", "Export/mob1Loc.pod"		    , e_ShaderPongTexAni);
		AddModelChain("Model1", "Export/mob1IdleLoc.pod"        , e_ShaderPongTexAni, e_Idle);    
		AddModelChain("Model1", "Export/mob1DamageLoc.pod"      , e_ShaderPongTexAni, e_Damage);
		AddModelChain("Model1", "Export/mob1RunLoc.pod"         , e_ShaderPongTexAni, e_Run);
		AddModelChain("Model1", "Export/mob1WalkLoc.pod"        , e_ShaderPongTexAni, e_Walk);
		AddModelChain("Model1", "Export/mob1DeathLoc.pod"       , e_ShaderPongTexAni, e_Death);
		AddModelChain("Model1", "Export/mob1AttackLoc.pod"      , e_ShaderPongTexAni, e_Attack);
		//-------------------------------------------------------------------------------//
		// 애니 한세트 추가
		//-------------------------------------------------------------------------------//
		CreateBoneAni("Model1", "Export/mob1BoneIdle.pod"       , e_Idle);
		CreateBoneAni("Model1", "Export/mob1BoneDamage.pod"     , e_Damage);
		CreateBoneAni("Model1", "Export/mob1BoneRun.pod"        , e_Run);
		CreateBoneAni("Model1", "Export/mob1BoneWalk.pod"       , e_Walk);
		CreateBoneAni("Model1", "Export/mob1BoneDeath.pod"	    , e_Death);
		CreateBoneAni("Model1", "Export/mob1BoneAttack.pod"     , e_Attack);
		//-------------------------------------------------------------------------------//

		//-------------------------------------------------------------------------------//
		// 모델 한세트 추가
		//-------------------------------------------------------------------------------//
		CreateModel  ("Model2", "Export/mob2Loc.pod"		    , e_ShaderPongTexAni);
		AddModelChain("Model2", "Export/mob2IdleLoc.pod"        , e_ShaderPongTexAni, e_Idle);    
		AddModelChain("Model2", "Export/mob2DamageLoc.pod"      , e_ShaderPongTexAni, e_Damage);
		AddModelChain("Model2", "Export/mob2RunLoc.pod"         , e_ShaderPongTexAni, e_Run);
		AddModelChain("Model2", "Export/mob2WalkLoc.pod"        , e_ShaderPongTexAni, e_Walk);
		AddModelChain("Model2", "Export/mob2DeathLoc.pod"       , e_ShaderPongTexAni, e_Death);
		AddModelChain("Model2", "Export/mob2AttackLoc.pod"      , e_ShaderPongTexAni, e_Attack);
		//-------------------------------------------------------------------------------//
		// 애니 한세트 추가
		//-------------------------------------------------------------------------------//
		CreateBoneAni("Model2", "Export/mob2BoneIdle.pod"       , e_Idle);
		CreateBoneAni("Model2", "Export/mob2BoneDamage.pod"     , e_Damage);
		CreateBoneAni("Model2", "Export/mob2BoneRun.pod"        , e_Run);
		CreateBoneAni("Model2", "Export/mob2BoneWalk.pod"       , e_Walk);
		CreateBoneAni("Model2", "Export/mob2BoneDeath.pod"	    , e_Death);
		CreateBoneAni("Model2", "Export/mob2BoneAttack.pod"     , e_Attack);
		//-------------------------------------------------------------------------------//

		// 만들어진 모델 등록
		ModelRegistration();
	}

	// 특정 모델의 본 그리기
	void DrawBone(string _modelName, string _aniName, int _aniKey)
	{
		cAniManager* mAniManager = cAniManager::GetInstance();
		auto Stroage = mAniManager->mData[_modelName][_aniName]->GetLapStorage(_aniKey);
		for (unsigned int i = 0; i < Stroage.size(); ++i)
		{
			AddModel("BOX1", Stroage[i]);
		}
	}
	
	// 특정 모델의 본 그리기
	void DrawBone(string _modelName, string _aniName, float _x, float _y, float _z, int _aniKey)
	{
		cAniManager* mAniManager = cAniManager::GetInstance();
		auto Stroage = mAniManager->mData[_modelName][_aniName]->GetLapStorage(_aniKey);
		for (unsigned int i = 0; i < Stroage.size(); ++i)
		{
			// 위치 값 조정
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

	// 맵 만들기
	void AddMap(int _key, string _name, float _x, float _y, float _z)
	{
		AddModel(_key, _name, _x , _y, _z);

		// 기본 맵
		getModelData(_key, _name).mAniType = 0;

		// 텍스처 크기 갱신
		getModelData(_key, _name).mTexWidth  = mAllModelData[_name]->mSkinModelTex[0]->mTexWidth;
		getModelData(_key, _name).mTexHeight = mAllModelData[_name]->mSkinModelTex[0]->mTexHeight;

		// 현재 맵 이름
		cMapManager::GetInstance()->SetMapStage(_name);
	}

	// 큐브맵 만들기
	void AddCubeMap(int _key, string _name, float _x, float _y, float _z)
	{
		AddModel(_key, _name, _x, _y, _z);

		// 현재 맵 이름
		cMapManager::GetInstance()->SetCubeMapStage(_name);

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
		
		// 애니메이션 연결
		LinkAni();
	}

	// 애니메이션 연결
	void LinkAni()
	{
		// 모델을 전부 순회한다.
		for (map<string, InitMetaData*>::iterator itor = mAllModelData.begin(); itor != mAllModelData.end(); ++itor)
		{
			// 애니가 있는 모델 데이터라면 
			if (itor->second->mShaderMode == e_ShaderPongTexAni)
			{
				// 현재 선택한 모델의 애니 데이터
				auto tAniData = mAniManager->mData[itor->first];

				// 전체 순회
				for (auto itor2 = tAniData.begin(); itor2 != tAniData.end(); ++itor2)
				{
					// 애니메이션 등록
					int _aniType = (int)itor2->second->mSkinTex.mAniType;
					itor->second->mSkinTex[_aniType] = &itor2->second->mSkinTex;
				}
			}
		}
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

	// 모델 이동하기
	void SetPos(int _uniqueCode, string _Name, XMFLOAT3 _pos)
	{
		string _SlectModel;

		// 모델의 체인만큼 (서브 모델)
		for (unsigned int i = 0; i < mModelChain[_Name].size(); ++i)
		{
			// 체인에 있는 모델을 순차적으로 선택
			_SlectModel = mModelChain[_Name][i];

			// 체인에 있는 모델 모두 이동
			mAllModelData[_SlectModel]->SetPos(_uniqueCode, _pos.x, _pos.y, _pos.z);
			_SlectModel.clear();
		}
	}

	// 모델 이동하기
	void SetPosXZ(int _uniqueCode, string _Name, float _x, float _z)
	{
		string _SlectModel;

		// 모델의 체인만큼 (서브 모델)
		for (unsigned int i = 0; i < mModelChain[_Name].size(); ++i)
		{
			// 체인에 있는 모델을 순차적으로 선택
			_SlectModel = mModelChain[_Name][i];

			// 체인에 있는 모델 모두 이동
			mAllModelData[_SlectModel]->SetPosXZ(_uniqueCode, _x, _z);
			_SlectModel.clear();
		}
	}

	// 모델 이동하기
	void SetPosXZ(int _uniqueCode, string _Name, XMFLOAT3 _pos)
	{
		string _SlectModel;

		// 모델의 체인만큼 (서브 모델)
		for (unsigned int i = 0; i < mModelChain[_Name].size(); ++i)
		{
			// 체인에 있는 모델을 순차적으로 선택
			_SlectModel = mModelChain[_Name][i];

			// 체인에 있는 모델 모두 이동
			mAllModelData[_SlectModel]->SetPosXZ(_uniqueCode, _pos.x, _pos.z);
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

	// 모델 회전하기
	void SetRotate(int _uniqueCode, string _Name, XMFLOAT3 _pos)
	{
		string _SlectModel;

		// 모델의 체인만큼 (서브 모델)
		for (unsigned int i = 0; i < mModelChain[_Name].size(); ++i)
		{
			// 체인에 있는 모델을 순차적으로 선택
			_SlectModel = mModelChain[_Name][i];

			// 체인에 있는 모델 모두 회전
			mAllModelData[_SlectModel]->SetRotate(_uniqueCode, _pos.x, _pos.y, _pos.z);
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

	XMFLOAT3 GetPointDir(int _uniqueCode, string _Name, XMFLOAT3 _pos)
	{
		// 체인에 있는 모델 하나만 선택해서 방향 보냄
		XMFLOAT3 ptDir;
		XMStoreFloat3(&ptDir, mAllModelData[mModelChain[_Name][0]]->GetPointDir(_uniqueCode, _pos.x, _pos.y, _pos.z));

		return ptDir;
	}

	// 해당 위치로 이동
	void MovePoint(int _uniqueCode, string _Name, float _x, float _y, float _z, float _speed)
	{
		// 체인에 있는 모델 하나만 선택해서 방향 보냄
		XMFLOAT3 ptDir;
		XMStoreFloat3(&ptDir, mAllModelData[mModelChain[_Name][0]]->GetPointDir(_uniqueCode, _x, _y, _z));

		XMFLOAT3 tPos = GetPos(_uniqueCode, _Name);

		// 거리 계산
		XMFLOAT3 _lengthPos;
		_lengthPos.x = tPos.x - _x;
		_lengthPos.y = tPos.y - _y;
		_lengthPos.z = tPos.z - _z;

		XMVECTOR _length = XMVector3Length(XMLoadFloat3(&_lengthPos));
		XMStoreFloat3(&_lengthPos, _length);

		// 해당 거리보다 커야 실행
		if (_lengthPos.x > 20)
		{
			tPos.x = tPos.x + ptDir.x * _speed;
			tPos.y = tPos.y + ptDir.y * _speed;
			tPos.z = tPos.z + ptDir.z * _speed;

			SetRotate(_uniqueCode, _Name, tPos);
			SetPos   (_uniqueCode, _Name, tPos);
		}
	}

	// 해당 위치로 이동
	void MovePoint(int _uniqueCode, string _Name, XMFLOAT3 _pos, float _speed)
	{
		// 체인에 있는 모델 하나만 선택해서 방향 보냄
		XMFLOAT3 ptDir;
		XMStoreFloat3(&ptDir, mAllModelData[mModelChain[_Name][0]]->GetPointDir(_uniqueCode, _pos.x, _pos.y, _pos.z));

		XMFLOAT3 tPos = GetPos(_uniqueCode, _Name);

		// 거리 계산
		XMFLOAT3 _lengthPos;
		_lengthPos.x = tPos.x - _pos.x;
		_lengthPos.y = tPos.y - _pos.y;
		_lengthPos.z = tPos.z - _pos.z;

		XMVECTOR _length = XMVector3Length(XMLoadFloat3(&_lengthPos));
		XMStoreFloat3(&_lengthPos, _length);

		// 해당 거리보다 커야 실행
		if (_lengthPos.x > 20)
		{
			tPos.x = tPos.x + ptDir.x * _speed;
			tPos.y = tPos.y + ptDir.y * _speed;
			tPos.z = tPos.z + ptDir.z * _speed;

			SetRotate(_uniqueCode, _Name, tPos);
			SetPos   (_uniqueCode, _Name, tPos);
		}
	}

	// 해당 위치로 이동
	void MoveToPlayer(int _uniqueCode, string _Name, float _speed)
	{
		// 체인에 있는 모델 하나만 선택해서 방향 보냄
		XMFLOAT3 ptDir;
		XMStoreFloat3(&ptDir, mAllModelData[mModelChain[_Name][0]]->GetPointDir(_uniqueCode, mPlayer.mPos));

		XMFLOAT3 tPos = GetPos(_uniqueCode, _Name);

		// 거리 계산
		XMFLOAT3 _lengthPos;
		_lengthPos.x = tPos.x - mPlayer.mPos.x;
		_lengthPos.y = tPos.y - mPlayer.mPos.y;
		_lengthPos.z = tPos.z - mPlayer.mPos.z;

		XMVECTOR _length = XMVector3Length(XMLoadFloat3(&_lengthPos));
		XMStoreFloat3(&_lengthPos, _length);

		// 해당 거리보다 커야 실행
		if (_lengthPos.x > 20)
		{
			tPos.x = tPos.x + ptDir.x * _speed;
			tPos.y = tPos.y + ptDir.y * _speed;
			tPos.z = tPos.z + ptDir.z * _speed;

			SetRotate(_uniqueCode, _Name, tPos);
			SetPos   (_uniqueCode, _Name, tPos);
		}
	}

	// 모델 위치 값
	XMFLOAT3 GetPos(int _uniqueCode, string _Name)
	{
		return mAllModelData[mModelChain[_Name][0]]->getPos(_uniqueCode);
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

	// 모델 데이터 가져오기
	ObjData& getModelData(int _uniqueCode, string _Name)
	{
		string _SlectModel;
		_SlectModel = mModelChain[_Name][0];
		return 	mAllModelData[_SlectModel]->getObj(_uniqueCode);
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
			map<int, ObjData> t_ObjDataStorage = mAllModelData[_SlectModel]->getAllObj();
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
		// 첫번째 스테이트에선 초기화 하지 않겠음
		static bool tStartPass = true;

		if (!tStartPass)
		{
			// 모델 데이터 삭제
			for (map<string, InitMetaData*>::iterator itor = mAllModelData.begin(); itor != mAllModelData.end(); ++itor)
			{
				// 모델 내부 객체 삭제
				itor->second->ClearWdMtx();
			}

			// 모델 인스턴스 버퍼 삭제
			ClearModelAndBuf();

			// 모델에 사용되었던 쉐이더 제거
			mUseAllShader.clear();
			mUseShader.clear();

			// 모델 이름 제거
			mUseModel.clear();

			// 스크린 클리어
			mScreen->ClearWdMtx();
			mScreenBuffer->ClearInsBuf();
		}
		else
		{
			tStartPass = false;
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
			mXMLParser.LoadXMLModel(tSelectModelRoute, mAllModelData[tNewName]);

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

	// 모델 체인 연결하기 (여러 애니메이션 처리를 위해)
	void AddModelChain(string _Name, string _ModelRoute, SHADER_TYPE _ShaderMode, FSM_TYPE _fsmType, D3D_PRIMITIVE_TOPOLOGY _D3D_PRIMITIVE_TOPOLOGY = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
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
			
			// 임시 데이터
			cout << " >> 스킨 모델 파싱 시작 <<" << endl;
			InitMetaData _ParsigModel(_ShaderMode, _D3D_PRIMITIVE_TOPOLOGY);

			//----------------------------------------------------//
			// 스킨 모델 데이터 파싱
			//----------------------------------------------------//
			mXMLParser.LoadXMLSkinModel(tSelectModelRoute, &_ParsigModel, mAllModelData[tNewName], _fsmType);

			// 포인터 삭제
			mXMLParser.ClearPointer();

			// string 삭제
			tSelectModelRoute.clear();
			tNewName.clear();
		}
		cout << " >> 스킨 모델 파싱 종료 <<" << endl;

		// 파서 버퍼 초기화 ( 다음 모델을 받기 위해 ) 
		mXMLParser.ClearClass();
		_Name.clear();
	}

	// 모델 생성하기
	void CreateBoneAni(string _Name, string _BoneRoute, FSM_TYPE _fsmType, D3D_PRIMITIVE_TOPOLOGY _D3D_PRIMITIVE_TOPOLOGY = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
	{
		//----------------------------------------------------//
		// 본 데이터 파싱
		//----------------------------------------------------//
		// 본은 데이터당 하나씩이기에 이런식으로 저장해도 됨.
		MyBoneData* mMyBoneData = new MyBoneData();

		// 1개 모델의 본 파싱
		mXMLParser.ReadDataMyFormat_Bone(_BoneRoute, mMyBoneData);

		// 텍스처 이름
		string _TexName = mAllModelData[_Name]->mObjName;
		_TexName += mMyBoneData->mAniName;

		// 데이터 계산
		mMyBoneData->CalData(_TexName, _fsmType);

		// 체인에 연결된 모델에 저장
		for (unsigned int i = 0; i < mModelChain[_Name].size(); ++i)
		{
			string SelectName = mModelChain[_Name][i];
			mAllModelData[SelectName]->mAniModel = true;
			mAniManager->mData[SelectName][mMyBoneData->mAniName] = mMyBoneData;
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
		_nowModel->mModelType  = e_BasicModel;
		_nowModel->mCreateName = _Name;
		strcpy(_nowModel->mObjName, _Name.c_str());

		// 파싱 시작
		mXMLParser.LoadBox(_nowModel, 10.0f, 10.0f, 10.0f);

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
		_nowModel->mModelType  = e_BasicModel;
		_nowModel->mCreateName = _Name;
		strcpy(_nowModel->mObjName, _Name.c_str());

		// 파싱 시작
		mXMLParser.LoadBox(_nowModel, _Size, _Size, _Size);

		// 쉐이더, 모델 등록 (필터링 용)
		UseAllShaderModel(_ShaderMode);
		UsingModel(_Name);
	}

	// 맵 생성
	void CreateMap(string _Name, string _FileName, float _Xwidth, float _Zdepth, float _CellSize, float _HeightScale, SHADER_TYPE _ShaderMode, D3D_PRIMITIVE_TOPOLOGY _D3D_PRIMITIVE_TOPOLOGY = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
	{
		// 모델 체인에 이름 등록
		mModelChain[_Name].push_back(_Name);

		// 모델 생성
		mAllModelData[_Name] = new InitMetaData(_Name.c_str(), _ShaderMode, _D3D_PRIMITIVE_TOPOLOGY);

		// 현재 모델 정보 얻기
		InitMetaData* _nowModel = mAllModelData[_Name];

		// 모델의 종류
		_nowModel->mModelType  = e_ParsingModel;
		_nowModel->mCreateName = _Name;
		strcpy(_nowModel->mObjName, _Name.c_str());

		// 파싱 시작 
		mXMLParser.LoadMap(_nowModel, _FileName, _Xwidth, _Zdepth, _CellSize, _HeightScale);

		// 변수 계산
		_nowModel->CalValueMap();

		// 쉐이더, 모델 등록 (필터링 용)
		UseAllShaderModel(_ShaderMode);
		UsingModel(_Name);

		// 맵 매니저 초기화
		cMapManager::GetInstance()->mData[_Name].mModel       = mAllModelData[_Name];
		cMapManager::GetInstance()->mData[_Name].mCellSize    = _CellSize;
		cMapManager::GetInstance()->mData[_Name].mHeightScale = _HeightScale;
		cMapManager::GetInstance()->mData[_Name].mXwidth      = _Xwidth;
		cMapManager::GetInstance()->mData[_Name].mZdepth      = _Zdepth;
	}

	// 맵 생성
	void CreateSkyBox(string _Name, string _FileName, float _radius, UINT _sliceCount, UINT _stackCount, SHADER_TYPE _ShaderMode, D3D_PRIMITIVE_TOPOLOGY _D3D_PRIMITIVE_TOPOLOGY = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
	{
		// 모델 체인에 이름 등록
		mModelChain[_Name].push_back(_Name);

		// 모델 생성
		mAllModelData[_Name] = new InitMetaData(_Name.c_str(), _ShaderMode, _D3D_PRIMITIVE_TOPOLOGY);

		// 현재 모델 정보 얻기
		InitMetaData* _nowModel = mAllModelData[_Name];

		// 모델의 종류
		_nowModel->mModelType  = e_ParsingModel;
		_nowModel->mCreateName = _Name;
		strcpy(_nowModel->mObjName, _Name.c_str());

		// 파싱 시작
		mXMLParser.LoadSphere(_nowModel, _radius, _sliceCount, _stackCount);

		// 쉐이더, 모델 등록 (필터링 용)
		UseAllShaderModel(_ShaderMode);
		UsingModel(_Name);

		// 계산
		_nowModel->CalValue();

		// 큐브맵 리소스 뷰 생성하기
		mMapManager->CreateRSV(_Name, _FileName);
	}

	// 스크린 모델 생성하기
	void CreateScreenModel(SHADER_TYPE _ShaderMode = e_ShaderDeferred, D3D_PRIMITIVE_TOPOLOGY _D3D_PRIMITIVE_TOPOLOGY = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
	{
		// 현재 모델 정보 얻기
		mScreen = new InitMetaData("GSreen", _ShaderMode, _D3D_PRIMITIVE_TOPOLOGY);

		// 모델의 종류
		mScreen->mModelType  = e_BasicModel;
		mScreen->mCreateName = "GSreen";
		strcpy(mScreen->mObjName, mScreen->mCreateName.c_str());

		// 파싱 시작 (풀 스크린 쿼드 만들기)
		mXMLParser.LoadScreen(mScreen, 1.0f, 1.0f); // 풀 스크린쿼드

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
		// 모델 파일에 있는 서브 모델 수 만큼
		for (unsigned int i = 0; i < mModelChain[_Name].size(); ++i)
		{
			// 서브 모델 추가
			addSubModel(mModelChain[_Name][i], _x, _y, _z, _moveAble);

			// 다시 만들기
			MakeModelInsBuf(mModelChain[_Name][i]);
		}
		_Name.clear();
	}	

	// 모델 추가하기
	void AddUpdateModel(string _Name, XMFLOAT4X4 _Mtx, OBJ_MOVEABLE _moveAble = e_StaticObj)
	{
		// 모델 파일에 있는 서브 모델 수 만큼
		for (unsigned int i = 0; i < mModelChain[_Name].size(); ++i)
		{
			// 서브 모델 추가
			addSubModel(mModelChain[_Name][i], _Mtx, _moveAble);

			// 다시 만들기
			MakeModelInsBuf(mModelChain[_Name][i]);
		}
		_Name.clear();
	}

	// 모델 추가하기
	void AddUpdateModel(int _key, string _Name, float _x, float _y, float _z, OBJ_MOVEABLE _moveAble = e_StaticObj)
	{
		// 모델 파일에 있는 서브 모델 수 만큼
		for (unsigned int i = 0; i < mModelChain[_Name].size(); ++i)
		{
			// 서브 모델 추가
			addSubModel(_key, mModelChain[_Name][i], _x, _y, _z, _moveAble);

			// 다시 만들기
			MakeModelInsBuf(mModelChain[_Name][i]);
		}
		_Name.clear();
	}

	// 모델 추가하기
	void AddUpdateModel(int _key, string _Name, XMFLOAT4X4 _Mtx, OBJ_MOVEABLE _moveAble = e_StaticObj)
	{
		// 모델 파일에 있는 서브 모델 수 만큼
		for (unsigned int i = 0; i < mModelChain[_Name].size(); ++i)
		{
			// 서브 모델 추가
			addSubModel(_key, mModelChain[_Name][i], _Mtx, _moveAble);

			// 다시 만들기
			MakeModelInsBuf(mModelChain[_Name][i]);
		}

		_Name.clear();
	}

	// 모델 추가하기 (애니 FSM)
	void AddUpdateModel(int _key, string _Name, float _x, float _y, float _z, FSM_TYPE _fsmType, OBJ_MOVEABLE _moveAble = e_StaticObj)
	{
		// 모델 파일에 있는 서브 모델 수 만큼
		for (unsigned int i = 0; i < mModelChain[_Name].size(); ++i)
		{
			// 서브 모델 추가
			addSubModel(_key, mModelChain[_Name][i], _x, _y, _z, _fsmType, _moveAble);

			// 일반 모델 인스턴스 버퍼 생성
			MakeModelInsBuf(mModelChain[_Name][i]);
		}
		_Name.clear();
	}

	// 모델 추가하기 (애니 FSM)
	void AddUpdateModel(int _key, string _Name, XMFLOAT4X4 _Mtx, FSM_TYPE _fsmType, OBJ_MOVEABLE _moveAble = e_StaticObj)
	{
		// 모델 파일에 있는 서브 모델 수 만큼
		for (unsigned int i = 0; i < mModelChain[_Name].size(); ++i)
		{
			// 서브 모델 추가
			addSubModel(_key, mModelChain[_Name][i], _Mtx, _fsmType, _moveAble);

			// 다시 만들기
			MakeModelInsBuf(mModelChain[_Name][i]);
		}
		_Name.clear();
	}

	// 모델 추가하기 (애니 FSM)
	void AddUpdateModel(int _key, string _Name, float _x, float _y, float _z, FSM_TYPE _fsmType, float _frame, OBJ_MOVEABLE _moveAble = e_StaticObj)
	{
		// 모델 파일에 있는 서브 모델 수 만큼
		for (unsigned int i = 0; i < mModelChain[_Name].size(); ++i)
		{
			// 서브 모델 추가
			addSubModel(_key, mModelChain[_Name][i], _x, _y, _z, _fsmType, _frame, _moveAble);

			// 다시 만들기
			MakeModelInsBuf(mModelChain[_Name][i]);
		}
		_Name.clear();
	}

	// 모델 추가하기 (애니 FSM)
	void AddUpdateModel(int _key, string _Name, XMFLOAT4X4 _Mtx, FSM_TYPE _fsmType, float _frame, OBJ_MOVEABLE _moveAble = e_StaticObj)
	{
		// 모델 파일에 있는 서브 모델 수 만큼
		for (unsigned int i = 0; i < mModelChain[_Name].size(); ++i)
		{
			// 서브 모델 추가
			addSubModel(_key, mModelChain[_Name][i], _Mtx, _fsmType, _frame, _moveAble);

			// 다시 만들기
			MakeModelInsBuf(mModelChain[_Name][i]);
		}
		_Name.clear();
	}


	// 모델 제거하기
	void EraseUpdateModel(int _key, string _Name)
	{
		// 모델 파일에 있는 서브 모델 수 만큼
		for (unsigned int i = 0; i < mModelChain[_Name].size(); ++i)
		{
			// 서브 모델 삭제
			EraseSubModel(_key, mModelChain[_Name][i]);

			// 다시 만들기
			MakeModelInsBuf(mModelChain[_Name][i]);
		}
		_Name.clear();
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

	// 모델 추가하기
	void AddModel(int _key, string _Name, float _x, float _y, float _z, OBJ_MOVEABLE _moveAble = e_StaticObj)
	{
		// 모델 파일에 있는 서브 모델 수 만큼
		for (unsigned int i = 0; i < mModelChain[_Name].size(); ++i)
		{
			// 서브 모델 추가
			addSubModel(_key, mModelChain[_Name][i], _x, _y, _z, _moveAble);
		}
		_Name.clear();
	}

	// 모델 추가하기
	void AddModel(int _key, string _Name, XMFLOAT4X4 _Mtx, OBJ_MOVEABLE _moveAble = e_StaticObj)
	{
		// 모델 파일에 있는 서브 모델 수 만큼
		for (unsigned int i = 0; i < mModelChain[_Name].size(); ++i)
		{
			// 서브 모델 추가
			addSubModel(_key, mModelChain[_Name][i], _Mtx, _moveAble);
		}
		_Name.clear();
	}

	// 모델 추가하기 (모델 FSM)
	void AddModel(int _key, string _Name, float _x, float _y, float _z, FSM_TYPE _fsmType, OBJ_MOVEABLE _moveAble = e_StaticObj)
	{
		// 모델 파일에 있는 서브 모델 수 만큼
		for (unsigned int i = 0; i < mModelChain[_Name].size(); ++i)
		{
			// 서브 모델 추가
			addSubModel(_key, mModelChain[_Name][i], _x, _y, _z, _fsmType, _moveAble);
		}
		_Name.clear();
	}

	// 모델 추가하기 (모델 FSM)
	void AddModel(int _key, string _Name, XMFLOAT4X4 _Mtx, FSM_TYPE _fsmType, OBJ_MOVEABLE _moveAble = e_StaticObj)
	{
		// 모델 파일에 있는 서브 모델 수 만큼
		for (unsigned int i = 0; i < mModelChain[_Name].size(); ++i)
		{
			// 서브 모델 추가
			addSubModel(_key, mModelChain[_Name][i], _Mtx, _fsmType, _moveAble);
		}
		_Name.clear();
	}

	// 모델 추가하기 (모델 FSM)
	void AddModel(int _key, string _Name, float _x, float _y, float _z, FSM_TYPE _fsmType, float _frame, OBJ_MOVEABLE _moveAble = e_StaticObj)
	{
		// 모델 파일에 있는 서브 모델 수 만큼
		for (unsigned int i = 0; i < mModelChain[_Name].size(); ++i)
		{
			// 서브 모델 추가
			addSubModel(_key, mModelChain[_Name][i], _x, _y, _z, _fsmType, _frame, _moveAble);
		}
		_Name.clear();
	}

	// 모델 추가하기 (모델 FSM)
	void AddModel(int _key, string _Name, XMFLOAT4X4 _Mtx, FSM_TYPE _fsmType, float _frame, OBJ_MOVEABLE _moveAble = e_StaticObj)
	{
		// 모델 파일에 있는 서브 모델 수 만큼
		for (unsigned int i = 0; i < mModelChain[_Name].size(); ++i)
		{
			// 서브 모델 추가
			addSubModel(_key, mModelChain[_Name][i], _Mtx, _fsmType, _frame, _moveAble);
		}
		_Name.clear();
	}

	// 모델 추가하기
	void AddScreen(float _x, float _y, float _z)
	{
		mScreen->AddModel(_x, _y, _z, e_StaticObj);
	}

	// HP 설정
	void SetHP(int _key, string _Name, float _hp)
	{
		// 모델 파일에 있는 서브 모델 수 만큼
		for (unsigned int i = 0; i < mModelChain[_Name].size(); ++i)
		{
			string _SlectModel = mModelChain[_Name][i];
			mAllModelData[_SlectModel]->SetHp(_key, _hp);
		}
	}

	// MP 설정
	void SetMP(int _key, string _Name, float _mp)
	{
		// 모델 파일에 있는 서브 모델 수 만큼
		for (unsigned int i = 0; i < mModelChain[_Name].size(); ++i)
		{
			string _SlectModel = mModelChain[_Name][i];
			mAllModelData[_SlectModel]->SetMp(_key, _mp);
		}
	}

	// 점프
	void Jump(int _key, string _Name)
	{
		// 모델 파일에 있는 서브 모델 수 만큼
		for (unsigned int i = 0; i < mModelChain[_Name].size(); ++i)
		{
			string _SlectModel = mModelChain[_Name][i];
			mAllModelData[_SlectModel]->Jump(_key);
		}
	}

	// 플레이어 전진, 후진
	XMFLOAT3 PlayerWalk(float _speed)
	{
		XMFLOAT3 _PlayerLookDir = PlayerLookDir();
		gCam.Walk(_speed);

		mPlayer.mPos = gCam.GetThirdPosition();
		SetRotate(mPlayer.mkey, mPlayer.mModelName, mPlayer.mPos);
		SetPosXZ (mPlayer.mkey, mPlayer.mModelName, mPlayer.mPos);

		// y좌표 업데이트
		float tYpos = mMapManager->GetHeightMap(mPlayer.mPos.x, mPlayer.mPos.z);
		gCam.SetCamY(tYpos);

		return mPlayer.mPos;
	}

	// 플레이어 방향벡터
	XMFLOAT3 PlayerLookDir()
	{
		return getModelData(mPlayer.mkey, mPlayer.mModelName).getLookDir();
	}
	

	// 플레이어 좌진, 우진
	XMFLOAT3 PlayerStrafe(float _speed)
	{
		gCam.Strafe(_speed);

		mPlayer.mPos = gCam.GetThirdPosition();
		SetRotate(mPlayer.mkey, mPlayer.mModelName, mPlayer.mPos);
		SetPosXZ (mPlayer.mkey, mPlayer.mModelName, mPlayer.mPos);

		// y좌표 업데이트
		float tYpos = mMapManager->GetHeightMap(mPlayer.mPos.x, mPlayer.mPos.z);
		gCam.SetCamY(tYpos);

		return mPlayer.mPos;
	}



	// 플레이어 셋팅
	void IniPlayer(int _key, string _model, float _x, float _y, float _z)
	{
		gCam.Change3PersonCam();
		gCam.initCam(_x, _y, _z);
		AddUpdateModel(_key, _model, _x, _y, _z);
		
		mPlayer.mkey       = _key;
		mPlayer.mModelName = _model;
		mPlayer.mPos       = gCam.GetThirdPosition();

		SetRotate(mPlayer.mkey, mPlayer.mModelName, mPlayer.mPos);
		SetPos   (mPlayer.mkey, mPlayer.mModelName, mPlayer.mPos);
	}

	// 플레이어 셋팅
	void IniPlayer(int _key, string _model, float _x, float _y, float _z, FSM_TYPE _fsmType)
	{
		gCam.Change3PersonCam();
		gCam.initCam(_x, _y, _z);
		AddUpdateModel(_key, _model, _x, _y, _z, _fsmType);
		
		mPlayer.mkey       = _key;
		mPlayer.mModelName = _model;
		mPlayer.mPos       = gCam.GetThirdPosition();

		SetRotate(mPlayer.mkey, mPlayer.mModelName, mPlayer.mPos);
		SetPos   (mPlayer.mkey, mPlayer.mModelName, mPlayer.mPos);
	}

	// 플레이어 셋팅
	void IniPlayer(int _key, string _model, float _x, float _y, float _z, FSM_TYPE _fsmType, float _frame)
	{
		gCam.Change3PersonCam();
		gCam.initCam(_x, _y, _z);
		AddUpdateModel(_key, _model, _x, _y, _z, _fsmType, _frame);
		
		mPlayer.mkey       = _key;
		mPlayer.mModelName = _model;
		mPlayer.mPos       = gCam.GetThirdPosition();

		SetRotate(mPlayer.mkey, mPlayer.mModelName, mPlayer.mPos);
		SetPos   (mPlayer.mkey, mPlayer.mModelName, mPlayer.mPos);
	}

	// 인스턴스 버퍼 만들기
	void MakeInsbuf()
	{
		// 버퍼에도 데이터 복사
		DataMove();

		// 일반 모델 인스턴스 버퍼 생성
		MakeModelInsBuf();

		// 스크린 인스턴스 버퍼 생성
		mScreenBuffer->MakeScreenInsBuf(mScreen);
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
			// 애니메이션 업데이트
			UpdateAni(itor->second, dt);
		}
	}

	// 애니메이션 시간 업데이트
	void UpdateAni(InitMetaData* _itor, float& _dt)
	{
		// 오브젝트가 있고, 애니메이션 모델이라면
		if (_itor->mObjData.size() && _itor->mShaderMode == e_ShaderPongTexAni)
		{
			for (auto itor = _itor->mObjData.begin(); itor != _itor->mObjData.end(); ++itor)
			{
				// y좌표 업데이트
				XMFLOAT3 mPos = itor->second.getPos();
				float tYpos = mMapManager->GetHeightMap(mPos.x, mPos.z);

				// 애니 업데이트
				itor->second.Update(_dt, tYpos);
			}
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

	// FSM 변경
	void SetFSM(int _unicode, string _Name, FSM_TYPE _modelFsm)
	{
		// 모델 파일에 있는 서브 모델 수 만큼
		for (unsigned int i = 0; i < mModelChain[_Name].size(); ++i)
		{
			// 체인에 있는 모델 모두 FSM 변화
			string _SlectModel = mModelChain[_Name][i];

			// 애니가 있는 모델이라면,
			if (mAllModelData[_SlectModel]->mShaderMode == e_ShaderPongTexAni)
			{
				mAllModelData[_SlectModel]->SetFSM(_unicode, _modelFsm);
			}
		}
	}

	// FSM 변경
	void SetFSM(int _unicode, string _Name, FSM_TYPE _modelFsm, float _Frame)
	{
		// 모델 파일에 있는 서브 모델 수 만큼
		for (unsigned int i = 0; i < mModelChain[_Name].size(); ++i)
		{
			// 체인에 있는 모델 모두 FSM 변화
			string _SlectModel = mModelChain[_Name][i];

			// 애니가 있는 모델이라면,
			if (mAllModelData[_SlectModel]->mShaderMode == e_ShaderPongTexAni)
			{
				mAllModelData[_SlectModel]->SetFSM(_unicode, _modelFsm, _Frame);
			}
		}
	}

	// FSM 변경
	void SetPlayerFSM(FSM_TYPE _modelFsm)
	{
		// 모델 파일에 있는 서브 모델 수 만큼
		for (unsigned int i = 0; i < mModelChain[mPlayer.mModelName].size(); ++i)
		{
			// 체인에 있는 모델 모두 FSM 변화
			string _SlectModel = mModelChain[mPlayer.mModelName][i];

			// 애니가 있는 모델이라면,
			if (mAllModelData[_SlectModel]->mShaderMode == e_ShaderPongTexAni)
			{
				mAllModelData[_SlectModel]->SetFSM(mPlayer.mkey, _modelFsm);
			}
		}
	}

	// FSM 변경
	void SetPlayerFSM(FSM_TYPE _modelFsm, float _Frame)
	{
		// 모델 파일에 있는 서브 모델 수 만큼
		for (unsigned int i = 0; i < mModelChain[mPlayer.mModelName].size(); ++i)
		{
			// 체인에 있는 모델 모두 FSM 변화
			string _SlectModel = mModelChain[mPlayer.mModelName][i];

			// 애니가 있는 모델이라면,
			if (mAllModelData[_SlectModel]->mShaderMode == e_ShaderPongTexAni)
			{
				mAllModelData[_SlectModel]->SetFSM(mPlayer.mkey, _modelFsm, _Frame);
			}
		}
	}


private:
	// 플레이어 와이값 가져오기
	float GetPlayerY()
	{
		return getModelData(mPlayer.mkey, mPlayer.mModelName).getPos().y;
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

	// 서브 모델들 모두 추가
	void addSubModel(int _key, string& _SlectModel, float& _x, float& _y, float& _z, OBJ_MOVEABLE& _moveAble)
	{
		// 체인에 있는 모델 모두 월드매트릭스에 추가
		mAllModelData[_SlectModel]->AddModel(_key, _x, _y, _z, _moveAble);

		// 쉐이더 추가
		addUseShader(_SlectModel);
	}

	// 서브 모델들 모두 추가
	void addSubModel(int _key, string& _SlectModel, XMFLOAT4X4 _Mtx, OBJ_MOVEABLE& _moveAble)
	{
		// 체인에 있는 모델 모두 월드매트릭스에 추가
		mAllModelData[_SlectModel]->AddModel(_key, _Mtx, _moveAble);

		// 쉐이더 추가
		addUseShader(_SlectModel);
	}

	// 서브 모델들 모두 추가 (애니 FSM)
	void addSubModel(int _key, string& _SlectModel, float& _x, float& _y, float& _z, FSM_TYPE _fsmType, OBJ_MOVEABLE& _moveAble)
	{
		// 체인에 있는 모델 모두 월드매트릭스에 추가
		mAllModelData[_SlectModel]->AddModel(_key, _x, _y, _z, _moveAble);

		// 애니 모델
		mAllModelData[_SlectModel]->SetFSM(_key, _fsmType);

		// 쉐이더 추가
		addUseShader(_SlectModel);
	}

	// 서브 모델들 모두 추가 (애니 FSM)
	void addSubModel(int _key, string& _SlectModel, XMFLOAT4X4 _Mtx, FSM_TYPE _fsmType, OBJ_MOVEABLE& _moveAble)
	{
		// 체인에 있는 모델 모두 월드매트릭스에 추가
		mAllModelData[_SlectModel]->AddModel(_key, _Mtx, _moveAble);

		// 애니 모델
		mAllModelData[_SlectModel]->SetFSM(_key, _fsmType);

		// 쉐이더 추가
		addUseShader(_SlectModel);
	}

	// 서브 모델들 모두 추가 (애니 FSM)
	void addSubModel(int _key, string& _SlectModel, float& _x, float& _y, float& _z, FSM_TYPE _fsmType, float _frame, OBJ_MOVEABLE& _moveAble)
	{
		// 체인에 있는 모델 모두 월드매트릭스에 추가
		mAllModelData[_SlectModel]->AddModel(_key, _x, _y, _z, _moveAble);

		// 애니 모델
		mAllModelData[_SlectModel]->SetFSM(_key, _fsmType, _frame);

		// 쉐이더 추가
		addUseShader(_SlectModel);
	}

	// 서브 모델들 모두 추가 (애니 FSM)
	void addSubModel(int _key, string& _SlectModel, XMFLOAT4X4 _Mtx, FSM_TYPE _fsmType, float _frame, OBJ_MOVEABLE& _moveAble)
	{
		// 체인에 있는 모델 모두 월드매트릭스에 추가
		mAllModelData[_SlectModel]->AddModel(_key, _Mtx, _moveAble);

		// 애니 모델
		mAllModelData[_SlectModel]->SetFSM(_key, _fsmType, _frame);

		// 쉐이더 추가
		addUseShader(_SlectModel);
	}


	// 서브 모델들 삭제
	void EraseSubModel(int _key, string& _SlectModel)
	{
		// 체인에 있는 모델 모두 월드매트릭스에 추가
		mAllModelData[_SlectModel]->EraseModel(_key);
	}


	// 사용한 쉐이더 추가
	void addUseShader(string& _SlectModel)
	{
		SHADER_TYPE _UseShader;
		_UseShader = mAllModelData[_SlectModel]->mShaderMode;
		UsingShaderModel(_UseShader);
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

	// 버퍼 클리어
	void ClearModelAndBuf()
	{
		for (auto itor = mBufferType.begin(); itor != mBufferType.end(); ++itor)
		{
			itor->second->initValue();
		}
	}

	// 버퍼 클리어
	void ClearBuf()
	{
		for (auto itor = mBufferType.begin(); itor != mBufferType.end(); ++itor)
		{
			itor->second->ClearInsBuf();
		}
	}

	// 모델 인스턴스 버퍼만 삭제
	void ClearModelInsBuf()
	{
		// 모델 인스턴스 버퍼 삭제
		for (unsigned int i = 0; i < mUseShader.size(); ++i)
		{
			// 버퍼 빌드
			mBufferType[mUseShader[i]]->ClearInsBuf();
		}
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

	// 모델 인스턴스 버퍼 만들기
	void MakeModelInsBuf(string& _name)
	{
		// 일반 모델 인스턴스 버퍼 생성
		SHADER_TYPE _sdType = mAllModelData[_name]->mShaderMode;
		mBufferType[_sdType]->ReMakeBuf(_name);
	}

	// 데이터 옮기기
	void DataMove()
	{
		for (unsigned int i = 0; i < mUseShader.size(); ++i)
		{
			// 버퍼에 모델 등록
			AddModelInBuffer(mUseShader[i]);
		}
	}

	// 버퍼에 모델 추가하기
	void AddModelInBuffer(SHADER_TYPE _ShaderMode)
	{
		for (map<string, InitMetaData*>::iterator itor = mAllModelData.begin(); itor != mAllModelData.end(); ++itor)
		{
			// 쉐이더 모드가 같다면, 추가한다.
			if (_ShaderMode == itor->second->mShaderMode)
			{
				mBufferType[_ShaderMode]->AddModel(itor->second->mCreateName, itor->second);
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
};

