#pragma once
#include "stdafx.h"
namespace ClientProtocol_Func
{
	typedef  void(*Func)(BYTE* _pPacket, DWORD& _Length);
	extern Func g_Protocol_Func[Client_Protocol::CPROTOCOL_FULL];

	void Connect_Complete_Func(BYTE* _pPacket, DWORD& _Length);
	void DisConnect(BYTE* _pPacket, DWORD& _Length);
	void Loading_Start_Func(BYTE* _pPacket, DWORD& _Length);
	void Player_Count(BYTE* _pPacket, DWORD& _Length);
	void Player_Info(BYTE* _pPacket, DWORD& _Length);
	void Player_Pos(BYTE* _pPacket, DWORD& _Length);
	void Monster_Init(BYTE* _pPacket, DWORD& _Length);
	void Monster_Pos(BYTE* _pPacket, DWORD& _Length);
}