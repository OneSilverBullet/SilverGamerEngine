#include "Entity.h"


Renderer::IEntity::IEntity(std::string modelDir)
{
	m_model = new SGModelBase(modelDir.c_str()); //加载model
	m_transform = new SGTransform();
	int finalIndex = modelDir.find_last_of('/');
	std::string modelFileDir = modelDir.substr(0, finalIndex); //Get Model File Dir
	std::string textureDir = modelFileDir + "/textures/";
	m_material = new SGMaterialPBRWithEmit(textureDir);

	std::function<void(int, int, int, int)> rotateFunc = [=](int key, int scancode, int action, int mode)
	{
		this->RotateLogic(key, scancode, action, mode);
	};
	SGInputManager::Instance()->RegistKeyCallbackFunc(rotateFunc);
}

void Renderer::IEntity::Render(CommonSceneInfo* commonSceneInfo, SGController* controller)
{
	//上传当前material
	m_material->Load();
	//upload controller
	controller->LoadToShader(m_material->GetShaderInstance());
	//上传upload
	commonSceneInfo->Upload(m_material->GetShaderInstance());
	//上传transform
	m_transform->Upload(m_material->GetShaderInstance());
	//绘制模型
	m_model->Draw(m_material->GetShaderInstance());
}

void Renderer::IEntity::SetParent(IEntity* entity)
{

}

void Renderer::IEntity::AddChild(IEntity* child)
{
}

void Renderer::IEntity::RemoveChild(IEntity* child)
{
}

void Renderer::IEntity::SetMaterial(SGMaterialBase* newMaterial)
{
}

void Renderer::IEntity::RotateLogic(GLint vKey, GLint vScancode, GLint vAction, GLint vMode)
{
	int moveX = 0;
	int moveY = 0;

	if (vKey == GLFW_KEY_W && vAction == GLFW_REPEAT) {
		moveY = 1;
	}
	if (vKey == GLFW_KEY_S && vAction == GLFW_REPEAT) {
		moveY = -1;
	}
	if (vKey == GLFW_KEY_A && vAction == GLFW_REPEAT) {
		moveX = -1;
	}
	if (vKey == GLFW_KEY_D && vAction == GLFW_REPEAT) {
		moveX = 1;
	}

	m_transform->RotateEuler(glm::vec3(moveX, moveY, 0.0f));
}
