#pragma once
#include "cXMLparser.h"

extern cCam gCam;

class cModelManager : public cSingleton<cModelManager>
{
public:
	map<int   , BufferType*>    mBufferType;   // ���� ���� �������� ������ ���� ( PNT, PL ��.. )
	map<string, InitMetaData*>  mAllModelData; // ��� ���� ������
	map<string, vector<string>> mModelChain;   // �𵨵��� �߰��Ҷ� �� �� ü��(���� �𵨶����� �ε����ϰ� ������������ -_-;;)

	vector<SHADER_TYPE> mUseAllShader;		   // ��ü �𵨿� ����� ���̴�
	vector<SHADER_TYPE> mUseShader;			   // ����� �𵨿� ����� ���̴�
	vector<string>		mUseModel;			   // ������� ��

	InitMetaData* mScreen       = nullptr;	   // G���� ��ũ��
	BufferType  * mScreenBuffer = nullptr;     // G���� ��ũ��
	XMFLOAT4X4    mScreenMtx;                  // G���� ��ũ��

	// �ļ�
	cXMLParser mXMLParser;
	
	// �ִ� �Ŵ���
	cAniManager* mAniManager = cAniManager::GetInstance();

	// �� �Ŵ���
	cMapManager* mMapManager = cMapManager::GetInstance();

	// �÷��̾�
	PlayerInfo mPlayer;
	
	bool _modelUpdate = false;
public:
	void Init()
	{
		// ��ũ�� �����
		CreateScreen();

		// �� ��� (�ٸ� ������ �𵨸� 1����)
		CreateBoxModel("BOX1", e_ShaderColor, 0.7f, D3D_PRIMITIVE_TOPOLOGY_LINESTRIP);
		
		CreateBoxModel("BOX2", e_ShaderLight);
		
		CreateBoxModel("BOX3", e_ShaderPongTex);
		AddTex        ("BOX3", "Export/WoodCrate01_diff.dds", e_DiffuseMap);
		AddTex        ("BOX3", "Export/WoodCrate01_norm.dds", e_NomalMap);
		AddTex        ("BOX3", "Export/WoodCrate01_spec.dds", e_SpecularMap);

		// �� �߰�
		CreateMap     ("Map1", "Export/HeightMap/Map1.raw"  , 100.0f, 100.0f, 50.0f, 1000.0f, e_ShaderPongTexMap);
		AddTex        ("Map1", "Export/ground_diff.dds"     , e_DiffuseMap);
		AddTex        ("Map1", "Export/ground_norm.dds"     , e_NomalMap);
		AddTex        ("Map1", "Export/ground_spec.dds"     , e_SpecularMap);

		// ť��� �߰�
		CreateSkyBox("CubeMap1", "Export/CubeMap/snowcube1024.dds", 0.5f, 20, 20, e_ShaderSkyBox);


		//-------------------------------------------------------------------------------//
		//
		//  �ڡ� ���� �𵨸�� ����̸� �ݵ�� �ٲ��ٰ� �ڡ�
		//
		//-------------------------------------------------------------------------------//
		// �� �Ѽ�Ʈ �߰�
		//-------------------------------------------------------------------------------//
		CreateModel  ("Model1", "Export/mob1Loc.pod"		    , e_ShaderPongTexAni);
		AddModelChain("Model1", "Export/mob1IdleLoc.pod"        , e_ShaderPongTexAni, e_Idle);    
		AddModelChain("Model1", "Export/mob1DamageLoc.pod"      , e_ShaderPongTexAni, e_Damage);
		AddModelChain("Model1", "Export/mob1RunLoc.pod"         , e_ShaderPongTexAni, e_Run);
		AddModelChain("Model1", "Export/mob1WalkLoc.pod"        , e_ShaderPongTexAni, e_Walk);
		AddModelChain("Model1", "Export/mob1DeathLoc.pod"       , e_ShaderPongTexAni, e_Death);
		AddModelChain("Model1", "Export/mob1AttackLoc.pod"      , e_ShaderPongTexAni, e_Attack);
		//-------------------------------------------------------------------------------//
		// �ִ� �Ѽ�Ʈ �߰�
		//-------------------------------------------------------------------------------//
		CreateBoneAni("Model1", "Export/mob1BoneIdle.pod"       , e_Idle);
		CreateBoneAni("Model1", "Export/mob1BoneDamage.pod"     , e_Damage);
		CreateBoneAni("Model1", "Export/mob1BoneRun.pod"        , e_Run);
		CreateBoneAni("Model1", "Export/mob1BoneWalk.pod"       , e_Walk);
		CreateBoneAni("Model1", "Export/mob1BoneDeath.pod"	    , e_Death);
		CreateBoneAni("Model1", "Export/mob1BoneAttack.pod"     , e_Attack);
		//-------------------------------------------------------------------------------//

		//-------------------------------------------------------------------------------//
		// �� �Ѽ�Ʈ �߰�
		//-------------------------------------------------------------------------------//
		CreateModel  ("Model2", "Export/mob2Loc.pod"		    , e_ShaderPongTexAni);
		AddModelChain("Model2", "Export/mob2IdleLoc.pod"        , e_ShaderPongTexAni, e_Idle);    
		AddModelChain("Model2", "Export/mob2DamageLoc.pod"      , e_ShaderPongTexAni, e_Damage);
		AddModelChain("Model2", "Export/mob2RunLoc.pod"         , e_ShaderPongTexAni, e_Run);
		AddModelChain("Model2", "Export/mob2WalkLoc.pod"        , e_ShaderPongTexAni, e_Walk);
		AddModelChain("Model2", "Export/mob2DeathLoc.pod"       , e_ShaderPongTexAni, e_Death);
		AddModelChain("Model2", "Export/mob2AttackLoc.pod"      , e_ShaderPongTexAni, e_Attack);
		//-------------------------------------------------------------------------------//
		// �ִ� �Ѽ�Ʈ �߰�
		//-------------------------------------------------------------------------------//
		CreateBoneAni("Model2", "Export/mob2BoneIdle.pod"       , e_Idle);
		CreateBoneAni("Model2", "Export/mob2BoneDamage.pod"     , e_Damage);
		CreateBoneAni("Model2", "Export/mob2BoneRun.pod"        , e_Run);
		CreateBoneAni("Model2", "Export/mob2BoneWalk.pod"       , e_Walk);
		CreateBoneAni("Model2", "Export/mob2BoneDeath.pod"	    , e_Death);
		CreateBoneAni("Model2", "Export/mob2BoneAttack.pod"     , e_Attack);
		//-------------------------------------------------------------------------------//

		// ������� �� ���
		ModelRegistration();
	}

	// Ư�� ���� �� �׸���
	void DrawBone(string _modelName, string _aniName, int _aniKey)
	{
		cAniManager* mAniManager = cAniManager::GetInstance();
		auto Stroage = mAniManager->mData[_modelName][_aniName]->GetLapStorage(_aniKey);
		for (unsigned int i = 0; i < Stroage.size(); ++i)
		{
			AddModel("BOX1", Stroage[i]);
		}
	}
	
	// Ư�� ���� �� �׸���
	void DrawBone(string _modelName, string _aniName, float _x, float _y, float _z, int _aniKey)
	{
		cAniManager* mAniManager = cAniManager::GetInstance();
		auto Stroage = mAniManager->mData[_modelName][_aniName]->GetLapStorage(_aniKey);
		for (unsigned int i = 0; i < Stroage.size(); ++i)
		{
			// ��ġ �� ����
			Stroage[i]._41 += _x;
			Stroage[i]._42 += _y;
			Stroage[i]._43 += _z;
	
			AddModel("BOX1", Stroage[i]);
		}
	}

	// ��ũ�� �����
	void CreateScreen()
	{
		if (mScreen == nullptr)
		{
			// ��ũ�� �� �����ϱ�
			CreateScreenModel();

			// ��ũ�� �� ���� ��Ʈ���� �ʱ�ȭ
			// ��Ʈ���� �ʱ�ȭ
			XMMATRIX I = XMMatrixIdentity();

			// ��ũ�� ���� ��Ʈ���� �ʱ�ȭ
			XMStoreFloat4x4(&mScreenMtx, I);
		}
	}

