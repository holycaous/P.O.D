// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma once
#pragma warning(disable:4996)
#pragma comment(lib,"ws2_32")
#pragma comment(lib,"Mswsock")
#pragma comment(lib,"winmm")

#define DEBUG_MODE

#include <iostream>
#include <algorithm>
#include <vector>
#include <list>
#include <map>
#include <tchar.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <mswsock.h>
#include <math.h>
#include "targetver.h"

using namespace std;

// ������ ������
#define WIN_X 800
#define WIN_Y 600

// ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
#define WIN32_LEAN_AND_MEAN           

// ���� ���
#include "cMathHelper.h"
#include "cXnaHelper.h"
#include "d3dx11Effect.h"

// ����� ���� Ŭ���� 
// << ��ü ���� >>
#include "cSingleton.h"
#include "cCoreStorage.h"

// << ����ü >>
#include "StructData.h"

// << �Ϲ� Ŭ���� >>
#include "cCam.h"
#include "cLightManager.h"
#include "cAniManager.h"
#include "cModelManager.h"
#include "cShaderManager.h"
#include "cMaterialManager.h"
#include "cDrawManager.h"


// <<���� ��ũ�� ������>>
#define MAX_CHARECTOR		 20
#define MAX_BUFFER_LENGTH	 4096
#define MAX_QUEUE_LENGTH	 4096
#define MAX_SESSION		10
#define MAX_MONSTER		10
#define DEFAULT_PORT	9876
#define SERVER_IP    "127.0.0.1"

#define SAFE_RELEASE(x) { if (x != NULL) x->Release(); x = NULL; }
#define SAFE_DELETE(x) { if(x != NULL) delete[] x; x = NULL; }
//<< ���� ����ü >>
enum IO_TYPE{
	IO_ACCEPT, // IO_ACCEPT : Accept�� �޾��� ��
	IO_READ,   // ID_READ : �б� �۾�
	IO_WRITE   // IO_WRITE : ���� �۾�
};
typedef struct overlapped_ex
{
	OVERLAPPED Overlapped;	// ���� ����ü
	IO_TYPE		IoType;		//IocpType
	VOID		*Object;	// �θ� ��ü �ּ�
}OVERLAPPED_EX;

enum UserStatus{
	GAMEING,
};
///////Client->Sever [Protocol]//////////
enum Server_Protocol
{
	SCONNET_COMPLETE = 0,
	SONIODISCONNECT,
	SPUSH_W,
	SPUSH_S,
	SPUSH_A,
	SPUSH_D,


	SPROTOCOL_FULL,
};
///////Server->Client [Porotocol]//////////
enum Client_Protocol
{
	CCONNET_COMPLETE = 0,
	CONIODISCONNECT,
	CSTART_INFO,
	CPlAYER_COUNT,
	CPLAYER_INFO,
	CPLAYER_POS,
	CMONSTER_INIT,
	CMONSTER_POS,
	CMONSTER_MOVE,
	CMONSTER_ATTACK,

	CPROTOCOL_FULL,
};
typedef struct PLAYER_INFO
{
	UINT ClientID;
	float Pos_x;
	float Pos_y;
	float Pos_z;
	float HP;
} PLAYER_INFO;

//<< ���� Ŭ���� >>
#include "CMultiThreadSync.h"
#include "CCircularQueue.h"
#include "CEventSelect.h"
#include "CNetworkSession.h"
#include "CPacketSession.h"
#include "CClientSession.h"
#include "cCla_SeverMGR.h"
#include "cla_Protocol.h"
#include "CObject.h"
#include "cPlayer_Info.h"
#include "cPlayer_InfoManager.h"
#include "cMonster_Info.h"
#include "cMonster_1.h"
#include "cMonster_InfoManger.h"


// <<���� �̱��� �ν��Ͻ�>>
#define Cla_ServerMGR cCla_ServerMGR::Instance()
#define Player_MGR cPlayer_InfoManager::GetInstance()
#define Monster_MGR cMonster_InfoManger::GetInstance()
