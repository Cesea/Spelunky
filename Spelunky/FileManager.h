#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include "singletonBase.h"

#include "stdafx.h"

struct FileReadResult
{
	void *content;
	DWORD fileSize;
};

class FileManager : public singletonBase<FileManager>
{
public :
	FileManager();
	~FileManager();
	  

	HRESULT Init();

	void Write(const TCHAR *fileName, void *content, DWORD bytesToWrite);
	FileReadResult Read(const TCHAR *fileName);

};


#endif