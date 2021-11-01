#pragma once

#include "RenderSetting.h"
#include "Material.h"
#include "BaseGeometry.h"
#include "Transform.h"
/*

*/

/*
* ���������˼·��
* ��1��Ӧ��material �� model��transform�ǲ��й�ϵ��
* ��2�����߹�ͬ��IEntity��Ԧ��
* ��3��Scene���ɸ���IEntity���ɵġ�
* ��4����ǰ�����Ĺ�Դ����պС����ֲ��������������������GameObject�޹ء�
* ��5��������������Ⱦʱ���Ƚ���Ӧ���������װ�ص�IEntity�С�
* 
* ��6������������������������Ⱦ��ܣ�����һ�й�������CPU��ɣ�װ�������ɺ�ͳһ����һ����render passװ��render queue��
* ͳһ����Ⱦ��������Ⱦ��
*/

namespace Renderer
{
	//��ǰEntity�Ļ���
	class IEntity
	{
	public:
		IEntity(std::string modelDir);



	private:
		SGModelBase* m_model;
		SGMaterialBase* m_material;
		SGTransform* m_transform; 
	};
}

