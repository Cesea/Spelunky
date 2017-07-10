#include "stdafx.h"
#include "FileManager.h"

FileManager::FileManager()
{
}

FileManager::~FileManager()
{
}

HRESULT FileManager::Init()
{
	return S_OK;
}

void FileManager::Write(const TCHAR *fileName, void *content, DWORD bytesToWrite)
{
	HANDLE file;
	DWORD bytesWritten;

	file = CreateFile(fileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (file)
	{
		if (WriteFile(file, content, bytesToWrite, &bytesWritten, NULL))
		{
//			Assert(bytesWritten == bytesToWrite);
		}
		CloseHandle(file);
	}
}

FileReadResult FileManager::Read(const TCHAR *fileName)
{
	FileReadResult result = {};
	HANDLE file;
	DWORD bytesReaded;

	file = CreateFile(fileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (file)
	{
		DWORD fileSize = GetFileSize(file, NULL);
		result.content = VirtualAlloc(0, fileSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

		if (ReadFile(file, result.content, fileSize, &bytesReaded, NULL) && fileSize == bytesReaded)
		{
			result.fileSize = fileSize;
		}
		else
		{
			VirtualFree(result.content, 0, MEM_RELEASE);
		}
		CloseHandle(file);
	}
	return result;
}