#include "Scene.h"

void Renderer::SGScene::Init()
{

}

void Renderer::SGScene::Render(SGController* controller)
{
	//TODO: Dynamic Lights
	for (int i = 0; i < m_entities.size(); ++i)
	{
		m_entities[i]->Render(&m_commonSceneInfo, controller);
	}
}

void Renderer::SGScene::AddEntity(IEntity* entity)
{
	m_entities.push_back(entity);
}

void Renderer::SGScene::AddPointLight(SGPointLight * light)
{
	m_commonSceneInfo.AddPointLight(light);
}

void Renderer::SGScene::SetDirLight(SGDirLight* light)
{
	m_commonSceneInfo.SetDirLight(light);
}

void Renderer::SGScene::UploadStaticLight(GLuint program)
{
	m_commonSceneInfo.Upload(program);
}
