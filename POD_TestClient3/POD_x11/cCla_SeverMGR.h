//**********************************************************************************
//	Cla_ServerMGR.h :Ŭ���̾�Ʈ ���� ���� ��� ����.
//
//	'�̱���'���� ����.
//	������ ���������� ����ü �� ���� ���� �Ŵ���
//**********************************************************************************
#pragma once
#include "stdafx.h"

class cCla_ServerMGR
{
private:
	static cCla_ServerMGR* m_pInstance;
protected:
	cCla_ServerMGR();
	~cCla_ServerMGR();
public:
	static cCla_ServerMGR* Instance(){
		if (m_pInstance == NULL)
			m_pInstance = new cCla_ServerMGR;
		return m_pInstance;
	}
private:
	cGameTimer m_Timer;
	bool m_bActivate;
	bool m_bCollisionBox;

	//��Ʈ��ũ ����
	CClientSession pClientNet;
	TCHAR SvIPAddr[128];
	TCHAR PtNumber[128];


public:
	
	cGameTimer& Timer() { return m_Timer; }
	CClientSession* ClientNet() { return &pClientNet; }
	TCHAR*   ServerIPAddr() { return SvIPAddr; }
	TCHAR*   PortNumber() { return PtNumber; }
	bool& Activate() { return m_bActivate; }
	bool& CollisionBox() { return m_bCollisionBox; }
	void Init();

	void Release() { if (m_pInstance != NULL) delete m_pInstance; }
};