#ifndef SG_BASE_GEOMETRY
#define SG_BASE_GEOMETRY

#include "RenderSetting.h"
#include "Shader.h"


namespace Renderer
{
	//��ǰģ�Ͷ���
	struct SGModelVertex
	{
		glm::vec3 m_pos;
		glm::vec3 m_normal;
		glm::vec2 m_uv;
		SGModelVertex(){}
		SGModelVertex(glm::vec3 _pos, glm::vec3 _normal, glm::vec2 _uv) :m_pos(_pos), m_normal(_normal), m_uv(_uv) {}
	};

	//�󲿷�ģ������Ҫ��ģ����ͼ
	struct SGModelTexture{
		unsigned int m_id;
		std::string m_type;
		std::string m_path; //��ǰ��ͼ��·��
	};

	class SGModelMesh
	{
	public:
		std::vector<SGModelVertex> m_vertices;
		std::vector<unsigned int> m_indices;
		std::vector<SGModelTexture> m_textures;
		SGModelMesh();
		SGModelMesh(std::vector<SGModelVertex>, std::vector<unsigned int>, std::vector<SGModelTexture>);
		void Draw(Renderer::SGShader shader); //ֱ�Ӵ���shader���л���
		void Draw(GLuint shaderID); //ֱ�Ӵ���shader id���л���    

	private:
		unsigned int m_vao, m_vbo, m_ebo;
		void ConstructMesh();
	};
	
	class IModel
	{
	public:
		virtual void Draw(Renderer::SGShader shader) = 0;
		virtual void Draw(GLuint shaderId) = 0;
	};

	class SGModelBase : public IModel
	{
	public:
		SGModelBase(const char* modelPath);
		void Draw(Renderer::SGShader shader) override;
		void Draw(GLuint shaderId) override;

	private:
		std::vector<SGModelMesh> m_meshes;
		std::string m_modelDir;
	};

	//Quad Structure: There is no need for user to call this structure
	class SGQuad : public IModel
	{
	public:
		SGQuad(); //Default All Screen Quad
		SGQuad(float minX, float minY, float maxX, float maxY);
		void Draw(Renderer::SGShader shader) override;
		void Draw(GLuint shaderId) override;

	public:
		unsigned int m_quadVAO, m_quadVBO;
	};


}
#endif