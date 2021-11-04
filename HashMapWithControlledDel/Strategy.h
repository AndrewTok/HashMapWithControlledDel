#pragma once

template<class KeyType>
class Strategy
{
public:
	virtual ~Strategy() = default;

	virtual bool access(const KeyType& key) = 0;
	virtual void remove(const KeyType& key) = 0;
	virtual bool insert(const KeyType& key) = 0;
	virtual bool check(const KeyType& key) = 0;
};
