#version 430 core
#extension GL_ARB_shading_language_include : require
#include "/common.glsl"

in vec3 fragPos;
in vec3 normal;
in vec2 uv;

out vec4 fragColor;

uniform sampler2D screenTexture;

void main(){
	fragColor = texture(screenTexture, uv);
}