	// �� �����
	void AddMap(int _key, string _name, float _x, float _y, float _z)
	{
		AddModel(_key, _name, _x , _y, _z);

		// �⺻ ��
		getModelData(_key, _name).mAniType = 0;

		// �ؽ�ó ũ�� ����
		getModelData(_key, _name).mTexWidth  = mAllModelData[_name]->mSkinModelTex[0]->mTexWidth;
		getModelData(_key, _name).mTexHeight = mAllModelData[_name]->mSkinModelTex[0]->mTexHeight;

		// ���� �� �̸�
		cMapManager::GetInstance()->SetMapStage(_name);
	}

	// ť��� �����
	void AddCubeMap(int _key, string _name, float _x, float _y, float _z)
	{
		AddModel(_key, _name, _x, _y, _z);

		// ���� �� �̸�
		cMapManager::GetInstance()->SetCubeMapStage(_name);

	}

	// ��ũ�� ����
	void ClearScreen()
	{
		mScreen->ClearWdMtx();
		mScreenBuffer->ClearInsBuf();

		SafeDelete(mScreenBuffer);
		SafeDelete(mScreen);
	}

	// �� ���
	void ModelRegistration()
	{
		// ���
		CalModel();

		// ���� ���
		AddBuffer();
		
		// �ִϸ��̼� ����
		LinkAni();
	}

	// �ִϸ��̼� ����
	void LinkAni()
	{
		// ���� ���� ��ȸ�Ѵ�.
		for (map<string, InitMetaData*>::iterator itor = mAllModelData.begin(); itor != mAllModelData.end(); ++itor)
		{
			// �ִϰ� �ִ� �� �����Ͷ�� 
			if (itor->second->mShaderMode == e_ShaderPongTexAni)
			{
				// ���� ������ ���� �ִ� ������
				auto tAniData = mAniManager->mData[itor->first];

				// ��ü ��ȸ
				for (auto itor2 = tAniData.begin(); itor2 != tAniData.end(); ++itor2)
				{
					// �ִϸ��̼� ���
					int _aniType = (int)itor2->second->mSkinTex.mAniType;
					itor->second->mSkinTex[_aniType] = &itor2->second->mSkinTex;
				}
			}
		}
	}

	// �� �̵��ϱ�
	void SetPos(int _uniqueCode, string _Name, float _x, float _y, float _z)
	{
		string _SlectModel;

		// ���� ü�θ�ŭ (���� ��)
		for (unsigned int i = 0; i < mModelChain[_Name].size(); ++i)
		{
			// ü�ο� �ִ� ���� ���������� ����
			_SlectModel = mModelChain[_Name][i];

			// ü�ο� �ִ� �� ��� �̵�
			mAllModelData[_SlectModel]->SetPos(_uniqueCode, _x, _y, _z);
			_SlectModel.clear();
		}
	}	

	// �� �̵��ϱ�
	void SetPos(int _uniqueCode, string _Name, XMFLOAT3 _pos)
	{
		string _SlectModel;

		// ���� ü�θ�ŭ (���� ��)
		for (unsigned int i = 0; i < mModelChain[_Name].size(); ++i)
		{
			// ü�ο� �ִ� ���� ���������� ����
			_SlectModel = mModelChain[_Name][i];

			// ü�ο� �ִ� �� ��� �̵�
			mAllModelData[_SlectModel]->SetPos(_uniqueCode, _pos.x, _pos.y, _pos.z);
			_SlectModel.clear();
		}
	}

	// �� �̵��ϱ�
	void SetPosXZ(int _uniqueCode, string _Name, float _x, float _z)
	{
		string _SlectModel;

		// ���� ü�θ�ŭ (���� ��)
		for (unsigned int i = 0; i < mModelChain[_Name].size(); ++i)
		{
			// ü�ο� �ִ� ���� ���������� ����
			_SlectModel = mModelChain[_Name][i];

			// ü�ο� �ִ� �� ��� �̵�
			mAllModelData[_SlectModel]->SetPosXZ(_uniqueCode, _x, _z);
			_SlectModel.clear();
		}
	}

	// �� �̵��ϱ�
	void SetPosXZ(int _uniqueCode, string _Name, XMFLOAT3 _pos)
	{
		string _SlectModel;

		// ���� ü�θ�ŭ (���� ��)
		for (unsigned int i = 0; i < mModelChain[_Name].size(); ++i)
		{
			// ü�ο� �ִ� ���� ���������� ����
			_SlectModel = mModelChain[_Name][i];

			// ü�ο� �ִ� �� ��� �̵�
			mAllModelData[_SlectModel]->SetPosXZ(_uniqueCode, _pos.x, _pos.z);
			_SlectModel.clear();
		}
	}

	// �� ȸ���ϱ�
	void SetRotate(int _uniqueCode, string _Name, float _x, float _y, float _z)
	{
		string _SlectModel;

		// ���� ü�θ�ŭ (���� ��)
		for (unsigned int i = 0; i < mModelChain[_Name].size(); ++i)
		{
			// ü�ο� �ִ� ���� ���������� ����
			_SlectModel = mModelChain[_Name][i];

			// ü�ο� �ִ� �� ��� ȸ��
			mAllModelData[_SlectModel]->SetRotate(_uniqueCode, _x, _y, _z);
			_SlectModel.clear();
		}
	}

	// �� ȸ���ϱ�
	void SetRotate(int _uniqueCode, string _Name, XMFLOAT3 _pos)
	{
		string _SlectModel;

		// ���� ü�θ�ŭ (���� ��)
		for (unsigned int i = 0; i < mModelChain[_Name].size(); ++i)
		{
			// ü�ο� �ִ� ���� ���������� ����
			_SlectModel = mModelChain[_Name][i];

			// ü�ο� �ִ� �� ��� ȸ��
			mAllModelData[_SlectModel]->SetRotate(_uniqueCode, _pos.x, _pos.y, _pos.z);
			_SlectModel.clear();
		}
	}

	// �ش� ��ġ�ΰ��� ����
	XMFLOAT3 GetPointDir(int _uniqueCode, string _Name, float _x, float _y, float _z)
	{
		// ü�ο� �ִ� �� �ϳ��� �����ؼ� ���� ����
		XMFLOAT3 ptDir;
		XMStoreFloat3(&ptDir, mAllModelData[mModelChain[_Name][0]]->GetPointDir(_uniqueCode, _x, _y, _z));

		return ptDir;
	}

	XMFLOAT3 GetPointDir(int _uniqueCode, string _Name, XMFLOAT3 _pos)
	{
		// ü�ο� �ִ� �� �ϳ��� �����ؼ� ���� ����
		XMFLOAT3 ptDir;
		XMStoreFloat3(&ptDir, mAllModelData[mModelChain[_Name][0]]->GetPointDir(_uniqueCode, _pos.x, _pos.y, _pos.z));

		return ptDir;
	}

	// �ش� ��ġ�� �̵�
	void MovePoint(int _uniqueCode, string _Name, float _x, float _y, float _z, float _speed)
	{
		// ü�ο� �ִ� �� �ϳ��� �����ؼ� ���� ����
		XMFLOAT3 ptDir;
		XMStoreFloat3(&ptDir, mAllModelData[mModelChain[_Name][0]]->GetPointDir(_uniqueCode, _x, _y, _z));

		XMFLOAT3 tPos = GetPos(_uniqueCode, _Name);

		// �Ÿ� ���
		XMFLOAT3 _lengthPos;
		_lengthPos.x = tPos.x - _x;
		_lengthPos.y = tPos.y - _y;
		_lengthPos.z = tPos.z - _z;

		XMVECTOR _length = XMVector3Length(XMLoadFloat3(&_lengthPos));
		XMStoreFloat3(&_lengthPos, _length);

		// �ش� �Ÿ����� Ŀ�� ����
		if (_lengthPos.x > 20)
		{
			tPos.x = tPos.x + ptDir.x * _speed;
			tPos.y = tPos.y + ptDir.y * _speed;
			tPos.z = tPos.z + ptDir.z * _speed;

			SetRotate(_uniqueCode, _Name, tPos);
			SetPos   (_uniqueCode, _Name, tPos);
		}
	}

