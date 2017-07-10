#ifndef DATA_SET_H
#define DATA_SET_H

#include <string>
#include <map>

#include "stdafx.h"

template <typename T>
class DataSet
{
public:
private:

	std::map<std::wstring, T> _mDataList;

public:
	HRESULT Init(void);

	void Release(void);

	void AddData(std::wstring strKey, T t);

	T FindData(std::wstring strKey);

	BOOL DeleteData(std::wstring strKey);

	BOOL DeleteAll(void);

	BOOL GetFileName(std::wstring& strFile);

	DataSet();
	virtual ~DataSet();
};

template<typename T>
inline HRESULT DataSet<T>::Init(void)
{
	return S_OK;
}

template<typename T>
inline void DataSet<T>::Release(void)
{
	DeleteAll();
}

template<typename T>
inline void DataSet<T>::AddData(std::wstring strKey, T t)
{
	T k = FindData(strKey);

	if (k) return;

	_mDataList.insert(std::make_pair(strKey, t));

}

template<typename T>
inline T DataSet<T>::FindData(std::wstring strKey)
{
	std::map<std::wstring, T>::iterator key = _mDataList.find(strKey);
	if (key != _mDataList.end())
	{
		return key->second;
	}
	return NULL;
}

template<typename T>
inline BOOL DataSet<T>::DeleteData(std::wstring strKey)
{
	std::map<std::wstring, T>::iterator key = _mDataList.find(strKey);

	if (key != _mDataList.end())
	{
		key->second->release();

		SAFE_DELETE(key->second);

		_mDataList.erase(key);

		return true;
	}

	return false;
}

template<typename T>
inline BOOL DataSet<T>::DeleteAll(void)
{
	auto iter = _mDataList.begin();

	for (; iter != _mDataList.end();)
	{
		if (iter->second != NULL)
		{
			iter->second->Release();
			delete iter->second;
			iter = _mDataList.erase(iter);
		}
		else
		{
			++iter;
		}
	}

	_mDataList.clear();

	return true;
}

template<typename T>
inline BOOL DataSet<T>::GetFileName(std::wstring & strFile)
{
	if (strFile.empty()) return false;

	size_t pos;

	pos = strFile.find_first_of("/");

	if (pos != std::wstring::npos)
	{
		strFile = strFile.substr(pos + 1, strFile.length());
	}
	else
	{
		pos = strFile.find_last_of("\\");
		if (pos != std::wstring::npos)
		{
			strFile = strFile.substr(pos + 1, strFile.length());
		}
	}

	pos = strFile.find(".");

	if (pos != std::wstring::npos) strFile = strFile.substr(0, pos);
	else return FALSE;

	return TRUE;
}

template<typename T>
inline DataSet<T>::DataSet()
{
}

template<typename T>
inline DataSet<T>::~DataSet()
{
}


#endif

