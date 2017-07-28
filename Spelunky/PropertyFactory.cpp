#include "stdafx.h""
#include "PropertyFactory.h"

PropertyFactory::PropertyFactory()
{
}

PropertyFactory::~PropertyFactory()
{
}

HRESULT PropertyFactory::Init()
{
	RegisterBuilders();
	return S_OK;
}

void PropertyFactory::Release()
{
	UnRegisterBuilders();
}

BaseProperty * PropertyFactory::Build(const std::wstring &str, const MapTool::PropertyInfo &info)
{
	BaseProperty *result = nullptr;
	PropertyBuilderMapIter &iter = _builders.find(str);
	if (iter != _builders.end())
	{
		result = iter->second->Build(str, info);
	}
	return result;
}

void PropertyFactory::RegisterBuilders()
{
	RegisterBuilder(L"tile", new TPropertyBuilder<TileProperty>());
	RegisterBuilder(L"tunnel", new TPropertyBuilder<TunnelProperty>());
	RegisterBuilder(L"gem", new TPropertyBuilder<GemProperty>());
	RegisterBuilder(L"bomb", new TPropertyBuilder<BombProperty>());
	RegisterBuilder(L"throws", new TPropertyBuilder<ThrowProperty>());
	RegisterBuilder(L"enemy", new TPropertyBuilder<EnemyProperty>());
	RegisterBuilder(L"crate", new TPropertyBuilder<CrateProperty>());
	RegisterBuilder(L"obstacle", new TPropertyBuilder<ObstacleProperty>());
}

void PropertyFactory::UnRegisterBuilders()
{
	UnRegisterBuilder(L"tile");
	UnRegisterBuilder(L"tunnel");
	UnRegisterBuilder(L"gem");
	UnRegisterBuilder(L"throws");
	UnRegisterBuilder(L"enemy");
	UnRegisterBuilder(L"crate");
	UnRegisterBuilder(L"obstacle");
}

void PropertyFactory::RegisterBuilder(const std::wstring & key, PropertyBuilder * builder)
{
	PropertyBuilderMapIter &iter = _builders.find(key);
	if (iter == _builders.end())
	{
		_builders.insert(std::make_pair(key, builder));
	}
	else
	{
		//Console::Log("Property Builder %s already exist", key);
	}
}

void PropertyFactory::UnRegisterBuilder(const std::wstring & key)
{
	PropertyBuilderMapIter &iter = _builders.find(key);
	if (iter != _builders.end())
	{
		_builders.erase(key);
	}
	else
	{
		//Console::Log("Property Builder %s doesn'y exist", key);
	}
}
