#version 430 core
#extension GL_ARB_shading_language_include : require
#include "/common.glsl"

in vec2 texCoords;

out vec4 fragColor;

uniform sampler2D screenTexture;

void main(){
	fragColor = vec4(texture(screenTexture, texCoords).xyz, 1.0);
}