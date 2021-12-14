#include "pch.h"
#include "HashMapControllDel.h"
#include "CountStrategy.h"
#include <iostream>


template<class Key, class ValueType>
void touchElement(HashMapControllDel<Key, ValueType>& map, Key key, size_t numberOfTouches)
{
	for (size_t i = 0; i < numberOfTouches; ++i)
	{
		ValueType val = map[key];
	}
}

void fillMap(HashMapControllDel<int, int>& map, int boundValue)
{
	for (int i = 0; i < boundValue; ++i)
	{
		map[i] = i * i;
	}
}

size_t checkRemainingNumberOfAccesses(HashMapControllDel<int, int>& map, int key, size_t maxCount)
{
	
	int startVal = map[key];
	if (startVal == 0)
	{
		return 0;
	}
	int currVal = startVal;
	size_t num = 1;
	while (startVal == currVal)
	{
		currVal = map[key];
		++num;	
		if (num > maxCount)
		{
			break;
		}
	}
	return num;
}


TEST(HashMapCountStrat, InsertGetTestNoDel)
{
	CountStrategy<int> strat(128);
	HashMapControllDel<int, int> mapDel(strat);
	for (int i = 0; i <= 100; i += 2)
	{
		mapDel.insert(std::pair<int, int>(i, i * i));
		mapDel[i + 1] = (i + 1) * (i + 1);
		ASSERT_EQ(i * i, mapDel[i]);
		ASSERT_EQ((i + 1) * (i + 1), mapDel[i + 1]);
	}
}

TEST(HashMapCountStrat, InsertGetTestWithDel)
{
	CountStrategy<int> strat(8);
	HashMapControllDel<int, int> mapDel(strat);
	int defaultValue = mapDel[-1];
	fillMap(mapDel, 128);
	for (int i = 0; i < 128; ++i)
	{
		touchElement(mapDel, i, i);
		if (i < 8)
		{
			ASSERT_EQ(i * i, mapDel[i]);
		}
		else
		{
			ASSERT_EQ(defaultValue, mapDel[i]);
		}
	}
}

TEST(HashMapCountStrat, InsertDeleteAccessTest)
{
	CountStrategy<int> strat(8);
	HashMapControllDel<int, int> mapDel(strat);
	int defaultValue = mapDel[-1];
	fillMap(mapDel, 128);
	for (int i = 0; i < 128; ++i)
	{
		mapDel.erase(i);
		ASSERT_EQ(defaultValue, mapDel[i]);
	}
}

TEST(HashMapCountStrat, InsertInsertAccessTest)
{
	CountStrategy<int> strat(8);
	HashMapControllDel<int, int> mapDel(strat);
	int defaultValue = mapDel[-1];
	fillMap(mapDel, 128);
	for (int i = 0; i < 128; ++i)
	{
		mapDel[i] = i + 1;
		ASSERT_EQ(i+1, mapDel[i]);
		ASSERT_EQ(7, checkRemainingNumberOfAccesses(mapDel, i, 8));
	}
}

TEST(HashMapCountStrat, CorrectLoopTest)
{
	CountStrategy<int> strat(1);
	HashMapControllDel<int, int> mapDel(strat);
	int defaultValue{};
	fillMap(mapDel, 128);
	for (auto& i : mapDel)
	{
		int key = i.first;
		int value = i.second;
		ASSERT_EQ(value, key*key);
	}
}

TEST(HashMapCountStrat, PartlyCorrectLoopTest)
{
	CountStrategy<int> strat(2);
	HashMapControllDel<int, int> mapDel(strat);
	int defaultValue{};
	fillMap(mapDel, 128);
	for (auto& i : mapDel)
	{
		int key = i.first;
		int value = i.second;
		ASSERT_EQ(value, key * key);
		if (key % 2 != 0)
		{
			touchElement(mapDel, key, 1);
		}
	}
	for (auto& i : mapDel)
	{
		int key = i.first;
		int value = i.second;
		ASSERT_EQ(value, key * key);
		ASSERT_TRUE(key % 2 == 0);
	}
}

TEST(HashMapCountStrat, IncorrectLoopTest)
{
	CountStrategy<int> strat(2);
	HashMapControllDel<int, int> mapDel(strat);
	int defaultValue{};
	fillMap(mapDel, 128);
	auto beginIter = mapDel.begin();
	for (auto& i : mapDel)
	{
		int key = i.first;
		int value = i.second;
		ASSERT_EQ(value, key * key);
		touchElement(mapDel, key, 1);
	}
	bool wasInLoop = false;
	for (auto& i : mapDel)
	{
		wasInLoop = true;
	}
	ASSERT_FALSE(wasInLoop);
}

TEST(HashMapCountStrat, IncorrectIteratorTest)
{
	CountStrategy<int> strat(2);
	HashMapControllDel<int, int> mapDel(strat);
	int defaultValue{};
	fillMap(mapDel, 128);
	auto beginIter = mapDel.begin();
	for (auto& i : mapDel)
	{
		int key = i.first;
		int value = i.second;
		ASSERT_EQ(value, key * key);
		touchElement(mapDel, key, 1);
	}
	bool exceptionAppears = false;
	try
	{
		int value = (*beginIter).second;
	}
	catch (std::out_of_range& e)
	{
		exceptionAppears = true;
	}
	ASSERT_TRUE(exceptionAppears);
}