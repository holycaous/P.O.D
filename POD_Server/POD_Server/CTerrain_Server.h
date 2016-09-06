//****************************************************************************************
//	CTerrain.h ���� ���� ��� ����.		
//****************************************************************************************
#pragma once
#include "stdafx.h"

///////////////////////////////////////////////////////////////////////////////////////////
//
// CTerrain
//
///////////////////////////////////////////////////////////////////////////////////////////
#define LAYERMAP_MAX 5
class CTerrain_Server
{
public:
	

	struct InitInfo{
		WCHAR* s_pHeightMapFilename;

		UINT s_nHeightMapWidth;
		UINT s_nHeightMapDepth;
		 
		// ���̸� ���Ͽ��� �о���� ���̰��鿡 ������ ��� ���.
		float s_fHeightScale;

		// �������� �� ĭ�� ũ��.
		float s_fCellSpacing; // 1���ؾ� ��ġ�� 1�̵�. 

		int s_nLV; //����  Ʈ������ 
	};

	struct VERTEX
	{
		D3DXVECTOR3 s_vPosition;
		D3DXVECTOR3 s_vNormal;
	};

public:
	CTerrain_Server(InitInfo _Info);
	virtual ~CTerrain_Server();
protected:
	InitInfo m_Info;

	UINT m_nPatch_CntX;
	UINT m_nPatch_CntZ;
	UINT m_nPatch_Offset;

	UINT m_nVtx_CntX;
	UINT m_nVtx_CntZ;
	UINT m_nOrgVtxCnt;

	UINT m_nPolygon_Cnt;

	float* m_HeightMapDataList;

	CTree m_Tree;

	UINT m_nFaceListCntX;
	UINT m_nFaceListCntY;
	vector<FACE> m_vFaceList;  //���̽�-��ġ�� �̷���ִ� �ﰢ�� ��ȣ
	FACE* m_pCurrFaceList;
	UINT m_nCurrPolygonCnt;
	//CNormalLine m_NormalLine;

	vector<VERTEX> m_vVertexList; //��������Ʈ: ���⿡ ���������� ����־���Ѵ�.


protected:
	bool _InitHeightMap();//���̸� �ʱ�ȭ�Լ�
	void _LoadHeightMap();//���� ���������Լ�
	void _HeightSmooth();
	float _Average(float _fX, float _fY);
public:
	// ��ġ�� ���� ���̰� ��ȯ.
	float GetHeight(D3DXVECTOR3& _vObjPos);
	void GetSize(float* _pWidth, float* _pDepth) { *_pWidth = (float)m_Info.s_nHeightMapWidth, *_pDepth = (float)m_Info.s_nHeightMapDepth; }
	
	CTree* GetTree() { return &m_Tree; }

};

//****************************** End of File "CTerrain.h" ******************************//
