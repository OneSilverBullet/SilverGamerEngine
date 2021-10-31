#include "Transform.h"

void Renderer::SGTransform::UpdateTransformMatrix()
{
	//update local transform matrix
	const glm::mat4 transformX = glm::rotate(glm::mat4(1.0f),
		glm::radians(m_eulerRot.x),
		glm::vec3(1.0f, 0.0f, 0.0f));
	const glm::mat4 transformY = glm::rotate(glm::mat4(1.0),
		glm::radians(m_eulerRot.y),
		glm::vec3(0.0f, 1.0f, 0.0f));
	const glm::mat4 transformZ = glm::rotate(glm::mat4(1.0),
		glm::radians(m_eulerRot.z),
		glm::vec3(0.0f, 0.0f, 1.0f));

	const glm::mat4 rotationMatrix = transformY * transformX * transformZ;

	m_localTransform = glm::translate(glm::mat4(1.0f), m_position) *
		rotationMatrix * glm::scale(glm::mat4(1.0f), m_scale);

	// set global matrix
	if (m_parent != nullptr) {
		glm::mat4 globalMatrix = m_parent->GetGlobalMatrix();
		m_globalTransform = globalMatrix * m_localTransform;
	}
	
	for (SGTransform* child : m_childs) {
		child->UpdateTransformMatrix(); 
	}
}

void Renderer::SGTransform::Upload(int program)
{
	int model_loc = glGetUniformLocation(program, MODEL_MATRIX_GPU_HOOK);
	if (model_loc != -1) {
		glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(m_globalTransform));
	}
}

void Renderer::SGTransform::RemoveChild(SGTransform* child)
{
	int i = 0;
	for (; i < m_childs.size(); ++i) {
		if (m_childs[i] == child) {
			break;
		}
	}
	m_childs.erase(m_childs.begin() + i);
}
