#ifndef SG_SCENE
#define SG_SCENE

#include "RenderSetting.h"
#include "BaseGeometry.h"
#include "Light.h"
#include "Controller.h"
#include "Entity.h"
#include "CommonSceneInfo.h"

namespace Renderer
{
	class SGScene
	{
	public:
		SGScene() {}
		void Init();  //初始化
		
		void Render(SGController*); //渲染接口
		void AddEntity(IEntity* entity);

		//增加PointLight
		void AddPointLight(SGPointLight* light); 
		void SetDirLight(SGDirLight* light);
		void UploadStaticLight(GLuint program);//进行更新当前灯光
		
	private:
		std::vector<IEntity*> m_entities;
		CommonSceneInfo m_commonSceneInfo; //公共消息
	};
}

#endif
