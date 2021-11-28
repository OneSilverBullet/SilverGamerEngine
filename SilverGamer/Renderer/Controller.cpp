#include "Controller.h"


Renderer::SGController::SGController(GLFWwindow* window) : m_window(window)
{
	ChangeCamera(CONTROLLER_CAMERA_TYPE_FIRST_ROLE); //建立第三人称摄像机
	RegistCameraLogic();
}

void Renderer::SGController::ChangeCamera(int cameraType)
{
	if (cameraType == CONTROLLER_CAMERA_TYPE_FIRST_ROLE) {
		m_camera = new SGCameraFirstRole();
	}
	if (cameraType == CONTROLLER_CAMERA_TYPE_THIRD_ROLE) {
		m_camera = new SGCameraThirdRole();
	}
}

void Renderer::SGController::RegistCameraLogic()
{
	SGInputManager::Instance()->Init(m_window); //初始化当前window
	std::function<void(double, double)> rotateFunc = [=](double x, double y) {
		this->RotateLogic(x, y);
	};
	SGInputManager::Instance()->RegistCursorCallback(rotateFunc);

	std::function<void(int, int, int, int)> moveFunc = [=](int key, int scancode, int action, int mode)
	{
		this->MoveLogic(key, scancode, action, mode);
	};
	SGInputManager::Instance()->RegistKeyCallbackFunc(moveFunc); 
}


void Renderer::SGController::MoveLogic(GLint vKey, GLint vScancode, GLint vAction, GLint vMode)
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

	//处理第一人称摄像机相关属性
	SGCameraFirstRole* firstRoleCamera = dynamic_cast<SGCameraFirstRole*>(m_camera); 
	if (firstRoleCamera != nullptr) {
		firstRoleCamera->Move(moveX, moveY);
	}
}

void Renderer::SGController::RotateLogic(double xPos, double yPos)
{
	double xRotate = SGInputManager::Instance()->GetCursorOffset(CURSOR_OFFSET_X);
	double yRotate = SGInputManager::Instance()->GetCursorOffset(CURSOR_OFFSET_Y);
	//旋转摄像机
	m_camera->RotateCamera(xRotate, yRotate);
}

void Renderer::SGController::Upload (int shaderProgram)
{
	m_camera->LoadToShader(shaderProgram); 
}
