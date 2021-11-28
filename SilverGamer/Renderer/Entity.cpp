#include "Entity.h"


Renderer::IEntity::IEntity(std::string modelDir)
{
	m_model = new SGModelBase(modelDir.c_str()); //加载model
	m_transform = new SGTransform();

	std::function<void(int, int, int, int)> rotateFunc = [=](int key, int scancode, int action, int mode)
	{
		this->RotateLogic(key, scancode, action, mode);
	};
	SGInputManager::Instance()->RegistKeyCallbackFunc(rotateFunc);
}

void Renderer::IEntity::Render(CommonSceneInfo* commonSceneInfo, SGController* controller)
{
	m_model->LoadTempRenderData(commonSceneInfo, controller, m_transform);
	//绘制模型
	m_model->Draw();
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
