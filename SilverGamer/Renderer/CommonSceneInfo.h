#ifndef SG_COMMON_SCENE_INFO
#define SG_COMMON_SCENE_INFO

#include "RenderSetting.h"
#include "Light.h"

namespace Renderer
{
	/*
	* CommonSceneInfo: Common Scene Information. These information is needed by every entity in scene.
	*/
	class CommonSceneInfo
	{
	public:
		CommonSceneInfo(){}
		
		void AddPointLight(SGPointLight* point) { m_pointLights.push_back(point); m_pointLightsNum++; }
		void SetDirLight(SGDirLight* dirLight) { m_dirLight = dirLight; }
		SGPointLight* GetPointLight(int index) { return m_pointLights[index]; }
		SGDirLight* GetDirLight() { return m_dirLight; }
		void Upload(GLint program);

	private:
		int m_pointLightsNum = 0; //point lights number
		std::vector<SGPointLight*> m_pointLights; //point lights
		SGDirLight* m_dirLight; //direction lights
	};

}


#endif