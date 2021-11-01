#include "CommonSceneInfo.h"


void Renderer::CommonSceneInfo::Upload(GLint program)
{
	//upload direction light
	m_dirLight->Upload(program, 0);
	//upload lights number
	int lightNumLocation = glGetUniformLocation(program, POINT_LIGHT_NUM);
	if (lightNumLocation != -1) {
		glUniform1i(lightNumLocation, m_pointLightsNum);
	}
	//upload point lights
	RENDER_WARDER(m_pointLights.size() < POINT_LIGHT_MAX_NUM);
	for (int i = 0; i < m_pointLights.size(); ++i) {
		m_pointLights[i]->Upload(program, i);
	}
}