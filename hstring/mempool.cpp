#include "mempool.h"
#include <iostream>
mempool::mempool(size_t size, size_t item) {
	if (item < sizeof(char*)) {
		//TODO: ��δ���block_size��С�����
		std::cout << "ERROR: block size Ӧ�ô���" << sizeof(char*) << "�ֽ�" << std::endl;
	}
	mem_block_size = item;
	mem_total_size = size;
	mem_total_block = (size + item - 1) / item;
	mem_free_block = mem_total_block;
	initMemory(mem_total_block);
}
mempool::~mempool() {
	destoryMempool();
}

bool mempool::initMemory(size_t size) {
	//ÿ��item��ǰchar*���ȵ��ֽڣ���Ϊnextָ��
	memory_free = (char*)malloc(size* mem_block_size);
	if (!memory_free) {
		std::cout << "ERROR: �ڴ����ʧ��, memory == nullptr" << std::endl;
		return false;
	}
	memset(memory_free, 0, size * mem_block_size);
	char* ptr = memory_free;
	for (size_t i = 0; i < size; i++) {
		//ÿ����ַ��ǰchar*���ֽڣ�ָ��nextָ��
		*(char**)ptr = ptr + mem_block_size;
		ptr += mem_block_size;
	}
	*(char**)ptr = nullptr;
	return true;
}
char* mempool::allocMemory() {
	if (!mem_free_block) {
		//��С�������Զ�����
		std::cout << "WARNING: �ڴ�ش�С�������Զ�����һ��" << std::endl;
		increaseMempool(this->mem_total_size);
		return nullptr;
	}
	mem_free_block--;
	char* ptr = memory_free;
	memory_free = *(char**)ptr;
	memset(ptr, 0, sizeof(char*));
	return ptr;
}
bool mempool::freeMemory(void* ptr) {
	if (!ptr) {
		std::cout << "WARING: �����ڴ�Ϊ��" << std::endl;
		return false;
	}
	//TODO: ptr��С����block_sizeʱ������жϣ�
	memset(ptr, 0, mem_block_size);
	//ptr��nextָ��memory_free
	*(char**)ptr = memory_free;
	//memory_freeָ��ptr
	memory_free = (char*)ptr;
	mem_free_block++;
	return true;
}
bool mempool::increaseMempool(size_t size) {
	size_t inc_block = (size + mem_block_size - 1) / mem_block_size;
	size_t tmp = mem_free_block;
	char* memory_end = memory_free;
	while (--tmp) {
		memory_end = *(char**)memory_end;
	}

	char* ptr = (char*)malloc(inc_block * mem_block_size);
	if (!ptr) {
		std::cout << "ERROR: �����ڴ�ش�Сʧ��" << std::endl;
		return false;
	}
	memset(ptr, 0, inc_block * mem_block_size);
	*(char**)memory_end = ptr;
	memory_end += mem_block_size;
	for (size_t i = 1; i < inc_block; i++) {
		*(char**)memory_end = memory_end + mem_block_size;
		memory_end += mem_block_size;
	}
	*(char**)memory_end = nullptr;
	mem_total_size += inc_block * mem_block_size;
	mem_total_block += inc_block;
	mem_free_block += inc_block;
	return true;
}
void mempool::destoryMempool() {
	//TODO������mempool����
	if (memory_free) {
		free(memory_free);
		memory_free = nullptr;
	}

}
