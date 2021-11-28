#ifndef SG_CONTROLLER
#define SG_CONTROLLER
#include "InputManager.h"
#include "RenderSetting.h"
#include "Camera.h"

#define CONTROLLER_CAMERA_TYPE_FIRST_ROLE 0
#define CONTROLLER_CAMERA_TYPE_THIRD_ROLE 1

namespace Renderer
{
	class SGController
	{
	public:
		SGController(GLFWwindow* window);
		void ChangeCamera(int cameraType);
		void RegistCameraLogic();
		void MoveLogic(GLint vKey, GLint vScancode, GLint vAction, GLint vMode); //移动逻辑
		void RotateLogic(double, double); //转向逻辑
		void Upload(int shaderProgram); //将当前controller相关信息上传shader
		SGCameraBase* GetCamera() { return m_camera; }

	private:
		SGCameraBase* m_camera; //涉及到摄像机
		GLFWwindow* m_window; //涉及到当前window
	};
}


#endif