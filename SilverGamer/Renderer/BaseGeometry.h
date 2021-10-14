#ifndef SG_BASE_GEOMETRY
#define SG_BASE_GEOMETRY

#include "RenderSetting.h"
#include "Shader.h"
#include "ResourceLoad.h"

namespace Renderer
{
	//当前模型顶点
	struct SGModelVertex
	{
		glm::vec3 m_pos;
		glm::vec3 m_normal;
		glm::vec2 m_uv;
		SGModelVertex(){}
		SGModelVertex(glm::vec3 _pos, glm::vec3 _normal, glm::vec2 _uv) :m_pos(_pos), m_normal(_normal), m_uv(_uv) {}
	};

	//大部分模型所需要的模型贴图
	struct SGModelTexture{
		unsigned int m_id;
		std::string m_type;
		std::string m_path; //当前贴图的路径
	};

	class SGModelMesh
	{
	public:
		std::vector<SGModelVertex> m_vertices;
		std::vector<unsigned int> m_indices;
		std::vector<SGModelTexture> m_textures;

		SGModelMesh(std::vector<SGModelVertex>, std::vector<unsigned int>, std::vector<SGModelTexture>);
		void Draw(Renderer::SGShader shader); //直接传入shader进行绘制
		void Draw(GLuint shaderID); //直接传入shader id进行绘制    

	private:
		unsigned int m_vao, m_vbo, m_ebo;
		void ConstructMesh();
	};
	
	class SGModelBase
	{
	public:
		SGModelBase(const char* modelPath);
		void Draw(Renderer::SGShader shader);
		void Draw(GLuint shaderId);

	private:
		std::vector<SGModelTexture> m_loadedTextures; //已加载贴图
		std::vector<SGModelMesh> m_meshes;
		std::string m_modelDir;
		std::string m_modelName;

		void LoadModel(std::string path);
		void ProcessNode(aiNode* node, const aiScene* scene);
		SGModelMesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<SGModelTexture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	};


}
#endif