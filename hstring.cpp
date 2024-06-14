#include "hstring.h"
#include "logger.h"
#define MEMORY_SIZE 3

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
hstring::hstring(float str) {
	init();

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
	free(memory);
}


void hstring::int2hstring(int num) {
	hstring_start = memory + mem_size - 1 ;
	*hstring_start = '\0';
	int tmp;
	while(num>0){
		tmp = num % 10;
		length++;
		if (length >= mem_size) {
			char* old_memory = memory;
			//内存大小不够，重新分配
 			initMemory(mem_size+MEMORY_SIZE);
			//将已经初始化的部分，copy到新内存的末尾
			char* tmp = memory + mem_size - length;
			memcpy(tmp, hstring_start, length);
			hstring_start = tmp;
			free(old_memory);
		}
		hstring_start--;
		*hstring_start = tmp + '0';
		num /= 10;

	}
}
void hstring::cstr2hstring(const char* str)
{
	const char* tmp = str;
	while (*tmp != '\0') {
		tmp++;
		length++;
	}
	if (length >= mem_size) {
		char* old_memory = memory;
		size_t resize = (length + MEMORY_SIZE) / MEMORY_SIZE;
		initMemory(resize * MEMORY_SIZE);
		free(old_memory);
	}
	memcpy(memory, str, length);
	*(memory + length) = '\0';
	hstring_start = memory;
}
void hstring::init() {
	initMemory(MEMORY_SIZE);
	length = 0;
}
bool hstring::initMemory(size_t size)
{
	mem_size = size;
	if (size <= 0)
	{
		logger::logger("ERROR:","内存分配失败, size<=0");
		return false;
	}
	memory = (char*)malloc(size);
	if (!memory) {
		logger::logger("ERROR:", "内存分配失败, memory == nullptr");
		return false;
	}
	memset(memory, 0, size);
	return true;
}

//KMP算法实现字符串匹配
int hstring::find(const hstring& str) const {
	if (this->length < str.length || str.length <= 0) {
		logger::logger("INFO:", "字符串中没有找到子串", str);
		return -1;
	}
	//1. 获取next数组
	int* next = (int*)malloc(str.length*sizeof(int));
	if (!next) {
		//logger::logger("ERROR:", "KMP算法分配next数组空间失败，next==nullptr");
		return -1;
	}
	memset(next, 0, str.length);
	size_t j = 0;
	next[0] = 0;
	for (int i = 1; i < str.length; i++) {
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
	for (int i = 0; i < this->length; i++) {
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
		size_t resize = (this->length + inc_size + MEMORY_SIZE) / MEMORY_SIZE;
		char* old_memory = memory;
		initMemory(resize * MEMORY_SIZE);
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
		free(old_memory);
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
	int res_size = memory + mem_size - hstring_start - length - 1;
	if (res_size < str.length) {
		//this空间长度不够str
		if (this->length + str.length >= mem_size) {
			//this+str 大于当前memory长度，扩容
			//this->length + str.length + 1 + MEMORY_SIZE - 1
			size_t resize = (this->length + str.length + MEMORY_SIZE) / MEMORY_SIZE;
			char* old_memory = memory;
			initMemory(resize * MEMORY_SIZE);
			memcpy(memory, hstring_start, length + 1);
			hstring_start = memory;
			free(old_memory);
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

const char& hstring::operator[](int index) const
{
	if (index >= this->length) {
		logger::logger("ERROR:", "数组越界");
		return '*';
	}
	return *(this->hstring_start + index);
}

hstring& hstring::operator=(const hstring& str)
{
	
	hstring_start = str.hstring_start;
	if (str.length + 1 > mem_size) {
		free(memory);
		int resize = (str.mem_size + MEMORY_SIZE - 1) / MEMORY_SIZE;
		initMemory(resize*MEMORY_SIZE);
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
