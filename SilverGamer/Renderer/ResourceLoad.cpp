#include "ResourceLoad.h"


/*
* SGShaderFactory
*/

GLint Renderer::SGShaderFactory::LoadShader(const std::string vertName, const std::string fragName)
{
	std::string vertFile = SHADER_FILE_DIR + vertName + ".vert";
	std::string fragFile = SHADER_FILE_DIR + fragName + ".frag";
	std::pair<std::string, std::string> shaderFile = std::make_pair(vertFile, fragFile);
	if (m_shaders.find(shaderFile) != m_shaders.end())
		return m_shaders[shaderFile]->GetShaderProgramId();

	SGShader* tempShader = new SGShader(vertFile, fragFile);
	m_shaders[shaderFile] = tempShader;
	return tempShader->GetShaderProgramId();
}

Renderer::SGShader* Renderer::SGShaderFactory::LoadShaderPtr(const std::string vertName, const std::string fragName)
{
	std::string vertFile = SHADER_FILE_DIR + vertName + ".vert";
	std::string fragFile = SHADER_FILE_DIR + fragName + ".frag";
	std::pair<std::string, std::string> shaderFile = std::make_pair(vertFile, fragFile);
	if (m_shaders.find(shaderFile) != m_shaders.end())
		return m_shaders[shaderFile];

	SGShader* tempShader = new SGShader(vertFile, fragFile);
	m_shaders[shaderFile] = tempShader;
	return tempShader;
}

//����fileName��������ǰshader file������
void Renderer::SGShaderFactory::AddCommonShaderFile(const std::string& fileName)
{
	if (fileName.empty())
		return;

	std::string fileDir = COMMON_SHADER_FILE_DIR + fileName + ".glsl";

	/// ��common shader�ļ�
	std::string code;
	std::ifstream shaderFile;
	shaderFile.exceptions(std::ifstream::badbit);
	try
	{
		/// ���ļ�
		shaderFile.open(fileDir);
		std::stringstream shaderStream;
		/// ��ȡ�ļ��Ļ������ݵ����У�
		shaderStream << shaderFile.rdbuf();
		/// �ر��ļ�
		shaderFile.close();
		/// ת������GLchar����
		code = shaderStream.str();
	}
	catch (std::ifstream::failure e) {
		std::cout << "Error : init shader error: failed to read shader file" << std::endl;
	}

	/// ��common shader�ļ�ͨ��glNamedStringARB��������GL�������ļ�ϵͳ
	/// ���紫���shader�ļ���Ϊ commonFunction.glsl
	/// ��ôglNamedStringARB�ĵڶ�������Ϊ��"/commonFunction.glsl"�� "/"��һ����Ҫ��
	/// ���ĸ�����Ϊ ������ַ���
	std::string fullFileName = "/" + fileName + ".glsl";
	glNamedStringARB(GL_SHADER_INCLUDE_ARB, fullFileName.size(), fullFileName.c_str(), code.size(), code.c_str());
}

void Renderer::SGShaderFactory::LoadCommonShader()
{
	AddCommonShaderFile("common"); //����common����shader
	AddCommonShaderFile("math");
}

/*
* ResourceLoad
*/

Renderer::SGTexture2D* Renderer::ResourceLoad::LoadTexture2DResource(SG_TEXTURE_TYPE textureType, std::string path)
{
	//������Լ��ع�������ôֱ�Ӿͷ��ض�Ӧ��texture
	if (m_textures.find(path) != m_textures.end())
		return m_textures[path];

	unsigned int texture;
	glGenTextures(1, &texture);

	// ���ز���������
	int width, height, nrChannels;
	unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		GLenum format;
		if (nrChannels == 1)
			format = GL_RED;
		else if (nrChannels == 3)
			format = GL_RGB;
		else if (nrChannels == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, texture);


		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ
		stbi_image_free(data);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
		return nullptr;
	}
	SGTexture2D* newTexture = new SGTexture2D(textureType, width, height, texture);
	m_textures[path] = newTexture;
	glBindTexture(GL_TEXTURE_2D, 0);
	return newTexture;
}

void Renderer::ResourceLoad::LoadModel(std::string path, std::vector<Renderer::SGModelMesh>* outMeshes)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "Error::Assimp::" << importer.GetErrorString() << std::endl;
		return;
	}
	ProcessNode(scene->mRootNode, scene, outMeshes);
}

void Renderer::ResourceLoad::ProcessNode(aiNode* node, const aiScene* scene, std::vector<Renderer::SGModelMesh>* outMeshes)
{
	//��ӵ�ǰ�ڵ������mesh
	for (GLuint i = 0; i < node->mNumMeshes; ++i) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		outMeshes->push_back(ProcessMesh(mesh, scene));
	}
	//�ݹ����ӽڵ�
	for (GLuint i = 0; i < node->mNumChildren; ++i) {
		ProcessNode(node->mChildren[i], scene, outMeshes);
	}
}

Renderer::SGModelMesh Renderer::ResourceLoad::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<SGModelVertex> vertices;
	std::vector<unsigned int> indicies;
	std::vector<SGModelTexture> textures;

	//������
	for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
		SGModelVertex vertex;
		glm::vec3 vertexData;
		glm::vec2 vertexUV;
		//����λ��
		vertexData.x = mesh->mVertices[i].x;
		vertexData.y = mesh->mVertices[i].y;
		vertexData.z = mesh->mVertices[i].z;
		vertex.m_pos = vertexData;
		//������
		vertexData.x = mesh->mNormals[i].x;
		vertexData.y = mesh->mNormals[i].y;
		vertexData.z = mesh->mNormals[i].z;
		vertex.m_normal = vertexData;
		//����UV
		if (mesh->mTextureCoords[0]) {
			vertexUV.x = mesh->mTextureCoords[0][i].x;
			vertexUV.y = mesh->mTextureCoords[0][i].y;
			vertex.m_uv = vertexUV;
		}
		else
			vertex.m_uv = glm::vec2(0);
		vertices.push_back(vertex);
	}
	//��������
	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++) {
			indicies.push_back(face.mIndices[j]);
		}
	}
	return SGModelMesh(vertices, indicies, textures);
}