#include "HashMapControllDel.h"
#include "CountStrategy.h"
#include <iostream>


int main()
{
	CountStrategy<int> strat(2);
	HashMapControllDel<int, int> mapDel(strat);
	//mapDel.insert(std::pair<const int, int>(25, 25));
	mapDel[25] = 37;
	mapDel[24] = 30;
	mapDel[42] = 43;
	//for (auto i : mapDel)
	//{
	//	std::cout << (i).second << std::endl;
	//}
	int a = mapDel[25];
	std::cout << a << std::endl;
	int b = mapDel[25];
	std::cout << b << std::endl;
	int c = mapDel[25];
	std::cout << c << std::endl;
	int d = mapDel[25];
	std::cout << d << std::endl;
}