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
		// ������ ���� �Ҹ��ڿ��� �� �������ְ� ����
		mData.clear();
	}
};

