//*************************************************************************************
//	
//	CTree.h : 4진트리 베이스 클래스.
//
//*************************************************************************************
#pragma once
#include "stdafx.h"

////////////////////////////////////////////////////////////////////////////////////////////
//
//	QUAD : 사각 정보를 담는 구조체.
//
////////////////////////////////////////////////////////////////////////////////////////////
enum{
	QD_CN,
	QD_LT,
	QD_RT,
	QD_LB,
	QD_RB,
	QD_MAX,
};

struct QUAD{
	POINT s_P[QD_MAX];	// 노드 구성하는 꼭지점 4개.
	UINT s_F[QD_MAX];	// 노드 구성하는 페이스 4개.
	UINT s_nLV;			// 노드 레벨.
	UINT s_nNum;		// 몇사분면 쿼드?
	UINT s_NodeNum;     //노드 넘버 
	FLOAT s_Radius;     //노드 반지름
	//vector<cMonster_Info*> Monster_arr; //몬스터1 오브젝트 배열
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
	CTreeNode*	m_pParentNode;	// 부모 노드.
	QUAD*		m_pCurrNode;	// 현재 노드.
	CTreeNode* m_pChildNode[4]; // 자식 노드( 한상 4개)

	UINT m_nLv;
	UINT m_nMaxLv;

protected:
	void _ClearChild();
	bool _InRect(POINT _Pos, QUAD* _pQuad);
	bool _InCircle(POINT _Pos, float _fRadius, QUAD* _Quad);

public:
	void Create(UINT _MaxLv, UINT _CurrLv, UINT _nNum, POINT _Pos[QD_MAX], UINT _nFace[QD_MAX], CTreeNode* _pParent);
	void CreateChild();

	void Release(bool _bClearChild = true);

	// 주어진 좌표가 현재 노드의 영역에 있는지 검사.
	QUAD* IsIn(POINT vPos);
	QUAD* IsCollision(POINT _vPos, float _Radius);

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
	CTreeNode* m_pTopNode; // 최상위 노드.

	UINT m_nMaxLv;
	UINT m_nQude_Index; //충돌체크쿼드배열 인덱스

public:
	int Create(UINT _nMaxLv, POINT _vPos[QD_MAX], UINT _nFace[QD_MAX]); // 영역을 일정 단계(Lv1)까지 나누기.
	void Release(UINT _nLv = 0);

	// 좌표를 기준 '속한 노드의 정보를 리턴.
	int Find(POINT _Pos, QUAD*& _pQuad);

	CTreeNode* GetTopNode() { return m_pTopNode; }
	void SetTop(CTreeNode* _pNode) { m_pTopNode = _pNode; }

	QUAD* Quad_Arr[4];//충돌된 쿼드트리노드 배열 
	
	//충돌체크쿼드배열인덱스 Get함수
	UINT Get_Index(){ return m_nQude_Index; }

	//충돌체크쿼드배열인덱스 초기화 함수
	void Reset_QudeIndex() { m_nQude_Index = 0; }
	
	//충돌체크쿼드배열인덱스 증가 함수
	void Plus_QudeIndex() {m_nQude_Index++;}

	//충돌체크쿼드배열인덱스 뽑는 함수
	QUAD* GetQudeIndex(int _Index) { return Quad_Arr[_Index]; }
	void SetQuadeIndex(int _index, QUAD* _Quade) { Quad_Arr[_index] = _Quade; }

};
 


//****************************** End of File "CTree.h" ******************************//