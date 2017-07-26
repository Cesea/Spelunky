#include "stdafx.h"
#include "RankingManager.h"

RankingManager::RankingManager()
{
}

RankingManager::~RankingManager()
{
}

HRESULT RankingManager::Init()
{
	ReadRankingFile();
	return S_OK;
}

void RankingManager::Release()
{
}

void RankingManager::InsertNewRank(RankingType type, RankingItem rankingItem)
{
}

void RankingManager::ReadRankingFile()
{
	std::wstring filePath{};
	filePath += LdataPath;
	filePath += L"ranking.rik";

	FileUtils::File readFile;
	if (readFile.Open(filePath, FileUtils::FileAccess::Read))
	{

	}
	else
	{
		WriteNewEmptyRankingFile();
	}
}

void RankingManager::WriteRankingFile()
{
}

void RankingManager::WriteNewEmptyRankingFile()
{
	std::wstring filePath{};
	filePath += LdataPath;
	filePath += L"ranking.rik";

	FileUtils::File writeFile;
	writeFile.Open(filePath, FileUtils::FileAccess::Write);

	_category.insert(std::make_pair(RankingType::RANKING_Floor, RankingItems()));
	_category.insert(std::make_pair(RankingType::RANKING_Gem, RankingItems()));
	_category.insert(std::make_pair(RankingType::RANKING_Kill, RankingItems()));

	for (int i = 0; i < 10; ++i)
	{
		_category.find(RANKING_Floor)->second.emplace_back(L"___", 0);
		_category.find(RANKING_Gem)->second.emplace_back(L"___", 0);
		_category.find(RANKING_Kill)->second.emplace_back(L"___", 0);
	}

	for (auto &cat : _category)
	{
		writeFile.Write(L"%d Category\n", (int)cat.first);
		for (auto &item : cat.second)
		{
			WriteRankingItemToFile(writeFile, item);
		}
	}
}

void RankingManager::WriteRankingItemToFile(FileUtils::File &file, const RankingItem &item)
{
	file.Write(L"Player Name : %s\n", item.name);
	file.Write(L"%d\n", item.value);
}