	// �ش� ��ġ�� �̵�
	void MovePoint(int _uniqueCode, string _Name, XMFLOAT3 _pos, float _speed)
	{
		// ü�ο� �ִ� �� �ϳ��� �����ؼ� ���� ����
		XMFLOAT3 ptDir;
		XMStoreFloat3(&ptDir, mAllModelData[mModelChain[_Name][0]]->GetPointDir(_uniqueCode, _pos.x, _pos.y, _pos.z));

		XMFLOAT3 tPos = GetPos(_uniqueCode, _Name);

		// �Ÿ� ���
		XMFLOAT3 _lengthPos;
		_lengthPos.x = tPos.x - _pos.x;
		_lengthPos.y = tPos.y - _pos.y;
		_lengthPos.z = tPos.z - _pos.z;

		XMVECTOR _length = XMVector3Length(XMLoadFloat3(&_lengthPos));
		XMStoreFloat3(&_lengthPos, _length);

		// �ش� �Ÿ����� Ŀ�� ����
		if (_lengthPos.x > 20)
		{
			tPos.x = tPos.x + ptDir.x * _speed;
			tPos.y = tPos.y + ptDir.y * _speed;
			tPos.z = tPos.z + ptDir.z * _speed;

			SetRotate(_uniqueCode, _Name, tPos);
			SetPos   (_uniqueCode, _Name, tPos);
		}
	}

	// �ش� ��ġ�� �̵�
	void MoveToPlayer(int _uniqueCode, string _Name, float _speed)
	{
		// ü�ο� �ִ� �� �ϳ��� �����ؼ� ���� ����
		XMFLOAT3 ptDir;
		XMStoreFloat3(&ptDir, mAllModelData[mModelChain[_Name][0]]->GetPointDir(_uniqueCode, mPlayer.mPos));

		XMFLOAT3 tPos = GetPos(_uniqueCode, _Name);

		// �Ÿ� ���
		XMFLOAT3 _lengthPos;
		_lengthPos.x = tPos.x - mPlayer.mPos.x;
		_lengthPos.y = tPos.y - mPlayer.mPos.y;
		_lengthPos.z = tPos.z - mPlayer.mPos.z;

		XMVECTOR _length = XMVector3Length(XMLoadFloat3(&_lengthPos));
		XMStoreFloat3(&_lengthPos, _length);

		// �ش� �Ÿ����� Ŀ�� ����
		if (_lengthPos.x > 20)
		{
			tPos.x = tPos.x + ptDir.x * _speed;
			tPos.y = tPos.y + ptDir.y * _speed;
			tPos.z = tPos.z + ptDir.z * _speed;

			SetRotate(_uniqueCode, _Name, tPos);
			SetPos   (_uniqueCode, _Name, tPos);
		}
	}

	// �� ��ġ ��
	XMFLOAT3 GetPos(int _uniqueCode, string _Name)
	{
		return mAllModelData[mModelChain[_Name][0]]->getPos(_uniqueCode);
	}

	// �� ũ�� Ű���
	void SetScale(int _uniqueCode, string _Name, float _x, float _y, float _z)
	{
		string _SlectModel;

		// ���� ü�θ�ŭ (���� ��)
		for (unsigned int i = 0; i < mModelChain[_Name].size(); ++i)
		{
			// ü�ο� �ִ� ���� ���������� ����
			_SlectModel = mModelChain[_Name][i];

			// ü�ο� �ִ� �� ��� �̵�
			mAllModelData[_SlectModel]->SetScale(_uniqueCode, _x, _y, _z);
			_SlectModel.clear();
		}
	}

	// �� ������ �������� (_outData�� ��� �� ������. _outData�� �ۿ��� �����ϰ� ������ �����������) // ����
	void getModelData(int _uniqueCode, string _Name, vector<ObjData>& _outData)
	{
		string _SlectModel;

		// ���� ü�θ�ŭ (���� ��)
		for (unsigned int i = 0; i < mModelChain[_Name].size(); ++i)
		{
			// ü�ο� �ִ� ���� ���������� ����
			_SlectModel = mModelChain[_Name][i];

			// ü�ο� �ִ� �� ��� ��ȯ
			_outData.push_back(mAllModelData[_SlectModel]->getObj(_uniqueCode));
			_SlectModel.clear();
		}
	}

	// �� ������ ��������
	ObjData& getModelData(int _uniqueCode, string _Name)
	{
		string _SlectModel;
		_SlectModel = mModelChain[_Name][0];
		return 	mAllModelData[_SlectModel]->getObj(_uniqueCode);
	}

	// �� ������ �������� (_outData�� ��� �� ������. _outData�� �ۿ��� �����ϰ� ������ �����������) // ����
	void getAllModelData(string _Name, vector<ObjData>& _outData)
	{
		string _SlectModel;

		// ���� ü�θ�ŭ (���� ��)
		for (unsigned int i = 0; i < mModelChain[_Name].size(); ++i)
		{
			// ü�ο� �ִ� ���� ���������� ����
			_SlectModel = mModelChain[_Name][i];

			// ������Ʈ ������ ��� ��������
			map<int, ObjData> t_ObjDataStorage = mAllModelData[_SlectModel]->getAllObj();
			for (unsigned int x = 0; x < t_ObjDataStorage.size(); ++x)
			{
				// ü�ο� �ִ� �� ��� ��ȯ
				_outData.push_back(t_ObjDataStorage[x]);
			}

			_SlectModel.clear();
			t_ObjDataStorage.clear();
		}
	}

