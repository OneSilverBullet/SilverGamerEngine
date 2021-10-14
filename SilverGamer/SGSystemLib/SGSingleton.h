#ifndef SG_SINGLETON_H
#define SG_SINGLETON_H

#include "MacroLib.h"
#include <memory>

namespace SilverEngineLib
{

	template <typename T>
	class SG_API SGSingleton
	{
	public:
		virtual ~SGSingleton() = default;
		static std::shared_ptr<T> Instance()
		{
			if (!m_pInstance)
				m_pInstance.reset(new T());    //Can not use std::make_shared<T>(), because private constructor of T cant be accessed in memory(shared_ptr object) 
			return m_pInstance;
		}

	protected:								   //protected, not private, because T's constuctor will call this CSingleton() constructor 
		SGSingleton() = default;

	private:
		static std::shared_ptr<T> m_pInstance;
	};

	template <typename T>
	std::shared_ptr<T> SGSingleton<T>::m_pInstance = nullptr;

}




#endif

