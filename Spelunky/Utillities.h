#ifndef UTILLITIES_H
#define UTILLITIES_H

#include "stdafx.h"


namespace Utils
{
	inline std::wstring GetWorkingDirectory()
	{
		HMODULE hModule = GetModuleHandle(NULL);
		if (!hModule)
		{
			return L"";
		}
		WCHAR path[256];
		GetModuleFileName(hModule, path, sizeof(path));
		PathRemoveFileSpec(path);

		return std::wstring(path) + L"\\";
	}

	inline std::vector<std::pair<std::wstring, bool>> GetFileList(const std::wstring &directory, const std::wstring search = L"*.*",
		bool findDirectoryies = false)
	{
		std::vector<std::pair<std::wstring, bool>> files;

		if (search.empty())
		{
			return files;
		}

		std::wstring path = directory + search;
		WIN32_FIND_DATA data; 
		HANDLE found = FindFirstFile(path.c_str(), &data);
		if (found == INVALID_HANDLE_VALUE)
		{
			return files;
		}
		do
		{
			if (!((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) || findDirectoryies))
			{
				files.emplace_back(std::make_pair(std::wstring(data.cFileName), (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0));
			}
		} while (FindNextFile(found, &data));
		FindClose(found);
		return files;
	}

}

#endif // !UTILLITIES_H
