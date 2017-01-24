#pragma once
class cUIManager : public cSingleton<cUIManager>
{	
	// 충돌체크, 클릭 유무
	map<string, UIObjData> mObjData; // 이름, 데이터
	
public:
	void Init()
	{



	}

	void ClearClass()
	{
		mObjData.clear();
	}

	// UI 생성
	void addUI(string _name, XMFLOAT3 _Pos, XMFLOAT3 _Center, XMFLOAT3 _Min, XMFLOAT3 _Max, float _Radius, UI_TEX_COL _ColTex = e_disable)
	{
		mObjData[_name].init(_Pos, _Center, _Min, _Max, _Radius, _ColTex);
	}

	// UI 충돌체크 (마우스 클릭시)
	void colUI()
	{

	}
};

