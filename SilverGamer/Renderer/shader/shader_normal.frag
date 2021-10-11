#version 430 core
#extension GL_ARB_shading_language_include : require
#include "/common.glsl"

in vec3 fragPos;
in vec3 normal;
in vec2 uv;

uniform vec3 camPos;
uniform MaterialPhongFlat mat; 

out vec4 fragcolor;

void main(){
	vec3 res = vec3(0.0);
	vec3 viewDir = normalize(camPos - fragPos);
	res = CalcDirLight(dirLight, mat, normal, viewDir, uv);
	for(int i=0; i<SG_RENDER_PIPELINE_LIGHT_NUM; ++i){
		res += CalcSpotLight(pointLights[i], pointLights, fragPos, normal, viewDir, uv);
	}
	fragcolor = vec4(res.xyz, 1);
}