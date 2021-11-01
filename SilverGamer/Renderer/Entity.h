#pragma once

#include "RenderSetting.h"
#include "Material.h"
#include "BaseGeometry.h"
#include "Transform.h"
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
*/

namespace Renderer
{
	//当前Entity的基类
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

