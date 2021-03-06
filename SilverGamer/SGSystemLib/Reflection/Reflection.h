#pragma once
#include <vector>
#include <iostream>
#include <string>
#include "../MacroLib.h"

/*
* Reflection
*/
namespace Reflect
{
	/*
	* Basic Type Descripter Structure
	*/
	struct SG_API TypeDescriptor
	{
		const char* m_typeName;
		size_t m_typeSize;
		//
		TypeDescriptor() {}
		TypeDescriptor(const char* name, size_t size) :m_typeName(name), m_typeSize(size) {}
		virtual ~TypeDescriptor() {}
		virtual std::string GetTypeName() const { return m_typeName; }
		virtual void Dump(const void* obj, int indentLevel = 0) const = 0;
	};

	/*
	* For Build-in Type: Given a function to get Type Descripter
	* Just a Declaration. 
	*/
	template<typename T>
	SG_API TypeDescriptor* GetPrimitiveTypeDescripter();

	/*
	* Helper Class: Use meta-program to choose the way that get the type descripter.
	*/
	struct SG_API DefaultResolver
	{
		//if T is reflected, func return a char(A)
		template<typename T> static char func(decltype(&T::Reflection)); 
		//if T is not reflected, func return an int. (A reload failed)
		template<typename T> static int func(...);
		//Create a structure to know whether T is reflected
		template<typename T>
		struct IsReflected
		{
			enum { value = (sizeof(func<T>(nullptr)) == sizeof(char)) };
		};

		//this version is called if T has a static method called "Reflection"
		//For Class/Structure
		template<typename T, typename std::enable_if<IsReflected<T>::value, int>::type = 0>
		static TypeDescriptor* get() {
			return &T::Reflection;
		}

		//For Build-in Type
		template<typename T, typename std::enable_if<!IsReflected<T>::value, int>::type = 0>
		static TypeDescriptor* get() {
			return GetPrimitiveTypeDescripter<T>();
		}
	};

	/*
	* 	The Primary Class for Resolve. A capsulation for default resolver.
	*/
	template<typename T>
	struct SG_API TypeResolver
	{
		static TypeDescriptor* get() {
			return DefaultResolver::get<T>();
		}
	};


	/*
	* The Class/Structure Type Descripter
	*/
	struct SG_API TypeDescripter_Structure : TypeDescriptor {
		struct Member {
			const char* m_name;
			size_t m_offset;
			TypeDescriptor* type;
		};
		//Custom Structure??s member
		std::vector<Member> members;

		TypeDescripter_Structure() {}

		//Send an initialize function to constructor, and call this function in constructor.
		TypeDescripter_Structure(void (*init)(TypeDescripter_Structure*)) : TypeDescriptor{ nullptr, 0 } {
			init(this);
		}

		TypeDescripter_Structure(const char* name, size_t size, const std::initializer_list<Member>& init)
			: TypeDescriptor{ nullptr, 0 }, members{ init }
		{}


		virtual void Dump(const void* obj, int indentLevel) const override
		{
			std::cout << m_typeName << "{" << std::endl;
			for (const Member& member : members) {
				std::cout << std::string(4 * (indentLevel + 1), ' ') << member.m_name << " = ";
				member.type->Dump((char*)obj + member.m_offset, indentLevel + 1);
				std::cout << std::endl;
			}
			std::cout << std::string(4 * indentLevel, ' ') << "}";
		}
	};


	/*
	* The std::vector Type Descripter
	*/
	struct SG_API TypeDescriptor_StdVector : TypeDescriptor {
		TypeDescriptor* m_itemType;
		size_t(*getSize)(const void*);
		const void* (*getItem)(const void*, size_t);

		template<typename ItemType>
		TypeDescriptor_StdVector(ItemType*) :
			TypeDescriptor{ "std::vector<>", sizeof(std::vector<ItemType>) },
			m_itemType{ TypeResolver<ItemType>::get() }
		{
			//Init get size function for all vector
			getSize = [](const void* vecPtr)-> size_t {
				const auto& vec = *(const std::vector<ItemType>*)vecPtr;
				return vec.size();
			};
			getItem = [](const void* vecPtr, size_t index)->const void* {
				const auto& vec = *(const std::vector<ItemType>*)vecPtr;
				return &vec[index];
			};
		}

		virtual std::string GetTypeName() const override {
			return std::string("std::vector<") + m_itemType->GetTypeName() + ">";
		}

		virtual void Dump(const void* obj, int indentLevel) const override {
			size_t numItems = getSize(obj);
			std::cout << GetTypeName();
			if (numItems == 0) {
				std::cout << "{}";
			}
			else {
				std::cout << "{" << std::endl;
				for (size_t index = 0; index < numItems; index++) {
					std::cout << std::string(4 * (indentLevel + 1), ' ') << "[" << index << "] ";
					m_itemType->Dump(getItem(obj, index), indentLevel + 1);
					std::cout << std::endl;
				}
				std::cout << std::string(4 * indentLevel, ' ') << "}";
			}
		}
	};

	/*
	* TypeResolver for vector<T>
	* Partially Specialize TypeResolver
	*/
	template<typename T>
	struct SG_API TypeResolver<std::vector<T>>
	{
		static TypeDescriptor* get() {
			//Only send a nullptr vec ptr is ok
			//The type is nessecary
			static TypeDescriptor_StdVector typeDescripter{ (T*)nullptr }; 
			return &typeDescripter;
		}
	};

	/*
	* Reflection Related Macro: for custom structure 
	*/
#define REFLECT_STRUCTURE() \
	friend struct Reflect::DefaultResolver; \ 
	static Reflect::TypeDescripter_Structure Reflection; \
	static void InitReflection(Reflect::TypeDescripter_Structure*);


#define REFLECT_STRUCTURE_BEGIN(type) \
	Reflect::TypeDescripter_Structure type::Reflection{type::InitReflection}; \
	void type::InitReflection(Reflect::TypeDescripter_Structure* typeDesc) { \
			using T = type; \
			typeDesc->m_typeName = #type; \
			typeDesc->m_typeSize = sizeof(T); \
			typeDesc->members = {\

#define REFLECT_STRUCTURE_MEMBER(name) \
				{#name, offsetof(T, name), Reflect::TypeResolver<decltype(T::name)>::get()},

#define REFLECT_STRUCTURE_END()\
				}; \
			}

}