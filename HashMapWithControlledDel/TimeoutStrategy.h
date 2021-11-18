#pragma once
#include <ctime>
#include <unordered_map>
#include "Strategy.h"

template<class KeyType>
class TimeoutStrategy final : public Strategy<KeyType>
{
	std::time_t timeout;

	std::unordered_map<KeyType, time_t> startLifeTimes;

	using iterator = typename std::unordered_map<KeyType, time_t>::iterator;

public:

	TimeoutStrategy(time_t _timeout) : timeout(_timeout) {}

	virtual bool access(const KeyType& key)
	{
		if (startLifeTimes.find(key) == startLifeTimes.end())
		{
			return false;
		}
		std::time_t lifeDuration = std::time(nullptr) - startLifeTimes[key];
		return lifeDuration < timeout;
	}

	virtual void remove(const KeyType& key)
	{
		startLifeTimes.erase(key);
	}

	virtual bool insert(const KeyType& key)
	{
		time_t currTime = std::time(nullptr); // check time for err
		std::pair<iterator, bool> insertResult = startLifeTimes.insert(std::pair<const KeyType, time_t>(key, currTime));
		return insertResult.second;
	}

	virtual bool check(const KeyType& key)
	{
		return ((startLifeTimes.find(key) != startLifeTimes.end()) && (startLifeTimes[key] < timeout));
	}
};