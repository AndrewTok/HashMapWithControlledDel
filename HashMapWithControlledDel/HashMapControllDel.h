#pragma once
#include "Strategy.h"
#include <iostream>
#include <unordered_map>
#include <stdlib.h>
#include "HashMapCDIter.h"


template<class ValueType, class KeyType, class Hash = std::hash<KeyType>,
	class KeyEqual = std::equal_to<KeyType>,
	class Allocator = std::allocator<std::pair<const KeyType, ValueType>>>
	class HashMapControllDel
{
private:
	// iter here
	// add empty strateg
	// redefinition members types 
	typedef KeyType key_type;
	typedef ValueType mapped_type;
	typedef std::pair<const KeyType, ValueType> value_type;
	typedef size_t size_type;
	typedef typename std::unordered_map<KeyType, ValueType>::difference_type difference_type;
	typedef Hash hasher;
	typedef KeyEqual key_equal;
	typedef Allocator allocator_type;
	typedef value_type& reference;
	typedef const value_type& const_reference;
	typedef typename std::allocator_traits<Allocator>::pointer pointer;
	typedef typename std::allocator_traits<Allocator>::const_pointer const_pointer;
	using iterator =  HashMapCDIter;
	using umapIterator = typename std::unordered_map<KeyType, ValueType>::iterator;
	//typedef typename std::unordered_map<KeyType, ValueType>::node_type node_type;


	//typedef std::pair<const KeyType, ValueType> mPair;

	

	std::unordered_map<KeyType, ValueType> umap;

	Strategy& strategy; // dynamic polymorphism


	iterator getIter(const key_type& key)
	{
		return iterator(umap, strategy, umap.find(key));
	}

public:

	HashMapControllDel(Strategy& _strategy) : strategy(_strategy) {}


	std::pair<iterator, bool> insert(const value_type& _pair) 
	{
		auto result = strategy.insert(_pair.first);
		if (result == INSERT)
		{
			std::pair<umapIterator, bool> insertedStat = umap.insert(_pair);
			return std::pair<iterator, bool>(iterator(umap, strategy, insertedStat.first), insertedStat.second);
		}
		else
		{
			return std::pair<iterator, bool>(getIter(_pair.first), false);
		}
		//return umap.insert(_pair); 
	}

	void erase(key_type& key)
	{
		strategy.remove(key);
		umap.erase(key);
	}

	value_type& operator[] (key_type& key)
	{
		action result = strategy.access(key);
		if (result == REMOVE)
		{
			umap.erase(key);
			return *this[key];
		}
		else if (result == INSERT || result == LOOK)
		{
			return umap[key];
		}
		else if (result == UNAVAILABLE)
		{
			throw std::out_of_range();
		}
	}


	iterator begin()
	{
		return iterator(umap, strategy, umap.begin());
	}

	iterator end()
	{
		return iterator(umap, strategy, umap.end());
	}

};