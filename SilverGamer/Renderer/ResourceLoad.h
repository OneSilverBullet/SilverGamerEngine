#ifndef SG_RESOURCE_LOAD
#define SG_RESOURCE_LOAD

#include "stb_image.h"
#include "RenderSetting.h"

namespace Renderer
{
	class ResourceLoad
	{
	public:
		static ResourceLoad* GetInstance() {
			if (instance == nullptr)
				instance = new ResourceLoad();
			return instance;
		}

		int LoadTexture2DResource(std::string name, std::string path);

	private:
		ResourceLoad() {}
		ResourceLoad(ResourceLoad&) = delete;
		ResourceLoad& operator=(const ResourceLoad&) = delete;
		static ResourceLoad* instance;
		std::map<std::string, int> m_textures; //key:textureId


	};


}


#endif