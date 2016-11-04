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
	UINT m_UniqueCode;//객체를 그릴떄 사용되는 코드.
	bool m_blive;
	 


	UINT m_State;
	float m_HP;

	float m_SearchRange;
	float m_AttackRange;

	float m_Pos_x;
	float m_Pos_y;
	float m_Pos_z;

	bool m_DrawFlag; //클라에서 몬스터를 그리는 플래그
	bool m_Object_Initflag; //오브젝트가 벡터에 입력되었는지 확인하는 변수

	

public:
	virtual void Update(float _fDTime){};
	virtual void ComUpdate(float _fDTime){};

	void SetClientID(UINT _nID) { m_nClientID = _nID; }
	UINT GetClientID() { return m_nClientID; }

	void Setlive(bool i) { m_blive = i; }
	bool Getlive() { return m_blive; }

	void Set_Posx(FLOAT posx) { m_Pos_x = posx; }
	FLOAT Get_Posx() { return m_Pos_x; }
	void Set_Posy(FLOAT posy) { m_Pos_y = posy; }
	FLOAT Get_Posy() { return m_Pos_y; }
	void Set_Posz(FLOAT posz) { m_Pos_z = posz; }
	FLOAT Get_Posz() { return m_Pos_z; }

	void Set_HP(FLOAT Hp) { m_HP = Hp; }
	FLOAT Get_HP() { return m_HP; }

	void Set_UniqueCode(UINT num) { m_UniqueCode = num; }
	UINT Get_UniqueCode() { return m_UniqueCode; }

	void SetDrawFlag(bool i) { m_DrawFlag = i; }
	bool GetDrawFlag() { return m_DrawFlag; }

	void SetObjectInitFlag(bool i) { m_Object_Initflag = i; }
	bool GetObjectInitFlag() { return m_Object_Initflag; }

	//void SetPosition(D3DXVECTOR3* _vPos) { m_vPos = *_vPos; }
	//D3DXVECTOR3 GetPosition() { return m_vPos; }




};