	// �� ���� ��Ʈ���� ����
	void ClearModel()
	{
		// ù��° ������Ʈ���� �ʱ�ȭ ���� �ʰ���
		static bool tStartPass = true;

		if (!tStartPass)
		{
			// �� ������ ����
			for (map<string, InitMetaData*>::iterator itor = mAllModelData.begin(); itor != mAllModelData.end(); ++itor)
			{
				// �� ���� ��ü ����
				itor->second->ClearWdMtx();
			}

			// �� �ν��Ͻ� ���� ����
			ClearModelAndBuf();

			// �𵨿� ���Ǿ��� ���̴� ����
			mUseAllShader.clear();
			mUseShader.clear();

			// �� �̸� ����
			mUseModel.clear();

			// ��ũ�� Ŭ����
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
		// ���� ������ ����
		for (map<int, BufferType*>::iterator itor = mBufferType.begin(); itor != mBufferType.end();)
		{
			// �� �ν��Ͻ� ���� ����
			for (auto itor2 = itor->second->mInstancedBuffer.begin(); itor2 != itor->second->mInstancedBuffer.end(); ++itor2)
			{
				ReleaseCOM(itor2->second);
			}

			// ���� ���� ��ü ����
			SafeDelete(itor->second);

			// ���� ������
			mBufferType.erase(itor++);
		}
		mBufferType.clear();

		// �� ������ ����
		for (map<string, InitMetaData*>::iterator itor = mAllModelData.begin(); itor != mAllModelData.end();)
		{
			// �� ���� ��ü ����
			itor->second->ClearWdMtx();

			// �� ���� ��ü ����
			SafeDelete(itor->second);

			// ���� ������
			mAllModelData.erase(itor++);
		}
		mAllModelData.clear();

		// �� ü�� Ŭ����
		for (auto itor = mModelChain.begin(); itor != mModelChain.end(); ++itor)
		{
			// Second �迭�� �����Ѵ�.
			// ������ ���ҿ� �����Ͽ� �����Ѵ�.
			for (unsigned int i = 0; i < itor->second.size(); ++i)
				itor->second[i].clear();

			// Second �迭 ��ü�� �����Ѵ�.
			itor->second.clear();
		}
		mModelChain.clear();

		// �𵨿� ���Ǿ��� ���̴� ����
		mUseAllShader.clear();
		mUseShader.clear();

		// �� �̸� ����
		mUseModel.clear();

		// ��ũ�� ����
		ClearScreen();
	}

	// �� �����ϱ�
	void CreateModel(string _Name, string _ModelRoute, SHADER_TYPE _ShaderMode, D3D_PRIMITIVE_TOPOLOGY _D3D_PRIMITIVE_TOPOLOGY = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
	{
		string tSelectModelRoute, tNewName;
		char _ModelNumBuf[128];
		//----------------------------------------------------//
		// �� ��� �Ľ�
		//----------------------------------------------------//
		// �ʱ�ȭ
		mXMLParser.LoadXMLRoute(_ModelRoute);

		// ���� ��θ�ŭ �Žõ����Ͱ� �����Ѵٴ� ���̹Ƿ�,
		// �̿� ���� �Ľ��� �����͸� ���� ������ Ȯ�����ش�.
		int _ReadLocNum = mXMLParser.mNewModleLoc.size();

		// ���� ��γ��� �� ������ŭ �ݺ�
		for (int i = 0; i < _ReadLocNum; ++i)
		{
			mXMLParser.InitClass();
			tSelectModelRoute = mXMLParser.mNewModleLoc[i].Data; // ���õ� ���� ��Ʈ

			// ù ���̶�� �ڽ��� �̸����� ���´�.
			tNewName = _Name;
			
			// ���� ���� �ִ� ���̶�� '_��ȣ'�� �߰��ؼ� ���´�.
			if (i > 0)
			{
				memset(&_ModelNumBuf, '\0', sizeof(_ModelNumBuf));
				itoa10(i, _ModelNumBuf);
				tNewName = tNewName + '_' + _ModelNumBuf + '\0';
			}

			// �� �߰� ���� (������ �̸����� ���� Ȯ��)
			mAllModelData[tNewName] = new InitMetaData(_ShaderMode, _D3D_PRIMITIVE_TOPOLOGY);

			// ���� ����
			mAllModelData[tNewName]->mModelType = e_ParsingModel;
			mAllModelData[tNewName]->mCreateName = tNewName;
			
			//----------------------------------------------------//
			// �� ������ �Ľ�
			//----------------------------------------------------//
			mXMLParser.LoadXMLModel(tSelectModelRoute, mAllModelData[tNewName]);

			// ���̴�&�� �ʱ�ȭ ����
			UseAllShaderModel(_ShaderMode);
			UsingModel(tNewName);

			// �� ü�ο� �̸� ���
			mModelChain[_Name].push_back(tNewName);

			// ������ ����
			mXMLParser.ClearPointer();

			// string ����
			tSelectModelRoute.clear();
			tNewName.clear();
		}

		// �ļ� ���� �ʱ�ȭ ( ���� ���� �ޱ� ���� ) 
		mXMLParser.ClearClass();
		_Name.clear();
	}

	// �� ü�� �����ϱ� (���� �ִϸ��̼� ó���� ����)
	void AddModelChain(string _Name, string _ModelRoute, SHADER_TYPE _ShaderMode, FSM_TYPE _fsmType, D3D_PRIMITIVE_TOPOLOGY _D3D_PRIMITIVE_TOPOLOGY = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
	{
		string tSelectModelRoute, tNewName;
		char _ModelNumBuf[128];
		//----------------------------------------------------//
		// �� ��� �Ľ�
		//----------------------------------------------------//
		// �ʱ�ȭ
		mXMLParser.LoadXMLRoute(_ModelRoute);

		// ���� ��θ�ŭ �Žõ����Ͱ� �����Ѵٴ� ���̹Ƿ�,
		// �̿� ���� �Ľ��� �����͸� ���� ������ Ȯ�����ش�.
		int _ReadLocNum = mXMLParser.mNewModleLoc.size();

		// ���� ��γ��� �� ������ŭ �ݺ�
		for (int i = 0; i < _ReadLocNum; ++i)
		{
			mXMLParser.InitClass();
			tSelectModelRoute = mXMLParser.mNewModleLoc[i].Data; // ���õ� ���� ��Ʈ

			// ù ���̶�� �ڽ��� �̸����� ���´�.
			tNewName = _Name;

			// ���� ���� �ִ� ���̶�� '_��ȣ'�� �߰��ؼ� ���´�.
			if (i > 0)
			{
				memset(&_ModelNumBuf, '\0', sizeof(_ModelNumBuf));
				itoa10(i, _ModelNumBuf);
				tNewName = tNewName + '_' + _ModelNumBuf + '\0';
			}
			
			// �ӽ� ������
			cout << " >> ��Ų �� �Ľ� ���� <<" << endl;
			InitMetaData _ParsigModel(_ShaderMode, _D3D_PRIMITIVE_TOPOLOGY);

			//----------------------------------------------------//
			// ��Ų �� ������ �Ľ�
			//----------------------------------------------------//
			mXMLParser.LoadXMLSkinModel(tSelectModelRoute, &_ParsigModel, mAllModelData[tNewName], _fsmType);

			// ������ ����
			mXMLParser.ClearPointer();

			// string ����
			tSelectModelRoute.clear();
			tNewName.clear();
		}
		cout << " >> ��Ų �� �Ľ� ���� <<" << endl;

		// �ļ� ���� �ʱ�ȭ ( ���� ���� �ޱ� ���� ) 
		mXMLParser.ClearClass();
		_Name.clear();
	}

	// �� �����ϱ�
	void CreateBoneAni(string _Name, string _BoneRoute, FSM_TYPE _fsmType, D3D_PRIMITIVE_TOPOLOGY _D3D_PRIMITIVE_TOPOLOGY = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
	{
		//----------------------------------------------------//
		// �� ������ �Ľ�
		//----------------------------------------------------//
		// ���� �����ʹ� �ϳ����̱⿡ �̷������� �����ص� ��.
		MyBoneData* mMyBoneData = new MyBoneData();

		// 1�� ���� �� �Ľ�
		mXMLParser.ReadDataMyFormat_Bone(_BoneRoute, mMyBoneData);

		// �ؽ�ó �̸�
		string _TexName = mAllModelData[_Name]->mObjName;
		_TexName += mMyBoneData->mAniName;

		// ������ ���
		mMyBoneData->CalData(_TexName, _fsmType);

		// ü�ο� ����� �𵨿� ����
		for (unsigned int i = 0; i < mModelChain[_Name].size(); ++i)
		{
			string SelectName = mModelChain[_Name][i];
			mAllModelData[SelectName]->mAniModel = true;
			mAniManager->mData[SelectName][mMyBoneData->mAniName] = mMyBoneData;
			SelectName.clear();
		}

		// �ļ� ���� �ʱ�ȭ ( ���� ���� �ޱ� ���� ) 
		mXMLParser.ClearClass();
	}

	// �ڽ� �� �����ϱ�
	void CreateBoxModel(string _Name, SHADER_TYPE _ShaderMode, D3D_PRIMITIVE_TOPOLOGY _D3D_PRIMITIVE_TOPOLOGY = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
	{
		// �� ü�ο� �̸� ���
		mModelChain[_Name].push_back(_Name);

		// �� ����
		mAllModelData[_Name] = new InitMetaData(_Name.c_str(), _ShaderMode, _D3D_PRIMITIVE_TOPOLOGY);

		// ���� �� ���� ���
		InitMetaData* _nowModel = mAllModelData[_Name];

		// ���� ����
		_nowModel->mModelType  = e_BasicModel;
		_nowModel->mCreateName = _Name;
		strcpy(_nowModel->mObjName, _Name.c_str());

		// �Ľ� ����
		mXMLParser.LoadBox(_nowModel, 10.0f, 10.0f, 10.0f);

		// ���̴�, �� ��� (���͸� ��)
		UseAllShaderModel(_ShaderMode);
		UsingModel(_Name);
	}

	void CreateBoxModel(string _Name, SHADER_TYPE _ShaderMode, float _Size, D3D_PRIMITIVE_TOPOLOGY _D3D_PRIMITIVE_TOPOLOGY = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
	{
		// �� ü�ο� �̸� ���
		mModelChain[_Name].push_back(_Name);

		// �� ����
		mAllModelData[_Name] = new InitMetaData(_Name.c_str(), _ShaderMode, _D3D_PRIMITIVE_TOPOLOGY);

		// ���� �� ���� ���
		InitMetaData* _nowModel = mAllModelData[_Name];

		// ���� ����
		_nowModel->mModelType  = e_BasicModel;
		_nowModel->mCreateName = _Name;
		strcpy(_nowModel->mObjName, _Name.c_str());

		// �Ľ� ����
		mXMLParser.LoadBox(_nowModel, _Size, _Size, _Size);

		// ���̴�, �� ��� (���͸� ��)
		UseAllShaderModel(_ShaderMode);
		UsingModel(_Name);
	}

	// �� ����
	void CreateMap(string _Name, string _FileName, float _Xwidth, float _Zdepth, float _CellSize, float _HeightScale, SHADER_TYPE _ShaderMode, D3D_PRIMITIVE_TOPOLOGY _D3D_PRIMITIVE_TOPOLOGY = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
	{
		// �� ü�ο� �̸� ���
		mModelChain[_Name].push_back(_Name);

		// �� ����
		mAllModelData[_Name] = new InitMetaData(_Name.c_str(), _ShaderMode, _D3D_PRIMITIVE_TOPOLOGY);

		// ���� �� ���� ���
		InitMetaData* _nowModel = mAllModelData[_Name];

		// ���� ����
		_nowModel->mModelType  = e_ParsingModel;
		_nowModel->mCreateName = _Name;
		strcpy(_nowModel->mObjName, _Name.c_str());

		// �Ľ� ���� 
		mXMLParser.LoadMap(_nowModel, _FileName, _Xwidth, _Zdepth, _CellSize, _HeightScale);

		// ���� ���
		_nowModel->CalValueMap();

		// ���̴�, �� ��� (���͸� ��)
		UseAllShaderModel(_ShaderMode);
		UsingModel(_Name);

		// �� �Ŵ��� �ʱ�ȭ
		cMapManager::GetInstance()->mData[_Name].mModel       = mAllModelData[_Name];
		cMapManager::GetInstance()->mData[_Name].mCellSize    = _CellSize;
		cMapManager::GetInstance()->mData[_Name].mHeightScale = _HeightScale;
		cMapManager::GetInstance()->mData[_Name].mXwidth      = _Xwidth;
		cMapManager::GetInstance()->mData[_Name].mZdepth      = _Zdepth;
	}

	// �� ����
	void CreateSkyBox(string _Name, string _FileName, float _radius, UINT _sliceCount, UINT _stackCount, SHADER_TYPE _ShaderMode, D3D_PRIMITIVE_TOPOLOGY _D3D_PRIMITIVE_TOPOLOGY = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
	{
		// �� ü�ο� �̸� ���
		mModelChain[_Name].push_back(_Name);

		// �� ����
		mAllModelData[_Name] = new InitMetaData(_Name.c_str(), _ShaderMode, _D3D_PRIMITIVE_TOPOLOGY);

		// ���� �� ���� ���
		InitMetaData* _nowModel = mAllModelData[_Name];

		// ���� ����
		_nowModel->mModelType  = e_ParsingModel;
		_nowModel->mCreateName = _Name;
		strcpy(_nowModel->mObjName, _Name.c_str());

		// �Ľ� ����
		mXMLParser.LoadSphere(_nowModel, _radius, _sliceCount, _stackCount);

		// ���̴�, �� ��� (���͸� ��)
		UseAllShaderModel(_ShaderMode);
		UsingModel(_Name);

		// ���
		_nowModel->CalValue();

		// ť��� ���ҽ� �� �����ϱ�
		mMapManager->CreateRSV(_Name, _FileName);
	}

	// ��ũ�� �� �����ϱ�
	void CreateScreenModel(SHADER_TYPE _ShaderMode = e_ShaderDeferred, D3D_PRIMITIVE_TOPOLOGY _D3D_PRIMITIVE_TOPOLOGY = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
	{
		// ���� �� ���� ���
		mScreen = new InitMetaData("GSreen", _ShaderMode, _D3D_PRIMITIVE_TOPOLOGY);

		// ���� ����
		mScreen->mModelType  = e_BasicModel;
		mScreen->mCreateName = "GSreen";
		strcpy(mScreen->mObjName, mScreen->mCreateName.c_str());

		// �Ľ� ���� (Ǯ ��ũ�� ���� �����)
		mXMLParser.LoadScreen(mScreen, 1.0f, 1.0f); // Ǯ ��ũ������

		// ���� ���
		mScreen->CalValue();

		// ���� ����
		mScreenBuffer = new BufferType();

		// ���� ����
		mScreenBuffer->Build_GScreen(mScreen);
	}

	// �𵨿� ����� ��� ���̴� �߰�
	void UseAllShaderModel(SHADER_TYPE _ShaderMode)
	{
		// ó������ ������ �˻�
		for (unsigned int i = 0; i < mUseAllShader.size(); ++i)
		{
			// �ȿ� �ִ� ������ ���ٸ�, �̹� ��� �� �����̹Ƿ� ����
			if (mUseAllShader[i] == _ShaderMode)
				return;
		}
		mUseAllShader.push_back(_ShaderMode);
	}

	// ������� �𵨿� ����� ���̴� �߰�
	void UsingShaderModel(SHADER_TYPE _ShaderMode)
	{
		// ó������ ������ �˻�
		for (unsigned int i = 0; i < mUseShader.size(); ++i)
		{
			// �ȿ� �ִ� ������ ���ٸ�, �̹� ��� �� �����̹Ƿ� ����
			if (mUseShader[i] == _ShaderMode)
				return;
		}
		mUseShader.push_back(_ShaderMode);
	}

	// ������� ��
	void UsingModel(string _ModelName)
	{
		// ó������ ������ �˻�
		for (unsigned int i = 0; i < mUseModel.size(); ++i)
		{
			// �ȿ� �ִ� ������ ���ٸ�, �̹� ��� �� �����̹Ƿ� ����
			if (mUseModel[i] == _ModelName)
				return;
		}
		mUseModel.push_back(_ModelName);
	}

	// �� �߰��ϱ�
	void AddUpdateModel(string _Name, float _x, float _y, float _z, OBJ_MOVEABLE _moveAble = e_StaticObj)
	{
		// �� ���Ͽ� �ִ� ���� �� �� ��ŭ
		for (unsigned int i = 0; i < mModelChain[_Name].size(); ++i)
		{
			// ���� �� �߰�
			addSubModel(mModelChain[_Name][i], _x, _y, _z, _moveAble);

			// �ٽ� �����
			MakeModelInsBuf(mModelChain[_Name][i]);
		}
		_Name.clear();
	}	

	// �� �߰��ϱ�
	void AddUpdateModel(string _Name, XMFLOAT4X4 _Mtx, OBJ_MOVEABLE _moveAble = e_StaticObj)
	{
		// �� ���Ͽ� �ִ� ���� �� �� ��ŭ
		for (unsigned int i = 0; i < mModelChain[_Name].size(); ++i)
		{
			// ���� �� �߰�
			addSubModel(mModelChain[_Name][i], _Mtx, _moveAble);

			// �ٽ� �����
			MakeModelInsBuf(mModelChain[_Name][i]);
		}
		_Name.clear();
	}

	// �� �߰��ϱ�
	void AddUpdateModel(int _key, string _Name, float _x, float _y, float _z, OBJ_MOVEABLE _moveAble = e_StaticObj)
	{
		// �� ���Ͽ� �ִ� ���� �� �� ��ŭ
		for (unsigned int i = 0; i < mModelChain[_Name].size(); ++i)
		{
			// ���� �� �߰�
			addSubModel(_key, mModelChain[_Name][i], _x, _y, _z, _moveAble);

			// �ٽ� �����
			MakeModelInsBuf(mModelChain[_Name][i]);
		}
		_Name.clear();
	}

	// �� �߰��ϱ�
	void AddUpdateModel(int _key, string _Name, XMFLOAT4X4 _Mtx, OBJ_MOVEABLE _moveAble = e_StaticObj)
	{
		// �� ���Ͽ� �ִ� ���� �� �� ��ŭ
		for (unsigned int i = 0; i < mModelChain[_Name].size(); ++i)
		{
			// ���� �� �߰�
			addSubModel(_key, mModelChain[_Name][i], _Mtx, _moveAble);

			// �ٽ� �����
			MakeModelInsBuf(mModelChain[_Name][i]);
		}

		_Name.clear();
	}

	// �� �߰��ϱ� (�ִ� FSM)
	void AddUpdateModel(int _key, string _Name, float _x, float _y, float _z, FSM_TYPE _fsmType, OBJ_MOVEABLE _moveAble = e_StaticObj)
	{
		// �� ���Ͽ� �ִ� ���� �� �� ��ŭ
		for (unsigned int i = 0; i < mModelChain[_Name].size(); ++i)
		{
			// ���� �� �߰�
			addSubModel(_key, mModelChain[_Name][i], _x, _y, _z, _fsmType, _moveAble);

			// �Ϲ� �� �ν��Ͻ� ���� ����
			MakeModelInsBuf(mModelChain[_Name][i]);
		}
		_Name.clear();
	}

	// �� �߰��ϱ� (�ִ� FSM)
	void AddUpdateModel(int _key, string _Name, XMFLOAT4X4 _Mtx, FSM_TYPE _fsmType, OBJ_MOVEABLE _moveAble = e_StaticObj)
	{
		// �� ���Ͽ� �ִ� ���� �� �� ��ŭ
		for (unsigned int i = 0; i < mModelChain[_Name].size(); ++i)
		{
			// ���� �� �߰�
			addSubModel(_key, mModelChain[_Name][i], _Mtx, _fsmType, _moveAble);

			// �ٽ� �����
			MakeModelInsBuf(mModelChain[_Name][i]);
		}
		_Name.clear();
	}

	// �� �߰��ϱ� (�ִ� FSM)
	void AddUpdateModel(int _key, string _Name, float _x, float _y, float _z, FSM_TYPE _fsmType, float _frame, OBJ_MOVEABLE _moveAble = e_StaticObj)
	{
		// �� ���Ͽ� �ִ� ���� �� �� ��ŭ
		for (unsigned int i = 0; i < mModelChain[_Name].size(); ++i)
		{
			// ���� �� �߰�
			addSubModel(_key, mModelChain[_Name][i], _x, _y, _z, _fsmType, _frame, _moveAble);

			// �ٽ� �����
			MakeModelInsBuf(mModelChain[_Name][i]);
		}
		_Name.clear();
	}

	// �� �߰��ϱ� (�ִ� FSM)
	void AddUpdateModel(int _key, string _Name, XMFLOAT4X4 _Mtx, FSM_TYPE _fsmType, float _frame, OBJ_MOVEABLE _moveAble = e_StaticObj)
	{
		// �� ���Ͽ� �ִ� ���� �� �� ��ŭ
		for (unsigned int i = 0; i < mModelChain[_Name].size(); ++i)
		{
			// ���� �� �߰�
			addSubModel(_key, mModelChain[_Name][i], _Mtx, _fsmType, _frame, _moveAble);

			// �ٽ� �����
			MakeModelInsBuf(mModelChain[_Name][i]);
		}
		_Name.clear();
	}


	// �� �����ϱ�
	void EraseUpdateModel(int _key, string _Name)
	{
		// �� ���Ͽ� �ִ� ���� �� �� ��ŭ
		for (unsigned int i = 0; i < mModelChain[_Name].size(); ++i)
		{
			// ���� �� ����
			EraseSubModel(_key, mModelChain[_Name][i]);

			// �ٽ� �����
			MakeModelInsBuf(mModelChain[_Name][i]);
		}
		_Name.clear();
	}

	// �� �߰��ϱ�
	void AddModel(string _Name, float _x, float _y, float _z, OBJ_MOVEABLE _moveAble = e_StaticObj)
	{
		// �� ���Ͽ� �ִ� ���� �� �� ��ŭ
		for (unsigned int i = 0; i < mModelChain[_Name].size(); ++i)
		{
			// ���� �� �߰�
			addSubModel(mModelChain[_Name][i], _x, _y, _z, _moveAble);
		}
		_Name.clear();
	}

	// �� �߰��ϱ�
	void AddModel(string _Name, XMFLOAT4X4 _Mtx, OBJ_MOVEABLE _moveAble = e_StaticObj)
	{
		// �� ���Ͽ� �ִ� ���� �� �� ��ŭ
		for (unsigned int i = 0; i < mModelChain[_Name].size(); ++i)
		{
			// ���� �� �߰�
			addSubModel(mModelChain[_Name][i], _Mtx, _moveAble);
		}
		_Name.clear();
	}

	// �� �߰��ϱ�
	void AddModel(int _key, string _Name, float _x, float _y, float _z, OBJ_MOVEABLE _moveAble = e_StaticObj)
	{
		// �� ���Ͽ� �ִ� ���� �� �� ��ŭ
		for (unsigned int i = 0; i < mModelChain[_Name].size(); ++i)
		{
			// ���� �� �߰�
			addSubModel(_key, mModelChain[_Name][i], _x, _y, _z, _moveAble);
		}
		_Name.clear();
	}

	// �� �߰��ϱ�
	void AddModel(int _key, string _Name, XMFLOAT4X4 _Mtx, OBJ_MOVEABLE _moveAble = e_StaticObj)
	{
		// �� ���Ͽ� �ִ� ���� �� �� ��ŭ
		for (unsigned int i = 0; i < mModelChain[_Name].size(); ++i)
		{
			// ���� �� �߰�
			addSubModel(_key, mModelChain[_Name][i], _Mtx, _moveAble);
		}
		_Name.clear();
	}

	// �� �߰��ϱ� (�� FSM)
	void AddModel(int _key, string _Name, float _x, float _y, float _z, FSM_TYPE _fsmType, OBJ_MOVEABLE _moveAble = e_StaticObj)
	{
		// �� ���Ͽ� �ִ� ���� �� �� ��ŭ
		for (unsigned int i = 0; i < mModelChain[_Name].size(); ++i)
		{
			// ���� �� �߰�
			addSubModel(_key, mModelChain[_Name][i], _x, _y, _z, _fsmType, _moveAble);
		}
		_Name.clear();
	}

	// �� �߰��ϱ� (�� FSM)
	void AddModel(int _key, string _Name, XMFLOAT4X4 _Mtx, FSM_TYPE _fsmType, OBJ_MOVEABLE _moveAble = e_StaticObj)
	{
		// �� ���Ͽ� �ִ� ���� �� �� ��ŭ
		for (unsigned int i = 0; i < mModelChain[_Name].size(); ++i)
		{
			// ���� �� �߰�
			addSubModel(_key, mModelChain[_Name][i], _Mtx, _fsmType, _moveAble);
		}
		_Name.clear();
	}

	// �� �߰��ϱ� (�� FSM)
	void AddModel(int _key, string _Name, float _x, float _y, float _z, FSM_TYPE _fsmType, float _frame, OBJ_MOVEABLE _moveAble = e_StaticObj)
	{
		// �� ���Ͽ� �ִ� ���� �� �� ��ŭ
		for (unsigned int i = 0; i < mModelChain[_Name].size(); ++i)
		{
			// ���� �� �߰�
			addSubModel(_key, mModelChain[_Name][i], _x, _y, _z, _fsmType, _frame, _moveAble);
		}
		_Name.clear();
	}

	// �� �߰��ϱ� (�� FSM)
	void AddModel(int _key, string _Name, XMFLOAT4X4 _Mtx, FSM_TYPE _fsmType, float _frame, OBJ_MOVEABLE _moveAble = e_StaticObj)
	{
		// �� ���Ͽ� �ִ� ���� �� �� ��ŭ
		for (unsigned int i = 0; i < mModelChain[_Name].size(); ++i)
		{
			// ���� �� �߰�
			addSubModel(_key, mModelChain[_Name][i], _Mtx, _fsmType, _frame, _moveAble);
		}
		_Name.clear();
	}

	// �� �߰��ϱ�
	void AddScreen(float _x, float _y, float _z)
	{
		mScreen->AddModel(_x, _y, _z, e_StaticObj);
	}

	// HP ����
	void SetHP(int _key, string _Name, float _hp)
	{
		// �� ���Ͽ� �ִ� ���� �� �� ��ŭ
		for (unsigned int i = 0; i < mModelChain[_Name].size(); ++i)
		{
			string _SlectModel = mModelChain[_Name][i];
			mAllModelData[_SlectModel]->SetHp(_key, _hp);
		}
	}

	// MP ����
	void SetMP(int _key, string _Name, float _mp)
	{
		// �� ���Ͽ� �ִ� ���� �� �� ��ŭ
		for (unsigned int i = 0; i < mModelChain[_Name].size(); ++i)
		{
			string _SlectModel = mModelChain[_Name][i];
			mAllModelData[_SlectModel]->SetMp(_key, _mp);
		}
	}

	// ����
	void Jump(int _key, string _Name)
	{
		// �� ���Ͽ� �ִ� ���� �� �� ��ŭ
		for (unsigned int i = 0; i < mModelChain[_Name].size(); ++i)
		{
			string _SlectModel = mModelChain[_Name][i];
			mAllModelData[_SlectModel]->Jump(_key);
		}
	}

	// �÷��̾� ����, ����
	XMFLOAT3 PlayerWalk(float _speed)
	{
		XMFLOAT3 _PlayerLookDir = PlayerLookDir();
		gCam.Walk(_speed);

		mPlayer.mPos = gCam.GetThirdPosition();
		SetRotate(mPlayer.mkey, mPlayer.mModelName, mPlayer.mPos);
		SetPosXZ (mPlayer.mkey, mPlayer.mModelName, mPlayer.mPos);

		// y��ǥ ������Ʈ
		float tYpos = mMapManager->GetHeightMap(mPlayer.mPos.x, mPlayer.mPos.z);
		gCam.SetCamY(tYpos);

		return mPlayer.mPos;
	}

	// �÷��̾� ���⺤��
	XMFLOAT3 PlayerLookDir()
	{
		return getModelData(mPlayer.mkey, mPlayer.mModelName).getLookDir();
	}
	

	// �÷��̾� ����, ����
	XMFLOAT3 PlayerStrafe(float _speed)
	{
		gCam.Strafe(_speed);

		mPlayer.mPos = gCam.GetThirdPosition();
		SetRotate(mPlayer.mkey, mPlayer.mModelName, mPlayer.mPos);
		SetPosXZ (mPlayer.mkey, mPlayer.mModelName, mPlayer.mPos);

		// y��ǥ ������Ʈ
		float tYpos = mMapManager->GetHeightMap(mPlayer.mPos.x, mPlayer.mPos.z);
		gCam.SetCamY(tYpos);

		return mPlayer.mPos;
	}



	// �÷��̾� ����
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

	// �÷��̾� ����
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

	// �÷��̾� ����
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

	// �ν��Ͻ� ���� �����
	void MakeInsbuf()
	{
		// ���ۿ��� ������ ����
		DataMove();

		// �Ϲ� �� �ν��Ͻ� ���� ����
		MakeModelInsBuf();

		// ��ũ�� �ν��Ͻ� ���� ����
		mScreenBuffer->MakeScreenInsBuf(mScreen);
	}


	// ���� ���
	BufferType* GetBuffer(SHADER_TYPE _ShaderMode)
	{
		return mBufferType[_ShaderMode];
	}

	// ���۳��� �� �ҷ�����
	InitMetaData* GetBufferInModel(SHADER_TYPE _ShaderMode, string _ModelName)
	{
		return mBufferType[_ShaderMode]->mModelList[_ModelName];
	}

	// ������Ʈ
	void Update(float& dt)
	{
		// ��� �𵨵鿡�� �ϰ����� ����� ������.
		for (map<string, InitMetaData*>::iterator itor = mAllModelData.begin(); itor != mAllModelData.end(); ++itor)
		{
			// �ִϸ��̼� ������Ʈ
			UpdateAni(itor->second, dt);
		}
	}

	// �ִϸ��̼� �ð� ������Ʈ
	void UpdateAni(InitMetaData* _itor, float& _dt)
	{
		// ������Ʈ�� �ְ�, �ִϸ��̼� ���̶��
		if (_itor->mObjData.size() && _itor->mShaderMode == e_ShaderPongTexAni)
		{
			for (auto itor = _itor->mObjData.begin(); itor != _itor->mObjData.end(); ++itor)
			{
				// y��ǥ ������Ʈ
				XMFLOAT3 mPos = itor->second.getPos();
				float tYpos = mMapManager->GetHeightMap(mPos.x, mPos.z);

				// �ִ� ������Ʈ
				itor->second.Update(_dt, tYpos);
			}
		}
	}

	// �ν��Ͻ� ���� ������Ʈ
	void UpdateIns()
	{
		for (unsigned int i = 0; i < mUseShader.size(); ++i)
		{
			mBufferType[mUseShader[i]]->UpdateIns();
		}

		// ��ũ�� �ν��Ͻ� ���� ������Ʈ
		mScreenBuffer->UpdateScreenIns(mScreen);
	}

	// FSM ����
	void SetFSM(int _unicode, string _Name, FSM_TYPE _modelFsm)
	{
		// �� ���Ͽ� �ִ� ���� �� �� ��ŭ
		for (unsigned int i = 0; i < mModelChain[_Name].size(); ++i)
		{
			// ü�ο� �ִ� �� ��� FSM ��ȭ
			string _SlectModel = mModelChain[_Name][i];

			// �ִϰ� �ִ� ���̶��,
			if (mAllModelData[_SlectModel]->mShaderMode == e_ShaderPongTexAni)
			{
				mAllModelData[_SlectModel]->SetFSM(_unicode, _modelFsm);
			}
		}
	}

	// FSM ����
	void SetFSM(int _unicode, string _Name, FSM_TYPE _modelFsm, float _Frame)
	{
		// �� ���Ͽ� �ִ� ���� �� �� ��ŭ
		for (unsigned int i = 0; i < mModelChain[_Name].size(); ++i)
		{
			// ü�ο� �ִ� �� ��� FSM ��ȭ
			string _SlectModel = mModelChain[_Name][i];

			// �ִϰ� �ִ� ���̶��,
			if (mAllModelData[_SlectModel]->mShaderMode == e_ShaderPongTexAni)
			{
				mAllModelData[_SlectModel]->SetFSM(_unicode, _modelFsm, _Frame);
			}
		}
	}

	// FSM ����
	void SetPlayerFSM(FSM_TYPE _modelFsm)
	{
		// �� ���Ͽ� �ִ� ���� �� �� ��ŭ
		for (unsigned int i = 0; i < mModelChain[mPlayer.mModelName].size(); ++i)
		{
			// ü�ο� �ִ� �� ��� FSM ��ȭ
			string _SlectModel = mModelChain[mPlayer.mModelName][i];

			// �ִϰ� �ִ� ���̶��,
			if (mAllModelData[_SlectModel]->mShaderMode == e_ShaderPongTexAni)
			{
				mAllModelData[_SlectModel]->SetFSM(mPlayer.mkey, _modelFsm);
			}
		}
	}

	// FSM ����
	void SetPlayerFSM(FSM_TYPE _modelFsm, float _Frame)
	{
		// �� ���Ͽ� �ִ� ���� �� �� ��ŭ
		for (unsigned int i = 0; i < mModelChain[mPlayer.mModelName].size(); ++i)
		{
			// ü�ο� �ִ� �� ��� FSM ��ȭ
			string _SlectModel = mModelChain[mPlayer.mModelName][i];

			// �ִϰ� �ִ� ���̶��,
			if (mAllModelData[_SlectModel]->mShaderMode == e_ShaderPongTexAni)
			{
				mAllModelData[_SlectModel]->SetFSM(mPlayer.mkey, _modelFsm, _Frame);
			}
		}
	}


private:
	// �÷��̾� ���̰� ��������
	float GetPlayerY()
	{
		return getModelData(mPlayer.mkey, mPlayer.mModelName).getPos().y;
	}

	// ���� �𵨵� ��� �߰�
	void addSubModel(string& _SlectModel, float& _x, float& _y, float& _z, OBJ_MOVEABLE& _moveAble)
	{
		// ü�ο� �ִ� �� ��� �����Ʈ������ �߰�
		mAllModelData[_SlectModel]->AddModel(_x, _y, _z, _moveAble);

		// ���̴� �߰�
		addUseShader(_SlectModel);
	}

	// ���� �𵨵� ��� �߰�
	void addSubModel(string& _SlectModel, XMFLOAT4X4 _Mtx, OBJ_MOVEABLE& _moveAble)
	{
		// ü�ο� �ִ� �� ��� �����Ʈ������ �߰�
		mAllModelData[_SlectModel]->AddModel(_Mtx, _moveAble);

		// ���̴� �߰�
		addUseShader(_SlectModel);
	}

	// ���� �𵨵� ��� �߰�
	void addSubModel(int _key, string& _SlectModel, float& _x, float& _y, float& _z, OBJ_MOVEABLE& _moveAble)
	{
		// ü�ο� �ִ� �� ��� �����Ʈ������ �߰�
		mAllModelData[_SlectModel]->AddModel(_key, _x, _y, _z, _moveAble);

		// ���̴� �߰�
		addUseShader(_SlectModel);
	}

	// ���� �𵨵� ��� �߰�
	void addSubModel(int _key, string& _SlectModel, XMFLOAT4X4 _Mtx, OBJ_MOVEABLE& _moveAble)
	{
		// ü�ο� �ִ� �� ��� �����Ʈ������ �߰�
		mAllModelData[_SlectModel]->AddModel(_key, _Mtx, _moveAble);

		// ���̴� �߰�
		addUseShader(_SlectModel);
	}

	// ���� �𵨵� ��� �߰� (�ִ� FSM)
	void addSubModel(int _key, string& _SlectModel, float& _x, float& _y, float& _z, FSM_TYPE _fsmType, OBJ_MOVEABLE& _moveAble)
	{
		// ü�ο� �ִ� �� ��� �����Ʈ������ �߰�
		mAllModelData[_SlectModel]->AddModel(_key, _x, _y, _z, _moveAble);

		// �ִ� ��
		mAllModelData[_SlectModel]->SetFSM(_key, _fsmType);

		// ���̴� �߰�
		addUseShader(_SlectModel);
	}

	// ���� �𵨵� ��� �߰� (�ִ� FSM)
	void addSubModel(int _key, string& _SlectModel, XMFLOAT4X4 _Mtx, FSM_TYPE _fsmType, OBJ_MOVEABLE& _moveAble)
	{
		// ü�ο� �ִ� �� ��� �����Ʈ������ �߰�
		mAllModelData[_SlectModel]->AddModel(_key, _Mtx, _moveAble);

		// �ִ� ��
		mAllModelData[_SlectModel]->SetFSM(_key, _fsmType);

		// ���̴� �߰�
		addUseShader(_SlectModel);
	}

	// ���� �𵨵� ��� �߰� (�ִ� FSM)
	void addSubModel(int _key, string& _SlectModel, float& _x, float& _y, float& _z, FSM_TYPE _fsmType, float _frame, OBJ_MOVEABLE& _moveAble)
	{
		// ü�ο� �ִ� �� ��� �����Ʈ������ �߰�
		mAllModelData[_SlectModel]->AddModel(_key, _x, _y, _z, _moveAble);

		// �ִ� ��
		mAllModelData[_SlectModel]->SetFSM(_key, _fsmType, _frame);

		// ���̴� �߰�
		addUseShader(_SlectModel);
	}

	// ���� �𵨵� ��� �߰� (�ִ� FSM)
	void addSubModel(int _key, string& _SlectModel, XMFLOAT4X4 _Mtx, FSM_TYPE _fsmType, float _frame, OBJ_MOVEABLE& _moveAble)
	{
		// ü�ο� �ִ� �� ��� �����Ʈ������ �߰�
		mAllModelData[_SlectModel]->AddModel(_key, _Mtx, _moveAble);

		// �ִ� ��
		mAllModelData[_SlectModel]->SetFSM(_key, _fsmType, _frame);

		// ���̴� �߰�
		addUseShader(_SlectModel);
	}


	// ���� �𵨵� ����
	void EraseSubModel(int _key, string& _SlectModel)
	{
		// ü�ο� �ִ� �� ��� �����Ʈ������ �߰�
		mAllModelData[_SlectModel]->EraseModel(_key);
	}


	// ����� ���̴� �߰�
	void addUseShader(string& _SlectModel)
	{
		SHADER_TYPE _UseShader;
		_UseShader = mAllModelData[_SlectModel]->mShaderMode;
		UsingShaderModel(_UseShader);
	}

	// �� ����ϱ�
	void CalModel()
	{
		for (unsigned int i = 0; i < mUseModel.size(); ++i)
		{
			if (mAllModelData[mUseModel[i]]->mModelType == e_ParsingModel)
				continue;
			mAllModelData[mUseModel[i]]->CalValue();
		}
	}

	// �ؽ�ó �߰��ϱ�
	void AddTex(string _Name, string _TexName, TEXTURE_TYPE _InitTexTpye)
	{
		mAllModelData[_Name]->LoadTex(_TexName, _InitTexTpye);
	}

	// ���� �߰��ϱ�
	void AddBuffer()
	{
		for (unsigned int i = 0; i < mUseAllShader.size(); ++i)
		{
			// ���� ����
			mBufferType[mUseAllShader[i]] = new BufferType(mUseAllShader[i]);

			// ���ۿ� �� ���
			AddModelInBuffer(mUseAllShader[i]);

			// ���� ����
			mBufferType[mUseAllShader[i]]->Build();
		}
	}

	// ���� Ŭ����
	void ClearModelAndBuf()
	{
		for (auto itor = mBufferType.begin(); itor != mBufferType.end(); ++itor)
		{
			itor->second->initValue();
		}
	}

	// ���� Ŭ����
	void ClearBuf()
	{
		for (auto itor = mBufferType.begin(); itor != mBufferType.end(); ++itor)
		{
			itor->second->ClearInsBuf();
		}
	}

	// �� �ν��Ͻ� ���۸� ����
	void ClearModelInsBuf()
	{
		// �� �ν��Ͻ� ���� ����
		for (unsigned int i = 0; i < mUseShader.size(); ++i)
		{
			// ���� ����
			mBufferType[mUseShader[i]]->ClearInsBuf();
		}
	}

	// �� �ν��Ͻ� ���� �����
	void MakeModelInsBuf()
	{
		// �Ϲ� �� �ν��Ͻ� ���� ����
		for (unsigned int i = 0; i < mUseShader.size(); ++i)
		{
			mBufferType[mUseShader[i]]->MakeInsBuf();
		}
	}

	// �� �ν��Ͻ� ���� �����
	void MakeModelInsBuf(string& _name)
	{
		// �Ϲ� �� �ν��Ͻ� ���� ����
		SHADER_TYPE _sdType = mAllModelData[_name]->mShaderMode;
		mBufferType[_sdType]->ReMakeBuf(_name);
	}

	// ������ �ű��
	void DataMove()
	{
		for (unsigned int i = 0; i < mUseShader.size(); ++i)
		{
			// ���ۿ� �� ���
			AddModelInBuffer(mUseShader[i]);
		}
	}

	// ���ۿ� �� �߰��ϱ�
	void AddModelInBuffer(SHADER_TYPE _ShaderMode)
	{
		for (map<string, InitMetaData*>::iterator itor = mAllModelData.begin(); itor != mAllModelData.end(); ++itor)
		{
			// ���̴� ��尡 ���ٸ�, �߰��Ѵ�.
			if (_ShaderMode == itor->second->mShaderMode)
			{
				mBufferType[_ShaderMode]->AddModel(itor->second->mCreateName, itor->second);
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
};

