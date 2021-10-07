#include "Shader.h"

Renderer::SGShaderFactory* Renderer::SGShaderFactory::m_instance = nullptr;

static const char* const searchPath = "/";

Renderer::SGShader::SGShader(std::string vertexShaderPath, std::string fragShaderPath)
{
	m_vertexShaderCode = LoadShaderCode(vertexShaderPath);
	m_fragShaderCode = LoadShaderCode(fragShaderPath);
	const char* vertexShaderCode = m_vertexShaderCode.c_str();
	const char* fragShaderCode = m_fragShaderCode.c_str();
	unsigned int vertexShaderId, fragShaderId;
	vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderId, 1, &vertexShaderCode, NULL);
	glCompileShaderIncludeARB(vertexShaderId, 1, &searchPath, nullptr);
	glCompileShader(vertexShaderId);
	CheckShaderCompileError(vertexShaderId, "VERTEX");
	fragShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShaderId, 1, &fragShaderCode, NULL);
	glCompileShaderIncludeARB(fragShaderId, 1, &searchPath, nullptr);
	glCompileShader(fragShaderId);
	CheckShaderCompileError(fragShaderId, "FRAGMENT");
	//链接着色器
	m_programId = glCreateProgram();
	glAttachShader(m_programId, vertexShaderId);
	glAttachShader(m_programId, fragShaderId);
	glLinkProgram(m_programId);
	CheckShaderCompileError(m_programId, "PROGRAM");
	glDeleteShader(vertexShaderId);
	glDeleteShader(fragShaderId);
}

Renderer::SGShader::SGShader(std::string vertexShaderPath, std::string geomPath, std::string fragShaderPath)
{
	m_vertexShaderCode = LoadShaderCode(vertexShaderPath);
	m_geomShaderCode = LoadShaderCode(geomPath);
	m_fragShaderCode = LoadShaderCode(fragShaderPath);
	const char* vertexShaderCode = m_vertexShaderCode.c_str();
	const char* geomShaderCode = m_geomShaderCode.c_str();
	const char* fragShaderCode = m_fragShaderCode.c_str();
	unsigned int vertexShaderId, fragShaderId, geomShaderId;
	vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderId, 1, &vertexShaderCode, NULL);
	glCompileShaderIncludeARB(vertexShaderId, 1, &searchPath, nullptr);
	glCompileShader(vertexShaderId);
	CheckShaderCompileError(vertexShaderId, "VERTEX");
	geomShaderId = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(geomShaderId, 1, &geomShaderCode, NULL);
	glCompileShaderIncludeARB(geomShaderId, 1, &searchPath, nullptr);
	glCompileShader(geomShaderId);
	CheckShaderCompileError(geomShaderId, "GEOMETRY");
	fragShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShaderId, 1, &fragShaderCode, NULL);
	glCompileShaderIncludeARB(fragShaderId, 1, &searchPath, nullptr);
	glCompileShader(fragShaderId);
	CheckShaderCompileError(fragShaderId, "FRAGMENT");
	//链接着色器
	m_programId = glCreateProgram();
	glAttachShader(m_programId, vertexShaderId);
	glAttachShader(m_programId, fragShaderId);
	glAttachShader(m_programId, geomShaderId);
	glLinkProgram(m_programId);
	CheckShaderCompileError(m_programId, "PROGRAM");
	glDeleteShader(vertexShaderId);
	glDeleteShader(fragShaderId);
	glDeleteShader(geomShaderId);
}

void Renderer::SGShader::SetShaderProgramId(GLuint newId)
{
	if (m_programId != -1)
	{
		glDeleteProgram(m_programId);
	}
	m_programId = newId;
}

std::string Renderer::SGShader::LoadShaderCode(std::string shaderPath)
{
	std::string shaderCode;
	std::ifstream shaderFile;
	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		shaderFile.open(shaderPath);
		std::stringstream shaderStream;
		shaderStream << shaderFile.rdbuf();
		shaderFile.close();
		shaderCode = shaderStream.str();
		return shaderCode;
	}
	catch (std::ifstream::failure& e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		return "";
	}
	return "";
}

void Renderer::SGShader::CheckShaderCompileError(GLuint shaderId, std::string type)
{
	GLint success;
	GLchar infoLog[1024];
	if (type != "PROGRAM") {
		glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(shaderId, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;			return;
		}
	}
	else
	{
		glGetShaderiv(shaderId, GL_LINK_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(shaderId, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_LINK_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;			return;
		}
	}
}

Renderer::SGShader * Renderer::SGShaderFactory::loadNormalShader()
{
	return new Renderer::SGShader("shader/shader_normal.vert", "shader/shader_normal.frag");
}

//参数fileName仅仅负责当前shader file的名字
void Renderer::SGShaderFactory::AddCommonShaderFile(const std::string& fileName)
{
	if (fileName.empty())
		return;

	std::string fileDir = COMMON_SHADER_FILE_DIR + fileName + ".glsl";

	/// 读common shader文件
	std::string code;
	std::ifstream shaderFile;
	shaderFile.exceptions(std::ifstream::badbit);
	try
	{
		/// 打开文件
		shaderFile.open(fileDir);
		std::stringstream shaderStream;
		/// 读取文件的缓冲内容到流中；
		shaderStream << shaderFile.rdbuf();
		/// 关闭文件
		shaderFile.close();
		/// 转换流至GLchar数组
		code = shaderStream.str();
	}
	catch (std::ifstream::failure e) {
		std::cout << "Error : init shader error: failed to read shader file" << std::endl;
	}

	/// 将common shader文件通过glNamedStringARB函数传入GL的虚拟文件系统
	/// 例如传入的shader文件名为 commonFunction.glsl
	/// 那么glNamedStringARB的第二个参数为："/commonFunction.glsl"， "/"是一定需要的
	/// 第四个参数为 代码的字符串
	std::string fullFileName = "/" + fileName + ".glsl";
	glNamedStringARB(GL_SHADER_INCLUDE_ARB, fullFileName.size(), fullFileName.c_str(), code.size(), code.c_str());
}

void Renderer::SGShaderFactory::LoadCommonShader()
{
	AddCommonShaderFile("common"); //增加common公共shader
}
