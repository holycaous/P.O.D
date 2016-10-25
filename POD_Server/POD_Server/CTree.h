//*************************************************************************************
//	
//	CTree.h : 4��Ʈ�� ���̽� Ŭ����.
//
//*************************************************************************************
#pragma once
#include "stdafx.h"

////////////////////////////////////////////////////////////////////////////////////////////
//
//	QUAD : �簢 ������ ��� ����ü.
//
////////////////////////////////////////////////////////////////////////////////////////////
enum{
<<<<<<< HEAD
	QD_CN,
=======
>>>>>>> 708df864ff9e2a39c89366124490a9738b2d496e
	QD_LT,
	QD_RT,
	QD_LB,
	QD_RB,
	QD_MAX,
};

struct QUAD{
<<<<<<< HEAD
	POINT s_P[QD_MAX];	// ��� �����ϴ� ������ 4��.
	UINT s_F[QD_MAX];	// ��� �����ϴ� ���̽� 4��.
	UINT s_nLV;			// ��� ����.
	UINT s_nNum;		// ���и� ����?
	UINT s_NodeNum;     //��� �ѹ� 
	FLOAT s_Radius;     //��� ������
=======
	POINT s_P[QD_MAX];	// ��� �����ϴ� ������ 5��.
	UINT s_F[QD_MAX];	// ��� �����ϴ� ���̽� 5��.
	UINT s_nLV;			// ��� ����.
	UINT s_nNum;		// ���и� ����?
	UINT s_NodeNum;     //��� �ѹ�
>>>>>>> 708df864ff9e2a39c89366124490a9738b2d496e
	vector<cMonster_Info*> Monster_arr; //����1 ������Ʈ �迭
};




////////////////////////////////////////////////////////////////////////////////////////////
//
//	CTreeNode 
//
////////////////////////////////////////////////////////////////////////////////////////////
class CTreeNode
{
public:
	CTreeNode();
	~CTreeNode();

private:
	static UINT m_nNodeTotalCnt;
	enum{ CLEAR_CHAILDLIST = true };

protected:
	CTreeNode*	m_pParentNode;	// �θ� ���.
	QUAD*		m_pCurrNode;	// ���� ���.
	CTreeNode* m_pChildNode[4]; // �ڽ� ���( �ѻ� 4��)

	UINT m_nLv;
	UINT m_nMaxLv;

protected:
	void _ClearChild();
	bool _InRect(POINT _Pos, QUAD* _pQuad);
<<<<<<< HEAD
	bool _InCircle(POINT _Pos, float _fRadius, QUAD* _Quad);
=======
>>>>>>> 708df864ff9e2a39c89366124490a9738b2d496e

public:
	void Create(UINT _MaxLv, UINT _CurrLv, UINT _nNum, POINT _Pos[QD_MAX], UINT _nFace[QD_MAX], CTreeNode* _pParent);
	void CreateChild();

	void Release(bool _bClearChild = true);

	// �־��� ��ǥ�� ���� ����� ������ �ִ��� �˻�.
	QUAD* IsIn(POINT vPos);
<<<<<<< HEAD
	QUAD* IsCollision(POINT _vPos, float _Radius);

	static UINT GetNodeTotalCnt() { return m_nNodeTotalCnt; }
	
=======

	static UINT GetNodeTotalCnt() { return m_nNodeTotalCnt; }
>>>>>>> 708df864ff9e2a39c89366124490a9738b2d496e
};




////////////////////////////////////////////////////////////////////////////////////////////
//
//	CTree 
//
////////////////////////////////////////////////////////////////////////////////////////////
class CTree
{
public:
	CTree();
	~CTree();
protected:
	CTreeNode* m_pTopNode; // �ֻ��� ���.

	UINT m_nMaxLv;
<<<<<<< HEAD
	UINT m_nQude_Index; //�浹üũ����迭 �ε���
=======
>>>>>>> 708df864ff9e2a39c89366124490a9738b2d496e

public:
	int Create(UINT _nMaxLv, POINT _vPos[QD_MAX], UINT _nFace[QD_MAX]); // ������ ���� �ܰ�(Lv1)���� ������.
	void Release(UINT _nLv = 0);

	// ��ǥ�� ���� '���� ����� ������ ����.
	int Find(POINT _Pos, QUAD*& _pQuad);

	CTreeNode* GetTopNode() { return m_pTopNode; }
	void SetTop(CTreeNode* _pNode) { m_pTopNode = _pNode; }
<<<<<<< HEAD

	QUAD* Quad_Arr[4];//�浹�� ����Ʈ����� �迭 
	
	//�浹üũ����迭�ε��� Get�Լ�
	UINT Get_Index(){ return m_nQude_Index; }

	//�浹üũ����迭�ε��� �ʱ�ȭ �Լ�
	void Reset_QudeIndex() { m_nQude_Index = 0; }
	
	//�浹üũ����迭�ε��� ���� �Լ�
	void Plus_QudeIndex() {m_nQude_Index++;}

	//�浹üũ����迭�ε��� �̴� �Լ�
	QUAD* GetQudeIndex(int _Index) { return Quad_Arr[_Index]; }
	void SetQuadeIndex(int _index, QUAD* _Quade) { Quad_Arr[_index] = _Quade; }

};
 
=======
};

>>>>>>> 708df864ff9e2a39c89366124490a9738b2d496e

//****************************** End of File "CTree.h" ******************************//