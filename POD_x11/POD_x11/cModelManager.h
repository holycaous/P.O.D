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
public:
	void Init()
	{
		// ��ũ�� �����
		CreateScreen();

		// �� ��� (�ٸ� ������ �𵨸� 1����)
		CreateBoxModel("BOX1", e_ShaderColor, 0.7f, D3D_PRIMITIVE_TOPOLOGY_LINESTRIP);
		
		CreateBoxModel("BOX2", e_ShaderLight);
		
		CreateBoxModel("BOX3", e_ShaderPongTex);
		AddTex	      ("BOX3", "WoodCrate01_diff.dds", e_DiffuseMap);
		AddTex		  ("BOX3", "WoodCrate01_norm.dds", e_NomalMap);
		AddTex	      ("BOX3", "WoodCrate01_spec.dds", e_SpecularMap);

		// �� �߰�
		CreateModel("Map1", "Export/FinTestMapLoc.pod", e_ShaderPongTex);

		// �� �߰�
		CreateModel("Model0", "Export/FinSkinning_TestLoc.pod", e_ShaderPongTex);
		CreateModel("Model1", "Export/FinAman_boyLoc.pod"     , e_ShaderPongTex);
		CreateModel("Model2", "Export/FinCat1Loc.pod"         , e_ShaderPongTex);
		CreateModel("Model3", "Export/FinAnonSoldierLoc.pod"  , e_ShaderPongTex);
		//CreateModel("Model4", "Export/FinCyclopsLoc.pod"      , e_ShaderPongTex);
		CreateModel("Model5", "Export/FinTestSkinLoc.pod"     , e_ShaderPongTex);

		// �ִ� �߰�
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
	
		// ������� �� ���
		ModelRegistration();
	}

	// Ư�� ���� �� �׸���
	void DrawBone(string _modelName, string _aniName)
	{
		auto Stroage = mAniManager->mData[_modelName][_aniName].GetLapStorage();
		for (unsigned int i = 0; i < Stroage.size(); ++i)
		{
			AddModel("BOX1", Stroage[i]);
		}
	}

	// Ư�� ���� �� �׸���
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

	// �ش� ��ġ�ΰ��� ����
	XMFLOAT3 GetPointDir(int _uniqueCode, string _Name, float _x, float _y, float _z)
	{
		// ü�ο� �ִ� �� �ϳ��� �����ؼ� ���� ����
		XMFLOAT3 ptDir;
		XMStoreFloat3(&ptDir, mAllModelData[mModelChain[_Name][0]]->GetPointDir(_uniqueCode, _x, _y, _z));

		return ptDir;
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
			vector<ObjData> t_ObjDataStorage = mAllModelData[_SlectModel]->getAllObj();
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
		// �� ������ ����
		for (map<string, InitMetaData*>::iterator itor = mAllModelData.begin(); itor != mAllModelData.end(); ++itor)
		{
			// �� ���� ��ü ����
			itor->second->ClearWdMtx();
		}

		// �� �ν��Ͻ� ���� ����
		ClearModelInsBuf();

		// �𵨿� ���Ǿ��� ���̴� ����
		mUseAllShader.clear();
		mUseShader.clear();

		// �� �̸� ����
		mUseModel.clear();

		// ��ũ�� Ŭ����
		mScreen->ClearWdMtx();
		mScreenBuffer->ClearInsBuf();
	}

	// �� �ν��Ͻ� ���۸� ����
	void ClearModelInsBuf()
	{
		// �� �ν��Ͻ� ���� ����
		for (map<int, BufferType*>::iterator itor = mBufferType.begin(); itor != mBufferType.end(); ++itor)
		{
			itor->second->ClearInsBuf();
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
			mXMLParser.LoadXMLModel(tSelectModelRoute, *mAllModelData[tNewName]);

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

	// �� �����ϱ�
	void CreateBoneAni(string _Name, string _BoneRoute, SHADER_TYPE _ShaderMode, D3D_PRIMITIVE_TOPOLOGY _D3D_PRIMITIVE_TOPOLOGY = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
	{
		//----------------------------------------------------//
		// �� ������ �Ľ�
		//----------------------------------------------------//
		// ���� �����ʹ� �ϳ����̱⿡ �̷������� �����ص� ��.
		MyBoneData mMyBoneData;

		// 1�� ���� �� �Ľ�
		mXMLParser.ReadDataMyFormat_Bone(_BoneRoute, &mMyBoneData);

		// ������ ���
		mMyBoneData.CalData();

		// ü�ο� ����� �𵨿� ����
		for (unsigned int i = 0; i < mModelChain[_Name].size(); ++i)
		{
			string SelectName = mModelChain[_Name][i];
			mAllModelData[SelectName]->mAniName = "Idle";
			mAniManager->mData[SelectName][mMyBoneData.mAniName] = mMyBoneData;
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
		_nowModel->mModelType = e_BasicModel;
		_nowModel->mCreateName = _Name;

		// �Ľ� ����
		mXMLParser.LoadBox(*_nowModel, 10.0f, 10.0f, 10.0f);

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
		_nowModel->mModelType = e_BasicModel;
		_nowModel->mCreateName = _Name;

		// �Ľ� ����
		mXMLParser.LoadBox(*_nowModel, _Size, _Size, _Size);

		// ���̴�, �� ��� (���͸� ��)
		UseAllShaderModel(_ShaderMode);
		UsingModel(_Name);
	}

	// ��ũ�� �� �����ϱ�
	void CreateScreenModel(SHADER_TYPE _ShaderMode = e_ShaderDeferred, D3D_PRIMITIVE_TOPOLOGY _D3D_PRIMITIVE_TOPOLOGY = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
	{
		// ���� �� ���� ���
		mScreen = new InitMetaData("GSreen", _ShaderMode, _D3D_PRIMITIVE_TOPOLOGY);

		// ���� ����
		mScreen->mModelType  = e_BasicModel;
		mScreen->mCreateName = "GSreen";

		// �Ľ� ����
		mXMLParser.LoadScreen(*mScreen, 1.0f, 1.0f); // Ǯ ��ũ������

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
		// ���� �ν��Ͻ� ���� ����
		ClearModelInsBuf();

		// �� ���Ͽ� �ִ� ���� �� �� ��ŭ
		for (unsigned int i = 0; i < mModelChain[_Name].size(); ++i)
		{
			// ���� �� �߰�
			addSubModel(mModelChain[_Name][i], _x, _y, _z, _moveAble);
		}

		_Name.clear();

		// �ٽ� �����
		MakeModelInsBuf();
	}	

	// �� �߰��ϱ�
	void AddUpdateModel(string _Name, XMFLOAT4X4 _Mtx, OBJ_MOVEABLE _moveAble = e_StaticObj)
	{
		// ���� �ν��Ͻ� ���� ����
		ClearModelInsBuf();

		// �� ���Ͽ� �ִ� ���� �� �� ��ŭ
		for (unsigned int i = 0; i < mModelChain[_Name].size(); ++i)
		{
			// ���� �� �߰�
			addSubModel(mModelChain[_Name][i], _Mtx, _moveAble);
		}

		_Name.clear();

		// �ٽ� �����
		MakeModelInsBuf();
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

	// ����� ���̴� �߰�
	void addUseShader(string& _SlectModel)
	{
		SHADER_TYPE _UseShader;
		_UseShader = mAllModelData[_SlectModel]->mShaderMode;
		UsingShaderModel(_UseShader);
	}

	// �� �߰��ϱ�
	void AddScreen(float _x, float _y, float _z)
	{
		mScreen->AddModel(_x, _y, _z, e_StaticObj);
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

	// �ν��Ͻ� ���� �����
	void MakeInsbuf()
	{
		// �Ϲ� �� �ν��Ͻ� ���� ����
		MakeModelInsBuf();

		// ��ũ�� �ν��Ͻ� ���� ����
		mScreenBuffer->MakeScreenInsBuf(mScreen);
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

	// ���ۿ� �� �߰��ϱ�
	void AddModelInBuffer(SHADER_TYPE _ShaderMode)
	{
		for (map<string, InitMetaData*>::iterator itor = mAllModelData.begin(); itor != mAllModelData.end(); ++itor)
		{
			// ���̴� ��尡 ���ٸ�, �߰��Ѵ�.
			if (_ShaderMode == itor->second->mShaderMode)
				mBufferType[_ShaderMode]->AddModel(itor->second->mCreateName, itor->second);
		}
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

