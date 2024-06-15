#pragma once
/*
自定义字符串类
功能：
	1. int、float、char*等类型初始化为hstring
	2. 重载+、-、new/delete
	3. 定义方法：find、subsplace
	4. 内存池统一管理new的内存
*/
#include <iostream>
#include "mempool.h"
class hstring
{
public:
	hstring();
	hstring(int num);
	hstring(const char* str);
	hstring(const hstring& str);
	~hstring();

	int find(const hstring& str) const;//hstring中查找hstring，成功返回位置，失败返回-1
	hstring& replace(const hstring& src, const hstring& dst);//"1234" 34->abc == "12abc"

	friend std::ostream& operator<<(std::ostream& _cout, hstring& str);
	hstring& operator+(const hstring& str);//"123" + "abc" == "123abc"
	hstring& operator-(const hstring& str);//"12345" - "34" == "125"
	hstring& operator=(const hstring& str);//str=str1;
	hstring& operator=(int num);//str = 123;
	const char& operator[](size_t index) const;//str[1] == "2"
private:
	bool initMemory();//从mempool分配一个block
	void init();//初始化hstring
	void int2hstring(int num);
	void cstr2hstring(const char* str);
	void allocBigMemory(size_t size);//分配size大小的内存，用于mempool的block太小的情况
	void freeHstring(void* ptr, bool _is_use_mempool);
public:
	size_t length;//hstring字符串的长度，不含结尾'\0'
private:
	char* memory;//指向缓存区的开始位置
	size_t mem_size;//缓存区的大小
	char* hstring_start;//hstring字符开始的位置，结尾为'\0'
	bool is_use_mempool;//当前字符串，是否使用mempool分配的内存
public:
	static mempool mem_pool;
};

std::ostream& operator<<(std::ostream& _cout, hstring& str);
/*
要求:
1.存储数据的时候使用缓冲区,使用缓冲区的目的是为了尽量减少频繁的NEW/DEL 
内存带来的资源损耗（缓冲区必须要有，不知道缓冲区的回去看基础课，new之前要加判断）
2.重载+ 增加数据,增加数据要实现的形态: “123456789” + “abc”  得到 “123456789abc”
（最多new一次）new之前判断是否超过缓冲区，超过了才new
3.重载- 删除数据,增加数据要实现的形态:”123456789” - “456”  得到 “123789” （函数中不使用new）
4.改数据 ,要求  “123456789”  34修改为 abc 得到 12abc56789 （最多new一次）原理同上
5.查 ,123456 差 34 得到 34的位置 2  （不使用new）
6.重载 = 实现 int 转 hstring 字符串 （最多new一次）
7.不能使用库函数（memcpy可使用）
8.每个功能都测试一遍之后再把项目发群里（注意：在查找、删除、修改的时找不到字符串要给出错误提示）
*/
