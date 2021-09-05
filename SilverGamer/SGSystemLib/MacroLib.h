#pragma once

#include  <assert.h>


//当前项目的宏定义API，用于动态输出
#ifdef SG_EXPORTS
#define SG_API __declspec(dllexport) 
#else
#define SG_API __declspec(dllimport) 
#endif

//当前守卫者
#define SG_WANDER(Expression) {assert(SUCCEEDED(Expression));}

//内联
#define FORCEINLINE __forceinline

//空指针
#define NULL nullptr


//一些重定义类型
#define SILVER_UINT unsigned int


