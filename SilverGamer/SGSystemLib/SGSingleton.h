#ifndef SG_SINGLETON_H
#define SG_SINGLETON_H

#include "MacroLib.h"

namespace SILVER_ENGINE
{
	template<typename T>
	class SGSingleton
	{
		static T* m_pSingleton;

	public:
		SGSingleton()
		{
			SG_WANDER(!m_pSingleton);
			m_pSingleton = static_cast<T*>(this);
		}

		~SGSingleton()
		{
			SG_WANDER(m_pSingleton);
			m_pSingleton = NULL;
		}

		static T& GetSingleton()
		{
			SG_WANDER(m_pSingleton);
			return m_pSingleton;
		}

		static T* GetSingletonPtr()
		{
			SG_WANDER(m_pSingleton);
			return (*m_pSingleton);
		}
	};

	template<typename T>
	T* SGSingleton<T>::m_pSingleton = NULL;
}




#endif

