#include "hstring.h"
#include "logger.h"
hstring::hstring(int num) {
	init();
	int2hstring(num);
	//TODO: ��������С�������Զ�����

}

hstring::hstring(float str) {
	init();

}
hstring::hstring(const char* str)
{
	init();
	cstr2hstring(str);
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
		num /= 10;
		hstring_start--;
		*hstring_start = tmp + '0';
	}
}
void hstring::cstr2hstring(const char* str)
{
	const char* tmp = str;
	while (*tmp != '\0') {
		tmp++;
		length++;
	}
	memcpy(memory, str, length);
	*(memory + length) = '\0';
	hstring_start = memory;
}
void hstring::init() {
	mem_size = 0x1000;//��ʼ4B��С������
	initMempool(mem_size);
	length = 0;
}
bool hstring::initMempool(size_t size)
{
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
	return true;
}

std::ostream& operator<<(std::ostream& _cout, hstring& str) {
	_cout << str.hstring_start;
	return _cout;
}

hstring& hstring::operator+(const hstring& str)
{
	// TODO: �ڴ˴����� return ���
}
