#pragma once

enum action
{
	UNAVAILABLE,
	REMOVE,
	LOOK,
	INSERT
};
// use bool except
template<class KeyType>
class Strategy
{
public:
	virtual ~Strategy() = default;
	// add check
	virtual action access(const KeyType& key) = 0;
	virtual void remove(const KeyType& key) = 0;
	virtual action insert(const KeyType& key) = 0;
};
