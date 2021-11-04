#pragma once
#include "Strategy.h"
#include <unordered_map>

template<class KeyType>
class CountStrategy final : public Strategy<KeyType>
{


	size_t numberWhenDel;

	std::unordered_map<KeyType, size_t> countTable;

	using iterator = typename std::unordered_map<KeyType, size_t>::iterator;

public:
	CountStrategy(size_t _numberWhenDel) : numberWhenDel(_numberWhenDel) {}

	virtual bool check(const KeyType& key) override
	{
		return ((countTable.find(key) != countTable.end()) && (countTable[key] != numberWhenDel));
	}


	virtual bool access(const KeyType& key) override
	{
		if (countTable.find(key) != countTable.end() && countTable[key] != numberWhenDel)
		{
			++countTable[key];
			return true;
		}
		return false;

	}

	virtual bool insert(const KeyType& key) override
	{
		std::pair<iterator, bool> insertResult = countTable.insert(std::pair<const KeyType, size_t>(key, 0));
		return insertResult.second;

	}

	virtual void remove(const KeyType& key) override
	{
		countTable.erase(key);
	}


};