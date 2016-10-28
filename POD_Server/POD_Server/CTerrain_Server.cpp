//****************************************************************************************
//	
//	CTerrain_Server.cpp 지형 관련 소스 파일.		
//****************************************************************************************
#include "stdafx.h"



///////////////////////////////////////////////////////////////////////////////////////////
// CTerrain()
CTerrain_Server::CTerrain_Server(InitInfo _Info)
{
	m_Info = _Info;

	m_nPatch_CntX = m_Info.s_nHeightMapWidth;
	m_nPatch_CntZ = m_Info.s_nHeightMapDepth;
	m_nPatch_Offset = 1;

	m_nVtx_CntX = 0;
	m_nVtx_CntZ = 0;
	m_nOrgVtxCnt = 0;

	_InitHeightMap();

	m_nPolygon_Cnt = m_nPatch_CntX * m_nPatch_CntZ * 2;

	//===============================================================
	// 트리 초기화.
	//===============================================================
	POINT P[QD_MAX];
	P[QD_LT].x = (long)(-(float)m_nPatch_CntX / 2.f); //x의 길이 (가로의 길이)
	P[QD_LT].y = (long)((float)m_nPatch_CntZ / 2.f);  //z의 길이 (세로)
	P[QD_RT].x = (long)((float)m_nPatch_CntX / 2.f);  
	P[QD_RT].y = (long)((float)m_nPatch_CntZ / 2.f);
	P[QD_LB].x = (long)(-(float)m_nPatch_CntX / 2.f);
	P[QD_LB].y = (long)(-(float)m_nPatch_CntZ / 2.f);
	P[QD_RB].x = (long)((float)m_nPatch_CntX / 2.f);
	P[QD_RB].y = (long)(-(float)m_nPatch_CntZ / 2.f);
	P[QD_CN].x = 0;
	P[QD_CN].y = 0;


	UINT F[QD_MAX];  //FACE= 삼각형 
	int columnCnt = m_nPatch_CntX * 2;
	F[QD_LT] = 0;
	F[QD_RT] = columnCnt - 1;
	F[QD_LB] = columnCnt * (m_nPatch_CntZ - 1);
	F[QD_RB] = columnCnt - 1 + (m_nPatch_CntZ - 1) * columnCnt;
	F[QD_CN] = 0;



	m_Tree.Create(m_Info.s_nLV, P, F); //(맵정보의 트리깊이,꼭짓점,페이스)

	//// FACE 버퍼 초기화. 
	m_nFaceListCntX = ((m_Info.s_nHeightMapWidth * 2) / pow(2, m_Info.s_nLV));
	m_nFaceListCntY = (m_Info.s_nHeightMapDepth / pow(2, m_Info.s_nLV));

	m_nCurrPolygonCnt = m_nFaceListCntX * m_nFaceListCntY;

	m_pCurrFaceList = new FACE[m_nFaceListCntX * m_nFaceListCntY];

}




///////////////////////////////////////////////////////////////////////////////////////////
// ~CTerrain()
CTerrain_Server::~CTerrain_Server()
{
	SAFE_DELETE(m_HeightMapDataList);
	//m_NormalLine.Release();

	if (m_pCurrFaceList) delete[] m_pCurrFaceList;
	m_vVertexList.clear();
	m_vFaceList.clear();
}


///////////////////////////////////////////////////////////////////////////////////////////
// _InitHeightMap()
bool CTerrain_Server::_InitHeightMap()
{ 
	//=======================================================================
	// 텍스처의 정보 가져오기.
	//=======================================================================

	m_nPatch_CntX = m_Info.s_nHeightMapWidth;
	m_nPatch_CntZ = m_Info.s_nHeightMapDepth;
	m_nPatch_Offset = 1;

	m_nVtx_CntX = m_Info.s_nHeightMapWidth + 1; 
	m_nVtx_CntZ = m_Info.s_nHeightMapDepth + 1;

	m_nOrgVtxCnt = m_nVtx_CntX * m_nVtx_CntZ;
	_LoadHeightMap();//높이가져오는함수
	_HeightSmooth(); //높이부드럽게하는함수

	//==============================================================================
	// 메쉬 생성.
	//==============================================================================
	// 정점 리스트
	m_nOrgVtxCnt = (m_nPatch_CntX + 1) * (m_nPatch_CntZ + 1);

	float halfWidth = 0.5f * (float)m_nPatch_CntX;
	float halfDepth = 0.5f * (float)m_nPatch_CntZ;

	float dx = m_nPatch_CntX / ((m_nPatch_CntZ + 1) - 1); //패치의 변화량
	float dz = m_nPatch_CntZ / ((m_nPatch_CntX + 1) - 1);

	m_vVertexList.resize(m_nOrgVtxCnt); 

	for (UINT i = 0; i < (m_nPatch_CntX + 1); ++i){
		float z = halfDepth - i * dz;
		for (UINT j = 0; j < (m_nPatch_CntZ + 1); ++j){
			float x = -halfWidth + j*dx;

			m_vVertexList[i * (m_nPatch_CntZ + 1) + j].s_vPosition = D3DXVECTOR3(x, 0.0f, z);
			
		}
	}

	UINT size = m_vVertexList.size();
	for (UINT i = 0; i < size; ++i){
		m_vVertexList[i].s_vPosition.y = m_HeightMapDataList[i];
	}

	// 페이스 리스트.
	m_vFaceList.resize(m_nPatch_CntX * m_nPatch_CntZ * 2);

	UINT k = 0;
	for (UINT i = 0; i <m_nPatch_CntX; ++i){
		for (UINT j = 0; j < m_nPatch_CntZ; ++j){
			m_vFaceList[k].s_iIndex[0] = i * (m_nPatch_CntZ + 1) + j;
			m_vFaceList[k].s_iIndex[1] = i* (m_nPatch_CntZ + 1) + j + 1;
			m_vFaceList[k].s_iIndex[2] = (i + 1)*(m_nPatch_CntZ + 1) + j;

			m_vFaceList[k + 1].s_iIndex[0] = (i + 1) * (m_nPatch_CntZ + 1) + j;
			m_vFaceList[k + 1].s_iIndex[1] = i*(m_nPatch_CntZ + 1) + j + 1;
			m_vFaceList[k + 1].s_iIndex[2] = (i + 1)*(m_nPatch_CntZ + 1) + j + 1;

			k += 2;
		}
	}
	return true;
}




