#pragma once

#include "singletonBase.h"
#include <bitset>

using namespace std;

#define KEYMAX 256

class keyManager : public singletonBase<keyManager>
{
private:
	std::bitset<KEYMAX> _keyUp;
	std::bitset<KEYMAX> _keyDown;

public:
	HRESULT Init(void);
	void Release(void);

	bool IsOnceKeyDown(int key);
	bool IsOnceKeyUp(int key);
	bool IsStayKeyDown(int key);
	bool IsToggleKey(int key);

	bitset<KEYMAX> getKeyUp() { return _keyUp; }
	bitset<KEYMAX> getKeykDown() { return _keyDown; }

	void setKeyDown(int key, bool state) { _keyDown.set(key, state); }
	void setKeyUp(int key, bool state) { _keyUp.set(key, state); }

	keyManager();
	virtual ~keyManager();
};

