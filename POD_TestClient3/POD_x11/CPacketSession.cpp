#include "stdafx.h"

CPacketSession::CPacketSession(VOID)
{
	ZeroMemory(mPacketBuffer, sizeof(mPacketBuffer));
	// CNetworksession 의 mReadBuffer에서 데이터를 가져올 버퍼
	// ZeroMemory 사용 이유
	// memset은 초기화 이외의 다른 역할로도 사용 될수 있기 때문에 초기화 할 때 메모리 사이즈를 잘못 입력해서
	// 메모리 문제가 발생 할 때 ZeroMemory를 이용해서 미리 작업해 놓을 경우 초기화에 관련된 내용을 한꺼번에 찾아 볼수 있기 때문에 편리함

	mRemainLength = 0; // mPacketBuffer에 남은 길이, 즉 패킷을 처리하고 nagle 알고리즘에 의해 완성되지 않은 패킷이 남은 길이

	mCurrentPacketNumber = 0; // 현재의 상대에 보낼 때 사용할 때킷 번호
	mLastReadPacketNumber = 0; // 마지막으로 받은 패킷 번호， 이것으로 기존에 보낸 패킷 번호인지를 검증
}

CPacketSession::~CPacketSession(VOID)
{

}


BOOL CPacketSession::Begin(VOID)
{
	CThreadSync Sync; // 동기화 개체

	// CNetworkSession에서 가져온 데이터를 패킷 분석 전에 담아둘 버퍼를 초기화
	ZeroMemory(mPacketBuffer, sizeof(mPacketBuffer));

	mRemainLength = 0;
	mCurrentPacketNumber = 0;
	mLastReadPacketNumber = 0;

	if (!mWriteQueue.Begin())
		// 보내는 데이터를 지정할 큐를 초기화
		return FALSE;

	return CNetworkSession::Begin(); // 상속받은 개체인 CNetWorkSession을 초기화
}

BOOL CPacketSession::End(VOID)
{
	CThreadSync Sync;

	if (!mWriteQueue.End())
		return FALSE;

	return CNetworkSession::End();
}

// 보내기 완료 시 큐에서 데이터를 빼주는 함수
BOOL CPacketSession::WriteComplete(VOID)
{

	CThreadSync Sync;
	// WriteQueue에서 Pop을 해주면 됨
	return mWriteQueue.Pop();
}

// 받은 패킷을 분석하는 함수
BOOL CPacketSession::GetPacket(DWORD &protocol, BYTE *packet, DWORD &packetLength)
{
	CThreadSync Sync;

	if (!packet)
		return FALSE;
	if (mRemainLength < sizeof(DWORD))
		// mRemainLength는 mPacketBuffer에 데이터 길이를 나타내는데,
		// 4바이트 이상을 받지 않으면 처리가 되지 않는다.
		return FALSE;

	INT PacketLength = 0;
	CopyMemory(&PacketLength, mPacketBuffer, sizeof(INT));
	// 패킷 앞에 4바이트를 읽어 패킷 길이 알아옴

	// 패킷 길이가 최대 버퍼 길이를 넘거나 0보다 작은 경우 잘못된 패킷으로 판단하고 버퍼를 지움
	if (PacketLength > MAX_BUFFER_LENGTH || PacketLength <= 0) // Invalid Packet
	{
		mRemainLength = 0;
		return FALSE;
	}

	// 읽은 패킷 길이가 버퍼에 있는 길이보다 작거나 같을 때, 즉 온전한 한 개의 패킷이 있거나 그 이상이 있을 때
	if (PacketLength <= mRemainLength)
	{
		DWORD PacketNumber = 0; 
		DWORD Protocol = 0;

		//패킷 길이와 번호를 알아옴
		CopyMemory(&PacketNumber, mPacketBuffer + sizeof(DWORD)/*LENGTH*/, sizeof(DWORD));
		CopyMemory(&Protocol, mPacketBuffer + sizeof(DWORD)/*LENGTH*/ +sizeof(DWORD)/*PACKETNUMBER*/, sizeof(DWORD));

		//매개변수에 입력
		protocol = Protocol;
		packetLength = PacketLength - sizeof(DWORD)/*LENGTH*/ -sizeof(DWORD)/*PACKETNUMBER*/ -sizeof(DWORD) /*PROTOCOL*/;


		// packet에 mPacketBuffer에 있는 데이터를 복사
		CopyMemory(packet, mPacketBuffer +
			sizeof(DWORD) /*LENGTH*/ +
			sizeof(DWORD) /*PACKETNUMBER*/ +
			sizeof(DWORD) /*PROTOCOL*/,
			packetLength);

		// 한 패킷을 처리하고 남은 길이가 0보다 클경우 뒤의 메모리를 앞으로 당김
		if (mRemainLength - PacketLength > 0){
			MoveMemory(mPacketBuffer, mPacketBuffer + PacketLength, mRemainLength - PacketLength);
		}
		//한 패킷을 처리한 만큼 남은 길이를 줄임
		mRemainLength -= PacketLength;

		//남은 길이가 0이거나 0보다 작은 경우 리셋
		if (mRemainLength <= 0){
			mRemainLength = 0;
			ZeroMemory(mPacketBuffer, sizeof(mPacketBuffer));
		}

		//패킷 번호가 같은 경우가 왔을 경우 버퍼에서 제외하고 무시
		if (PacketNumber <= mLastReadPacketNumber)
			//패킷번호가 이전 것이거나 같을 경우 (복사) 해당 패킷은 무시
			return FALSE;
		else{
			//새로운 패킷 번호일 경우 마지막받은 패킷을 저장하는 변수를 업데이트
			mLastReadPacketNumber = PacketNumber;
			return TRUE;
		}
	}

	return FALSE;
}

