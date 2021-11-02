#ifndef SG_MATERIAL
#define SG_MATERIAL

#include "RenderSetting.h"
#include "Texture2D.h"

namespace Renderer
{

	class SGMaterialBase
	{
	public:
		virtual void Load() = 0;
		virtual void Unload() = 0;
		virtual GLint GetShaderInstance() = 0;
	};

	/*
	* SGMaterialPhongFlat��ʹ��BlinnPhong��Flat����ģ��
	*/
	class SGMaterialPhongFlat : public SGMaterialBase
	{
	public:
		SGMaterialPhongFlat();
		SGMaterialPhongFlat(glm::vec3 diffuse, glm::vec3 specular, float shininess);
		
		void Load() override; //װ�����ڵĲ���
		void Unload() override; //ж�����ڵĲ���

		glm::vec3 GetDiffuse() { return m_diffuse; }
		void SetDiffuse(glm::vec3 v) { m_diffuse = v; }
		glm::vec3 GetSpecular() { return m_specular; }
		void SetSpecular(glm::vec3 v) { m_specular = v; }
		float GetShiniess() { return m_shininess; }
		void SetShininess(float v) { m_shininess = v; }

		GLint GetShaderInstance() override { return m_shader; }

	private:
		glm::vec3 m_diffuse;
		glm::vec3 m_specular;
		float m_shininess;

		GLint m_shader; //��ǰ��������shader
	};


	/*
	* SGMaterialPBR: ʹ��PBR�Ĳ�����ϵ
	*/
	class SGMaterialPBRWithEmit : public SGMaterialBase
	{
	public:
		SGMaterialPBRWithEmit(std::string matDir);
		
		void Load() override; //װ�����ڵĲ���
		void Unload() override; //ж�����ڵĲ���
		GLint GetShaderInstance() override { return m_shader; }

	private:
		SGTexture2D* m_diffuse; //diffuse color��ͼ
		SGTexture2D* m_normal; //normal ��ͼ
		SGTexture2D* m_roughness; //�ֲڶ�
		SGTexture2D* m_emit; //�Է���
		SGTexture2D* m_metalness; //������
		SGTexture2D* m_ao; //ao

		GLint m_shader; //��ǰ��������shader
	};

}


#endif