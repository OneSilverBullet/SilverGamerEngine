#pragma once

#include <vector>
#include <iostream>
#include <string>

/*
* Reflection
*/
namespace Reflection
{
	/*
	* Basic Type Descripter Structure
	*/
	struct TypeDescriptor
	{
		const char* m_typeName;
		size_t m_typeSize;
		//
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
	TypeDescriptor* GetPrimitiveTypeDescripter();

	/*
	* Helper Class: Use meta-program to choose the way that get the type descripter.
	*/
	struct DefaultResolver
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
	struct TypeResolver
	{
		static TypeDescriptor* get() {
			return DefaultResolver::get<T>();
		}
	};

	/*
	* The Class/Structure Type Descripter
	*/
	struct TypeDescripter_Structure : TypeDescriptor {
		struct Member {
			const char* m_name;
			size_t m_offset;
			TypeDescriptor* type;
		};
		//Custom Structure¡®s member
		std::vector<Member> members;

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





}