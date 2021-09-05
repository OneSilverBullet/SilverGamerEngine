#ifndef SILVER_GAMER_MEMORY_MANAGER
#define SILVER_GAMER_MEMORY_MANAGER

#include "MacroLib.h"

#include <Windows.h>
#include <new.h>

//Silver Gamer������ڴ����
namespace SilverGamer
{
	//�����ڴ�ܿ���
	class SG_API MemManagerBase
	{
	public :
		MemManagerBase();
		virtual ~MemManagerBase() = 0;
		//�����ڴ溯��
		virtual void* Allocat(SILVER_UINT uiSize, SILVER_UINT uiAlignment, bool isArray) = 0;
		//�ͷ��ڴ溯��
		virtual void Deallocate(char* pcAddr, SILVER_UINT uiAlignment, bool isArray) = 0;
	};

	class SG_API MemMangerDebug : public MemManagerBase
	{
	public:
		MemMangerDebug();
		~MemMangerDebug();
		virtual void* Allocat(SILVER_UINT uiSize, SILVER_UINT uiAlignment, bool isArray) = 0;
		virtual void Deallocate(char* pcAddr, SILVER_UINT uiAlignment, bool isArray) = 0;

	private:
		enum
		{

		};
		
	};



	//UE3���ڴ����뷽ʽ����ʽ��ʽ
	class SG_API MemManagerSG : public MemManagerBase
	{
	public:
		MemManagerSG();
		~MemManagerSG();
		virtual void* Allocat(SILVER_UINT uiSize, SILVER_UINT uiAlignment, bool isArray);
		virtual void Deallocate(char* pcAddr, SILVER_UINT uiAlignment, bool isArray);

	private:

		enum { POOL_COUNT = 42 };
		enum { POOL_MAX = 32768 + 1 };


		struct PoolInfo;
		struct PoolTable;


		struct FreeMem
		{
			FreeMem* next;
			DWORD blocks;
			PoolInfo* GetPool(); //�õ���ǰfreemem���ڵ�pool
		};


		struct PoolInfo
		{
			DWORD bytes; //��ǰpool������ֽ�
			DWORD offsetBytes;
			DWORD elementNum; //�����Ԫ������Ϊ0���ͷ�����Pool
			BYTE* mem;
			PoolTable* parentTabel;
			FreeMem* firstMem;
			PoolInfo* next;
			PoolInfo* prev;

			//����������������Ķ���PoolInfo�����ӡ��Ͽ����Ӻ���
			void Link(PoolInfo*& head) {
				if (head) {
					head->prev = this;
				}
				next = head;
				prev = nullptr;
				head = this;
			}

			void Unlink(PoolInfo*& head) {
				if (next) {
					next->prev = prev;
				}
				if (prev) {
					prev->next = next;
					prev = nullptr;
				}
				else {
					head = next;
				}
				next = nullptr;
			}
		};

		struct PoolTable
		{
			PoolInfo* freePools;
			PoolInfo* exhaustedPools;
			DWORD blockSize; //ÿһ�ο��Է���Ŀ��С
		};

		PoolTable poolTables[POOL_COUNT], offsetTable;
		PoolInfo* poolIndirect;
		PoolTable* memSizeToPoolTable[POOL_MAX];
		INT pageSize;

		PoolInfo* CreateIndirect() {
			PoolInfo* indirect = (PoolInfo*)VirtualAlloc(NULL, 2048 * sizeof(PoolInfo), MEM_COMMIT, PAGE_READWRITE);
			if (indirect == nullptr) {
				return nullptr;
			}
			return indirect;
		}


	};


}








#endif