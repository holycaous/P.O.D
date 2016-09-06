// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once
#pragma comment(lib, "d3dx9.lib")

//#pragma warning (disable:4996)
#pragma comment(lib,"ws2_32")
#pragma comment(lib,"Mswsock")
#pragma comment(lib,"winmm")

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <mswsock.h>
#include <iostream>
#include <vector>
#include <math.h>
#include <fstream>
#include <D3DX10math.h>
#include <time.h>

///////////////////////
using namespace std;
//////////////////////
#define MAX_CHARECTOR		 20

#define MAX_BUFFER_LENGTH	 4096
#define MAX_QUEUE_LENGTH	 4096
#define MAX_SESSION		10
#define MAX_MONSTER		10


#define DEFAULT_PORT	9876
#define SERVER_IP    "127.0.0.1"

#define SAFE_RELEASE(x) { if (x != NULL) x->Release(); x = NULL; }
#define SAFE_DELETE(x) { if(x != NULL) delete[] x; x = NULL; }
/////////////////////
enum IO_TYPE{
	IO_ACCEPT, // IO_ACCEPT : Accept를 받았을 때
	IO_READ,   // ID_READ : 읽기 작업
	IO_WRITE   // IO_WRITE : 쓰기 작업
};
typedef struct overlapped_ex
{
	OVERLAPPED Overlapped;	// 원래 구조체
	IO_TYPE		IoType;		//IocpType
	VOID		*Object;	// 부모 개체 주소
}OVERLAPPED_EX;

enum UserStatus{
	GAME_ING,
	GAME_END,
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

	CPROTOCOL_FULL,
};

////D3DVECTOR
//typedef struct _D3DVECTOR {
//	_D3DVECTOR() {};
//	_D3DVECTOR(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {};
//
//	float x;
//	float y;
//	float z;
//}D3DXVECTOR3;

struct FACE
{
	UINT s_iIndex[3];
	FACE(){};
	FACE(UINT _0, UINT _1, UINT _2){ s_iIndex[0] = _0, s_iIndex[1] = _1; s_iIndex[2] = _2; }
};
/////////////

#define ServerMGR		cServerMGR::GetInstance() // 싱글톤 오브젝트
#define MonsterMGR		cMonsterMGR::GetInstance()  //싱글톤 오브젝트
#define SERVERPROTOCOL_FUNE		ServerProtocol_Func::
////////////
//#include <Windows.h>
#include "CMultiThreadSync.h"
#include "CCircularQueue.h"
#include "CEventSelect.h"
#include "CNetworkSession.h"
#include "CPacketSession.h"
#include "CIocp.h"
#include "CObject.h"
#include "cPlayer_Info.h"
#include "cMonster_Info.h"
#include "cMonster_1.h"
#include "CConnectedUser.h"
#include "CConnectedUserManager.h"
#include "CTree.h"
#include "CTerrain_Server.h"
#include "CIocpServer.h"
#include "cMonsterMGR.h"
#include "cServerMGR.h"
#include "cPlayer_Info.h"
#include "Server_Protocol.h"


 


// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
 