#pragma once
#include <iostream>

namespace logger {
#if __cplusplus >= 201703L
	template<typename...Args>
	void logger(Args... args) {
		//cpp17�۵����ʽ
		((std::cout << args << " "), ...);
		std::cout << std::endl;
	}
#else
	//���ؿղ�logger����Ϊ�ݹ�ֹͣ
	void logger() {
		std::cout << std::endl;
	}
	template<typename T, typename... Args>
	void logger(T first, Args... args) {
		//Args... �еĵ�һ���������������ֵ�� first
		std::cout << first << " ";
		logger(args...);
	}
#endif
}
