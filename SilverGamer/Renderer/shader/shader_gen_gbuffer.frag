#version 430 core
#extension GL_ARB_shading_language_include : require
#include "/common.glsl"

layout(location = 0) out vec3 gPosition;
layout(location = 1) out vec3 gNormal;
layout(location = 2) out vec3 gDiffuse;
layout(location = 3) out vec3 gMRA;
layout(location = 4) out vec3 gEmit;

in vec3 fragPos;
in vec3 normal;
in vec2 uv;

uniform vec3 camPos;

uniform MaterialPBREmit material;

void main(){
	gPosition = fragPos;
	gNormal = normalize(normal);
	gDiffuse = texture(material.diffuse, uv).rgb;
	gMRA = vec3(texture(material.metallic, uv).r,
						  texture(material.roughness, uv).r,
						  texture(material.ao, uv).r);
	gEmit = texture(material.emit, uv).rgb;
}