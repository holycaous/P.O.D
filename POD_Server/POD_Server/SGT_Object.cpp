#include "StdAfx.h"
#include "SGT_Object.h"

SGT_Object*		SGT_Object::m_pInstance = NULL;

SGT_Object::SGT_Object(void)
{

}
SGT_Object::~SGT_Object(void)
{
}

SGT_Object*	SGT_Object::GetInstance(void)
{
	if (m_pInstance == NULL)
	{
		m_pInstance = new SGT_Object;
	}

	return m_pInstance;
}

void		SGT_Object::Destroy(void)
{
	if (m_pInstance != NULL)
	{
		delete m_pInstance;
	}
}