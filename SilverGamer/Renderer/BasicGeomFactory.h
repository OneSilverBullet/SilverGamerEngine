#ifndef SG_BASIC_GEOM_FACTORY
#define SG_BASIC_GEOM_FACTORY

#include "RenderSetting.h"
#include "Singleton.h"
#include "BaseGeometry.h"

namespace Renderer
{
	class BasicGeomFactory : public SGSingleton<BasicGeomFactory>
	{
	public:
		//基础Box几何体
		void BasicBoxGeom(float length, std::vector<SGModelVertex>& outVertexVec, std::vector<unsigned int>& outIndexVec);
		//基础Plane几何体
		void BasicPlaneGeom(float width, float height, std::vector<SGModelVertex>& outVertexVec, std::vector<unsigned int>& outIndexVec, int textureLoopNum = 1);
		//基础球体
		void BasicSphereGeom(float radius, std::vector<SGModelVertex>& outVertexVec, std::vector<unsigned int>& outIndexVec);
		//基础圆柱体
		void BasicCylinderGeom(float radius, float thickness, std::vector<SGModelVertex>& outVertexVec, std::vector<unsigned int>& outIndexVec);

	};


}


#endif