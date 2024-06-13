#pragma once
#include <iostream>

namespace logger {
#if __cplusplus >= 201703L
	template<typename...Args>
	void logger(Args... args) {
		//cpp17折叠表达式
		((std::cout << args << " "), ...);
		std::cout << std::endl;
	}
#else
	//重载空参logger，作为递归停止
	void logger() {
		std::cout << std::endl;
	}
	template<typename T, typename... Args>
	void logger(T first, Args... args) {
		//Args... 中的第一个参数被解包并赋值给 first
		std::cout << first << " ";
		logger(args...);
	}
#endif
}
