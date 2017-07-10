#include "stdafx.h"
#include "Console.h"

char Console::buffer[] = {};

Console::Console()
{
}

Console::~Console()
{
	Release();
}

HRESULT Console::Init()
{
	HRESULT result = S_OK;
	if (AllocConsole())
	{
		freopen("CONIN$", "rb", stdin);
		freopen("CONOUT$", "wb", stdout);
		freopen("CONOUT$", "wb", stderr);
		_allocated = true;
	}
	else
	{
		result = E_FAIL;
	}
	return result;
}

void Console::Release()
{
	if (_allocated)
	{
		FreeConsole();
	}
}

void Console::Log(char *str, ...)
{
	ZeroMemory(buffer, sizeof(buffer));
	va_list ap;

	va_start(ap, str);
	vsprintf(buffer + strlen(buffer), str, ap);
	va_end(ap);

	printf(buffer);
}
