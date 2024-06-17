#include "./hstring/hstring.h"
#include <iostream>
#include "./hstring/mempool.h"
//#include "logger.h"
#define MEMPOOL_BLOCK_SIZE 2
#define MEMPOOL_TOTAL_SIZE 160
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
#elif 0
	hstring str(33);
	hstring str1("1111");
	str = str + str1;
	std::cout <<"str= "<< str << std::endl;
	std::cout << str1 << std::endl;
#else

	hstring str("123abc");
	std::cout << "str = " << str << std::endl;
	//printf("str = %s\n", str);
	hstring str1 = "QWE";
	std::cout << "str1 = " << str1 << std::endl;
	//printf("str1 = %s\n", str1);

	std::cout << "*******************" << std::endl;

	str = str + str1;
	std::cout << "str = " << str << std::endl;
	//printf("str = %s\n", str);

	str1 = str1 + "ABC";
	std::cout << "str1 = " << str1 << std::endl;
	//printf("str1 = %s\n", str1);

	std::cout << "*******************" << std::endl;

	hstring str2 = "123abc  !@#_QWE";
	str2 = str2 - "123";
	std::cout << "str2 = " << str2 << std::endl;
	//printf("str2 = %s\n", str2);
	str2 = str2 - "&*";
	std::cout << "str2 = " << str2 << std::endl;
	//printf("str2 = %s\n", str2);
	hstring str3 = "@#_Q";
	str2 = str2 - str3;
	std::cout << "str2 = " << str2 << std::endl;
	//printf("str2 = %s\n", str2);


	std::cout << "*******************" << std::endl;

	hstring str4 = "AS123as456  !@#()";
	int pos = str4.find("as");
	std::cout << "pos = " << pos << std::endl;
	int pos1 = str4.find(" QWE");
	std::cout << "pos1 = " << pos1 << std::endl;

	std::cout << "*******************" << std::endl;

	str4.replace("as", "RT");
	std::cout << "str4 = " << str4 << std::endl;
	std::cout << "end" << std::endl;
#endif
}