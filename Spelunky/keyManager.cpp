#include "stdafx.h"
#include "keyManager.h"


keyManager::keyManager()
{
}


keyManager::~keyManager()
{
}

HRESULT keyManager::Init(void)
{
	for (int i = 0; i < KEYMAX; i++)
	{
		this->setKeyUp(i, false);
		this->setKeyDown(i, false);
	}

	return S_OK;
}

void keyManager::Release(void)
{
}

//키가 한번 눌렸는지 체크...
bool keyManager::IsOnceKeyDown(int key)
{
	if (GetAsyncKeyState(key) & 0x8000)
	{
		if (!this->getKeykDown()[key])
		{
			this->setKeyDown(key, true);
			return true;
		}
	}
	else this->setKeyDown(key, false);

	return false;
}

//키를 떼었을때 (한번만)... 
bool keyManager::IsOnceKeyUp(int key)
{
	if (GetAsyncKeyState(key) & 0x8000)
	{
		this->setKeyUp(key, true);
	}
	else
	{
		if (this->getKeyUp()[key])
		{
			this->setKeyUp(key, false);
			return true;
		}
	}

	return false;
}

//계속 눌려있냐?
bool keyManager::IsStayKeyDown(int key)
{
	if (GetAsyncKeyState(key) & 0x8000) return true;
	return false;
}

//토글키...
bool keyManager::IsToggleKey(int key)
{
	if (GetAsyncKeyState(key) & 0x0001) return true;
	return false;
}
