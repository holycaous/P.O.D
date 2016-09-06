#pragma once
#include "stdafx.h"
class cSeverMGR
{
private:
	static cSeverMGR* m_pInstance;  //	유일한 인스턴스

protected:
	cSeverMGR(void);
	~cSeverMGR(void);

private:
	CIocpServer pIocpServer;

public:
	static cSeverMGR* GetInstance();   //유일한 접근점	
	CIocpServer* ServerNet() { return &pIocpServer; }
	void    Init(); //초기화함수
	void	Destroy(void); //인스턴스 해제함수

public:
	int ClientCnt;
	int SGT_Charector;
	bool bGame;
};