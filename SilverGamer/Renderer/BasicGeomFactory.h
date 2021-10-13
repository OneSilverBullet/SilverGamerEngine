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
		//����Box������
		void BasicBoxGeom(float length, std::vector<SGModelVertex>& outVertexVec, std::vector<unsigned int>& outIndexVec);
		//����Plane������
		void BasicPlaneGeom(float width, float height, std::vector<SGModelVertex>& outVertexVec, std::vector<unsigned int>& outIndexVec, int textureLoopNum = 1);
		//��������
		void BasicSphereGeom(float radius, std::vector<SGModelVertex>& outVertexVec, std::vector<unsigned int>& outIndexVec);
		//����Բ����
		void BasicCylinderGeom(float radius, float thickness, std::vector<SGModelVertex>& outVertexVec, std::vector<unsigned int>& outIndexVec);

	};


}


#endif