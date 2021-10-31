#include "Material.h"

#include "ResourceLoad.h"

/*
void Renderer::SGMaterial::Upload(GLuint program)
{
	glUniform3fv(glGetUniformLocation(program, MATERIAL_VARIBLE_DIFFUSE), 1, glm::value_ptr(m_diffuseColor));
	glUniform1f(glGetUniformLocation(program, MATERIAL_VARIBLE_DIFFUSE_REFLECTANCE),  m_diffuseReflectivity);
	glUniform3fv(glGetUniformLocation(program, MATERIAL_VARIBLE_SPECULAR), 1, glm::value_ptr(m_specularColor));
	glUniform1f(glGetUniformLocation(program, MATERIAL_VARIBLE_SPECULAR_REFLECTANCE), m_specularReflectivity);
	glUniform1f(glGetUniformLocation(program, MATERIAL_VARIBLE_TRANSPARENCY), m_transparency);
	glUniform1f(glGetUniformLocation(program, MATERIAL_VARIBLE_EMIT), m_emissivity);
	glUniform1f(glGetUniformLocation(program, MATERIAL_VARIBLE_SHINESS), m_shiness);
}
*/

/*
* SGMaterialPhongFlat
*/
Renderer::SGMaterialPhongFlat::SGMaterialPhongFlat(glm::vec3 diffuse, glm::vec3 specular, float shininess)
	: m_diffuse(diffuse), m_specular(specular), m_shininess(shininess)
{
	m_shader = SGShaderFactory::Instance()->LoadShader("shader_normal", "shader_normal"); //¼ÓÔØpbr shader
}

void Renderer::SGMaterialPhongFlat::Load()
{
	glUseProgram(m_shader);
	glUniform3fv(glGetUniformLocation(m_shader, MATERIAL_VARIBLE_DIFFUSE), 1, glm::value_ptr(m_diffuse));
	glUniform3fv(glGetUniformLocation(m_shader, MATERIAL_VARIBLE_SPECULAR), 1, glm::value_ptr(m_specular));
	glUniform1f(glGetUniformLocation(m_shader, MATERIAL_VARIBLE_SHINESS), m_shininess);
}

void Renderer::SGMaterialPhongFlat::Unload()
{
}

Renderer::SGMaterialPhongFlat::SGMaterialPhongFlat()
	: m_diffuse(glm::vec3(0.7f)), m_specular(glm::vec3(0.3f)), m_shininess(0.03f)
{
	m_shader = SGShaderFactory::Instance()->LoadShader("shader_pbr", "shader_pbr"); //¼ÓÔØnormal shader
}

Renderer::SGMaterialPBRWithEmit::SGMaterialPBRWithEmit(std::string matDir)
{
	std::string aoDir = matDir + "ao.jpg";
	std::string diffuseDir = matDir + "color.jpg";
	std::string emitDir = matDir + "emit.jpg";
	std::string metallicDir = matDir + "metalness.jpg";
	std::string normalDir = matDir + "normal.jpg";
	std::string roughnessDir = matDir + "roughness.jpg";

	m_diffuse = ResourceLoad::Instance()->LoadTexture2DResource(SG_TEXTURE_TYPE::TEXTURE_DIFFUSE, diffuseDir);
	m_normal = ResourceLoad::Instance()->LoadTexture2DResource(SG_TEXTURE_TYPE::TEXTURE_NORMAL, normalDir);
	m_metalness = ResourceLoad::Instance()->LoadTexture2DResource(SG_TEXTURE_TYPE::TEXTURE_METALLIC, metallicDir);
	m_ao = ResourceLoad::Instance()->LoadTexture2DResource(SG_TEXTURE_TYPE::TEXTURE_AO, aoDir);
	m_emit = ResourceLoad::Instance()->LoadTexture2DResource(SG_TEXTURE_TYPE::TEXTURE_EMIT, emitDir);
	m_roughness = ResourceLoad::Instance()->LoadTexture2DResource(SG_TEXTURE_TYPE::TEXTURE_ROUGHNESS, roughnessDir);

	m_shader = SGShaderFactory::Instance()->LoadShader("shader_pbr", "shader_pbr"); //¼ÓÔØpbr shader
}

void Renderer::SGMaterialPBRWithEmit::Load()
{
	glUseProgram(m_shader);
	m_diffuse->Upload(m_shader, SG_TEXTURE_ACTIVE_SLOT0);
	m_normal->Upload(m_shader, SG_TEXTURE_ACTIVE_SLOT1);
	m_metalness->Upload(m_shader, SG_TEXTURE_ACTIVE_SLOT2);
	m_roughness->Upload(m_shader, SG_TEXTURE_ACTIVE_SLOT3);
	m_ao->Upload(m_shader, SG_TEXTURE_ACTIVE_SLOT4);
	m_emit->Upload(m_shader, SG_TEXTURE_ACTIVE_SLOT5);
}

void Renderer::SGMaterialPBRWithEmit::Unload()
{
}
