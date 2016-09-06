#pragma once 
#include "CCriticalSection.h"
template <class T>
class CMultiThreadSync
{
	friend class CThreadSync;
private:
	static CCriticalSection Sync; //CriticalSection  객체
public:
	class CThreadSync
	{
	public:
		CThreadSync(void)
		{
			T::Sync.Enter(); // 해당 클래스의 동기화 개체 Enter() 호출
		}
		~CThreadSync(void)
		{
			T::Sync.Leave(); // 해당 클래스의 동기화 개체 Leave() 호출
		}
	};
};

template <class T>
CCriticalSection CMultiThreadSync<T>::Sync;
