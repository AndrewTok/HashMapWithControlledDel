#include "pch.h"
#include "HashMapControllDel.h"
#include "CountStrategy.h"
#include <iostream>

#include "EmptyStrategy.h"
#include <string>
#include <math.h>

#include "HashMapControllDel.h"
#include "TimeoutStrategy.h"

using namespace std::chrono;

template<class Key, class ValueType>
void touchElement(HashMapControllDel<Key, ValueType>& map, Key key, size_t numberOfTouches)
{
	for (size_t i = 0; i < numberOfTouches; ++i)
	{
		ValueType val = map[key];
	}
}

template<class KeyType, class ValueType>
void fillMap(HashMapControllDel<KeyType, ValueType>& map, KeyType endValue, KeyType startValue = KeyType())
{
	for (KeyType i = startValue; i < endValue; ++i)
	{	

		map[i] = i * i;
	}
}

void fillMap(HashMapControllDel<std::string, double>& map, double endValue, double startValue = 0)
{
	for (double i = startValue; i < endValue; ++i)
	{
		std::string key = std::to_string(i);
		map[key] = (i * i);
	}
}

system_clock::duration checkRemainingTimeOfAccesses(HashMapControllDel<int, char>& map, int key, system_clock::duration maxWaitTimeSeconds)
{
	system_clock::time_point startTime = system_clock::now();
	char startVal = map[key];
	if (startVal == char{})
	{
		return system_clock::duration(0);
	}
	char currVal = startVal;
	system_clock::time_point currTime = system_clock::now();
	while (startVal == currVal)
	{
		currVal = map[key];
		currTime = system_clock::now();
		if ((currTime - startTime) > maxWaitTimeSeconds)
		{
			break;
		}
	}
	return currTime - startTime;
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
		ASSERT_EQ(i + 1, mapDel[i]);
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
		ASSERT_EQ(value, key * key);
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
	ASSERT_THROW((*beginIter).second, std::out_of_range);
}


TEST(HashMapTimeoutStrat, InsertAccessNotimeoutTest)
{
	TimeoutStrategy<int> strat(360s);
	HashMapControllDel<int, char> mapDel(strat);
	for (int i = 0; i <= 100; i += 2)
	{
		mapDel.insert(std::pair<int, char>(i, i * i));
		mapDel[i + 1] = (i + 1) * (i + 1);
		ASSERT_EQ(char(i * i), mapDel[i]);
		ASSERT_EQ(char((i + 1) * (i + 1)), mapDel[i + 1]);
	}
}

TEST(HashMapTimeoutStrat, InsertAccessTimeoutAccessTest)
{
	TimeoutStrategy<int> strat(2s);
	HashMapControllDel<int, char> mapDel(strat);
	char defaultValue{};
	fillMap(mapDel, 128);
	system_clock::time_point startTime = system_clock::now();
	for (int i = 0; i < 128; ++i)
	{
		if (system_clock::now() - startTime < system_clock::duration(2s))
		{
			ASSERT_EQ(char(i * i), mapDel[i]);
		}
		else
		{
			ASSERT_EQ(defaultValue, mapDel[i]);
		}
	}
	std::this_thread::sleep_for(2s);

	for (int i = 0; i < 128; ++i)
	{
		if (system_clock::now() - startTime < system_clock::duration(2s))
		{
			ASSERT_EQ(char(i * i), mapDel[i]);
		}
		else
		{
			ASSERT_EQ(defaultValue, mapDel[i]);
		}
	}

}

TEST(HashMapTimeoutStrat, InsertDeleteAccessTest)
{
	TimeoutStrategy<int> strat(360s);
	HashMapControllDel<int, char> mapDel(strat);
	char defaultValue{};
	fillMap(mapDel, 128);
	for (int i = 0; i < 128; ++i)
	{
		ASSERT_EQ(char(i * i), mapDel[i]);
		mapDel.erase(i);
		ASSERT_EQ(defaultValue, mapDel[i]);
	}
}

