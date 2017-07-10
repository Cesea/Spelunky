#ifndef CONSOLE_H
#define CONSOLE_H

#include "stdafx.h"
#include <stdarg.h>
#include <stdio.h>

class Console
{
public:
	Console();
	virtual ~Console();

	HRESULT Init();
	void Release();

	static void Log(char *str, ...);
private:

	static char buffer[128];

	bool _allocated{ false };
};

#endif