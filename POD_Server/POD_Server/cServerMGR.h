#pragma once
#include "stdafx.h"
class cServerMGR
{
private:
	static cServerMGR* m_pInstance;  //	유일한 인스턴스

protected:
	cServerMGR(void);
	~cServerMGR(void);

private:
	CTerrain_Server *pTerrainServer;

public:
	static cServerMGR* GetInstance();   //유일한 접근점	
	CTerrain_Server* GetTerrainServer() { return pTerrainServer; }
	void    Init(); //초기화함수
	void	TerrainInit(CTerrain_Server::InitInfo* TerrainInfo); //맵 초기화 함수
	void	MemoryDelete(void); //메모리 해제함수
	void	Destroy(void); //인스턴스 해제함수
	void    ConnectedUserMGRInit(void);
public:
	int ClientCnt;//현재 클라이언트 접속 인원 
	int PlayerID; //플레이어 ID
	float TerrainWidth; //맵가로길이
	float TerrainDepth; //맵세로길이
	bool bGame; //게임 시작 토큰
	CTerrain_Server::InitInfo Terrain_info; //맵 정보 (크기)

};