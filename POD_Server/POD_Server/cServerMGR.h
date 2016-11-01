#pragma once
#include "stdafx.h"
class cServerMGR
{
private:
	static cServerMGR* m_pInstance;  //	������ �ν��Ͻ�

protected:
	cServerMGR(void);
	~cServerMGR(void);

private:
	CTerrain_Server *pTerrainServer;

public:
	static cServerMGR* GetInstance();   //������ ������	
	CTerrain_Server* GetTerrainServer() { return pTerrainServer; }
	void    Init(); //�ʱ�ȭ�Լ�
	void	TerrainInit(CTerrain_Server::InitInfo* TerrainInfo); //�� �ʱ�ȭ �Լ�
	void	MemoryDelete(void); //�޸� �����Լ�
	void	Destroy(void); //�ν��Ͻ� �����Լ�
	void    ConnectedUserMGRInit(void);
public:
	int ClientCnt;//���� Ŭ���̾�Ʈ ���� �ο� 
	int PlayerID; //�÷��̾� ID
	float TerrainWidth; //�ʰ��α���
	float TerrainDepth; //�ʼ��α���
	bool bGame; //���� ���� ��ū
	CTerrain_Server::InitInfo Terrain_info; //�� ���� (ũ��)

};