#include "TextDataManager.h"
#include "stdafx.h"

HRESULT TextDataManager::Init(void)
{
	return S_OK;
}

void TextDataManager::Release(void)
{
}

std::vector<vector<std::wstring>> TextDataManager::TextLoad(LPCTSTR loadFileName)
{
	//HANDLE file;
	//TCHAR str[1024];
	//DWORD read;

	//memset(str, 0, 1024);
	//file = CreateFile(loadFileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	//ReadFile(file, str, 1024, &read, NULL);
	//CloseHandle(file);
	std::vector<std::vector<std::wstring>> result;

	FileReadResult fileReadResult{};
	fileReadResult = FILEMANAGER->Read(loadFileName);

	WCHAR *readWChar = (WCHAR *)fileReadResult.content;

	std::vector<std::wstring> separated = CharArrayLineSeparation(readWChar);

	for (int i = 0; i < separated.size(); ++i)
	{
		std::vector<std::wstring> wordResult = CharArrayWordSeparation(separated[i], L" ");
		result.push_back(wordResult);
	}

	return result;
}

vector<std::wstring> TextDataManager::CharArrayLineSeparation(WCHAR charArray[])
{
	vector<std::wstring> vArray;
	WCHAR* tempChar;
	WCHAR* separator = L"\n";
	WCHAR* token;

	token = wcstok(charArray, separator);

	size_t stringLength = wcslen(token);
	token[stringLength - 1] = 0;

	vArray.push_back(token);

	while (NULL != (token = wcstok(NULL, separator)))
	{
		stringLength = wcslen(token);
		token[stringLength - 1] = 0;
		if ((stringLength = wcslen(token)))
		{
			vArray.push_back(token);
		}
	}

	return vArray;
}

vector<std::wstring> TextDataManager::CharArrayWordSeparation(std::wstring lineStrings, WCHAR *separator)
{
	vector<std::wstring> vArray;
	WCHAR* tempChar;
	//	WCHAR* separator = L"\n";
	WCHAR* token;

	//for (uint32 i = 0; i < lineStrings.size(); ++i)
	//{
	WCHAR *editableString = new WCHAR[lineStrings.size() + 1];
	std::copy(lineStrings.begin(), lineStrings.end(), editableString);
	editableString[lineStrings.size()] = L'\0';

	token = wcstok(editableString, separator);

	//size_t stringLength = wcslen(token);
	//	token[stringLength - 1] = 0;
	vArray.push_back(token);

	while (NULL != (token = wcstok(NULL, separator)))
	{
		vArray.push_back(token);
		//stringLength = wcslen(token);

		//token[stringLength - 1] = 0;
		//if ((stringLength = wcslen(token)))
		//{
		//}
	}
	//}

	return vArray;
}

TextDataManager::TextDataManager()
{
}

TextDataManager::~TextDataManager()
{
}
