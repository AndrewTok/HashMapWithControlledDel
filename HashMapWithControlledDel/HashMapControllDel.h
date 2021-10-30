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
	using iterator = HashMapCDIter<KeyType, ValueType>;
	using umapIterator = typename std::unordered_map<KeyType, ValueType>::iterator;
	//typedef typename std::unordered_map<KeyType, ValueType>::node_type node_type;


	//typedef std::pair<const KeyType, ValueType> mPair;

	

	std::unordered_map<KeyType, ValueType> umap;

	Strategy<key_type>& strategy; 


	iterator getIter(const key_type& key)
	{
		return iterator(umap, strategy, umap.find(key));
	}

public:

	HashMapControllDel(Strategy<key_type>& _strategy) : strategy(_strategy) {}


	std::pair<iterator, bool> insert(const value_type& _pair) 
	{
		// check exsitstance of an element
		auto result = strategy.insert(_pair.first);
		if (result == INSERT)
		{
			// use default value to insert if dell
			std::pair<umapIterator, bool> insertedStat = umap.insert(_pair);
			iterator tmpIter(umap, strategy, insertedStat.first);
			bool success = insertedStat.second;
			return std::pair<iterator, bool>(tmpIter, success);
		}
		else
		{
			return std::pair<iterator, bool>(getIter(_pair.first), false);
		} 
	}

	void erase(key_type& key)
	{
		strategy.remove(key);
		umap.erase(key);
	}

	ValueType& operator[] (const key_type& key)
	{
		// add default value to insert
		action result = strategy.access(key);
		if (result == REMOVE)
		{
			umap.erase(key);
			// insert default 
			return (*this)[key];
		}
		else if (result == INSERT)
		{
			return umap[key];
		}
		else if (result == LOOK)
		{
			return umap.at(key);
		}
		else if (result == UNAVAILABLE)
		{
			throw std::out_of_range("unavailable obj");
		}
	}


	iterator begin() const 
	{
		iterator tmp(umap, strategy, umap.begin());
		return tmp;
	}

	iterator end() const 
	{
		iterator tmp(umap, strategy, umap.end());
		return tmp;
	}

};