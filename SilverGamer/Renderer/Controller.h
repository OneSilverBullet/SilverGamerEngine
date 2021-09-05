#ifndef SG_CONTROLLER
#define SG_CONTROLLER


#include "RenderSetting.h"
#include "Camera.h"


namespace Renderer
{

	class SGControllerFPS
	{
	public:
		SGControllerFPS(SGCamera*, int w, int h);
		~SGControllerFPS();
		void Move();
		void Init();
		void Update();

		//回调函数
		void OnMouseMove(int x, int y);
		void OnKeyboard(unsigned char key, int x, int y);
		void OnKeyboardUp(unsigned char key, int x, int y);

		glm::vec3 Forward() { return m_camera->GetForward(); }
		glm::vec3 Right() { return glm::cross(m_camera->GetForward(), m_camera->GetUp()); }
		glm::vec3 Up() { return m_camera->GetUp(); }



		//控制参数
		float m_mouseSensitivity{ 0.3f };
		float m_step{ 0.05f };
		float m_horizontal{ 0 };
		float m_vertical{ 0 };
		float m_gravity{ 0 };


	protected:
		//据参数
		SGCamera* m_camera;
		float m_yaw = 0.0f;
		float m_pitch = 0.0f;
		int m_width, m_height;
		float m_mouseX, m_mouseY;
		bool m_useCursor{ false };
	};


}


#endif