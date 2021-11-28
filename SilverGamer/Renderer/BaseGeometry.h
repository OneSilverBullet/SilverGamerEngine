#ifndef SG_BASE_GEOMETRY
#define SG_BASE_GEOMETRY

#include "RenderSetting.h"
#include "Shader.h"
#include "Material.h"
#include "Controller.h"
#include "CommonSceneInfo.h"
#include "Transform.h"


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
		SGMaterialPBRWithEmit* m_material; //sub mesh material

		SGModelMesh();
		SGModelMesh(std::vector<SGModelVertex>, std::vector<unsigned int>, SGMaterialPBRWithEmit* mat);
		void UploadMeshMaterial(); //Upload Submesh Material
		void Draw(); //直接传入shader进行绘制
		void Draw(GLuint shaderID); //直接传入shader id进行绘制    

	private:
		unsigned int m_vao, m_vbo, m_ebo;
		void ConstructMesh();
	};
	
	class IModel
	{
	public:
		virtual void Draw() = 0;
	};

	class SGModelBase : public IModel
	{
	public:
		SGModelBase(const char* modelPath);
		void LoadTempRenderData(CommonSceneInfo* sinfo, SGController* ctrl, SGTransform* trans);
		void Draw() override;

	private:
		//Temp Data
		CommonSceneInfo* sceneInfo = nullptr;
		SGController* controller = nullptr;
		SGTransform* transform = nullptr;

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
		void Draw() override;

	public:
		unsigned int m_quadVAO, m_quadVBO;
	};


}
#endif