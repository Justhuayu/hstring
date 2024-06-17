#include "mempool.h"
#include <iostream>
mempool::mempool(size_t size, size_t block_size) {
	if (block_size < sizeof(char*)) {
		//TODO: ��δ���block_size��С�����
		std::cout<<"Waring��block size Ӧ�ô���" << sizeof(char*) << "�ֽ�" << std::endl;
		std::cout<<"����block size Ϊ��С" << sizeof(char*) << "�ֽ�" << std::endl;
		block_size = sizeof(char*);
	}
	mem_block_size = block_size;
	mem_total_size = size;
	mem_total_block = (size + block_size - 1) / block_size;
	mem_free_block = mem_total_block;
	initMemory(mem_total_block);
}
mempool::~mempool() {
	destoryMempool();
}

bool mempool::initMemory(size_t size) {
	//ÿ��item��ǰchar*���ȵ��ֽڣ���Ϊnextָ��
	memory_free = (char*)malloc(size* mem_block_size);
	mempool_release.push_back(memory_free);
	if (!memory_free) {
		std::cout << "ERROR: �ڴ����ʧ��, memory == nullptr" << std::endl;
		return false;
	}
	memset(memory_free, 0, size * mem_block_size);
	
	char* ptr = memory_free;
	for (size_t i = 0; i < size-1; i++) {
		//ÿ����ַ��ǰchar*���ֽڣ�ָ��nextָ��
		*(char**)ptr = ptr + mem_block_size;
		ptr += mem_block_size;
	}
	*(char**)ptr = nullptr;
	ptr = nullptr;
	
	return true;
}
char* mempool::allocMemory() {
	if (!mem_free_block) {
		//��С�������Զ�����
		std::cout << "WARNING: �ڴ�ش�С�������Զ�����һ��" << std::endl;
		increaseMempool(this->mem_total_size);
	}
	mem_free_block--;
	char* ptr = memory_free;
	
	memory_free = *(char**)ptr;//����ظ�ͼ���һ�����ƻ���Ĳ���ϵͳ��������ں˵�ַ�����windows��ͬ������Ҫȥ��һ�£�
	memset(ptr, 0, sizeof(char*));//���*(char**)ptr����ôֱ��ת���ǲ���ȫ�ģ�2��ת1���Ĳ����������������Ȼ���ʺϷ��ˣ���ȥ���
	return ptr;//����ʱ�������Ҳ������
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
	//����ռ�
	char* ptr = (char*)malloc(inc_block * mem_block_size);
	mempool_release.push_back(ptr);
	if (!ptr) {
		std::cout << "ERROR: �����ڴ�ش�Сʧ��" << std::endl;
		return false;
	}
	memset(ptr, 0, inc_block * mem_block_size);
	//�����¿ռ�
	size_t tmp = mem_free_block;
	char* memory_end = memory_free;
	if (memory_end) {
		//ʣ��ռ䲻Ϊ��
		while (--tmp) {
			memory_end = *(char**)memory_end;
		}
		*(char**)memory_end = ptr;
		memory_end += mem_block_size;
	}
	else {
		//ʣ��ռ�0
		memory_end = ptr;
		memory_free = ptr;
	}

	for (size_t i = 1; i < inc_block; i++) {
		*(char**)memory_end = memory_end + mem_block_size;
		memory_end += mem_block_size;
	}
	*(char**)memory_end = nullptr;
	mem_total_size += (inc_block * mem_block_size);
	mem_total_block += inc_block;
	mem_free_block += inc_block;
	return true;
}
void mempool::destoryMempool() {
	for (char* item : mempool_release) {
		free(item);
		item = nullptr;
	}
	mempool_release.clear();

}
