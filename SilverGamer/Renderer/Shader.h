#ifndef SG_SHADER
#define SG_SHADER

#include "RenderSetting.h"

namespace Renderer
{
	class SGShader;

	class SGShaderFactory
	{
	public:
		~SGShaderFactory() {}
		static SGShaderFactory* GetInstance() {
			if (m_instance == nullptr) {
				m_instance = new SGShaderFactory();
			}
			return m_instance;
		}
		//加载正常Shader
		SGShader* loadNormalShader();
		//增加可以include的shader编译
		void AddCommonShaderFile(const std::string& fileName);
		void LoadCommonShader();



	private:
		SGShaderFactory() { LoadCommonShader(); }
		static SGShaderFactory* m_instance;
	};
	


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