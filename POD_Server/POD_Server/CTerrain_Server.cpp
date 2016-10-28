//****************************************************************************************
//	
//	CTerrain_Server.cpp ���� ���� �ҽ� ����.		
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
	// Ʈ�� �ʱ�ȭ.
	//===============================================================
	POINT P[QD_MAX];
	P[QD_LT].x = (long)(-(float)m_nPatch_CntX / 2.f); //x�� ���� (������ ����)
	P[QD_LT].y = (long)((float)m_nPatch_CntZ / 2.f);  //z�� ���� (����)
	P[QD_RT].x = (long)((float)m_nPatch_CntX / 2.f);  
	P[QD_RT].y = (long)((float)m_nPatch_CntZ / 2.f);
	P[QD_LB].x = (long)(-(float)m_nPatch_CntX / 2.f);
	P[QD_LB].y = (long)(-(float)m_nPatch_CntZ / 2.f);
	P[QD_RB].x = (long)((float)m_nPatch_CntX / 2.f);
	P[QD_RB].y = (long)(-(float)m_nPatch_CntZ / 2.f);
	P[QD_CN].x = 0;
	P[QD_CN].y = 0;


	UINT F[QD_MAX];  //FACE= �ﰢ�� 
	int columnCnt = m_nPatch_CntX * 2;
	F[QD_LT] = 0;
	F[QD_RT] = columnCnt - 1;
	F[QD_LB] = columnCnt * (m_nPatch_CntZ - 1);
	F[QD_RB] = columnCnt - 1 + (m_nPatch_CntZ - 1) * columnCnt;
	F[QD_CN] = 0;



	m_Tree.Create(m_Info.s_nLV, P, F); //(�������� Ʈ������,������,���̽�)

	//// FACE ���� �ʱ�ȭ. 
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
	// �ؽ�ó�� ���� ��������.
	//=======================================================================

	m_nPatch_CntX = m_Info.s_nHeightMapWidth;
	m_nPatch_CntZ = m_Info.s_nHeightMapDepth;
	m_nPatch_Offset = 1;

	m_nVtx_CntX = m_Info.s_nHeightMapWidth + 1; 
	m_nVtx_CntZ = m_Info.s_nHeightMapDepth + 1;

	m_nOrgVtxCnt = m_nVtx_CntX * m_nVtx_CntZ;
	_LoadHeightMap();//���̰��������Լ�
	_HeightSmooth(); //���̺ε巴���ϴ��Լ�

	//==============================================================================
	// �޽� ����.
	//==============================================================================
	// ���� ����Ʈ
	m_nOrgVtxCnt = (m_nPatch_CntX + 1) * (m_nPatch_CntZ + 1);

	float halfWidth = 0.5f * (float)m_nPatch_CntX;
	float halfDepth = 0.5f * (float)m_nPatch_CntZ;

	float dx = m_nPatch_CntX / ((m_nPatch_CntZ + 1) - 1); //��ġ�� ��ȭ��
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

	// ���̽� ����Ʈ.
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
	vector<UCHAR> in(m_Info.s_nHeightMapWidth * m_Info.s_nHeightMapDepth); //���̸��� ���������� �����ϴ� �ӽú���

	std::ifstream inFile;
	inFile.open(m_Info.s_pHeightMapFilename, ios_base::binary);

	if (inFile){
		inFile.read((char*)&in[0], (streamsize)in.size());
		inFile.close();
	}

	m_HeightMapDataList = new float[m_nOrgVtxCnt]; //���������Ͱ� ����Ǵ� �������

	for (UINT z = 0; z < m_nVtx_CntZ; ++z){
		for (UINT x = 0; x < m_nVtx_CntX; ++x){
			//������ ���� ������ ���̸��� ������ �����ϴ� ���ǹ�
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
				//(in[addr] / 255.0 = ��: RGB������ ���̸� ��Ÿ���⶧���� 255�� ���� �ִ���̸� 1�� ������ * scale�� �Ͽ� ���̸� ���� �����Ѵ�.
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
