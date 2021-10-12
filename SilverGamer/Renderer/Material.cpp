#include "Material.h"

Renderer::SGMaterial::SGMaterial(glm::vec3 _diffuseColor, glm::vec3 _specularColor, float _diffuseReflectivity, float _specularReflectivity, float _transparency, float _emissivity, float _shiness):
	m_diffuseColor(_diffuseColor), m_specularColor(_specularColor),
	m_diffuseReflectivity(_diffuseReflectivity),
	m_specularReflectivity(_specularReflectivity),
	m_emissivity(_emissivity),
	m_shiness(_shiness),
	m_transparency(_transparency)
{
}

void Renderer::SGMaterial::Upload(GLuint program)
{
	glUniform3fv(glGetUniformLocation(program, MATERIAL_VARIBLE_DIFFUSE), 1, glm::value_ptr(m_diffuseColor));
	glUniform1f(glGetUniformLocation(program, MATERIAL_VARIBLE_DIFFUSE_REFLECTANCE),  m_diffuseReflectivity);
	glUniform3fv(glGetUniformLocation(program, MATERIAL_VARIBLE_SPECULAR), 1, glm::value_ptr(m_specularColor));
	glUniform1f(glGetUniformLocation(program, MATERIAL_VARIBLE_SPECULAR_REFLECTANCE), m_specularReflectivity);
	glUniform1f(glGetUniformLocation(program, MATERIAL_VARIBLE_TRANSPARENCY), m_transparency);
	glUniform1f(glGetUniformLocation(program, MATERIAL_VARIBLE_EMISSIVITY), m_emissivity);
	glUniform1f(glGetUniformLocation(program, MATERIAL_VARIBLE_SHINESS), m_shiness);
}

void Renderer::SGMaterial::SetDiffuseColor(glm::vec3 diffuseColorVal)
{
	m_diffuseColor = diffuseColorVal;
}

glm::vec3 Renderer::SGMaterial::GetDiffuseColor()
{
	return m_diffuseColor;
}

Renderer::SGMaterialPhongFlat::SGMaterialPhongFlat(glm::vec3 diffuse, glm::vec3 specular, float shininess)
	: m_diffuse(diffuse), m_specular(specular), m_shininess(shininess)
{
}

Renderer::SGMaterialPhongFlat::SGMaterialPhongFlat()
	: m_diffuse(glm::vec3(0.7f)), m_specular(glm::vec3(0.3f)), m_shininess(0.03f)
{

}

void Renderer::SGMaterialPhongFlat::Upload(GLuint programId)
{
	glUniform3fv(glGetUniformLocation(programId, MATERIAL_VARIBLE_DIFFUSE), 1, glm::value_ptr(m_diffuse));
	glUniform3fv(glGetUniformLocation(programId, MATERIAL_VARIBLE_SPECULAR), 1, glm::value_ptr(m_specular));
	glUniform1f(glGetUniformLocation(programId, MATERIAL_VARIBLE_SHINESS), m_shininess);
}
