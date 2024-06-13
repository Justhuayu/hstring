#pragma once
/*
�Զ����ַ�����
���ܣ�
	1. int��float��char*�����ͳ�ʼ��Ϊhstring
	2. ����+��-��new/delete
	3. ���巽����find��subsplace
	4. �ڴ��ͳһ����new���ڴ�
*/
#include <iostream>

class hstring
{
public:
	hstring() = default;
	hstring(int num);
	hstring(float num);
	hstring(const char* str);
	~hstring();
	
	friend std::ostream& operator<<(std::ostream& _cout, hstring& str);
	hstring& operator+(const hstring& str);//"123" + "abc" == "123abc"
	hstring& operator-(const hstring& str);//"12345" - "34" == "125"
	const char& operator[](int index) const;//str[1] == "2"
public:
	size_t length;//hstring�ַ����ĳ��ȣ�������β'\0'
	int find(const hstring& str) const;//hstring�в���hstring���ɹ�����λ�ã�ʧ�ܷ���-1
	hstring& replace(const hstring& src, const hstring& dst);//"1234" 34->abc == "12abc"
private:
	bool initMemory(size_t size);//����size��С���ڴ�
	void init();//��ʼ��hstring
	void int2hstring(int num);
	void cstr2hstring(const char* str);
private:
	char* memory;//ָ�򻺴����Ŀ�ʼλ��
	size_t mem_size;//�������Ĵ�С
	char* hstring_start;//hstring�ַ���ʼ��λ�ã���βΪ'\0'

};

std::ostream& operator<<(std::ostream& _cout, hstring& str);
/*
Ҫ��:
1.�洢���ݵ�ʱ��ʹ�û�����,ʹ�û�������Ŀ����Ϊ�˾�������Ƶ����NEW/DEL 
�ڴ��������Դ��ģ�����������Ҫ�У���֪���������Ļ�ȥ�������Σ�new֮ǰҪ���жϣ�
2.����+ ��������,��������Ҫʵ�ֵ���̬: ��123456789�� + ��abc��  �õ� ��123456789abc��
�����newһ�Σ�new֮ǰ�ж��Ƿ񳬹��������������˲�new
3.����- ɾ������,��������Ҫʵ�ֵ���̬:��123456789�� - ��456��  �õ� ��123789�� �������в�ʹ��new��
4.������ ,Ҫ��  ��123456789��  34�޸�Ϊ abc �õ� 12abc56789 �����newһ�Σ�ԭ��ͬ��
5.�� ,123456 �� 34 �õ� 34��λ�� 2  ����ʹ��new��
6.���� = ʵ�� int ת hstring �ַ��� �����newһ�Σ�
7.����ʹ�ÿ⺯����memcpy��ʹ�ã�
8.ÿ�����ܶ�����һ��֮���ٰ���Ŀ��Ⱥ�ע�⣺�ڲ��ҡ�ɾ�����޸ĵ�ʱ�Ҳ����ַ���Ҫ����������ʾ��
*/
