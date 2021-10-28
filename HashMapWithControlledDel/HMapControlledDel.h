#pragma once
#include <iostream>
#include <unordered_map>



template<class KeyType>
class CountStrategy final
{

	size_t numberWhenDel;

	std::unordered_map<KeyType, size_t> countTable;
	//std::unordered_map<KeyType, ValueType>& umap;

public:

	CountStrategy(size_t _numberWhenDel) : numberWhenDel(_numberWhenDel + 1) {}
	// insert access remove check
	void updateElement(const KeyType& key)
	{
		auto itrValue = countTable.find(key);
		if (itrValue == countTable.end())
		{
			processNewElement(key);
		}
		else
		{
			countTable[key] = 0; 
		}
	}

	void processNewElement(const KeyType& key)
	{
		if (countTable.find(key) != countTable.end())
		{
			return;
		}
		//std::cout << key << std::endl;
		countTable[key] = 0;
	}

	void processTouchOfElement(const KeyType& key)
	{
		
		++countTable.at(key);
	}

	bool shouldBeRemoved(const KeyType& key) 
	{
		return countTable.at(key) == numberWhenDel;
	}

	bool tryForcedRemoval(const KeyType& key) 
	{
		return countTable.erase(key) == 1;
	}
};


template<class ValueType, class KeyType, class DelStrategy, class Hash = std::hash<KeyType>, 
		class KeyEqual = std::equal_to<KeyType>,
		class Allocator = std::allocator<std::pair<const KeyType, ValueType>>>
class HMapControlledDel
{
private:
	// iter here
	//add empty strateg
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
	//iterator
	//const iterator
	//typedef typename std::unordered_map<KeyType, ValueType>::node_type node_type;


	typedef std::pair<const KeyType, ValueType> mPair;

	//typedef typename std::unordered_map<KeyType, ValueType>::iterator iterator;

	std::unordered_map<KeyType, ValueType> umap;

	DelStrategy strategy;

public:

	HMapControlledDel(DelStrategy& _strategy) : strategy(_strategy) {}


	//bool insert(const mPair& _pair) 
	//{
	//	strategy.processNewElement(_pair.first);
	//	return umap.insert(_pair); 
	//}


	//ValueType& at(const KeyType& key)
	//{
	//	if (strategy.shouldRemove(key))
	//	{
	//		umap.erase(key);
	//	}
	//	strategy.processTouchOfElement(key);
	//	return umap.at(key); //exception if removed
	//}

	//ValueType& rewriteValueByKey(const KeyType& key)
	//{
	//	strategy.updateElement(key);
	//	return umap[key];
	//}

	//const ValueType& getValueByKey(const KeyType& key) const
	//{
	//	if (strategy.shouldBeRemoval(key))
	//	{
	//		umap.erase(key);
	//	}
	//	strategy.processTouchOfElement(key);
	//	return umap.at(key);
	//}

	const ValueType& operator[] (const KeyType& key) const
	{
		//ValueType& value = umap.at(key);
		//auto itrValue = umap.find(key);
		
		strategy.processTouchOfElement(key);

		if (strategy.shouldRemove(key))
		{
			umap.erase(key);
		}


		return umap.at(key);
	}

	ValueType& operator[] (const KeyType& key)
	{
		strategy.updateElement(key);
		return umap[key];
	}


	size_t erase(const KeyType& key)
	{
		if (strategy.tryForcedRemoval(key))
		{
			return umap.erase(key);
		}
		return 0;
	}

};