// CNetworkSession 에서 데이터를 받아오는 함수
// ReadPacketForIocp는 FALSE가 떨어질 때까지 While문 진행
BOOL CPacketSession::ReadPacketForIocp(DWORD readLength)
{
	CThreadSync Sync;

	//CNetworkSession의 함수를 이용해서 mPacketBuffer에 받은 데이터를 입력
	if (!CNetworkSession::ReadForIocp(mPacketBuffer + mRemainLength, readLength))
		return FALSE;

	// mPAcketBuffer에 데이터 길이를 증가
	mRemainLength += readLength;

	return TRUE;
}

BOOL CPacketSession::ReadPacketForEventSelect(VOID)
{

	CThreadSync Sync;

	DWORD ReadLength = 0;

	if (!CNetworkSession::ReadForEventSelect(mPacketBuffer + mRemainLength, ReadLength))
		return FALSE;

	mRemainLength += ReadLength;

	//return getPacket(protocol, packet, packetLength);
	return TRUE;

}

// 패킷을 보내는 함수
BOOL CPacketSession::WritePacket(DWORD protocol, const BYTE *packet, DWORD packetLength)
{
	CThreadSync Sync;

	if (!packet)
		return FALSE;

	//LENGTH(4) | PACKET_NUMBER(4) | PROTOCOL(4) | DATA(4084)
	//패킷 전체 길이계산
	//패킷 전체 길이 = 패킷 길이  + 패킷 고유번호 + 프로토콜 + 데이터길이

	DWORD PacketLength = sizeof(DWORD)+sizeof(DWORD)+sizeof(DWORD)+packetLength;

	//패킷 길이가 최대 버퍼 길이보다 길면 실패
	if (PacketLength >= MAX_BUFFER_LENGTH)
		return FALSE;

	//패킷 번호를 하나 증가
	mCurrentPacketNumber++;

	//들어온 데이터를 이용해서 패킷을 만들기 위한 버퍼
	BYTE TempBuffer[MAX_BUFFER_LENGTH] = { 0, };

	//패킷을 만듬

	//패킷에 길이를 입력
	CopyMemory(TempBuffer, &PacketLength, sizeof(DWORD));

	//패킷 번호입력
	CopyMemory(TempBuffer + sizeof(DWORD), &mCurrentPacketNumber, sizeof(DWORD));

	//패킷 프로토콜 입력
	CopyMemory(TempBuffer + sizeof(DWORD)+sizeof(DWORD), &protocol, sizeof(DWORD));

	//데이터 입력
	CopyMemory(TempBuffer + sizeof(DWORD)+sizeof(DWORD)+sizeof(DWORD), packet, packetLength);


	//WirteQueue를 이용하여 패킷이 전송 완료되었을때까지 메모리를 살려둠
	BYTE* WriteData = mWriteQueue.Push(this, TempBuffer, PacketLength);
	//큐에 넣고 반환되는 포인터는 wirteQueue에 미리 할당된 메모리 포인터

	return CNetworkSession::Write(WriteData, PacketLength);
	// 큐의 데이터 포인터를 넣어서 보냄
}