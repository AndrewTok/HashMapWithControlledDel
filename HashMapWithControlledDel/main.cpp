#include "HMapControlledDel.h"
#include <iostream>


int main()
{
	CountStrategy<int> strat(1);
	HMapControlledDel<int, int, CountStrategy<int>> mapDel(strat);
	mapDel[25] = 25;
	int a = mapDel[25];
	int b = mapDel[25];
	int c = mapDel[25];
	int d = mapDel[25];
	std::cout << d << std::endl;
}