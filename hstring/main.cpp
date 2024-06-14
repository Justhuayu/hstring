#include "hstring.h"
#include <iostream>
#include "mempool.h"
//#include "logger.h"
#define MEMPOOL_TOTAL_SIZE 30
#define MEMPOOL_BLOCK_SIZE 4
mempool hstring::mem_pool(MEMPOOL_TOTAL_SIZE, MEMPOOL_BLOCK_SIZE);

int main() {
#if 1
	hstring str(98761234);
	hstring str1("11113333");
	hstring str3(22224444);

	str = str + str1;
	std::cout << str << std::endl;
	str = str + str3;
	std::cout << str << std::endl;
	hstring str4(1234567);
	std::cout << str4.replace("23", "") << std::endl;
	hstring str5(2222);
	str5 = 1234;
	std::cout << str5 << std::endl;
#else

#endif
}