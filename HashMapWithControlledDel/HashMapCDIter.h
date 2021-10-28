#pragma once
//#include "HashMapControllDel.h"
#include "Strategy.h"
#include <unordered_map>


template<class ValueType, class KeyType>
class HashMapCDIter final
{
	typedef std::pair<const KeyType, ValueType> value_type;
	std::unordered_map<KeyType, ValueType>& umap;
	Strategy& strategy; 
	typename std::unordered_map<KeyType, ValueType>::iterator iter;
	//KeyType& key;
public:
	HashMapCDIter(std::unordered_map<KeyType, ValueType>& _umap, 
		Strategy& _strategy, std::unordered_map<KeyType, ValueType>::iterator _currIter, KeyType& _key)
		: umap(_umap), strategy(_strategy), iter(_currIter) {}



	HashMapCDIter& operator++ () 
	{
		++iter;
		KeyType key = *iter.first();
		// access??? no
		return *this;
	}

	value_type operator*() const
	{
		action result = strategy.access(key);
		if (result == REMOVE)
		{
			//throw out of range

		}
		else if (result == LOOK)
		{
			KeyType key = (*iter).first;
			return value_type(key, umap[key]);
		}
		else if (result == INSERT || result == UNAVAILABLE)
		{
			//throw
		}
		
	}

	bool operator!=(const HashMapCDIter& other) const { return iter == other.iter }



};
