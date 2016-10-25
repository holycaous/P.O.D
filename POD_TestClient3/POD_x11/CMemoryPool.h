#pragma once

template <class T, int ALLOC_BLOCK_SIZE = 50>
class CMemoryPool : public CMultiThreadSync<T>
{
public:
	static VOID* operator new(std::size_t allocLength)
		// new를 했을 경우 호출.
	{
		CThreadSync Sync;

		assert(sizeof(T) == allocLength);
		assert(sizeof(T) >= sizeof(UCHAR*));

		// 더 이상 할당할 수 있는 공간이 없을 경우 새로 할당
		if (!mFreePointer)
			allocBlock();

		UCHAR *ReturnPointer = mFreePointer;
		mFreePointer = *reinterpret_cast<UCHAR**>(ReturnPointer);
		// mFreePointer에리턴하는 블록 앞에 4바이트로 있던 주소가 들어간다

		return ReturnPointer;
	}

	static VOID	operator delete(VOID* deletePointer)	// delete를 했을때
	{
		CThreadSync Sync;

		*reinterpret_cast<UCHAR**>(deletePointer) = mFreePointer;
		// delete된 블록의 NEXT에 현재 mFreePointer의 주소를 넣는다.
		mFreePointer = static_cast<UCHAR*>(deletePointer);
		// mFreePointer를 업데이트
	}

private:
	static VOID	allocBlock()
	{
		mFreePointer = new UCHAR[sizeof(T)* ALLOC_BLOCK_SIZE];
		// mFreepoi nterOIl 사용할 크기의 메모리를 할당

		UCHAR **Current = reinterpret_cast<UCHAR **>(mFreePointer);
		// 할당한 메모리의 첫 블록의 포인터를 Current에 넣는다
		UCHAR *Next = mFreePointer;	// 할당된 메모리 첫 블록

		for (INT i = 0; i<ALLOC_BLOCK_SIZE - 1; ++i)
		{
			Next += sizeof(T);		// 다음 블록을 계산
			*Current = Next;		// 할당된 메모리 앞 4바이트에 다음 블록의 주소를 넣는다.
			Current = reinterpret_cast<UCHAR**>(Next);
		}

		*Current = 0;				// 마지막일 경우 앞에 4바이트는 NULL
	}

private:
	static UCHAR	*mFreePointer;

protected:
	~CMemoryPool()
	{
	}
};

template <class T, int ALLOC_BLOCK_SIZE>
UCHAR* CMemoryPool<T, ALLOC_BLOCK_SIZE>::mFreePointer;
