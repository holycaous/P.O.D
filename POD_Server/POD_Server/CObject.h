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

	UINT m_NodeNum; //현재 쿼드트리노드에서의 위치 

	//시간
	DWORD ET = timeGetTime();//현재시간
	DWORD DT = timeGetTime();//경과시간
	float fDt = 0.f; //누적시간
	float fAccdt = 0.f; //총 누적시간

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