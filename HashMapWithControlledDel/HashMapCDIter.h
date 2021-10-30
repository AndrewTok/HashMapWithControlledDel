#pragma once
//#include "HashMapControllDel.h"
#include "Strategy.h"
#include <unordered_map>


template<class KeyType, class ValueType>
class HashMapCDIter final
{
	typedef typename std::unordered_map<KeyType, ValueType>::iterator umapIter;
	typedef std::pair<const KeyType, ValueType> value_type;
	std::unordered_map<KeyType, ValueType>& umap;
	Strategy<KeyType>& strategy; 
	umapIter iter;

public:
	HashMapCDIter(std::unordered_map<KeyType, ValueType>& _umap, 
		Strategy<KeyType>& _strategy, umapIter _currIter)
		: umap(_umap), strategy(_strategy), iter(_currIter) {}



	HashMapCDIter& operator++ () 
	{
		++iter;
		KeyType key = (*iter).first;
		// check if del go next
		return *this;
	}

	value_type operator*() const
	{
		KeyType key = (*iter).first;
		action result = strategy.access(key);
		if (result == REMOVE)
		{
			umap.erase(key);
			throw std::out_of_range("unavailable object");
		}
		else if (result == LOOK)
		{
			KeyType key = (*iter).first;
			return value_type(key, umap[key]);
		}
		else if (result == INSERT || result == UNAVAILABLE)
		{
			throw std::out_of_range("unavailable object");
		}
		
	}

	bool operator!=(const HashMapCDIter& other) const { return iter == other.iter; }



};
