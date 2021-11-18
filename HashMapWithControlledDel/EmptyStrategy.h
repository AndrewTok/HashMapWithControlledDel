#pragma once
#include "Strategy.h"
#include <unordered_set>

template<class KeyType>
class EmptyStrategy final : public Strategy<KeyType>
{
	std::unordered_set<KeyType> keys;
	using iterator = typename std::unordered_set<KeyType>::iterator;

	virtual bool access(const KeyType& key)
	{
		return check(key);
	}

	virtual void remove(const KeyType& key)
	{
		keys.erase(key);
	}
	virtual bool insert(const KeyType& key)
	{
		std::pair<iterator, bool> insertResult = keys.insert(key);
		return insertResult.second;
	}
	virtual bool check(const KeyType& key)
	{
		return (keys.find(key) != keys.end());
	}
};