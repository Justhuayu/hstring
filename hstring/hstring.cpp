#include "hstring.h"
#include <iostream>

hstring::hstring() {
	length = 0;
	memory = nullptr;
	hstring_start = nullptr;
	mem_size = 0;
}
hstring::hstring(int num) {
	init();
	int2hstring(num);

}

hstring::hstring(const char* str)
{
	init();
	cstr2hstring(str);
}
hstring::hstring(const hstring& str)
{
	//=运算符重构过，直接使用
	*this = str;
}
hstring::~hstring() {
	freeHstring(memory,is_use_mempool);
}

void hstring::int2hstring(int num) {
	//num == 0，默认什么也没有
	//处理num为负数情况
	bool negative = false;
	if (num < 0) {
		num *= -1;
		negative = true;
	}
	//计算当前分配的内存大小，够不够放下num
	int width = mem_pool.mem_block_size;
	int tmp = num;
	int num_length = negative?2:1;
	while (tmp > 0) {
		tmp /=  10;
		num_length++;
	}
	if (num_length > width) {
		allocBigMemory(num_length);
	}
	hstring_start = memory + mem_size - 1;
	*hstring_start = '\0';
	while(num>0){
		tmp = num % 10;
		length++;
		hstring_start--;
		*hstring_start = tmp + '0';
		num /= 10;
	}
	//负号
	if (negative) {
		length++;
		hstring_start--;
		*hstring_start = '-';
	}
}
void hstring::cstr2hstring(const char* str)
{
	const char* tmp = str;
	while (*tmp != '\0') {
		tmp++;
		length++;
	}
	if (length+1 > mem_size) {
		freeHstring(memory,is_use_mempool);
		allocBigMemory(length + 1);
	}
	memcpy(memory, str, length);
	*(memory + length) = '\0';
	hstring_start = memory;
}
void hstring::init() {
	initMemory();
	length = 0;
}
void hstring::freeHstring(void* ptr,bool _is_use_mempool) {
	if (_is_use_mempool) {
		mem_pool.freeMemory(ptr);
	}
	else {
		free(ptr);
	}
}
void hstring::allocBigMemory(size_t size) {
	memory = (char*)malloc(size);
	if (!memory) {
		std::cout << "ERROR: 内存分配失败" << std::endl;
		return;
	}
	memset(memory, 0, size);
	mem_size = size;
	is_use_mempool = false;
}
bool hstring::initMemory()
{
	mem_size = mem_pool.mem_block_size;
	memory = mem_pool.allocMemory();
	is_use_mempool = true;
	return true;
}

//KMP算法实现字符串匹配
int hstring::find(const hstring& str) const {
	if (this->length < str.length || str.length <= 0) {
		std::cout << "INFO: 字符串中没有找到子串,"<< std::endl;;
		return -1;
	}
	//1. 获取next数组
	int* next = (int*)malloc(str.length*sizeof(int));
	if (!next) {
		std::cout << "ERROR: KMP算法分配next数组空间失败，next==nullptr" << std::endl;
		return -1;
	}
	memset(next, 0, str.length);
	size_t j = 0;
	next[0] = 0;
	for (size_t i = 1; i < str.length; i++) {
		while (j > 0 && str[i] != str[j]) {
			j = next[j];
		}
		if (str[i] == str[j]) {
			j++;
		}
		next[i] = j;
	}
	//2. kmp匹配
	j = 0;
	for (size_t i = 0; i < this->length; i++) {
		while (j > 0 && (*this)[i] != str[j]) {
			j = next[j - 1];
		}
		if ((*this)[i] == str[j]) {
			j++;
		}
		if (j == str.length) {
			free(next);
			return i-j+1;
		}
	}
	free(next);
	//TODO:logger打印hstring报错
	//logger::logger("INFO:", "字符串中没有找到子串", str);
	std::cout<< "INFO: 字符串中没有找到子串" << std::endl;
	return -1;
}
hstring& hstring::replace(const hstring& src, const hstring& dst) {
	int index = find(src);
	if (index == -1) {
		return *this;
	}
	int inc_size = dst.length - src.length;
	//剩余空间不够，需要扩容
	if (this->length + inc_size >= mem_size) {
		//this->length + str.length + 1 + MEMORY_SIZE - 1
		char* old_memory = memory;
		bool use_flag = is_use_mempool;
		allocBigMemory(this->length + inc_size + 1);
		//分别copy src前、dst、src后
		memcpy(memory, hstring_start, index);
		memcpy(memory + index, dst.hstring_start, dst.length);
		if (index + src.length < this->length) {
			//src后还有字符串
			memcpy(memory + index + dst.length,
				hstring_start + index + src.length,
				this->length - index - src.length);
		}
		*(memory + this->length + inc_size) = '\0';
		hstring_start = memory;
		freeHstring(old_memory, use_flag);
		this->length += inc_size;
		return *this;
	}
	else {
		//不用扩容
		int res_size = memory + mem_size - hstring_start - length - 1;
		if (res_size < inc_size) {
			//this后面空间放不下，内存对齐
			memcpy(memory,hstring_start,this->length+1);
			hstring_start = memory;
		}
		if (index + src.length < this->length) {
			//src后还有字符串
			memmove(hstring_start + index + dst.length,
				hstring_start + index + src.length,
				this->length - index - src.length);
		}
		memcpy(hstring_start + index, dst.hstring_start, dst.length);
		*(hstring_start + this->length + inc_size) = '\0';
		this->length += inc_size;
		return *this;

	}
}

std::ostream& operator<<(std::ostream& _cout, hstring& str) {
	_cout << str.hstring_start;
	return _cout;
}
hstring& hstring::operator+(const hstring& str)
{
	//this后空间能否直接拼接str
	size_t res_size = memory + mem_size - hstring_start - length - 1;
	if (res_size < str.length) {
		//this空间长度不够str
		if (this->length + str.length >= mem_size) {
			//this+str 大于当前memory长度，扩容
			//this->length + str.length + 1 + MEMORY_SIZE - 1
			char* old_memory = memory;
			bool used_flag = is_use_mempool;
			allocBigMemory(this->length + str.length + 1);
			memcpy(memory, hstring_start, length);
			hstring_start = memory;
			freeHstring(old_memory, used_flag);
		}
		else {
			//this+str 小于memory长度，只是存在内存浪费
			//内存对齐
			memcpy(memory, hstring_start, length + 1);
			hstring_start = memory;
		}
	}
	//在hstring_start后面直接拼接str
	memcpy(hstring_start+length,str.hstring_start,str.length+1);
	length += str.length;
	return *this;
}

hstring& hstring::operator-(const hstring& str) {
	*this = this->replace(str, "");
	return *this;
}

const char& hstring::operator[](size_t index) const
{
	if (index >= this->length) {
		std::cout << "ERROR: 数组越界" << std::endl;
		//TODO: 越界返回什么
		return '*';
	}
	return *(this->hstring_start + index);
}

hstring& hstring::operator=(const hstring& str)
{	
	if (str.length + 1 > mem_size) {
		freeHstring(memory,is_use_mempool);
		allocBigMemory(str.mem_size);
	}
	memcpy(memory, str.hstring_start, str.length + 1);
	hstring_start = memory;
	length = str.length;
	return *this;
	// TODO: 如果是浅拷贝，将memory设为shared指针，这里用weak指针
}

hstring& hstring::operator=(int num)
{
	if (memory == nullptr) {
		init();
	}
	else {
		length = 0;
		memset(memory, 0, mem_size);
		hstring_start = memory;
	}
	int2hstring(num);
	return *this;
}
