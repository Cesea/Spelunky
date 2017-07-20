#ifndef PROPERTY_BUILDER_H
#define PROPERTY_BUILDER_H

#include "Property.h"

#include "singletonBase.h"

#include "TileInfo.h"

class PropertyBuilder
{
public:
	virtual ~PropertyBuilder() {}
	virtual BaseProperty *Build(const std::wstring &str, const MapTool::PropertyInfo &info) = 0;
};

template<typename T>
class TPropertyBuilder : public PropertyBuilder
{
public:
	virtual ~TPropertyBuilder() {}
	T *Build(const std::wstring &str, const MapTool::PropertyInfo &info) override
	{
		T *result = new T;
		result->Init(info);
		return result;
	}
};

class PropertyFactory : public singletonBase<PropertyFactory>
{
public:
	typedef std::unordered_map<std::wstring, PropertyBuilder *> PropertyBuilderMap;
	typedef std::unordered_map<std::wstring, PropertyBuilder *>::iterator PropertyBuilderMapIter;

	PropertyFactory();
	virtual ~PropertyFactory();

	HRESULT Init();
	void Release();

	BaseProperty *Build(const std::wstring &str, const MapTool::PropertyInfo &info);

private:
	void RegisterBuilders();
	void UnRegisterBuilders();

	void RegisterBuilder(const std::wstring &key, PropertyBuilder *builder);
	void UnRegisterBuilder(const std::wstring &key);

	PropertyBuilderMap _builders;
};


#endif