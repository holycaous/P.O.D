#include "StdAfx.h"

cServerMGR*		cServerMGR::m_pInstance = NULL;

cServerMGR::cServerMGR(void)
{

}
cServerMGR::~cServerMGR(void)
{
}

cServerMGR*	cServerMGR::GetInstance(void)
{
	if (m_pInstance == NULL)
	{
		m_pInstance = new cServerMGR;
	}

	return m_pInstance;
}

void		cServerMGR::Destroy(void)
{
	if (m_pInstance != NULL)
	{
		delete m_pInstance;
	}
}
void		cServerMGR::Init(void)
{
	ClientCnt = 0; //클라이언트수 초기화
	PlayerID = MAX_SESSION;
	bGame = FALSE;
}
void		cServerMGR::TerrainInit(CTerrain_Server::InitInfo* Terrain_info)
{
	Terrain_info->s_pHeightMapFilename = L"../Terrain/terrain2_1024x1024.raw";
	Terrain_info->s_fHeightScale = 0;
<<<<<<< HEAD
	Terrain_info->s_nHeightMapDepth = 512;
	Terrain_info->s_nHeightMapWidth = 512;
	Terrain_info->s_nLV = 3;
=======
	Terrain_info->s_nHeightMapDepth = 1024;
	Terrain_info->s_nHeightMapWidth = 1024;
	Terrain_info->s_nLV = 1;
>>>>>>> 708df864ff9e2a39c89366124490a9738b2d496e
	pTerrainServer = new CTerrain_Server(*Terrain_info);

}
void		cServerMGR::MemoryDelete(void)
{
	delete pTerrainServer;
}
void		cServerMGR::ConnectedUserMGRInit(void)
{
}