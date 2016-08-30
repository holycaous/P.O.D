#pragma once
class cAniManager : public cSingleton<cAniManager>
{
public:
	map<string, map<string, MyBoneData> > mData;

public:
	void ClearClass()
	{
		for (auto itor = mData.begin(); itor != mData.end(); ++itor)
		{
			//for (auto itor2 = mData[itor->first].begin(); itor2 != mData[itor->first].end(); ++itor2)
			//{
			//	itor2->second.ClearClass();
			//}
			mData[itor->first].clear();
		}
		// 어차피 내부 소멸자에서 다 제거해주고 있음
		mData.clear();
	}
};