///////////////////////////////////////////////////////////////////////////////////////////
// _LoadHeightMap()
void CTerrain_Server::_LoadHeightMap()
{
	vector<UCHAR> in(m_Info.s_nHeightMapWidth * m_Info.s_nHeightMapDepth); //높이맵의 정점정보를 저장하는 임시변수

	std::ifstream inFile;
	inFile.open(m_Info.s_pHeightMapFilename, ios_base::binary);

	if (inFile){
		inFile.read((char*)&in[0], (streamsize)in.size());
		inFile.close();
	}

	m_HeightMapDataList = new float[m_nOrgVtxCnt]; //실제데이터가 저장되는 멤버변수

	for (UINT z = 0; z < m_nVtx_CntZ; ++z){
		for (UINT x = 0; x < m_nVtx_CntX; ++x){
			//이전의 값을 가져와 높이맵의 지형을 구현하는 조건문
			if (x == m_nVtx_CntX - 1){
				UINT addr = z * m_nVtx_CntX + (x - 1);
				m_HeightMapDataList[z * m_nVtx_CntX + x] = m_HeightMapDataList[addr]; 
			}
			else if (z == m_nVtx_CntZ - 1){
				UINT addr = (z - 1) * m_nVtx_CntX + x;
				m_HeightMapDataList[z * m_nVtx_CntX + x] = m_HeightMapDataList[addr];
			}
			else{
				UINT addr = z * (m_nVtx_CntX - 1) + x;
				//(in[addr] / 255.0 = 뜻: RGB값으로 높이를 나타내기때문에 255로 나눠 최대높이를 1로 만든후 * scale을 하여 높이를 새로 보정한다.
				m_HeightMapDataList[z * m_nVtx_CntX + x] = (in[addr] / 255.0f) * m_Info.s_fHeightScale;
			}
		}
	}
}




///////////////////////////////////////////////////////////////////////////////////////////
//_HeightSmooth()
void CTerrain_Server::_HeightSmooth()
{
	float* dest = new float[m_nOrgVtxCnt];
	for (UINT i = 0; i < m_nVtx_CntZ; ++i){
		for (UINT j = 0; j < m_nVtx_CntX; ++j){
			dest[i * m_nVtx_CntX + j] = _Average(i, j);
		}
	}
	SAFE_DELETE(m_HeightMapDataList);
	m_HeightMapDataList = dest;
}




///////////////////////////////////////////////////////////////////////////////////////////
//_Average()
float CTerrain_Server::_Average(float _fX, float _fY)
{
	float avg = 0.0f;
	float num = 0.0f;

	for (int i = (int)_fX - 1; i <= (int)_fX + 1; ++i){
		for (int j = (int)_fY - 1; j <= (int)_fY + 1; ++j){
			if (i >= 0 && i < (int)m_nVtx_CntZ &&
				j >= 0 && j < (int)m_nVtx_CntX){
				avg += m_HeightMapDataList[i * m_nVtx_CntX + j];
				num += 1.0f;
			}
		}
	}
	return avg / num;
}

///////////////////////////////////////////////////////////////////////////////////////////
//GetHeight()
float CTerrain_Server::GetHeight(D3DXVECTOR3& _vObjPos)
{
	POINT pos{ _vObjPos.x, _vObjPos.z };
	QUAD* Q;
	m_Tree.Find(pos, Q);
	if (Q == NULL) return 0.0f;
	int k = 0;
	int Width = Q->s_F[QD_RT] - Q->s_F[QD_LT] + 1;
	int Height = Width / 2;


	for (int i = 0; i < Height; ++i){
		for (int j = 0; j < Width; ++j){
			int a = (Q->s_F[QD_LT] + j) + (i*(m_Info.s_nHeightMapWidth * 2));
			m_pCurrFaceList[k] = m_vFaceList[a];
			k++;
		}
	}

	float y = _vObjPos.y;
	D3DXVECTOR3 vPos[3];

	float fdist;

	for (int i = 0; i < m_nCurrPolygonCnt; ++i){
		int i0 = m_pCurrFaceList[i].s_iIndex[0];
		int i1 = m_pCurrFaceList[i].s_iIndex[1];
		int i2 = m_pCurrFaceList[i].s_iIndex[2];

		vPos[0] = m_vVertexList[i0].s_vPosition;
		vPos[1] = m_vVertexList[i1].s_vPosition;
		vPos[2] = m_vVertexList[i2].s_vPosition;

		float u, v;
		if (D3DXIntersectTri(&vPos[0], &vPos[1], &vPos[2],
			&(_vObjPos + D3DXVECTOR3(0, 100, 0)),
			&D3DXVECTOR3(0, -1, 0),
			&u, &v, &fdist)){
			y = vPos[0].y + (u * (vPos[1].y - vPos[0].y)) + (v * (vPos[2].y - vPos[0].y));
			break;
		}
	}
	return y;
}
