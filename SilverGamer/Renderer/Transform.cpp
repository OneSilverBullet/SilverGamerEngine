#include "Transform.h"

Renderer::SGTransform::SGTransform(glm::vec3 pos, glm::vec3 s)
{
	m_Translate = glm::translate(pos);
	m_Rotation = glm::rotate(0.f, glm::vec3(0, 0, 1));
	m_Scale = glm::scale(s); 
	UpdateTransformMatrix();
}

void Renderer::SGTransform::Translate(glm::vec3 trans)
{
	m_Translate = glm::translate(trans);
	UpdateTransformMatrix();
}

void Renderer::SGTransform::Rotate(float angle, glm::vec3 axis)
{
	m_Rotation = glm::rotate(angle, axis);
	UpdateTransformMatrix();
}

void Renderer::SGTransform::Scale(glm::vec3 s)
{
	m_Scale = glm::scale(s);
	UpdateTransformMatrix();
}

void Renderer::SGTransform::UpdateTransformMatrix()
{
	m_Transform = m_Translate * m_Rotation * m_Scale;
}
