#include "./hstring/hstring.h"
#include <iostream>
#include "./hstring/mempool.h"
//#include "logger.h"
#define MEMPOOL_TOTAL_SIZE 30
#define MEMPOOL_BLOCK_SIZE 5
mempool hstring::mem_pool(MEMPOOL_TOTAL_SIZE, MEMPOOL_BLOCK_SIZE);

int main() {
#if 0
	hstring str(9876);
	hstring str1("1111");
	hstring str3(2222);
	str = str + str1;
	std::cout << str << std::endl;
	str = str - "111";
	std::cout << str << std::endl;
	hstring str4(1234);
	std::cout << str4.replace("23", "") << std::endl;
	hstring str5(2222);
	str5 = 1234;
	std::cout << str5 << std::endl;
#elif 1
	hstring str(33);
	hstring str1("1111");
	str = str + str1;
	std::cout <<"str= "<< str << std::endl;
	std::cout << str1 << std::endl;
#else


#endif
}