#include "BaseGeometry.h"
#include "RenderSetting.h"

Renderer::SGModelMesh::SGModelMesh(std::vector<SGModelVertex> verts, std::vector<unsigned int> indices, std::vector<SGModelTexture> texes)
	:m_vertices(verts), m_indices(indices), m_textures(texes)
{
}

void Renderer::SGModelMesh::Draw(Renderer::SGShader shader)
{
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

		glUniform1f(glGetUniformLocation(shader.GetShaderProgramId(), ("material." + name + number).c_str()), i);
		glBindTexture(GL_TEXTURE_2D, m_textures[i].m_id);
	}
	glActiveTexture(GL_TEXTURE0);

	// 绘制Mesh
	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
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

Renderer::SGModelBase::SGModelBase(const char * modelPath)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(modelPath, aiProcess_Triangulate | aiProcess_FlipUVs);
}

void Renderer::SGModelBase::Draw(Renderer::SGShader shader)
{
	for (int i = 0; i < m_meshes.size(); ++i) {
		m_meshes[i].Draw(shader);
	}
}

void Renderer::SGModelBase::LoadModel(std::string path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "Error::Assimp::" << importer.GetErrorString() << std::endl;
		return;
	}
	m_modelDir = path.substr(0, path.find_last_of('/'));
	ProcessNode(scene->mRootNode, scene);
}

void Renderer::SGModelBase::ProcessNode(aiNode * node, const aiScene * scene)
{
	//添加当前节点的所有mesh
	for (GLuint i = 0; i < node->mNumMeshes; ++i) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		m_meshes.push_back(ProcessMesh(mesh, scene));
	}
	//递归孙子节点
	for (GLuint i = 0; i < node->mNumChildren; ++i) {
		ProcessNode(node->mChildren[i], scene);
	}
}


Renderer::SGModelMesh Renderer::SGModelBase::ProcessMesh(aiMesh * mesh, const aiScene * scene)
{
	std::vector<SGModelVertex> vertices;
	std::vector<unsigned int> indicies;
	std::vector<SGModelTexture> textures;

	//处理顶点
	for(unsigned int i=0; i<mesh->mNumVertices; ++i){
		SGModelVertex vertex;
		glm::vec3 vertexData;
		glm::vec2 vertexUV;
		//处理位置
		vertexData.x = mesh->mVertices[i].x;
		vertexData.y = mesh->mVertices[i].y;
		vertexData.z = mesh->mVertices[i].z;
		vertex.m_pos = vertexData;
		//处理法线
		vertexData.x = mesh->mNormals[i].x;
		vertexData.y = mesh->mNormals[i].y;
		vertexData.z = mesh->mNormals[i].z;
		vertex.m_normal = vertexData;
		//处理UV
		if (mesh->mTextureCoords[0]) {
			vertexUV.x = mesh->mTextureCoords[0][i].x;
			vertexUV.y = mesh->mTextureCoords[0][i].y;
			vertex.m_uv = vertexUV;
		}
		else
			vertex.m_uv = glm::vec2(0);
		vertices.push_back(vertex);
	}
	//处理索引
	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++) {
			indicies.push_back(face.mIndices[j]);
		}
	}
	//处理材质
	if(mesh->mMaterialIndex > 0){
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		//diffuse Map
		std::vector<SGModelTexture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		//specular
		std::vector<SGModelTexture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		//normal
		std::vector<SGModelTexture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
		//高度贴图
		std::vector<SGModelTexture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
		textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
	}
	return SGModelMesh(vertices, indicies, textures);
}

std::vector<Renderer::SGModelTexture> Renderer::SGModelBase::loadMaterialTextures(aiMaterial * mat, aiTextureType type, std::string typeName)
{
	std::vector<Renderer::SGModelTexture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); ++i) {
		aiString str;
		mat->GetTexture(type, i, &str); //得到当前贴图的路径
		bool skip = false;
		//进行优化检查
		for (unsigned int j = 0; j < m_loadedTextures.size(); ++i) {
			//如果当前加载贴图是已有贴图
			if (std::strcmp(m_loadedTextures[j].m_path.data(), str.C_Str()) == 0) {
				textures.push_back(m_loadedTextures[j]);
				skip = true;
				break;
			}
		}
		if (!skip) { //创建新的路径
			SGModelTexture tmpTexture;
			std::string modelName = str.C_Str();
			std::string modelDir = m_modelDir;
			tmpTexture.m_id = Renderer::ResourceLoad::GetInstance()->LoadTexture2DResource(modelName, modelDir + modelName);
			tmpTexture.m_type = typeName;
			tmpTexture.m_path = str.C_Str();
			textures.push_back(tmpTexture);
			m_loadedTextures.push_back(tmpTexture);
		}
	}
	return std::vector<Renderer::SGModelTexture>();
}
