#pragma once
class mempool
{
public:
	mempool() = default;
	mempool(size_t size,size_t item);//����pool��СΪsize����С��λΪitem������ȡ��
	~mempool();
	mempool(const mempool&) = delete;
	mempool& operator=(const mempool&) = delete;

	char* allocMemory();//�ӳ��ӷ���һ��block
	bool freeMemory(void* ptr);//����һ���ڴ�
	bool increaseMempool(size_t size);//����size��С�ڴ棬��block_size����ȡ��
	void destoryMempool();
private:
	bool initMemory(size_t block_size);//��ʼ��block_size�������ڴ��
public:
	size_t mem_total_size;//mempool ���ܴ�С
	size_t mem_block_size;//mempool �����ڴ����С��λ
private:
	size_t mem_total_block;//mempool �ܵ��ڴ������
	size_t mem_free_block;//mempool ���е��ڴ������
	char*  memory_free;//ָ��mempool �����ڴ��
};

