#include "hstring.h"
#include <iostream>

//#include "logger.h"
int main() {
	hstring str(9876);
	hstring str1(1111);
	hstring str3(2222);

	str = str + str1;
	std::cout << str << std::endl;
	str = str + str3;
	std::cout << str << std::endl;
	hstring str4(1234567);
	std::cout << str4.replace("23", "") << std::endl;
	hstring str5(123);
	str5 = str5 - "1";
	std::cout << str5 << std::endl;
	//TODO£ºÎö¹¹±¨´í£¿
}