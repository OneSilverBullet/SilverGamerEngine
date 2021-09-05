#ifndef SG_SYNCHRONIZE_H
#define SG_SYNCHRONIZE_H

#include "SGSystem.h"

namespace SILVER_ENGINE
{
	//自旋锁封装
	class SG_API SGCriticalSection {
		CRITICAL_SECTION criticalSection;

	public:
		//构建自旋锁
		FORCEINLINE SGCriticalSection() {
			InitializeCriticalSection(&criticalSection);
			SetCriticalSectionSpinCount(&criticalSection, 4000); //构建自旋锁
		}

		//删除锁
		FORCEINLINE ~SGCriticalSection() {
			DeleteCriticalSection(&criticalSection);
		}

		FORCEINLINE void Lock() {
			EnterCriticalSection(&criticalSection);
		}

		FORCEINLINE void Unlock() {
			LeaveCriticalSection(&criticalSection);
		}

	};




}




#endif



