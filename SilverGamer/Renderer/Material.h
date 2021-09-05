#ifndef SG_MATERIAL
#define SG_MATERIAL

#include "RenderSetting.h"

namespace Renderer
{
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