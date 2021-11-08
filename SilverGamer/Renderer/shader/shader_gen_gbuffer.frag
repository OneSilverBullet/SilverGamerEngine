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

vec3 GetNormalFromMap()
{
    vec3 tangentNormal = texture(material.normal, uv).xyz * 2.0 - 1.0;

	vec3 Q1 = dFdx(fragPos);
	vec3 Q2 = dFdy(fragPos);
	vec2 st1 = dFdx(uv);
	vec2 st2 = dFdy(uv);

	vec3 N = normalize(normal);
	vec3 T= normalize(Q1 * st2.t - Q2 * st1.t);
	vec3 B = -normalize(cross(N, T));
	mat3 TBN = mat3(T,B,N);

	return normalize(TBN * tangentNormal);
}


void main(){
	gPosition = fragPos;
	gNormal = GetNormalFromMap();
	gDiffuse = texture(material.diffuse, uv).rgb;
	gMRA = vec3(texture(material.metallic, uv).r,
						  texture(material.roughness, uv).r,
						  texture(material.ao, uv).r);
	gEmit = texture(material.emit, uv).rgb;
}