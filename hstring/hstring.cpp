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
	//=������ع�����ֱ��ʹ��
	*this = str;
}
hstring::~hstring() {
	freeHstring(memory,is_use_mempool);
}

void hstring::int2hstring(int num) {
	//num == 0��Ĭ��ʲôҲû��
	//����numΪ�������
	bool negative = false;
	if (num < 0) {
		num *= -1;
		negative = true;
	}
	//���㵱ǰ������ڴ��С������������num
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
	//����
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
		std::cout << "ERROR: �ڴ����ʧ��" << std::endl;
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

//KMP�㷨ʵ���ַ���ƥ��
int hstring::find(const hstring& str) const {
	if (this->length < str.length || str.length <= 0) {
		std::cout << "INFO: �ַ�����û���ҵ��Ӵ�,"<< std::endl;;
		return -1;
	}
	//1. ��ȡnext����
	int* next = (int*)malloc(str.length*sizeof(int));
	if (!next) {
		std::cout << "ERROR: KMP�㷨����next����ռ�ʧ�ܣ�next==nullptr" << std::endl;
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
	//2. kmpƥ��
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
	//TODO:logger��ӡhstring����
	//logger::logger("INFO:", "�ַ�����û���ҵ��Ӵ�", str);
	std::cout<< "INFO: �ַ�����û���ҵ��Ӵ�" << std::endl;
	return -1;
}
hstring& hstring::replace(const hstring& src, const hstring& dst) {
	int index = find(src);
	if (index == -1) {
		return *this;
	}
	int inc_size = dst.length - src.length;
	//ʣ��ռ䲻������Ҫ����
	if (this->length + inc_size >= mem_size) {
		//this->length + str.length + 1 + MEMORY_SIZE - 1
		char* old_memory = memory;
		bool use_flag = is_use_mempool;
		allocBigMemory(this->length + inc_size + 1);
		//�ֱ�copy srcǰ��dst��src��
		memcpy(memory, hstring_start, index);
		memcpy(memory + index, dst.hstring_start, dst.length);
		if (index + src.length < this->length) {
			//src�����ַ���
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
		//��������
		int res_size = memory + mem_size - hstring_start - length - 1;
		if (res_size < inc_size) {
			//this����ռ�Ų��£��ڴ����
			memcpy(memory,hstring_start,this->length+1);
			hstring_start = memory;
		}
		if (index + src.length < this->length) {
			//src�����ַ���
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
	//this��ռ��ܷ�ֱ��ƴ��str
	size_t res_size = memory + mem_size - hstring_start - length - 1;
	if (res_size < str.length) {
		//this�ռ䳤�Ȳ���str
		if (this->length + str.length >= mem_size) {
			//this+str ���ڵ�ǰmemory���ȣ�����
			//this->length + str.length + 1 + MEMORY_SIZE - 1
			char* old_memory = memory;
			bool used_flag = is_use_mempool;
			allocBigMemory(this->length + str.length + 1);
			memcpy(memory, hstring_start, length);
			hstring_start = memory;
			freeHstring(old_memory, used_flag);
		}
		else {
			//this+str С��memory���ȣ�ֻ�Ǵ����ڴ��˷�
			//�ڴ����
			memcpy(memory, hstring_start, length + 1);
			hstring_start = memory;
		}
	}
	//��hstring_start����ֱ��ƴ��str
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
		std::cout << "ERROR: ����Խ��" << std::endl;
		//TODO: Խ�緵��ʲô
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
	// TODO: �����ǳ��������memory��Ϊsharedָ�룬������weakָ��
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
