#pragma once
class cAniManager : public cSingleton<cAniManager>
{
public:
	map<string, MyBoneData> mData;

public:
	void ClearClass()
	{
		// ������ ���� �Ҹ��ڿ��� �� �������ְ� ����
		mData.clear();
	}
};

