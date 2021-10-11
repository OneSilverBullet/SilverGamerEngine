#ifndef SG_MATERIAL
#define SG_MATERIAL

#include "RenderSetting.h"

namespace Renderer
{

	class SGMaterialBase
	{
	public:
		virtual void Upload(GLuint programId) = 0;
	};

	/*
	* SGMaterialPhongFlat：使用BlinnPhong的Flat基础模型
	*/
	class SGMaterialPhongFlat : public SGMaterialBase
	{
	public:
		SGMaterialPhongFlat(glm::vec3 diffuse, glm::vec3 specular, float shininess);
		SGMaterialPhongFlat();
		void Upload(GLuint programId); //上传对应属性

		glm::vec3 GetDiffuse() { return m_diffuse; }
		void SetDiffuse(glm::vec3 v) { m_diffuse = v; }
		glm::vec3 GetSpecular() { return m_specular; }
		void SetSpecular(glm::vec3 v) { m_specular = v; }
		float GetShiniess() { return m_shininess; }
		void SetShininess(float v) { m_shininess = v; }

	private:
		glm::vec3 m_diffuse;
		glm::vec3 m_specular;
		float m_shininess;
	};



	class SGMaterial
	{
	public:
		SGMaterial(glm::vec3 _diffuseColor = glm::vec3(1.0f), 
			glm::vec3 _specularColor = glm::vec3(1.0f),
			float _diffuseReflectivity = 1,
			float _specularReflectivity = 0.1,
			float _transparency = 0,
			float _emissivity = 0, 
			float _shiness = 3);
		void Upload(GLuint program);
		void SetDiffuseColor(glm::vec3 diffuseColorVal);
		glm::vec3 GetDiffuseColor();

		static SGMaterial* WhiteMaterial() {
			return new SGMaterial(glm::vec3(1, 1, 1));
		}
	protected:
		glm::vec3 m_diffuseColor;
		glm::vec3 m_specularColor;
		float m_diffuseReflectivity;
		float m_specularReflectivity;
		float m_transparency;
		float m_emissivity;
		float m_shiness;


	};



}


#endif