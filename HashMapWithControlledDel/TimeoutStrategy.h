#pragma once
#include <thread>
#include <chrono>
#include <unordered_map>
#include "Strategy.h"

template<class KeyType>
class TimeoutStrategy final : public Strategy<KeyType>
{

	using duration = std::chrono::system_clock::duration;

	using time_point = std::chrono::system_clock::time_point;

	duration timeout;



	std::unordered_map<KeyType, time_point> startLifeTimes;

	using iterator = typename std::unordered_map<KeyType, time_point>::iterator;

public:

	TimeoutStrategy(duration _timeout) : timeout(_timeout) {}

	virtual bool access(const KeyType& key)
	{
		if (startLifeTimes.find(key) == startLifeTimes.end())
		{
			return false;
		}
		//duration lifeDuration = std::time(0) - startLifeTimes[key];
		return std::chrono::system_clock::now() - startLifeTimes[key] < timeout; //lifeDuration < timeout;
	}

	virtual void remove(const KeyType& key)
	{
		startLifeTimes.erase(key);
	}

	virtual bool insert(const KeyType& key)
	{
		time_point currTime = std::chrono::system_clock::now();
		std::pair<iterator, bool> insertResult = startLifeTimes.insert(std::pair<const KeyType, time_point>(key, currTime));
		return insertResult.second;
	}

	virtual bool check(const KeyType& key)
	{
		return ((startLifeTimes.find(key) != startLifeTimes.end()) && ((std::chrono::system_clock::now() - startLifeTimes[key]) < timeout));
	}
};