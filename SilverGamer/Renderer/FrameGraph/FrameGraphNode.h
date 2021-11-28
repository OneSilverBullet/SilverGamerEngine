#ifndef SG_FRAME_GRAPH_NODE
#define SG_FRAME_GRAPH_NODE

#include <string>

namespace Renderer
{
	namespace FrameGraph
	{
		//Basic Frame Graph Node Class
		class IFrameGraphNode
		{
		public:
			IFrameGraphNode(std::string )


		};


		class ResourceNode {
		public:
			ResourceNode(std::string name)
				: name{ std::move(name) } {}

			std::string Name() const noexcept { return name; }
		private:
			std::string name;
		};
	}
}


#endif