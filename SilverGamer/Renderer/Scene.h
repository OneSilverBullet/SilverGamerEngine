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
		void Init();  //��ʼ��
		
		void Render(SGController*); //��Ⱦ�ӿ�
		void AddEntity(IEntity* entity);

		//����PointLight
		void AddPointLight(SGPointLight* light); 
		void SetDirLight(SGDirLight* light);
		void UploadStaticLight(GLuint program);//���и��µ�ǰ�ƹ�
		
	private:
		std::vector<IEntity*> m_entities;
		CommonSceneInfo m_commonSceneInfo; //������Ϣ
	};
}

#endif
