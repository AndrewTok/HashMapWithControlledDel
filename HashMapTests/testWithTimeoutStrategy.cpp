#include "pch.h"
#include "HashMapControllDel.h"
#include "TimeoutStrategy.h"
#include <Windows.h> // for sleep

void fillMap(HashMapControllDel<int, char>& map, int bound)
{
	for (int i = 0; i < bound; ++i)
	{
		map[char(i)] = (i * i);
	}
}

time_t checkRemainingTimeOfAccesses(HashMapControllDel<int, char>& map, int key, time_t maxWaitTimeSeconds)
{
	time_t startTime = time(0);
	char startVal = map[key];
	if (startVal == char{})
	{
		return 0;
	}
	char currVal = startVal;
	time_t currTime = time(0);
	while (startVal == currVal)
	{
		currVal = map[key];
		currTime = time(0);
		if ((currTime - startTime) > maxWaitTimeSeconds)
		{
			break;
		}
	}
	return currTime - startTime;
}


TEST(HashMapTimeoutStrat, InsertAccessNotimeoutTest)
{
	TimeoutStrategy<int> strat(360);
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
	TimeoutStrategy<int> strat(10);
	HashMapControllDel<int, char> mapDel(strat);
	char defaultValue{};
	fillMap(mapDel, 128);
	time_t startTime = time(0);
	for (int i = 0; i < 128; ++i)
	{
		if (time(0) - startTime < 10)
		{
			ASSERT_EQ(char(i * i), mapDel[i]);
		}
		else
		{
			ASSERT_EQ(defaultValue, mapDel[i]);
		}
	}
	Sleep(10);
	for (int i = 0; i < 128; ++i)
	{
		if (time(0) - startTime < 10)
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
	TimeoutStrategy<int> strat(360);
	HashMapControllDel<int, char> mapDel(strat);
	char defaultValue{};
	fillMap(mapDel, 128);
	//time_t startTime = time(0);
	for (int i = 0; i < 128; ++i)
	{
		ASSERT_EQ(char(i * i), mapDel[i]);
		mapDel.erase(i);
		ASSERT_EQ(defaultValue, mapDel[i]);
	}
}

TEST(HashMapTimeoutStrat, InsertInsertAccessTest)
{
	TimeoutStrategy<int> strat(360);
	HashMapControllDel<int, char> mapDel(strat);
	char defaultValue{};
	fillMap(mapDel, 128);
	time_t startTime = time(0);
	for (int i = 0; i < 128; ++i)
	{
		mapDel[i] = i + 1;
		ASSERT_EQ(char(i + 1), mapDel[i]);
	}
}