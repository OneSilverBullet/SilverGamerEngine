#ifndef SG_CAMERA
#define SG_CAMERA

#include "RenderSetting.h"

namespace Renderer
{
	class SGCamera
	{
	public:
		SGCamera(glm::vec3 pos = glm::vec3(0, 0, 2), 
			glm::vec3 target = glm::vec3(0, 0, 0),
			glm::vec3 up = glm::vec3(0, 1, 0));
		void Upload(GLuint program);

		glm::vec3 GetTarget() { return m_pos + m_dir; }
		glm::mat4 getPerspectiveMatrix(int w, int h);
		glm::mat4 getViewMatrix();

		glm::vec3 GetForward() { return m_dir; }
		glm::vec3 GetUp() { return m_up;   }
		glm::vec3 GetPosition() { return m_pos; }

		void SetForward(glm::vec3 a) { m_dir = a; }
		void SetUp(glm::vec3 a) { m_up = a; }
		void SetPosition(glm::vec3 a) { m_pos = a; }

	private:
		glm::vec3 m_pos;
		glm::vec3 m_dir;
		glm::vec3 m_up;
	};
}

#endif
