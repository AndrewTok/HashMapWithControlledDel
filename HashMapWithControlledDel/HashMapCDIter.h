#pragma once
//#include "HashMapControllDel.h"
#include "Strategy.h"
#include <unordered_map>


template<class KeyType, class ValueType>
class HashMapCDIterOld final
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
		while (iter != umap.end())
		{
			KeyType key = (*iter).first;
			if (strategy.check(key))
			{
				break;
			}
			++iter;
		}
		return *this;
	}

	value_type operator*() const
	{
		KeyType key = (*iter).first;
		bool isAccessible = strategy.access(key);
		if (isAccessible)
		{
			return value_type(key, umap.at(key));
		}
		else
		{
			throw std::out_of_range("unavailable object");
		}
		
	}

	bool operator!=(const HashMapCDIter& other) const { return iter != other.iter; }

};
