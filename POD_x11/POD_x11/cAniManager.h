#pragma once
class cAniManager : public cSingleton<cAniManager>
{
public:
	// �� �̸�, �ִ��̸�, �� ������
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
		// ������ ���� �Ҹ��ڿ��� �� �������ְ� ����
		mData.clear();
	}
};

