#pragma once
class cUIManager : public cSingleton<cUIManager>
{	
	// �浹üũ, Ŭ�� ����
	map<string, UIObjData> mObjData; // �̸�, ������
	
public:
	void Init()
	{



	}

	void ClearClass()
	{
		mObjData.clear();
	}

	// UI ����
	void addUI(string _name, XMFLOAT3 _Pos, XMFLOAT3 _Center, XMFLOAT3 _Min, XMFLOAT3 _Max, float _Radius, UI_TEX_COL _ColTex = e_disable)
	{
		mObjData[_name].init(_Pos, _Center, _Min, _Max, _Radius, _ColTex);
	}

	// UI �浹üũ (���콺 Ŭ����)
	void colUI()
	{

	}
};

