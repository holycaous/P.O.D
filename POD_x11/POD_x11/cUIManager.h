#pragma once
class cUIManager : public cSingleton<cUIManager>
{
	// 1. UI���� ���̴��� �ʿ��ҵ� (.fx ������ ���ϴ°�.. ���̴� ���Ϸ� �������� �ٸ���??)
	// 2. ��ο� �Ŵ������� setLens �ٲ������
	// 3. ���̴� ������ XMFLOAT4X4 mUiView; �־���� ��.
	// 4. UI ��ġ��ų �ʱ���ġ ���� (-y ��ǥ) <-- Cam�� �����Ǿ�����. <-- stdafx ���� define���� �����ұ�..
	// 5. �̰����� UI �浹üũ ����??
	// 6. ������Ʈ ���� UI ������Ѿ��� ��? <-- �ƴϸ� �ϳ��ϳ� ���?

public:
	void Init()
	{
		//XMMatrixOrthographicLH // http://blog.naver.com/masca140/220710652000
	}

	// ������Ʈ �� �浹üũ
	void Update()
	{

	}

	void ClearClass()
	{

	}

};

