#ifndef SG_SHADER
#define SG_SHADER

#include "RenderSetting.h"
#include "SGSingleton.h"

namespace Renderer
{



	class SGShader
	{
	public:
		SGShader(std::string vertexShader, std::string fragShader);
		SGShader(std::string vertexShader, std::string geoShader, std::string fragShader);

		GLuint GetShaderProgramId() { return m_programId; }
		void SetShaderProgramId(GLuint newId);


	protected:
		std::string LoadShaderCode(std::string shaderPath);
		void CheckShaderCompileError(GLuint shaderId, std::string type);


		GLuint m_programId;
		std::string m_vertexShaderCode;
		std::string m_fragShaderCode;
		std::string m_geomShaderCode;
	};
}



#endif