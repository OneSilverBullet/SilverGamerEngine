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
* 初步的设计思路：
* （1）应该material 与 model，transform是并行关系。
* （2）两者共同被IEntity驾驭。
* （3）Scene是由各种IEntity构成的。
* （4）当前场景的光源、天空盒、各种参数隶属于世界参数，与GameObject无关。
* （5）在驱动世界渲染时候，先将对应的世界参数装载到IEntity中。
* 
* （6）后续操作：构建真正的渲染框架，上述一切功能是在CPU完成，装填参数完成后，统一生成一个个render pass装入render queue。
* 统一由渲染器进行渲染。
* 
* （7）模型、灯光、摄像机、世界等等无非是virtual world的组成部分，这些东西组成一个个render pass。 集合好render pass之后，
* 再将诸多render pass 装入到 render queue中进行解析、渲染。
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
		//传入一个当前场景的common scene info
		void Render(CommonSceneInfo* sceneInfo, SGController* controller);
		//设置Entity的位置
		void SetEntityPosition(glm::vec3 position) { m_transform->SetPosition(position); }
		//设置Entity的欧拉角
		void SetEntityEulerRot(glm::vec3 eulerRot) { m_transform->SetEulerRot(eulerRot); }
		//设置Entity的尺寸
		void SetEntityScale(glm::vec3 scale) { m_transform->SetScale(scale); }
		//移动函数
		void MovePosition(glm::vec3 addDistance) { m_transform->MovePosition(addDistance); }
		//旋转函数
		void RotateEuler(glm::vec3 rotateValue) { m_transform->RotateEuler(rotateValue); }
		//Set Parent Entity
		void SetParent(IEntity* entity);
		//Add Child Entity
		void AddChild(IEntity* child);
		//remove Child Entity
		void RemoveChild(IEntity* child);
		//change material
		void SetMaterial(SGMaterialBase* newMaterial);
		//绑定控制
		void RotateLogic(GLint vKey, GLint vScancode, GLint vAction, GLint vMode); //移动逻辑

	private:
		IEntity* m_parent; 
		std::vector<IEntity*> m_childs; 
		SGModelBase* m_model;
		SGMaterialBase* m_material;
		SGTransform* m_transform; 
	};
}

