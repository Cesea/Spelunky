#ifndef RANKING_MANAGER_H
#define RANKING_MANAGER_H

#include "singletonBase.h"

enum RankingType
{
	RANKING_None,
	RANKING_Floor,
	RANKING_Gem,
	RANKING_Kill,
};

struct RankingItem
{
	RankingItem(const WCHAR *str, int value)
	{
		wcscpy(name, str);
		this->value = value;
	}
	WCHAR name[20]{};
	int value{ 0 };
};


class RankingManager : public singletonBase<RankingManager>
{
	typedef std::vector<RankingItem> RankingItems;
	typedef std::unordered_map<RankingType, RankingItems> RankingCategory;
public :
	RankingManager();
	~RankingManager();
	HRESULT Init();
	void Release();

	void InsertNewRank(RankingType type, RankingItem rankingItem);

private :
	void ReadRankingFile();
	void WriteRankingFile();

	void WriteNewEmptyRankingFile();

	void WriteRankingItemToFile(FileUtils::File &file, const RankingItem &item);

private :
	RankingCategory _category;
};


#endif