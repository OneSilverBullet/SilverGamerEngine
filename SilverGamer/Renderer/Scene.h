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
		void Init();  //初始化
		void Render(GLuint shaderProgram);
		void AddModel(std::string modelDir);
		void AddLight(SGPointLight* light); 
		void UploadLight(GLuint program);//进行更新当前灯光
		
		SGCamera* GetCamera() { return m_camera; }

	private:
		std::vector<SGModelBase*> m_models;
		std::vector<SGPointLight*> m_lights;
		SGCamera* m_camera;





	};
}

#endif
