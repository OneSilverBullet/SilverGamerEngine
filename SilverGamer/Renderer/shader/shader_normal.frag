#version 430 core
#extension GL_ARB_shading_language_include : require
#include "/common.glsl"

in vec3 fragPos;
in vec3 normal;
in vec2 uv;

uniform vec3 camPos;

uniform MaterialPBREmit mat;


uniform int lightsNum;
uniform vec3 lightPositions[4];
uniform vec3 lightColors[4];



out vec4 fragcolor;

//tangent normal calculator
vec3 GetNormalFromMap()
{
    vec3 tangentNormal = texture(mat.normal, uv).xyz * 2.0 - 1.0;

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
	vec3 diffuse = pow(texture(mat.diffuse, uv),2.2);
	vec3 emit = pow(texture(mat.emit, uv), 2.2);
	float metallic = texture(mat.metallic, uv).r;
	float roughness = texture(mat.roughness, uv).r;
	float ao = texture(mat.ao, uv).r;

	vec3 N = GetNormalFromMap();
	vec3 V = normalize(camPos - fragPos);

	vec3 F0 = mix(METAL_WORKFLOW_F0, diffuse, metallic);

	//calculate the render equation
	vec3 res = vec3(0.0);
	for(int i=0; i <lightsNum; ++i)
	{
		vec3 L = normalize(lightPositions[i] - fragPos);
		vec3 H =  normalize(L + V); // half vec
		float distance =  length(lightPositions[i] - fragPos);
		float attenuation = 1.0 / (distance * distance);
		vec3 radiance =  lightColors[i] * attenuation;

		//Cook Torrance
		float NDF = DistributionGGX(N, H, roughness);
		float G = GeometrySmith(N, V, L, roughness);
		vec3 F = FresnelEquation(max(dot(H,V),0.0), F0);

		vec3 numerator = NDF * G * F;
		float denominator = 4 * max(dot(N,V),0.0) * max(dot(N, L), 0.0) + 0.0001;
		vec3 specular = numerator / denominator;

		vec3 kS = F;
		vec3 kD = vec3(1.0) - kS;
		kD *= 1.0 - metallic;

		float NdotL = max(dot(N, L), 0.0);

		res += (kD * diffuse / PI + specular) * radiance * NdotL;
	}

	vec3 ambient = vec3(0.03) * diffuse * ao;
	vec3 color = ambient + res + emit; //

	//tone mapping
	color = color / (color + vec3(1.0));

	//gamma
	color = pow(color, vec3(1.0 / 2.2));

	fragcolor = color;
}