#include <thread>
#include <mutex>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <cstdio>
#include "picosha2.h"

int main()
{
	std::cout << "test" << " space " << "endl \n";
	std::cout << "next string" << "\n";
	freopen( "test.log", "a", stdout );
	std::cout << "valid hash found " << "123" << " seed is " << "some seed" << "\n";

	fclose(stdout);
	unsigned int amount = std::thread::hardware_concurrency();
	std::cout << amount << "\n";

	return 0;
}