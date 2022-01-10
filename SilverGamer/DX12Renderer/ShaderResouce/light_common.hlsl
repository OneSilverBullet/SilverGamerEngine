
#define MaxLights 16

struct Light
{
	float3 strength;
	float fallOffStart;
	float3 position;
	float fallOffEnd;
	float3 direction;
	float spotPower;
};

struct Material
{
	float4 diffuseAlbedo;
	float3 fresnelR0;
	float shininess;
};


float CalcAttenuation(float d, float falloffStart, float falloffEnd) {
	return saturate((falloffEnd - d)/(falloffEnd - falloffStart));
}


float3 SchlickFresnel(float3 R0, float3 normal, float3 lightVec) {
	float cosIncidentAngle = saturate(dot(normal, lightVec));
	float f0 = 1.0f - cosIncidentAngle;
	float3 reflectPercent = R0 + (1.0f - R0) * (f0 * f0 * f0 * f0 * f0);
	return reflectPercent;
}


//Light Model Blinn Phong
float3 BlinnPhong(float3 lightStrength, float3 lightVec, float3 normal, float3 toEye, Material mat)
{
	const float m = mat.shininess * 256.0f;
	float3 halfVec = normalize(lightVec + toEye);

	float roughnessFactor = (m + 8.0f) * pow(max(dot(halfVec, normal), 0.0f), m) / 8.0f;
	float3 fresnelFactor = SchlickFresnel(mat.fresnelR0, halfVec, lightVec);
	  
	float3 specAlbedo = fresnelFactor * roughnessFactor;
	specAlbedo = specAlbedo / (specAlbedo + 1.0f);
	return (mat.diffuseAlbedo.rgb + specAlbedo) * lightStrength;
}

float3 ComputeDirectionalLight(Light L, Material mat, float3 normal, float3 toEye) {
	// The light vector aims opposite the direction the light rays travel.
	float3 lightVec = -L.direction;

	// Scale light down by Lambert's cosine law.
	float ndotl = max(dot(lightVec, normal), 0.0f);
	float3 lightStrength = L.strength * ndotl;

	return BlinnPhong(lightStrength, lightVec, normal, toEye, mat);
}

//Compute the Lighting
float4 ComputeLighting(Light lights[MaxLights], Material mat,
	float3 pos, float3 normal, float3 toEye, float3 shadowFactor)
{
	float4 result = 0.0f;
#if (NUM_DIR_LIGHTS > 0)
	for (int i = 0; i < NUM_DIR_LIGHTS; ++i)
	{
		result.xyz += shadowFactor[i] * ComputeDirectionalLight(lights[i], mat, normal, toEye);
	}
#endif
	return result;
}
