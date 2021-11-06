#ifndef SG_LIGHT
#define SG_LIGHT

#include "RenderSetting.h"

namespace Renderer
{

	//TODO: Area Light, Spot Light
	//TODO: Light Base Class
	//TODO: static light or dynamic light flag
	class SGDirLight
	{
	public:
		SGDirLight();
		~SGDirLight() {}
		void Upload(GLuint program, GLuint index);
		void SetColor(glm::vec3 color) { m_color = color; }
		void SetDirection(glm::vec3 direction) { m_direction = direction; }
		void AddDirection(glm::vec3 addDirection);
		void SetDiffuse(glm::vec3 diffuse) { m_diffuse = diffuse; }
		void SetSpecular(glm::vec3 specular) { m_specular = specular; }

	private:
		glm::vec3 m_color; 
		glm::vec3 m_direction;
		glm::vec3 m_diffuse;
		glm::vec3 m_specular;
	};


	class SGPointLight
	{
	public:
		SGPointLight(glm::vec3 pos = glm::vec3(0));
		void Upload(GLuint program, GLuint index);

		//相关接口
		void SetConstant(float constant) { m_constant = constant; }
		float GetConstant() { return m_constant; }
		void SetLinear(float linear) { m_linear = linear; }
		float GetLinear() { return m_linear; }
		void SetQuadratic(float quadratic) { m_quadratic = quadratic; }
		float GetQuadratic() { return m_quadratic; }
		void SetPosition(glm::vec3 pos) { m_pos = pos; }
		glm::vec3 GetPosition() { return m_pos; }
		void SetColor(glm::vec3 color) { m_color = color; }
		glm::vec3 GetColor() { return m_color; }

		void SetDiffuse(glm::vec3 diffuse) { m_diffuseColor = diffuse; }
		glm::vec3 GetDiffuse() { return m_diffuseColor; }
		void SetAmbient(glm::vec3 ambient) { m_ambientColor = ambient; }
		glm::vec3 GetAmbient() { return m_ambientColor; }
		void SetSpecular(glm::vec3 specular) { m_specularColor = specular; }
		glm::vec3 GetSpecular() { return m_specularColor; }

	private:
		glm::vec3 m_color;
		glm::vec3 m_pos;
		
		float m_constant;
		float m_linear;
		float m_quadratic;

		glm::vec3 m_diffuseColor;
		glm::vec3 m_ambientColor;
		glm::vec3 m_specularColor;
	};
}

#endif