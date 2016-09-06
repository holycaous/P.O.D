#include "stdafx.h"

CPacketSession::CPacketSession(VOID)
{
	ZeroMemory(mPacketBuffer, sizeof(mPacketBuffer));
	// CNetworksession �� mReadBuffer���� �����͸� ������ ����
	// ZeroMemory ��� ����
	// memset�� �ʱ�ȭ �̿��� �ٸ� ���ҷε� ��� �ɼ� �ֱ� ������ �ʱ�ȭ �� �� �޸� ����� �߸� �Է��ؼ�
	// �޸� ������ �߻� �� �� ZeroMemory�� �̿��ؼ� �̸� �۾��� ���� ��� �ʱ�ȭ�� ���õ� ������ �Ѳ����� ã�� ���� �ֱ� ������ ����

	mRemainLength = 0; // mPacketBuffer�� ���� ����, �� ��Ŷ�� ó���ϰ� nagle �˰��� ���� �ϼ����� ���� ��Ŷ�� ���� ����

	mCurrentPacketNumber = 0; // ������ ��뿡 ���� �� ����� ��Ŷ ��ȣ
	mLastReadPacketNumber = 0; // ���������� ���� ��Ŷ ��ȣ�� �̰����� ������ ���� ��Ŷ ��ȣ������ ����
}

CPacketSession::~CPacketSession(VOID)
{

}


BOOL CPacketSession::Begin(VOID)
{
	CThreadSync Sync; // ����ȭ ��ü

	// CNetworkSession���� ������ �����͸� ��Ŷ �м� ���� ��Ƶ� ���۸� �ʱ�ȭ
	ZeroMemory(mPacketBuffer, sizeof(mPacketBuffer));

	mRemainLength = 0;
	mCurrentPacketNumber = 0;
	mLastReadPacketNumber = 0;

	if (!mWriteQueue.Begin())
		// ������ �����͸� ������ ť�� �ʱ�ȭ
		return FALSE;

	return CNetworkSession::Begin(); // ��ӹ��� ��ü�� CNetWorkSession�� �ʱ�ȭ
}

BOOL CPacketSession::End(VOID)
{
	CThreadSync Sync;

	if (!mWriteQueue.End())
		return FALSE;

	return CNetworkSession::End();
}

// ������ �Ϸ� �� ť���� �����͸� ���ִ� �Լ�
BOOL CPacketSession::WriteComplete(VOID)
{

	CThreadSync Sync;
	// WriteQueue���� Pop�� ���ָ� ��
	return mWriteQueue.Pop();
}

// ���� ��Ŷ�� �м��ϴ� �Լ�
BOOL CPacketSession::GetPacket(DWORD &protocol, BYTE *packet, DWORD &packetLength)
{
	CThreadSync Sync;

	if (!packet)
		return FALSE;
	if (mRemainLength < sizeof(DWORD))
		// mRemainLength�� mPacketBuffer�� ������ ���̸� ��Ÿ���µ�,
		// 4����Ʈ �̻��� ���� ������ ó���� ���� �ʴ´�.
		return FALSE;

	INT PacketLength = 0;
	CopyMemory(&PacketLength, mPacketBuffer, sizeof(INT));
	// ��Ŷ �տ� 4����Ʈ�� �о� ��Ŷ ���� �˾ƿ�

	// ��Ŷ ���̰� �ִ� ���� ���̸� �Ѱų� 0���� ���� ��� �߸��� ��Ŷ���� �Ǵ��ϰ� ���۸� ����
	if (PacketLength > MAX_BUFFER_LENGTH || PacketLength <= 0) // Invalid Packet
	{
		mRemainLength = 0;
		return FALSE;
	}

	// ���� ��Ŷ ���̰� ���ۿ� �ִ� ���̺��� �۰ų� ���� ��, �� ������ �� ���� ��Ŷ�� �ְų� �� �̻��� ���� ��
	if (PacketLength <= mRemainLength)
	{
		DWORD PacketNumber = 0; 
		DWORD Protocol = 0;

		//��Ŷ ���̿� ��ȣ�� �˾ƿ�
		CopyMemory(&PacketNumber, mPacketBuffer + sizeof(DWORD)/*LENGTH*/, sizeof(DWORD));
		CopyMemory(&Protocol, mPacketBuffer + sizeof(DWORD)/*LENGTH*/ +sizeof(DWORD)/*PACKETNUMBER*/, sizeof(DWORD));

		//�Ű������� �Է�
		protocol = Protocol;
		packetLength = PacketLength - sizeof(DWORD)/*LENGTH*/ -sizeof(DWORD)/*PACKETNUMBER*/ -sizeof(DWORD) /*PROTOCOL*/;


		// packet�� mPacketBuffer�� �ִ� �����͸� ����
		CopyMemory(packet, mPacketBuffer +
			sizeof(DWORD) /*LENGTH*/ +
			sizeof(DWORD) /*PACKETNUMBER*/ +
			sizeof(DWORD) /*PROTOCOL*/,
			packetLength);

		// �� ��Ŷ�� ó���ϰ� ���� ���̰� 0���� Ŭ��� ���� �޸𸮸� ������ ���
		if (mRemainLength - PacketLength > 0){
			MoveMemory(mPacketBuffer, mPacketBuffer + PacketLength, mRemainLength - PacketLength);
		}
		//�� ��Ŷ�� ó���� ��ŭ ���� ���̸� ����
		mRemainLength -= PacketLength;

		//���� ���̰� 0�̰ų� 0���� ���� ��� ����
		if (mRemainLength <= 0){
			mRemainLength = 0;
			ZeroMemory(mPacketBuffer, sizeof(mPacketBuffer));
		}

		//��Ŷ ��ȣ�� ���� ��찡 ���� ��� ���ۿ��� �����ϰ� ����
		if (PacketNumber <= mLastReadPacketNumber)
			//��Ŷ��ȣ�� ���� ���̰ų� ���� ��� (����) �ش� ��Ŷ�� ����
			return FALSE;
		else{
			//���ο� ��Ŷ ��ȣ�� ��� ���������� ��Ŷ�� �����ϴ� ������ ������Ʈ
			mLastReadPacketNumber = PacketNumber;
			return TRUE;
		}
	}

	return FALSE;
}

