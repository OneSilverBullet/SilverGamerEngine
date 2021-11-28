#include "BaseGeometry.h"
#include "RenderSetting.h"
#include "ResourceLoad.h"

Renderer::SGModelMesh::SGModelMesh()
{
}

Renderer::SGModelMesh::SGModelMesh(std::vector<SGModelVertex> verts, std::vector<unsigned int> indices, SGMaterialPBRWithEmit* mat)
	:m_vertices(verts), m_indices(indices), m_material(mat)
{
	//在赋值当前mesh对应数据后，构建VAO、VBO等数据结构
	ConstructMesh();
}

void Renderer::SGModelMesh::Draw()
{
	// Draw Mesh
	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Renderer::SGModelMesh::Draw(GLuint shaderID)
{
	/*
	GLuint diffuseNr = 1;
	GLuint specularNr = 1;
	for (GLuint i = 0; i < m_textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i); // 在绑定纹理前需要激活适当的纹理单元
		// 检索纹理序列号 (N in diffuse_textureN)
		std::stringstream ss;
		std::string number;
		std::string name = m_textures[i].m_type;
		if (name == "texture_diffuse")
			ss << diffuseNr++; // 将GLuin输入到string stream
		else if (name == "texture_specular")
			ss << specularNr++; // 将GLuin输入到string stream
		number = ss.str();

		glUniform1f(glGetUniformLocation(shaderID, ("mat." + name + number).c_str()), i);
		glBindTexture(GL_TEXTURE_2D, m_textures[i].m_id);
	}
	glActiveTexture(GL_TEXTURE0);
	*/

	// 绘制Mesh
	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Renderer::SGModelMesh::UploadMeshMaterial()
{
	//Active submesh material
	m_material->Load();
}

void Renderer::SGModelMesh::ConstructMesh()
{
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ebo);
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(SGModelVertex), &m_vertices[0], GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint), &m_indices[0], GL_STATIC_DRAW);
	//顶点几何布局
	glEnableVertexAttribArray(GEOMETRY_BIND_LOCATION_POSITION);
	glVertexAttribPointer(GEOMETRY_BIND_LOCATION_POSITION, 3, GL_FLOAT, false, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(GEOMETRY_BIND_LOCATION_NORMAL);
	glVertexAttribPointer(GEOMETRY_BIND_LOCATION_NORMAL, 3, GL_FLOAT, false, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(GEOMETRY_BIND_LOCATION_TEXCOORD);
	glVertexAttribPointer(GEOMETRY_BIND_LOCATION_TEXCOORD, 2, GL_FLOAT, false, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glBindVertexArray(0);
}

void Renderer::SGModelBase::LoadTempRenderData(CommonSceneInfo* sinfo, SGController* ctrl, SGTransform* trans)
{
	sceneInfo = sinfo;
	controller = ctrl;
	transform = trans;
}

Renderer::SGModelBase::SGModelBase(const char * modelPath)
{
	std::string path = modelPath;
	ResourceLoad::Instance()->LoadModel(modelPath, &m_meshes);
}

void Renderer::SGModelBase::Draw()
{
	RENDER_WARDER_NULL_ALERT(sceneInfo);
	RENDER_WARDER_NULL_ALERT(transform);
	RENDER_WARDER_NULL_ALERT(controller);
	for (int i = 0; i < m_meshes.size(); ++i) {
		//upload submesh material info
		m_meshes[i].UploadMeshMaterial();
		//upload scene information
		sceneInfo->Upload(m_meshes[i].m_material->GetShaderInstance());
		transform->Upload(m_meshes[i].m_material->GetShaderInstance());
		controller->Upload(m_meshes[i].m_material->GetShaderInstance());
		//draw meshes
		m_meshes[i].Draw();
	}
}

/*
* SGQuad
*/
Renderer::SGQuad::SGQuad()
{
	GLfloat quadVertices[] = {
		// Positions        // Texture Coords
		-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	};

	glGenVertexArrays(1, &m_quadVAO);
	glGenBuffers(1, &m_quadVBO);
	glBindVertexArray(m_quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glBindVertexArray(0);
}

Renderer::SGQuad::SGQuad(float minX, float minY, float maxX, float maxY)
{
	GLfloat quadVertices[] = {
		// Positions        // Texture Coords
		minX, maxY, 0.0f, 0.0f, 1.0f,
		minX, minY, 0.0f, 0.0f, 0.0f,
		maxX, maxY, 0.0f, 1.0f, 1.0f,
		maxX, minY, 0.0f, 1.0f, 0.0f,
	};

	glGenVertexArrays(1, &m_quadVAO);
	glGenBuffers(1, &m_quadVBO);
	glBindVertexArray(m_quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glBindVertexArray(0);
}

void Renderer::SGQuad::Draw()
{
	glBindVertexArray(m_quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}