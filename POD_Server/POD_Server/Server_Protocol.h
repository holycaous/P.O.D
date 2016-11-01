#pragma once
#include "stdafx.h"
namespace ServerProtocol_Func
{
	typedef  void(*Func)(BYTE* _pPacket, DWORD& _Length);
	extern Func g_Protocol_Func[Server_Protocol::SPROTOCOL_FULL];

	void Connect_Complete_Func(BYTE* _pPacket, DWORD& _Length);
	void DisConnect(BYTE* _pPacket, DWORD& _Length);
	void Player_Push_W(BYTE* _pPacket, DWORD& _Length);
	void Player_Push_S(BYTE* _pPacket, DWORD& _Length);
	void Player_Push_A(BYTE* _pPacket, DWORD& _Length);
	void Player_Push_D(BYTE* _pPacket, DWORD& _Length);
}