// CNetworkSession ���� �����͸� �޾ƿ��� �Լ�
// ReadPacketForIocp�� FALSE�� ������ ������ While�� ����
BOOL CPacketSession::ReadPacketForIocp(DWORD readLength)
{
	CThreadSync Sync;

	//CNetworkSession�� �Լ��� �̿��ؼ� mPacketBuffer�� ���� �����͸� �Է�
	if (!CNetworkSession::ReadForIocp(mPacketBuffer + mRemainLength, readLength))
		return FALSE;

	// mPAcketBuffer�� ������ ���̸� ����
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

// ��Ŷ�� ������ �Լ�
BOOL CPacketSession::WritePacket(DWORD protocol, const BYTE *packet, DWORD packetLength)
{
	CThreadSync Sync;

	if (!packet)
		return FALSE;

	//LENGTH(4) | PACKET_NUMBER(4) | PROTOCOL(4) | DATA(4084)
	//��Ŷ ��ü ���̰��
	//��Ŷ ��ü ���� = ��Ŷ ����  + ��Ŷ ������ȣ + �������� + �����ͱ���

	DWORD PacketLength = sizeof(DWORD)+sizeof(DWORD)+sizeof(DWORD)+packetLength;

	//��Ŷ ���̰� �ִ� ���� ���̺��� ��� ����
	if (PacketLength >= MAX_BUFFER_LENGTH)
		return FALSE;

	//��Ŷ ��ȣ�� �ϳ� ����
	mCurrentPacketNumber++;

	//���� �����͸� �̿��ؼ� ��Ŷ�� ����� ���� ����
	BYTE TempBuffer[MAX_BUFFER_LENGTH] = { 0, };

	//��Ŷ�� ����

	//��Ŷ�� ���̸� �Է�
	CopyMemory(TempBuffer, &PacketLength, sizeof(DWORD));

	//��Ŷ ��ȣ�Է�
	CopyMemory(TempBuffer + sizeof(DWORD), &mCurrentPacketNumber, sizeof(DWORD));

	//��Ŷ �������� �Է�
	CopyMemory(TempBuffer + sizeof(DWORD)+sizeof(DWORD), &protocol, sizeof(DWORD));

	//������ �Է�
	CopyMemory(TempBuffer + sizeof(DWORD)+sizeof(DWORD)+sizeof(DWORD), packet, packetLength);


	//WirteQueue�� �̿��Ͽ� ��Ŷ�� ���� �Ϸ�Ǿ��������� �޸𸮸� �����
	BYTE* WriteData = mWriteQueue.Push(this, TempBuffer, PacketLength);
	//ť�� �ְ� ��ȯ�Ǵ� �����ʹ� wirteQueue�� �̸� �Ҵ�� �޸� ������

	return CNetworkSession::Write(WriteData, PacketLength);
	// ť�� ������ �����͸� �־ ����
}