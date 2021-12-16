#include "HashMapControllDel.h"
#include "CountStrategy.h"
#include "TimeoutStrategy.h"
#include "EmptyStrategy.h"
#include <iostream>

using namespace std::chrono;

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
	TimeoutStrategy<int> tmStrategy(10s);
	HashMapControllDel<int, int> mapTime(tmStrategy);
	mapTime[2] = 4;
	std::this_thread::sleep_for(5s);
	std::cout << mapTime[2] << std::endl;
	std::this_thread::sleep_for(5s);
	std::cout << tmStrategy.check(2) << " " << mapTime[2] << std::endl;
	EmptyStrategy<int> emptyStrat;
	HashMapControllDel<int, int> mapEmpty(emptyStrat);
	mapEmpty[2] = 4;
	mapEmpty[3] = 9;
	std::cout << mapEmpty[2] << " " << mapEmpty[3] << std::endl;
	
}