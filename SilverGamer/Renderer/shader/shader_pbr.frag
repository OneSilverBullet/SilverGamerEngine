#version 430 core
#extension GL_ARB_shading_language_include : require
#include "/common.glsl"
#include "/math.glsl"

in vec3 fragPos;
in vec3 normal;
in vec2 uv;

uniform vec3 camPos;

uniform MaterialPBREmit material;


uniform int lightsNum;


out vec4 fragcolor;

//tangent normal calculator
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
	vec3 diffuse = pow(texture(material.diffuse, uv).xyz, vec3(2.2));
	vec3 emit = pow(texture(material.emit, uv).xyz, vec3(2.2));
	float metallic = texture(material.metallic, uv).r;
	float roughness = 1.0 - texture(material.roughness, uv).r;
	float ao = texture(material.ao, uv).r;

	vec3 N = GetNormalFromMap();
	vec3 V = normalize(camPos - fragPos);

	vec3 F0 = mix(METAL_WORKFLOW_F0, diffuse, metallic);

	//calculate the render equation
	vec3 res = vec3(0.0);
	
	for(int i=0; i <lightsNum; ++i)
	{
		vec3 L = normalize(pointLights[i].position - fragPos);
		vec3 H =  normalize(L + V); // half vec
		float distance =  length(pointLights[i].position - fragPos);
		float attenuation = 1.0 / (distance * distance);
		vec3 radiance =  pointLights[i].light_color * attenuation;

		//Cook Torrance
		float NDF = DistributionGGX(N, H, roughness);
		float G = GeometrySmith(N, V, L, roughness);
		vec3 F = FresnelEquation(max(dot(H,V),0.0), F0);

		vec3 numerator = NDF * G * F;
		float denominator = 4 * max(dot(N,V),0.0) * max(dot(N, L), 0.0) + 0.001;
		vec3 specular = numerator / denominator;

		vec3 kS = F;
		vec3 kD = vec3(1.0) - kS;
		kD *= 1.0 - metallic;

		float NdotL = max(dot(N, L), 0.0);

		res += (kD * diffuse / PI + specular) * radiance * NdotL;
	}
	


	vec3 ambient = vec3(0.03) * diffuse * ao;
	vec3 color = ambient + res + emit * 3.0f; //

	//tone mapping
	color = color / (color + vec3(1.0));

	//gamma
	color = pow(color, vec3(1.0 / 2.2));

	fragcolor = vec4(color, 1.0);
}