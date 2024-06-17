#include "mempool.h"
#include <iostream>
mempool::mempool(size_t size, size_t block_size) {
	if (block_size < sizeof(char*)) {
		//TODO: 如何处理block_size过小的情况
		std::cout<<"Waring：block size 应该大于" << sizeof(char*) << "字节" << std::endl;
		std::cout<<"设置block size 为最小" << sizeof(char*) << "字节" << std::endl;
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
	//每个item的前char*长度的字节，作为next指针
	memory_free = (char*)malloc(size* mem_block_size);
	mempool_release.push_back(memory_free);
	if (!memory_free) {
		std::cout << "ERROR: 内存分配失败, memory == nullptr" << std::endl;
		return false;
	}
	memset(memory_free, 0, size * mem_block_size);
	
	char* ptr = memory_free;
	for (size_t i = 0; i < size-1; i++) {
		//每个地址的前char*个字节，指向next指针
		*(char**)ptr = ptr + mem_block_size;
		ptr += mem_block_size;
	}
	*(char**)ptr = nullptr;
	ptr = nullptr;
	
	return true;
}
char* mempool::allocMemory() {
	if (!mem_free_block) {
		//大小不够，自动扩容
		std::cout << "WARNING: 内存池大小不够，自动扩容一倍" << std::endl;
		increaseMempool(this->mem_total_size);
	}
	mem_free_block--;
	char* ptr = memory_free;
	
	memory_free = *(char**)ptr;//这里截个图，我怀疑是苹果的操作系统，这里的内核地址管理和windows不同，我需要去查一下，
	memset(ptr, 0, sizeof(char*));//这里，*(char**)ptr，这么直接转换是不安全的，2级转1级的操作，但是你这里居然访问合法了，我去查查
	return ptr;//先暂时这样，我查完回你
}
bool mempool::freeMemory(void* ptr) {
	if (!ptr) {
		std::cout << "WARING: 回收内存为空" << std::endl;
		return false;
	}
	//TODO: ptr大小不是block_size时，如何判断？
	memset(ptr, 0, mem_block_size);
	//ptr的next指向memory_free
	*(char**)ptr = memory_free;
	//memory_free指向ptr
	memory_free = (char*)ptr;
	mem_free_block++;
	return true;
}
bool mempool::increaseMempool(size_t size) {
	size_t inc_block = (size + mem_block_size - 1) / mem_block_size;
	//分配空间
	char* ptr = (char*)malloc(inc_block * mem_block_size);
	mempool_release.push_back(ptr);
	if (!ptr) {
		std::cout << "ERROR: 增加内存池大小失败" << std::endl;
		return false;
	}
	memset(ptr, 0, inc_block * mem_block_size);
	//链接新空间
	size_t tmp = mem_free_block;
	char* memory_end = memory_free;
	if (memory_end) {
		//剩余空间不为空
		while (--tmp) {
			memory_end = *(char**)memory_end;
		}
		*(char**)memory_end = ptr;
		memory_end += mem_block_size;
	}
	else {
		//剩余空间0
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
