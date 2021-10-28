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
	CountStrategy(size_t _numberWhenDel) : numberWhenDel(_numberWhenDel + 1) {}
	// insert access remove check
	virtual action access(const KeyType& key) override
	{
		if (countTable.find(key) == countTable.end())
		{
			countTable[key] = 0;
			return INSERT;
		}
		if (countTable[key] == numberWhenDel)
		{
			countTable[key] = 0;
			return REMOVE;
		}
		else
		{
			return LOOK;
		}
	}

	virtual action insert(const KeyType& key) override
	{
		std::pair<iterator, bool> insertResult = countTable.insert(std::pair<const KeyType, size_t>(key, 0));
		if (insertResult.second == false)
		{
			return UNAVAILABLE;
		}
		else
		{
			return INSERT;
		}
	}

	virtual void remove(const KeyType& key) override
	{
		countTable.erase(key);
	}


};