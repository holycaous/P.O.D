#pragma once
#include "stdafx.h"
class cSeverMGR
{
private:
	static cSeverMGR* m_pInstance;  //	������ �ν��Ͻ�

protected:
	cSeverMGR(void);
	~cSeverMGR(void);

private:
	CIocpServer pIocpServer;

public:
	static cSeverMGR* GetInstance();   //������ ������	
	CIocpServer* ServerNet() { return &pIocpServer; }
	void    Init(); //�ʱ�ȭ�Լ�
	void	Destroy(void); //�ν��Ͻ� �����Լ�

public:
	int ClientCnt;
	int SGT_Charector;
	bool bGame;
};