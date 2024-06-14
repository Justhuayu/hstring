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
	//=������ع�����ֱ��ʹ��
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
			//�ڴ��С���������·���
 			initMemory(mem_size+MEMORY_SIZE);
			//���Ѿ���ʼ���Ĳ��֣�copy�����ڴ��ĩβ
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
		logger::logger("ERROR:","�ڴ����ʧ��, size<=0");
		return false;
	}
	memory = (char*)malloc(size);
	if (!memory) {
		logger::logger("ERROR:", "�ڴ����ʧ��, memory == nullptr");
		return false;
	}
	memset(memory, 0, size);
	return true;
}

//KMP�㷨ʵ���ַ���ƥ��
int hstring::find(const hstring& str) const {
	if (this->length < str.length || str.length <= 0) {
		logger::logger("INFO:", "�ַ�����û���ҵ��Ӵ�", str);
		return -1;
	}
	//1. ��ȡnext����
	int* next = (int*)malloc(str.length*sizeof(int));
	if (!next) {
		//logger::logger("ERROR:", "KMP�㷨����next����ռ�ʧ�ܣ�next==nullptr");
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
	//2. kmpƥ��
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
	//TODO:logger��ӡhstring����
	//logger::logger("INFO:", "�ַ�����û���ҵ��Ӵ�", str);
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
		size_t resize = (this->length + inc_size + MEMORY_SIZE) / MEMORY_SIZE;
		char* old_memory = memory;
		initMemory(resize * MEMORY_SIZE);
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
		free(old_memory);
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
	int res_size = memory + mem_size - hstring_start - length - 1;
	if (res_size < str.length) {
		//this�ռ䳤�Ȳ���str
		if (this->length + str.length >= mem_size) {
			//this+str ���ڵ�ǰmemory���ȣ�����
			//this->length + str.length + 1 + MEMORY_SIZE - 1
			size_t resize = (this->length + str.length + MEMORY_SIZE) / MEMORY_SIZE;
			char* old_memory = memory;
			initMemory(resize * MEMORY_SIZE);
			memcpy(memory, hstring_start, length + 1);
			hstring_start = memory;
			free(old_memory);
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

const char& hstring::operator[](int index) const
{
	if (index >= this->length) {
		logger::logger("ERROR:", "����Խ��");
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
