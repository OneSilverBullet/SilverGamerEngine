#include "Light.h"

Renderer::SGPointLight::SGPointLight(glm::vec3 pos)
	:m_pos(pos)
{
	m_color = glm::vec3(1.0f, 1.0f, 1.0f);
	m_ambientColor = glm::vec3(0.05f, 0.05f, 0.05f);
	m_diffuseColor = glm::vec3(0.8f, 0.8f, 0.8f);
	m_specularColor = glm::vec3(1.0f, 1.0f, 1.0f);
	m_constant = 1.0;
	m_linear = (float)0.9;
	m_quadratic = (float)0.032;
}

void Renderer::SGPointLight::Upload(GLuint program, GLuint index)
{
	glUniform3fv(glGetUniformLocation(program, ("pointLights[" + std::to_string(index) + "].light_color").c_str()), 1, &m_color[0]);
	glUniform3fv(glGetUniformLocation(program, ("pointLights[" + std::to_string(index) + "].position").c_str()), 1, &m_pos[0]);
	glUniform3fv(glGetUniformLocation(program, ("pointLights[" + std::to_string(index) + "].ambient").c_str()), 1, &m_ambientColor[0]);
	glUniform3fv(glGetUniformLocation(program, ("pointLights[" + std::to_string(index) + "].diffuse").c_str()), 1, &m_diffuseColor[0]);
	glUniform3fv(glGetUniformLocation(program, ("pointLights[" + std::to_string(index) + "].specular").c_str()), 1, &m_specularColor[0]);
	
	glUniform1f(glGetUniformLocation(program, ("pointLights[" + std::to_string(index) + "].constant").c_str()), m_constant);
	glUniform1f(glGetUniformLocation(program, ("pointLights[" + std::to_string(index) + "].linear").c_str()), m_linear);
	glUniform1f(glGetUniformLocation(program, ("pointLights[" + std::to_string(index) + "].quadratic").c_str()), m_quadratic);
}

Renderer::SGDirLight::SGDirLight()
{
	m_color = glm::vec3(1.0f, 1.0f, 1.0f);
	m_direction = glm::vec3(1.0f, -1.0f, -1.0f);
	m_diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
	m_specular = glm::vec3(1.0f, 1.0f, 1.0f);
}

void Renderer::SGDirLight::Upload(GLuint program, GLuint index)
{
	glUniform3fv(glGetUniformLocation(program, "dirLight.light_color"), 1, &m_color[0]);
	glUniform3fv(glGetUniformLocation(program, "dirLight.direction"), 1, &m_direction[0]);
	glUniform3fv(glGetUniformLocation(program, "dirLight.diffuse"), 1, &m_diffuse[0]);
	glUniform3fv(glGetUniformLocation(program, "dirLight.specular"), 1, &m_specular[0]);
}

void Renderer::SGDirLight::AddDirection(glm::vec3 addDirection)
{
	m_direction += addDirection;
	m_direction = glm::normalize(m_direction);
}
