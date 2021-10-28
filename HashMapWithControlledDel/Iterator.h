#pragma once
#include "HMapControlledDel.h"

template<class KeyType, class ValueType>
class HMapIter
{
private:

	std::unordered_map<KeyType, ValueType>& umap;


public:

	HMapIter(std::unordered_map<KeyType, ValueType>& map) : umap(map) {}

};