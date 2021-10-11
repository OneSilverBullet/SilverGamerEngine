#include "Scene.h"

void Renderer::SGScene::Init()
{
	m_camera = new SGCameraFirstRole(); //´´½¨ÉãÏñ»ú
}

void Renderer::SGScene::Render(GLuint shaderProgram)
{
	m_camera->LoadToShader(shaderProgram);
	for (int i = 0; i < m_models.size(); ++i) {
		m_models[i]->Draw(shaderProgram);
	}
}

void Renderer::SGScene::AddModel(SGModelBase* modelBasePtr)
{
	m_models.push_back(modelBasePtr);
}

void Renderer::SGScene::AddModel(std::string modelDir)
{
	SGModelBase* newModel = new SGModelBase(modelDir.c_str());
	m_models.push_back(newModel);
}

void Renderer::SGScene::AddLight(SGPointLight * light)
{
	m_pointLights.push_back(light);
}

void Renderer::SGScene::UploadStaticLight(GLuint program)
{
	RENDER_WARDER(m_pointLights.size() < POINT_LIGHT_MAX_NUM);
	for (int i = 0; i < m_pointLights.size(); ++i) {
		m_pointLights[i]->Upload(program, i);
	}
}
