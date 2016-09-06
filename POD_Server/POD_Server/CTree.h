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
	QD_LT,
	QD_RT,
	QD_LB,
	QD_RB,
	QD_MAX,
};

struct QUAD{
	POINT s_P[QD_MAX];	// ��� �����ϴ� ������ 5��.
	UINT s_F[QD_MAX];	// ��� �����ϴ� ���̽� 5��.
	UINT s_nLV;			// ��� ����.
	UINT s_nNum;		// ���и� ����?
	UINT s_NodeNum;     //��� �ѹ�
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

public:
	void Create(UINT _MaxLv, UINT _CurrLv, UINT _nNum, POINT _Pos[QD_MAX], UINT _nFace[QD_MAX], CTreeNode* _pParent);
	void CreateChild();

	void Release(bool _bClearChild = true);

	// �־��� ��ǥ�� ���� ����� ������ �ִ��� �˻�.
	QUAD* IsIn(POINT vPos);

	static UINT GetNodeTotalCnt() { return m_nNodeTotalCnt; }
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

public:
	int Create(UINT _nMaxLv, POINT _vPos[QD_MAX], UINT _nFace[QD_MAX]); // ������ ���� �ܰ�(Lv1)���� ������.
	void Release(UINT _nLv = 0);

	// ��ǥ�� ���� '���� ����� ������ ����.
	int Find(POINT _Pos, QUAD*& _pQuad);

	CTreeNode* GetTopNode() { return m_pTopNode; }
	void SetTop(CTreeNode* _pNode) { m_pTopNode = _pNode; }
};


//****************************** End of File "CTree.h" ******************************//