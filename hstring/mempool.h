#pragma once
class mempool
{
public:
	mempool() = default;
	mempool(size_t size,size_t item);//分配pool大小为size，最小单位为item，向上取整
	~mempool();
	mempool(const mempool&) = delete;
	mempool& operator=(const mempool&) = delete;

	char* allocMemory();//从池子分配一个block
	bool freeMemory(void* ptr);//回收一块内存
	bool increaseMempool(size_t size);//增加size大小内存，按block_size向上取整
	void destoryMempool();
private:
	bool initMemory(size_t block_size);//初始化block_size数量的内存块
public:
	size_t mem_total_size;//mempool 的总大小
	size_t mem_block_size;//mempool 分配内存的最小单位
private:
	size_t mem_total_block;//mempool 总的内存块数量
	size_t mem_free_block;//mempool 空闲的内存块数量
	char*  memory_free;//指向mempool 可用内存块
};

