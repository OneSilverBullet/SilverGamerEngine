#pragma once

#include  <assert.h>


//��ǰ��Ŀ�ĺ궨��API�����ڶ�̬���
#ifdef SG_EXPORTS
#define SG_API __declspec(dllexport) 
#else
#define SG_API __declspec(dllimport) 
#endif

//��ǰ������
#define SG_WANDER(Expression) {assert(SUCCEEDED(Expression));}

//����
#define FORCEINLINE __forceinline

//��ָ��
#define NULL nullptr


//һЩ�ض�������
#define SILVER_UINT unsigned int


