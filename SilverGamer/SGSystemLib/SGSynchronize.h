#ifndef SG_SYNCHRONIZE_H
#define SG_SYNCHRONIZE_H

#include "SGSystem.h"

namespace SILVER_ENGINE
{
	//��������װ
	class SG_API SGCriticalSection {
		CRITICAL_SECTION criticalSection;

	public:
		//����������
		FORCEINLINE SGCriticalSection() {
			InitializeCriticalSection(&criticalSection);
			SetCriticalSectionSpinCount(&criticalSection, 4000); //����������
		}

		//ɾ����
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



