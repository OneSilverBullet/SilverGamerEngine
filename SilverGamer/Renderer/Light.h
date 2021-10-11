#ifndef SG_LIGHT
#define SG_LIGHT

#include "RenderSetting.h"

namespace Renderer
{

	class SGDirLight
	{
	public:
		SGDirLight(glm::vec3 pos = glm::vec3(-1));
		~SGDirLight() {}
		void Upload(GLuint program, GLuint index);

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

		void SetConstant(float);
		float GetConstant();
		void SetLinear(float);
		float GetLinear();
		void SetQuadratic(float);
		float GetQuadratic();


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