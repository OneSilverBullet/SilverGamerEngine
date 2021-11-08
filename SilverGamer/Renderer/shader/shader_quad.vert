#version 430 core
#extension GL_ARB_shading_language_include : require
#include "/common.glsl"

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aUV;

out vec2 texCoords;

void main(){
	texCoords = aUV;
	gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
}