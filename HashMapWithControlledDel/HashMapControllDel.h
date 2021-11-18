#pragma once
#include "Strategy.h"
#include <iostream>
#include <unordered_map>
#include <stdlib.h>
//#include "HashMapCDIter.h"


template<class ValueType, class KeyType, class Hash = std::hash<KeyType>,
	class KeyEqual = std::equal_to<KeyType>,
	class Allocator = std::allocator<std::pair<const KeyType, ValueType>>>
class HashMapControllDel
{
	using umap_type = std::unordered_map<KeyType, ValueType, Hash, KeyEqual, Allocator>;
public:
	using value_type = std::pair<const KeyType, ValueType>;
	using size_type = size_t;
	//using difference_type = typename umap_type::difference_type;
	//using hasher = Hash;
	using key_type = KeyType;
	using mapped_type = ValueType;
	//using key_equal = KeyEqual;
	//using allocator_type = Allocator;
	using reference = value_type&;
	//using const_reference = const value_type&;
	using pointer = typename std::allocator_traits<Allocator>::pointer;
	//using const_pointer = typename std::allocator_traits<Allocator>::const_pointer;
	
	using umapIterator = typename umap_type::iterator;

private:

	umap_type umap;

	Strategy<KeyType>& strategy; 

	class HashMapCDIter final
	{
		friend HashMapControllDel;
		//typedef typename std::unordered_map<KeyType, ValueType>::iterator umapIter;
		//typedef std::pair<const KeyType, ValueType> value_type;
		umap_type& umap;
		Strategy<KeyType>& strategy;
		umapIterator iter;

		HashMapCDIter(std::unordered_map<KeyType, ValueType>& _umap,
			Strategy<KeyType>& _strategy, umapIterator _currIter)
			: umap(_umap), strategy(_strategy), iter(_currIter) {}

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
			iterator tmpIter(umap, strategy, insertedStat.first);
			bool success = insertedStat.second;
			return std::pair<iterator, bool>(tmpIter, success);
		}
		else
		{
			return std::pair<iterator, bool>(getIter(_pair.first), false);
		}
	}

	void erase(key_type&& key)
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
		iterator tmp(umap, strategy, umap.begin());
		return tmp;
	}

	iterator end() 
	{
		iterator tmp(umap, strategy, umap.end());
		return tmp;
	}

private:

	iterator getIter(const key_type& key)
	{
		return iterator(umap, strategy, umap.find(key));
	}

};