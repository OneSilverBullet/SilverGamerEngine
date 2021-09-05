#ifndef SILVER_GAMER_MEMORY_MANAGER
#define SILVER_GAMER_MEMORY_MANAGER

#include "MacroLib.h"

#include <Windows.h>
#include <new.h>

//Silver Gamer引擎的内存管理
namespace SilverGamer
{
	//基础内存管控类
	class SG_API MemManagerBase
	{
	public :
		MemManagerBase();
		virtual ~MemManagerBase() = 0;
		//申请内存函数
		virtual void* Allocat(SILVER_UINT uiSize, SILVER_UINT uiAlignment, bool isArray) = 0;
		//释放内存函数
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



	//UE3的内存申请方式，正式方式
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
			PoolInfo* GetPool(); //得到当前freemem所在的pool
		};


		struct PoolInfo
		{
			DWORD bytes; //当前pool分配的字节
			DWORD offsetBytes;
			DWORD elementNum; //分配的元数量，为0则释放整个Pool
			BYTE* mem;
			PoolTable* parentTabel;
			FreeMem* firstMem;
			PoolInfo* next;
			PoolInfo* prev;

			//以下两个函数传入的都是PoolInfo的链接、断开链接函数
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
			DWORD blockSize; //每一次可以分配的块大小
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