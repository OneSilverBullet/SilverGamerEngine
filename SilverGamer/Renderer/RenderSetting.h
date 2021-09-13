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



enum SG_MESH_MODE {
	SIMPLE, COMPLEX
};




#endif
