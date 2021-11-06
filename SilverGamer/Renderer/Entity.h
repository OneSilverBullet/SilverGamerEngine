#pragma once

#include "RenderSetting.h"
#include "Material.h"
#include "BaseGeometry.h"
#include "Transform.h"
#include "CommonSceneInfo.h"
#include "Controller.h"
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
* 
* ��7��ģ�͡��ƹ⡢�����������ȵ��޷���virtual world����ɲ��֣���Щ�������һ����render pass�� ���Ϻ�render pass֮��
* �ٽ����render pass װ�뵽 render queue�н��н�������Ⱦ��
* 
* virtual world <---------------> Render Queue <--------> Render Target <-------> Final Image
*							RenderPass									Map								 Merge
*/

namespace Renderer
{
	/*
	* IEntity: The basis class of render model.
	* TODO: A model contains many submeshes. One submesh map a IEntity.
	*/
	class IEntity
	{
	public:
		IEntity(std::string modelDir);
		//����һ����ǰ������common scene info
		void Render(CommonSceneInfo* sceneInfo, SGController* controller);
		//����Entity��λ��
		void SetEntityPosition(glm::vec3 position) { m_transform->SetPosition(position); }
		//����Entity��ŷ����
		void SetEntityEulerRot(glm::vec3 eulerRot) { m_transform->SetEulerRot(eulerRot); }
		//����Entity�ĳߴ�
		void SetEntityScale(glm::vec3 scale) { m_transform->SetScale(scale); }
		//�ƶ�����
		void MovePosition(glm::vec3 addDistance) { m_transform->MovePosition(addDistance); }
		//��ת����
		void RotateEuler(glm::vec3 rotateValue) { m_transform->RotateEuler(rotateValue); }
		//Set Parent Entity
		void SetParent(IEntity* entity);
		//Add Child Entity
		void AddChild(IEntity* child);
		//remove Child Entity
		void RemoveChild(IEntity* child);
		//change material
		void SetMaterial(SGMaterialBase* newMaterial);
		//�󶨿���
		void RotateLogic(GLint vKey, GLint vScancode, GLint vAction, GLint vMode); //�ƶ��߼�

	private:
		IEntity* m_parent; 
		std::vector<IEntity*> m_childs; 
		SGModelBase* m_model;
		SGMaterialBase* m_material;
		SGTransform* m_transform; 
	};
}

