// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
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

// 윈도우 사이즈
#define WIN_X 800
#define WIN_Y 600

// 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
#define WIN32_LEAN_AND_MEAN           

// 보조 헤더
#include "cMathHelper.h"
#include "cXnaHelper.h"
#include "d3dx11Effect.h"

// 사용자 정의 클래스 
// << 전체 공통 >>
#include "cSingleton.h"
#include "cCoreStorage.h"

// << 구조체 >>
#include "StructData.h"

// << 일반 클래스 >>
#include "cCam.h"
#include "cLightManager.h"
#include "cAniManager.h"
#include "cModelManager.h"
#include "cShaderManager.h"
#include "cMaterialManager.h"
#include "cDrawManager.h"


// <<서버 매크로 지정자>>
#define MAX_CHARECTOR		 20
#define MAX_BUFFER_LENGTH	 4096
#define MAX_QUEUE_LENGTH	 4096
#define MAX_SESSION		10
#define MAX_MONSTER		10
#define DEFAULT_PORT	9876
#define SERVER_IP    "127.0.0.1"

#define SAFE_RELEASE(x) { if (x != NULL) x->Release(); x = NULL; }
#define SAFE_DELETE(x) { if(x != NULL) delete[] x; x = NULL; }
//<< 서버 구조체 >>
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

//<< 서버 클래스 >>
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


// <<서버 싱글톤 인스턴스>>
#define Cla_ServerMGR cCla_ServerMGR::Instance()
#define Player_MGR cPlayer_InfoManager::GetInstance()
#define Monster_MGR cMonster_InfoManger::GetInstance()
