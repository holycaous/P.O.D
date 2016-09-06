//**************************************************************************************
//	
//	CTree.cpp : 4�� Ʈ�� ���� �ҽ� ����.
//
//**************************************************************************************
//#include "CGameEngine.h"
#include "stdafx.h"


UINT CTreeNode::m_nNodeTotalCnt = 0;
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// CTreeNode
//
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
// CTreeNode()
CTreeNode::CTreeNode()
{
	m_pParentNode = NULL;
	m_pCurrNode = NULL;
	ZeroMemory(m_pChildNode, sizeof(CTreeNode*)* 4);
}




//////////////////////////////////////////////////////////////////////////////////////////
// ~CTreeNode()
CTreeNode::~CTreeNode()
{
	_ClearChild();
	SAFE_DELETE(m_pCurrNode);

	m_pParentNode - NULL;

}




//////////////////////////////////////////////////////////////////////////////////////////
// _ClearChild()
void CTreeNode::_ClearChild()
{
	for (UINT i = 0; i < 4; i++){
		if (m_pChildNode[i]){
			m_pChildNode[i]->Release();
		}
	}
}




//////////////////////////////////////////////////////////////////////////////////////////
// _InRect()
bool CTreeNode::_InRect(POINT _Pos, QUAD* _pQuad)
{
	if ((_Pos.x >= _pQuad->s_P[QD_LT].x &&  _Pos.x <= _pQuad->s_P[QD_RT].x) &&
		(_Pos.y >= _pQuad->s_P[QD_LB].y &&  _Pos.y <= _pQuad->s_P[QD_LT].y)
		)
	{
		//�ȿ� ����...ok.
		return TRUE;
	}

	return FALSE;
}





//////////////////////////////////////////////////////////////////////////////////////////
// Create()
void CTreeNode::Create(UINT _nMaxLv, UINT _CurrLv, UINT _nNum, POINT _Pos[QD_MAX], UINT _nFace[QD_MAX], CTreeNode* _pParent)
{
	// ���� ��� ����.
	QUAD* pQuad = new QUAD;
	for (int i = 0; i < QD_MAX; i++){
		pQuad->s_P[i] = _Pos[i];
		pQuad->s_F[i] = _nFace[i];
	}
	pQuad->s_nLV = _CurrLv;  //���緹������
	pQuad->s_nNum = _nNum;  //

	// ���� ��� ���� ���.
	m_pCurrNode = pQuad;
	m_nLv = _CurrLv;
	m_nMaxLv = _nMaxLv;
	m_pParentNode = _pParent;
	++m_nNodeTotalCnt;
	pQuad->s_NodeNum = m_nNodeTotalCnt;
}

//////////////////////////////////////////////////////////////////////////////////////////
// CreateChild()
void CTreeNode::CreateChild()
{

	// ���� ��� 4���. 
	POINT& sp = m_pCurrNode->s_P[QD_LT];
	POINT& ep = m_pCurrNode->s_P[QD_RB];

	UINT& sf = m_pCurrNode->s_F[QD_LT];
	UINT& ef = m_pCurrNode->s_F[QD_RB];

	//fabs()�Լ�- ���밪�� ��ȯ�ϴ� �Լ�      
	float w = fabs((float)m_pCurrNode->s_P[QD_RT].x - m_pCurrNode->s_P[QD_LT].x) / 2;
	float h = fabs((float)m_pCurrNode->s_P[QD_RB].y - m_pCurrNode->s_P[QD_RT].y) / 2;

	int d = ((abs((int)m_pCurrNode->s_F[QD_RT] - (int)m_pCurrNode->s_F[QD_LT])) + 1) * pow(2, m_nLv);
	int fw = (abs((int)m_pCurrNode->s_F[QD_RT] - (int)m_pCurrNode->s_F[QD_LT]) + 1) / 2;
	int fh = (fw / 2);

	POINT s;
	POINT P[4];
	UINT F[4];

	int k = 0;
	UINT f;
	for (int j = 0; j < 2; j++){
		for (int i = 0; i < 2; i++){
			//---------------------------------------------
			// ��ǥ
			//---------------------------------------------
			// ��� ���� ��ǥ �̵�.(���μ��� 1/2 ũ��)

			s.x = sp.x + (i * w);
			s.y = sp.y - (j * h);

			// ����� �� �ڳ� ��ġ.
			P[QD_LT] = s;
			P[QD_RT].x = s.x + w;
			P[QD_RT].y = s.y;
			P[QD_LB].x = s.x;
			P[QD_LB].y = s.y - h;
			P[QD_RB].x = s.x + w;
			P[QD_RB].y = s.y - h;

			//----------------------------------------------
			// ���̽�.
			//----------------------------------------------
			// ����� ���� ���̽�.
			f = (i*fw) + (j * (d * fh));
			f += sf;

			F[QD_LT] = f;
			F[QD_RT] = f + (fw - 1);
			F[QD_LB] = f + (d * (fh - 1));
			F[QD_RB] = (f + (d * (fh - 1))) + fw - 1;

			// ���ϵ� ���
			m_pChildNode[k] = new CTreeNode;
			m_pChildNode[k]->Create(m_nMaxLv, m_nLv + 1, k, P, F, this);

			if (m_nLv + 1 < m_nMaxLv){
				m_pChildNode[k]->CreateChild(); //�ش緹������ �ڽĳ�� ���ȣ��
			}
			++k;
		}
	}
}




//////////////////////////////////////////////////////////////////////////////////////////
// Release()
void CTreeNode::Release(bool _bClearChild)
{
	if (_bClearChild) _ClearChild();

	if (m_pParentNode)
		m_pParentNode->m_pChildNode[m_pCurrNode->s_nNum] = NULL;

	SAFE_DELETE(m_pCurrNode);
}




//////////////////////////////////////////////////////////////////////////////////////////
// IsIn()
QUAD* CTreeNode::IsIn(POINT _Pos)
{
	if (_InRect(_Pos, m_pCurrNode) == false) return NULL;

	QUAD* pQuad = NULL;
	for (int i = 0; i < 4; i++){
		if (m_pChildNode[i] == NULL) continue;

		pQuad = m_pChildNode[i]->IsIn(_Pos);
		if (pQuad) return pQuad;
	}

	return m_pCurrNode;
}






//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// CTree
//
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
// CTree()
CTree::CTree()
{
	m_pTopNode = NULL;
	m_nMaxLv = 0;
}




//////////////////////////////////////////////////////////////////////////////////////////
// ~CTree()
CTree::~CTree()
{
	Release();
}




//////////////////////////////////////////////////////////////////////////////////////////
// Create()
int CTree::Create(UINT _nMaxLv, POINT _vPos[QD_MAX], UINT _nFace[QD_MAX])
{
	m_nMaxLv = _nMaxLv;

	// �ֻ��� ��� ����.
	CTreeNode* pNode = new CTreeNode;
	pNode->Create(_nMaxLv, 0, 0, _vPos, _nFace, NULL);

	m_pTopNode = pNode;

	m_pTopNode->CreateChild();

	return CTreeNode::GetNodeTotalCnt();
}




//////////////////////////////////////////////////////////////////////////////////////////
// Release()
void CTree::Release(UINT _nLv)
{
	m_pTopNode->Release();
}




//////////////////////////////////////////////////////////////////////////////////////////
// Find()
int CTree::Find(POINT _Pos, QUAD*& _pQuad)
{
	_pQuad = m_pTopNode->IsIn(_Pos);
	return(_pQuad) ? true : false;
}









//****************************** End of File "CTree.cpp" ******************************//