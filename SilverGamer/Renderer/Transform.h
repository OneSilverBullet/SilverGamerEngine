#ifndef SG_TRANSFORM
#define SG_TRANSFORM

#include "RenderSetting.h"

namespace Renderer
{
	class SGTransform
	{
	public:
		SGTransform() {}

		void UpdateTransformMatrix();

		void Upload(int program); //в╟ть

		glm::vec3 GetPosition() { return m_position; }
		glm::vec3 GetEulerRot() { return m_eulerRot; }
		glm::vec3 GetScale() { return m_scale; }
		glm::mat4 GetLocalMatrix() { return m_localTransform; }
		glm::mat4 GetGlobalMatrix() { return m_globalTransform; }
		void SetPosition(glm::vec3 position) { m_position = position; }
		void SetEulerRot(glm::vec3 eulerRot) { m_eulerRot = eulerRot; }
		void SetScale(glm::vec3 scale) { m_scale = scale; }
		void MovePosition(glm::vec3 addDistance) { m_position += addDistance; }
		void RotateEuler(glm::vec3 rotateValue);


		void SetParent(SGTransform* parent) { m_parent = parent; }
		void AddChild(SGTransform* child) { m_childs.push_back(child); }
		void RemoveChild(SGTransform* child);

	private:
		SGTransform* m_parent; // parent transform
		std::vector<SGTransform*> m_childs; //child transform

		glm::vec3 m_position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_eulerRot = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_scale = { 1.0f, 1.0f, 1.0f };
		glm::mat4 m_localTransform = glm::mat4(1.0f);
		glm::mat4 m_globalTransform = glm::mat4(1.0f);
	};
}

#endif