TEST(HashMapTimeoutStrat, InsertInsertAccessTest)
{
	TimeoutStrategy<int> strat(360s);
	HashMapControllDel<int, char> mapDel(strat);
	char defaultValue{};
	fillMap(mapDel, 128);
	for (int i = 0; i < 128; ++i)
	{
		mapDel[i] = i + 1;
		ASSERT_EQ(char(i + 1), mapDel[i]);
	}
}

TEST(HashMapTimeoutStrat, CorrectLoopTest)
{
	TimeoutStrategy<int> strat(100s);
	HashMapControllDel<int, char> mapDel(strat);
	char defaultValue{};
	fillMap(mapDel, 128);
	for (auto& i : mapDel)
	{
		int key = i.first;
		char value = i.second;
		ASSERT_EQ(value, char(key * key));
	}
}

TEST(HashMapTimeoutStrat, PartlyCorrectLoopTest)
{
	TimeoutStrategy<int> strat(2s);
	HashMapControllDel<int, char> mapDel(strat);
	char defaultValue{};
	fillMap(mapDel, 128);
	std::this_thread::sleep_for(1s);
	for (auto& i : mapDel)
	{
		if (i.first % 2 == 0)
		{
			mapDel[i.first] = i.first * i.first; // time to live is updated
		}
	}
	std::this_thread::sleep_for(1s);
	for (auto& i : mapDel)
	{
		int key = i.first;
		int value = i.second;
		ASSERT_EQ(value, key * key);
		ASSERT_TRUE(key % 2 == 0);
	}
}

TEST(HashMapTimeoutStrat, IncorrectLoopTest)
{
	TimeoutStrategy<int> strat(1s);
	HashMapControllDel<int, char> mapDel(strat);
	char defaultValue{};
	fillMap(mapDel, 128);
	std::this_thread::sleep_for(1s);

	bool wasInLoop = false;
	for (auto& i : mapDel)
	{
		wasInLoop = true;
	}
	ASSERT_FALSE(wasInLoop);
}

TEST(HashMapTimeoutStrat, IncorrectIteratorTest)
{
	TimeoutStrategy<int> strat(2s);
	HashMapControllDel<int, char> mapDel(strat);
	char defaultValue{};
	fillMap(mapDel, 128);
	auto beginIter = mapDel.begin();
	ASSERT_NE(beginIter, mapDel.end());

	std::this_thread::sleep_for(2s);

	ASSERT_THROW((*beginIter).second, std::out_of_range);

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

TEST(HashMapEmptyStrat, CorrectLoopTest)
{
	EmptyStrategy<std::string> strat;
	HashMapControllDel<std::string, double> mapDel(strat);
	double defaultValue{};
	fillMap(mapDel, 128);
	for (auto& i : mapDel)
	{
		std::string key = i.first;
		double value = i.second;
		ASSERT_EQ(std::to_string(sqrt(value)), key);
	}
}

TEST(HashMapEmptyStrat, PartlyCorrectLoopTest)
{
	EmptyStrategy<std::string> strat;
	HashMapControllDel<std::string, double> mapDel(strat);
	double defaultValue{};
	fillMap(mapDel, 128);


	//удалим нечетные значения
	for (double i = 0; i < 128; ++i)
	{
		if (int(i*i) % 2 != 0)
		{
			mapDel.erase(std::to_string(i));
		}
	}

	for (auto& i : mapDel)
	{
		std::string key = i.first;
		double value = i.second;
		ASSERT_EQ(std::to_string(sqrt(value)), key);
		ASSERT_TRUE(int(value) % 2 == 0);
	}
}

TEST(HashMapEmptyStrat, IncorrectLoopTest)
{
	EmptyStrategy<std::string> strat;
	HashMapControllDel<std::string, double> mapDel(strat);
	double defaultValue{};
	fillMap(mapDel, 128);
	
	for (double i = 0; i < 128; ++i)
	{
		mapDel.erase(std::to_string(i));
	}

	bool wasInLoop = false;
	for (auto& i : mapDel)
	{
		wasInLoop = true;
	}
	ASSERT_FALSE(wasInLoop);
}