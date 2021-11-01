#include "Scene.h"

void Renderer::SGScene::Init()
{

}

void Renderer::SGScene::Render(SGController* controller)
{

}

void Renderer::SGScene::AddEntity(IEntity* entity)
{
	m_models.push_back(entity);
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

}
