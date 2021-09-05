#ifndef SG_TRANSFORM
#define SG_TRANSFORM

#include "RenderSetting.h"

namespace Renderer
{
	class SGTransform
	{
	public:
		SGTransform(glm::vec3 pos = glm::vec3(0), glm::vec3 s = glm::vec3(1));
		glm::mat4 m_Rotation, m_Translate, m_Scale, m_Transform;
		void Translate(glm::vec3 trans);
		void Rotate(float angle, glm::vec3 axis);
		void Scale(glm::vec3 s);
		void UpdateTransformMatrix();
	};
}

#endif
