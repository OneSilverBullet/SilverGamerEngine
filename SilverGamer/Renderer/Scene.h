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
		void AddModel(std::string modelDir);
		void AddLight(SGPointLight* light); 
		void UploadLight(GLuint program);//���и��µ�ǰ�ƹ�
		
		SGCamera* GetCamera() { return m_camera; }

	private:
		std::vector<SGModelBase*> m_models;
		std::vector<SGPointLight*> m_lights;
		SGCamera* m_camera;





	};
}

#endif
