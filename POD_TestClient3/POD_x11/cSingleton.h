#pragma once

template <class ClassType>
class cSingleton
{
private:
	static ClassType* m_pInstance;
protected:
	cSingleton() {};
	cSingleton(ClassType& _Instance) {};
	virtual ~cSingleton() {};

public:
	static ClassType* GetInstance()
	{
		if (m_pInstance == nullptr)
		{
			m_pInstance = new ClassType;
		}

		return m_pInstance;
	}

	static void DestroyInstance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}
};

template <typename ClassType>
ClassType * cSingleton<ClassType>::m_pInstance = 0;

