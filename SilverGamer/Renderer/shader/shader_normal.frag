#version 450 core
#include "common.glsl"

in vec3 fragPos;
in vec3 normal;
in vec2 uv;

uniform MaterialPhong mat;
uniform vec3 viewPos;

out vec4 fragcolor;

void main(){
	vec3 res = vec4(0);
	vec3 viewDir = normalize(viewPos - FragPos);
	for(int i=0; i<4; ++i){
		
		res += CalcSpotLight(pointLights[i], mat, fragPos, normal, viewDir, uv);
	}
	res += CalcDirLight(dirLight, mat, normal, viewDir, uv);
	fragcolor = vec4(res, 1);
}