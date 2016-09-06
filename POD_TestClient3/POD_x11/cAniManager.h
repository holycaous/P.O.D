#pragma once
class cAniManager : public cSingleton<cAniManager>
{
public:
	map<string, MyBoneData> mData;

public:
	void ClearClass()
	{
		// 어차피 내부 소멸자에서 다 제거해주고 있음
		mData.clear();
	}
};

