#ifndef TEXT_DATA_MANAGER_H
#define TEXT_DATA_MAANGER_H

#include "singletonBase.h"
#include <string>
#include <vector>

using namespace std;

//=================================================
//================ 20170420 #txtDataLoadSave# =============
//=================================================

class TextDataManager : public singletonBase <TextDataManager>
{
public:
	HRESULT Init(void);
	void Release(void);

	std::vector<vector<std::wstring>> TextLoad(LPCTSTR loadFileName);
	vector<std::wstring> CharArrayLineSeparation(WCHAR charArray[]);
	vector<std::wstring> CharArrayWordSeparation(std::wstring lineStrings, WCHAR *separator);

	TextDataManager();
	~TextDataManager();
};

#endif