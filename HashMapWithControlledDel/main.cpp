#include "HashMapControllDel.h"
#include "CountStrategy.h"
#include <iostream>


int main()
{
	CountStrategy<int> strat(2);
	HashMapControllDel<int, int> mapDel(strat);
	mapDel.insert(std::pair<int, int>(1, 1));
	mapDel[2] = 4;
	mapDel[3] = 9;
	mapDel[4] = 16;
	for (auto i : mapDel)
	{
		std::cout << i.first << " " << (i).second << std::endl;
	}
	int a = mapDel[3];
	std::cout << a << std::endl;
	int b = mapDel[3];
	std::cout << b << std::endl;
	int c = mapDel[2];
	std::cout << c << std::endl;
}