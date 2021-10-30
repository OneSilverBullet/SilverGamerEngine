#ifndef SG_SCENE
#define SG_SCENE

#include "RenderSetting.h"
#include "BaseGeometry.h"
#include "Light.h"
#include "Camera.h"

namespace Renderer
{
	class SGScene
	{
	public:
		SGScene() {}
		void Init();  //��ʼ��
		void Render(GLuint shaderProgram);
		void AddModel(SGModelBase* modelBasePtr);
		void AddModel(std::string modelDir);
		void AddPointLight(SGPointLight* light); 
		void SetDirLight(SGDirLight* light);
		void UploadStaticLight(GLuint program);//���и��µ�ǰ�ƹ�
		
	private:
		std::vector<SGModelBase*> m_models;
		std::vector<SGPointLight*> m_pointLights;
		SGDirLight* m_dirLight;
	};
}

#endif
