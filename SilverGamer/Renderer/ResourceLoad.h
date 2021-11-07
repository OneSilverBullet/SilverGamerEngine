#ifndef SG_RESOURCE_LOAD
#define SG_RESOURCE_LOAD

#include "stb_image.h"
#include "RenderSetting.h"
#include "SGSingleton.h"
#include "Shader.h"
#include "BaseGeometry.h"
#include "Texture2D.h"

namespace Renderer
{
	class SGShaderFactory : public SGSingleton<SGShaderFactory>
	{
		friend class SGSingleton<SGShaderFactory>;
	public:
		~SGShaderFactory() {}

		//加载正常Shader
		GLint LoadShader(const std::string vertName, const std::string fragName);
		SGShader* LoadShaderPtr(const std::string vertName, const std::string fragName);
		//增加可以include的shader编译
		void AddCommonShaderFile(const std::string& fileName);
		void LoadCommonShader();

	private:
		//（vert, frag）-> shader
		std::map<std::pair<std::string, std::string>, SGShader*> m_shaders; //当前所有shader库

		SGShaderFactory() { LoadCommonShader(); }
	};



	class ResourceLoad : public SGSingleton<ResourceLoad>
	{
		friend class  SGSingleton<ResourceLoad>;
	public:
		//Load Texture return textureId
		SGTexture2D* LoadTexture2DResource(SG_TEXTURE_TYPE textureType, std::string path);

		//Load Model: Only Process verts
		void LoadModel(std::string path,  std::vector<Renderer::SGModelMesh>* outMeshes);
	

	private:
		ResourceLoad() {}
		ResourceLoad(ResourceLoad&) = delete;
		ResourceLoad& operator=(const ResourceLoad&) = delete;
		void ProcessNode(aiNode* node, const aiScene* scene, std::vector<Renderer::SGModelMesh>* outMeshes);
		Renderer::SGModelMesh ProcessMesh(aiMesh* mesh, const aiScene* scene);

		static ResourceLoad* m_instance;

		std::map<std::string, SGTexture2D*> m_textures; //key:textureId
	};
	

}


#endif