#include "hstring.h"
#include <iostream>

//#include "logger.h"
int main() {
	std::cout << __cplusplus << std::endl;
	hstring str("asd");
	std::cout << str << std::endl;
	std::cout << str.length << std::endl;
}