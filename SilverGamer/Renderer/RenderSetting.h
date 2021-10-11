#ifndef SILVER_RENDER_SETTING
#define SILVER_RENDER_SETTING

#include <glad/glad.h>
#include <GLFW/glfw3.h>
//#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <map>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <chrono>

#define RENDER_WARDER(X) assert(X)
#define RENDER_WARDER_NULL_ALERT(X) RENDER_WARDER(X!=nullptr)

#define GEOMETRY_BIND_LOCATION_POSITION 0
#define GEOMETRY_BIND_LOCATION_NORMAL 1
#define GEOMETRY_BIND_LOCATION_TEXCOORD 2

#define MATERIAL_VARIBLE_DIFFUSE_COLOR "material.diffuseColor"
#define MATERIAL_VARIBLE_SPECULAR_COLOR "material.specularColor"
#define MATERIAL_VARIBLE_EMISSIVITY "material.emissivity"
#define MATERIAL_VARIBLE_TRANSPARENCY "material.transparency"
#define MATERIAL_VARIBLE_SPECULAR_REFLECTANCE "material.specularReflectivity"
#define MATERIAL_VARIBLE_DIFFUSE_REFLECTANCE "material.diffuseReflectivity"
#define MATERIAL_VARIBLE_SHINESS "material.shiness"

#define MATH_PI 3.141592f
#define MATH_PI_DIV_4 0.785398
#define MATH_PI_DIV_2 1.570796

#define APPLICATION_WIDTH 1280
#define APLLICATION_HEIGHT 720

#define AXIS_X glm::vec3(0, 1, 0)
#define AXIS_Y glm::vec3(1, 0, 0)
#define AXIS_Z glm::vec3(0, 0, 1)

//引擎灯光相关宏设置
#define POINT_LIGHT_MAX_NUM 4 



//Camera相关宏设置
#define CAMERA_DEFAULT_FOV MATH_PI_DIV_4
#define CAMERA_DEFAULT_ASPECT 1.33F
#define CAMREA_DEFAULT_NEAR_Z 0.1F
#define CAMERA_DEFAULT_FAR_Z 100.0F

#define CAMERA_THIRD_ROLE_LIMIT_PICTH 0.0f
#define CAMERA_THIRD_ROLE_LIMIT_YAW -89.0f
#define CAMERA_THIRD_ROLE_DEFAULT_TARGET glm::vec3(0.0f, 0.0f ,0.0f)
#define CAMERA_THIRD_ROLE_DEFAULT_UPDIR glm::vec3(0.0f, 1.0f, 0.0f)
#define CAMERA_THIRD_ROLE_DEFAULT_SENSITY 0.3f
#define CAMERA_THIRD_ROLE_DEFAULT_DISTANCE 7.0f
#define CAMERA_THIRD_ROLE_DEFAULT_MAX_DISTANCE 20.0f
#define CAMERA_THIRD_ROLE_DEFAULT_MIN_DISTANCE 2.0f

#define CAMERA_FIRST_ROLE_LIMIT_MAX_PITCH 89.0f
#define CAMERA_FIRST_ROLE_LIMIT_MIN_PITCH -89.0f
#define CAMERA_FIRST_ROLE_DEFAULT_POSITION glm::vec3(3.0f, 0.0f, 0.0f)
#define CAMERA_FIRST_ROLE_DEFAULT_UPDIR glm::vec3(0.0f, 1.0f, 0.0f)
#define CAMERA_FIRST_ROLE_DEFAULT_ROTATE_SPEED 0.3f


#define COMMON_SHADER_FILE_DIR "shader/common/"



enum SG_MESH_MODE {
	SIMPLE, COMPLEX
};


class RenderConfig
{
public:
	
	void Initialize();
	std::string GetConfigParam(std::string var);
private:
	RenderConfig();
	std::unordered_map<std::string, std::string> m_config;
};




#endif
