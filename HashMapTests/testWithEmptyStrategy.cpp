#include "pch.h"
#include "HashMapControllDel.h"
#include "EmptyStrategy.h"
#include <string>



void fillMap(HashMapControllDel<std::string, double>& map, double bound)
{
	for (double i = 0; i < bound; ++i)
	{
		std::string key = std::to_string(i);
		map[key] = (i*i);
	}
}


TEST(HashMapEmptyStrat, InsertAccessTest)
{
	EmptyStrategy<std::string> strat;
	HashMapControllDel<std::string, double> mapDel(strat);
	for (double i = 0; i <= 100; i += 2)
	{
		mapDel.insert(std::pair<std::string, double>(std::to_string(i), (i * i)));
		mapDel[std::to_string(i + 1)] = ((i + 1) * (i + 1));
		ASSERT_EQ((i * i), mapDel[std::to_string(i)]);
		ASSERT_EQ(((i + 1) * (i + 1)), mapDel[std::to_string(i + 1)]);
	}
}

TEST(HashMapEmptyStrat, InsertAccessDeleteAccessTest)
{
	EmptyStrategy<std::string> strat;
	HashMapControllDel<std::string, double> mapDel(strat);
	double defaultValue{};
	fillMap(mapDel, 128);
	for (double i = 0; i < 128; ++i)
	{
		ASSERT_EQ(i * i, mapDel[std::to_string(i)]);
		mapDel.erase(std::to_string(i));
		ASSERT_EQ(defaultValue, mapDel[std::to_string(i)]);
	}
}

TEST(HashMapEmptyStrat, InsertInsertAccessTest)
{
	EmptyStrategy<std::string> strat;
	HashMapControllDel<std::string, double> mapDel(strat);
	double defaultValue{};
	fillMap(mapDel, 128);
	for (double i = 0; i < 128; ++i)
	{
		mapDel[std::to_string(i)] = i + 1;
		ASSERT_EQ(i + 1, mapDel[std::to_string(i)]);
	}
}

