#pragma once
#include "Strategy.h"
#include <iostream>
#include <unordered_map>
#include <stdlib.h>


template<class KeyType, class ValueType, class Hash = std::hash<KeyType>,
	class KeyEqual = std::equal_to<KeyType>,
	class Allocator = std::allocator<std::pair<const KeyType, ValueType>>>
class HashMapControllDel
{
	using umap_type = std::unordered_map<KeyType, ValueType, Hash, KeyEqual, Allocator>;
public:
	using value_type = std::pair<const KeyType, ValueType>;
	using size_type = size_t;
	using key_type = KeyType;
	using mapped_type = ValueType;
	using reference = value_type&;
	using pointer = typename std::allocator_traits<Allocator>::pointer;
	
	using umapIterator = typename umap_type::iterator;

private:

	umap_type umap;

	Strategy<KeyType>& strategy; 

	class HashMapCDIter final
	{
		HashMapControllDel<KeyType, ValueType>& map;
		friend HashMapControllDel;
		umap_type& umap;
		Strategy<KeyType>& strategy;
		
		umapIterator iter;
		
		HashMapCDIter(HashMapControllDel<KeyType, ValueType>& _map, umapIterator _currIter) 
			: map(_map), iter(_currIter), umap(map.umap), strategy(map.strategy) {}

	public:
		using difference_type = size_type;
		using value_type = typename HashMapControllDel::value_type;
		using mapped_type = typename HashMapControllDel::mapped_type;
		using key_type = typename HashMapControllDel::key_type;
		using pointer = typename HashMapControllDel::pointer;
		using reference = typename HashMapControllDel::reference;
		using iterator_category = std::forward_iterator_tag;



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
	

public:
	using iterator = HashMapCDIter;

	HashMapControllDel(Strategy<key_type>& _strategy) : strategy(_strategy) {}

	std::pair<iterator, bool> insert(const value_type& _pair) 
	{

		bool isInsertable = strategy.insert(_pair.first);
		if (isInsertable)
		{
			std::pair<umapIterator, bool> insertedStat = umap.insert(_pair);
			iterator tmpIter(*this, insertedStat.first); 
			bool success = insertedStat.second;
			return std::pair<iterator, bool>(tmpIter, success);
		}
		else
		{
			return std::pair<iterator, bool>(getIter(_pair.first), false);
		}
	}

	void erase(key_type key)
	{
		strategy.remove(key);
		umap.erase(key);
	}

	ValueType& operator[] (const key_type& key)
	{

		bool isAvailable = strategy.access(key);
		if (!isAvailable)
		{
			if (umap.find(key) != umap.end())
			{
				strategy.remove(key);
				umap.erase(key);
			}
			bool isInsertable = strategy.insert(key);
			if (!isInsertable)
			{
				throw std::out_of_range("unavailable operation");
			}
		}
		return umap[key];
	}


	iterator begin() 
	{
		auto umapIter = umap.begin();
		if (umapIter == umap.end())
		{
			return iterator(*this, umapIter);
		}
		key_type key = (*umapIter).first;
		while (!strategy.check(key))
		{
			++umapIter;
			if (umapIter == umap.end())
			{
				break;
			}
			else
			{
				key = (*umapIter).first;
			}
		}
		return iterator(*this, umapIter);
	}

	iterator end() 
	{
		return iterator(*this, umap.end());
	}

private:

	iterator getIter(const key_type& key)
	{
		return iterator(*this, umap.find(key)); 
	}

};