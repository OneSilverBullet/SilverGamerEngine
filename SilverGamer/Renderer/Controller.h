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
		void MoveLogic(GLint vKey, GLint vScancode, GLint vAction, GLint vMode); //�ƶ��߼�
		void RotateLogic(double, double); //ת���߼�
		void Upload(int shaderProgram); //����ǰcontroller�����Ϣ�ϴ�shader
		SGCameraBase* GetCamera() { return m_camera; }

	private:
		SGCameraBase* m_camera; //�漰�������
		GLFWwindow* m_window; //�漰����ǰwindow
	};
}


#endif