//****************************************************************************************
//	CTerrain.h 지형 관련 헤더 파일.		
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
		 
		// 높이맵 파일에서 읽어들인 높이값들에 적용할 비례 계수.
		float s_fHeightScale;

		// 지형격자 각 칸의 크기.
		float s_fCellSpacing; // 1로해야 패치가 1이됨. 

		int s_nLV; //맵의  트리깊이 
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
	vector<FACE> m_vFaceList;  //페이스-패치를 이루고있는 삼각형 번호
	FACE* m_pCurrFaceList;
	UINT m_nCurrPolygonCnt;
	//CNormalLine m_NormalLine;

	vector<VERTEX> m_vVertexList; //정점리스트: 여기에 정점정보를 집어넣어야한다.


protected:
	bool _InitHeightMap();//높이맵 초기화함수
	void _LoadHeightMap();//높이 가져오는함수
	void _HeightSmooth();
	float _Average(float _fX, float _fY);
public:
	// 위치에 따른 높이값 반환.
	float GetHeight(D3DXVECTOR3& _vObjPos);
	void GetSize(float* _pWidth, float* _pDepth) { *_pWidth = (float)m_Info.s_nHeightMapWidth, *_pDepth = (float)m_Info.s_nHeightMapDepth; }
	
	CTree* GetTree() { return &m_Tree; }

};

//****************************** End of File "CTerrain.h" ******************************//
