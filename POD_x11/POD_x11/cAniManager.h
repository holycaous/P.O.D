#pragma once
class cAniManager : public cSingleton<cAniManager>
{
public:
	// 모델 이름, 애니이름, 본 데이터
	map<string, map<string, MyBoneData*> > mData;

public:
	void ClearClass()
	{
		for (auto itor = mData.begin(); itor != mData.end(); ++itor)
		{
			for (auto itor2 = mData[itor->first].begin(); itor2 != mData[itor->first].end(); ++itor2)
			{
				if (itor2->second->mSaveBoneData.size())
					SafeDelete(itor2->second);
			}
			mData[itor->first].clear();
		}
		// 어차피 내부 소멸자에서 다 제거해주고 있음
		mData.clear();
	}
};

