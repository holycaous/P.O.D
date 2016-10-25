#pragma once
#include <iostream>
class SGT_Object
{
private:
	static SGT_Object* m_pInstance;  //	������ �ν��Ͻ�

private:
	SGT_Object(void);
	~SGT_Object(void);

public:
	static SGT_Object* GetInstance();   //������ ������
	void	Destroy(void);
	
public:
	int ClientCnt;
	int SGT_Charector;
	bool bGame;
}; 