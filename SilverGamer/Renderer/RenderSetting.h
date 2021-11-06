#pragma once
#ifndef SILVER_RENDER_SETTING
#define SILVER_RENDER_SETTING

//外部库链接:SGSingleton
#include "SGSingleton.h"
#pragma comment(lib,"SGSystemLib.lib") 
using namespace SilverEngineLib;


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
#include <functional>
#include <array>

#define RENDER_WARDER(X) assert(X)
#define RENDER_WARDER_NULL_ALERT(X) RENDER_WARDER(X!=nullptr)

#define GEOMETRY_BIND_LOCATION_POSITION 0
#define GEOMETRY_BIND_LOCATION_NORMAL 1
#define GEOMETRY_BIND_LOCATION_TEXCOORD 2

//Phong模型宏
#define MATERIAL_VARIBLE_DIFFUSE "material.diffuse"
#define MATERIAL_VARIBLE_SPECULAR "material.specular"
#define MATERIAL_VARIBLE_SHINESS "material.shininess"
//PBR模型钩子宏
#define MATERIAL_VARIBLE_NORMAL "material.normal"
#define MATERIAL_VARIBLE_METALLIC "material.metallic"
#define MATERIAL_VARIBLE_ROUGHNESS "material.roughness"
#define MATERIAL_VARIBLE_AO "material.ao"
#define MATERIAL_VARIBLE_EMIT "material.emit"

#define MATERIAL_VARIBLE_TRANSPARENCY "material.transparency"
#define MATERIAL_VARIBLE_SPECULAR_REFLECTANCE "material.specularReflectivity"
#define MATERIAL_VARIBLE_DIFFUSE_REFLECTANCE "material.diffuseReflectivity"

#define MODEL_MATRIX_GPU_HOOK "model"
#define VIEW_MATRIX_GPU_HOOK "view"
#define PROJECTION_MATRIX_GPU_HOOK "projection"


#define MATH_PI 3.141592f
#define MATH_PI_DIV_4 0.785398
#define MATH_PI_DIV_2 1.570796
#define MATH_RADIANCE 0.017453292f

#define APPLICATION_WIDTH 1280
#define APLLICATION_HEIGHT 720

#define AXIS_X glm::vec3(0, 1, 0)
#define AXIS_Y glm::vec3(1, 0, 0)
#define AXIS_Z glm::vec3(0, 0, 1)

//引擎灯光相关宏设置
#define POINT_LIGHT_MAX_NUM 4 
#define POINT_LIGHT_NUM "lightsNum"


//Camera相关宏设置
#define CAMERA_DEFAULT_FOV MATH_PI_DIV_4
#define CAMERA_DEFAULT_ASPECT 1.33F
#define CAMREA_DEFAULT_NEAR_Z 0.1F
#define CAMERA_DEFAULT_FAR_Z 100.0F

#define CAMERA_THIRD_ROLE_LIMIT_PICTH 89.0f
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
#define SHADER_FILE_DIR "shader/"

#define SG_TEXTURE_ACTIVE_SLOT0 0
#define SG_TEXTURE_ACTIVE_SLOT1 1
#define SG_TEXTURE_ACTIVE_SLOT2 2
#define SG_TEXTURE_ACTIVE_SLOT3 3
#define SG_TEXTURE_ACTIVE_SLOT4 4
#define SG_TEXTURE_ACTIVE_SLOT5 5
#define SG_TEXTURE_ACTIVE_SLOT6 6
#define SG_TEXTURE_ACTIVE_SLOT7 7
#define SG_TEXTURE_ACTIVE_SLOT8 8
#define SG_TEXTURE_ACTIVE_SLOT9 9
#define SG_TEXTURE_ACTIVE_SLOT10 10
#define SG_TEXTURE_ACTIVE_SLOT11 11
#define SG_TEXTURE_ACTIVE_SLOT12 12


enum SG_MESH_MODE {
	SIMPLE, COMPLEX
};

enum SG_TEXTURE_TYPE {
	TEXTURE_CUSTOM = 0,
	TEXTURE_NORMAL = 1,
	TEXTURE_DIFFUSE = 2,
	TEXTURE_AO = 3,
	TEXTURE_ROUGHNESS = 4,
	TEXTURE_METALLIC = 5,
	TEXTURE_EMIT = 6,
};

/*
//Global Setting
std::map<int, std::string> SG_TEXTURE_NAME = {
	{1, "normal.jpg"},
	{2, "color.jpg"},
	{6, "emit.jpg"},
	{4, "roughness.jpg"},
	{5, "metalness.jpg"},
	{3, "ao.jpg"},
};*/




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
