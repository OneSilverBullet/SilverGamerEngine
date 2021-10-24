#include "Reflection.h"

namespace Reflect
{
	/*
	* Primitive Type Type Descriptor		 
	*/

	/*
	* Type Descriptor for int
	*/
	struct TypeDescriptor_Int : TypeDescriptor {
		TypeDescriptor_Int() : TypeDescriptor{ "int", sizeof(int) } {}
		virtual void Dump(const void* obj, int) const override {
			std::cout << "int{" << *(const int*)obj << "}";
		}
	};

	//Partially Specialize TypeResolver for int
	template<>
	SG_API TypeDescriptor* GetPrimitiveTypeDescripter<int>() {
		static TypeDescriptor_Int typeDesc;
		return &typeDesc;
	}

	/*
	* Type Descriptor for string
	*/
	struct TypeDescriptor_StdString : TypeDescriptor {
		TypeDescriptor_StdString() : TypeDescriptor{ "std::string", sizeof(std::string) } {
		}
		virtual void Dump(const void* obj, int)const override {
			std::cout << "std::string{\"" << *(const std::string*)obj << "\"}";
		}
	};

	//Partially Specialize TypeResolver for std::string
	template<>
	SG_API TypeDescriptor* GetPrimitiveTypeDescripter<std::string>() {
		static TypeDescriptor_StdString typeDesc;
		return &typeDesc;
	}

}













