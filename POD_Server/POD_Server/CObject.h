#pragma once
#include "stdafx.h"
class CObject
{
public:
	CObject() 
	{
		m_nClientID = 0;
		m_blive = true;
	};
	virtual ~CObject() {};
protected:
	UINT m_nClientID;

	FLOAT m_posx;
	FLOAT m_posy;
	FLOAT m_posz;
	FLOAT m_Radius;
	FLOAT m_dirX, m_dirY, m_dirZ;
	
	FLOAT m_HP;
	bool m_blive;

	UINT m_NodeNum; //���� ����Ʈ����忡���� ��ġ 

	//�ð�
	DWORD ET = timeGetTime();//����ð�
	DWORD DT = timeGetTime();//����ð�
	float fDt = 0.f; //�����ð�
	float fAccdt = 0.f; //�� �����ð�

public:
	virtual void Update(float _fDTime){};
	virtual void ComUpdate(float _fDTime){};

	void SetClientID(UINT _nID) { m_nClientID = _nID; }
	UINT GetClientID() { return m_nClientID; }

	void SetNodeNum(UINT _NodeNum) { m_NodeNum = _NodeNum; }
	UINT GetNodeNum() { return m_NodeNum; }


	void SetHP(FLOAT hp) { m_HP = hp; }
	FLOAT GetHP() { return m_HP; }
	/*void Setlive(bool i) { m_blive = i; }
	bool Getlive() { return m_blive; }*/

	void SetPosition(int pos_x, int pos_y, int pos_z) { m_posx = pos_x; m_posy = pos_y; m_posz = pos_z; }
	FLOAT  GetPositionX() { return m_posx; }
	FLOAT  GetPositionY() { return m_posy; }
	FLOAT  GetPositionZ() { return m_posz; }

};