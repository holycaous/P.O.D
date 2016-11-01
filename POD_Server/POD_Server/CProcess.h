#pragma once
#include "stdafx.h"
namespace ServerProtocol_Func
{
	typedef  void(*Func)(BYTE* _pPacket, DWORD& _Length);
	extern Func g_Protocol_Func[Server_Protocol::SPROTOCOL_FULL];

	//void Connect_Complete_Func(BYTE* _pPacket, DWORD& _Length);
	void DisCunnect(BYTE* _pPacket, DWORD& _Length);
	void Loading_Start_Func(BYTE* _pPacket, DWORD& _Length);
	void Loading_Complete_Func(BYTE* _pPacket, DWORD& _Length);
	void Object_Info_Func(BYTE* _pPacket, DWORD& _Length);
	void ComInfo_Func(BYTE* _pPacket, DWORD& _Length);
	void Char_Cnt_Func(BYTE* _pPacket, DWORD& _Length);
}