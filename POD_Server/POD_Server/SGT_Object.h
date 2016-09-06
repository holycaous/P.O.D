#pragma once
#include <iostream>
class SGT_Object
{
private:
	static SGT_Object* m_pInstance;  //	유일한 인스턴스

private:
	SGT_Object(void);
	~SGT_Object(void);

public:
	static SGT_Object* GetInstance();   //유일한 접근점
	void	Destroy(void);
	
public:
	int ClientCnt;
	int SGT_Charector;
	bool bGame;
}; 