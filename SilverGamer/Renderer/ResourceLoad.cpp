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

Renderer::SGTexture2D* Renderer::ResourceLoad::LoadDefaultBlackTexture2DResource(SG_TEXTURE_TYPE textureType, std::string path)
{
	std::string defaultTextureDir = "F:/CGProjection/SilverGameEngine/SilverGamerEngine/SilverGamer/Resource/DefaultTextures/default_emit.png";
	return LoadTexture2DResource(textureType, defaultTextureDir);
}

void Renderer::ResourceLoad::LoadModel(std::string path, std::vector<Renderer::SGModelMesh>* outMeshes)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "Error::Assimp::" << importer.GetErrorString() << std::endl;
		return;
	}
	ProcessNode(scene->mRootNode, scene, outMeshes, path);
}

void Renderer::ResourceLoad::ProcessNode(aiNode* node, const aiScene* scene, std::vector<Renderer::SGModelMesh>* outMeshes, std::string modelPath)
{
	//��ӵ�ǰ�ڵ������mesh
	for (GLuint i = 0; i < node->mNumMeshes; ++i) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		outMeshes->push_back(ProcessMesh(mesh, scene, modelPath));
	}
	//�ݹ����ӽڵ�
	for (GLuint i = 0; i < node->mNumChildren; ++i) {
		ProcessNode(node->mChildren[i], scene, outMeshes, modelPath);
	}
}

Renderer::SGModelMesh Renderer::ResourceLoad::ProcessMesh(aiMesh* mesh, const aiScene* scene, std::string modelPath)
{
	std::vector<SGModelVertex> vertices;
	std::vector<unsigned int> indicies;

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

	SGMaterialPBRWithEmit* subMeshMaterial = nullptr;
	// material related 
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		std::cout << material->GetName().C_Str() << std::endl;
		aiString materialName;
		GLint TextureCount = material->GetTextureCount(aiTextureType_DIFFUSE);
		RENDER_WARDER(TextureCount == 1);
		material->GetTexture(aiTextureType_DIFFUSE, 0, &materialName);
		TextureCount = material->GetTextureCount(aiTextureType_AMBIENT);
		std::cout << "		material Name:" << materialName.C_Str() << std::endl;
		aiString materialCoef;
		if (TextureCount == 1) {
			material->GetTexture(aiTextureType_AMBIENT, 0, &materialCoef);
			std::cout << "		material Coef:" << materialCoef.C_Str() << std::endl;

		}
		//Process the material coefs
		subMeshMaterial = ProcessSubMeshPBRMaterial(modelPath, materialName.C_Str(), TextureCount > 0 ? materialCoef.C_Str() : "");

	}
	return SGModelMesh(vertices, indicies, subMeshMaterial);
}

Renderer::SGMaterialPBRWithEmit* Renderer::ResourceLoad::ProcessSubMeshPBRMaterial(std::string modelDir, std::string materialName, std::string materialCoef /*=""*/)
{
	SGMaterialPBRWithEmit* newMaterial = new SGMaterialPBRWithEmit();
	int lastDirFlag = (int)modelDir.find_last_of('/');
	std::string modelFileDir = modelDir.substr(0, lastDirFlag);
	std::string texturesDir = modelFileDir + "/textures_pbr/" + materialName;
	std::string diffuseTextureDir = texturesDir + materialCoef + "_diff.png";
	std::string normalTextureDir = texturesDir + "_norm.png";
	std::string metalTextureDir = texturesDir + "_roughness.png";
	std::string roughnessTextureDir = texturesDir + "_metal.png";
	std::string aoTextureDir = texturesDir + "_ao.png";
	std::string emitTextureDir = texturesDir + "_emit.png";

	assert(SGSystem::IsFileExist(diffuseTextureDir)); //diffuseTexture must exist
	SGTexture2D* diffuseTexture = LoadTexture2DResource(SG_TEXTURE_TYPE::TEXTURE_DIFFUSE, diffuseTextureDir);
	newMaterial->SetDiffuseTexture2DPtr(diffuseTexture);
	assert(SGSystem::IsFileExist(normalTextureDir));//normalTexture must exist
	SGTexture2D* normalTexture = LoadTexture2DResource(SG_TEXTURE_TYPE::TEXTURE_NORMAL, normalTextureDir);
	newMaterial->SetNormalTexture2DPtr(normalTexture);
	assert(SGSystem::IsFileExist(metalTextureDir));//metal must exist
	SGTexture2D* metalTexture = LoadTexture2DResource(SG_TEXTURE_TYPE::TEXTURE_METALLIC, metalTextureDir);
	newMaterial->SetMetalnessTexture2DPtr(metalTexture);
	assert(SGSystem::IsFileExist(roughnessTextureDir));//normalTexture must exist
	SGTexture2D* roughnessTexture = LoadTexture2DResource(SG_TEXTURE_TYPE::TEXTURE_ROUGHNESS, roughnessTextureDir);
	newMaterial->SetRoughnessTexture2DPtr(roughnessTexture);
	assert(SGSystem::IsFileExist(aoTextureDir));//normalTexture must exist
	SGTexture2D* aoTexture = LoadTexture2DResource(SG_TEXTURE_TYPE::TEXTURE_AO, aoTextureDir);
	newMaterial->SetAOTexture2DPtr(aoTexture);

	//check ao texture is exist
	if (SGSystem::IsFileExist(emitTextureDir))
	{
		SGTexture2D* emitTexture = LoadTexture2DResource(SG_TEXTURE_TYPE::TEXTURE_EMIT, emitTextureDir);
		newMaterial->SetEmitTexture2DPtr(emitTexture);
	}
	else
	{
		SGTexture2D* emitTexture = LoadDefaultBlackTexture2DResource(SG_TEXTURE_TYPE::TEXTURE_EMIT, emitTextureDir);
		newMaterial->SetEmitTexture2DPtr(emitTexture);
	}

	return newMaterial;
}
