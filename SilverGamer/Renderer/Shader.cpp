#include "Shader.h"


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
	//??????ɫ??
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
	//??????ɫ??
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
