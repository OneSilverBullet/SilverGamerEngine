
#define MaxLights 16;

struct Light
{
	float3 strength;
	float fallOffStart;
	float3 direction;
	float fallOffEnd;
	float3 position;
	float spotPower;
};

struct Material
{
	float4 diffuseAlbedo;
	float3 fresnelR0;
	float shininess;
};

//Light Model Blinn Phong
float3 BlinnPhong(float3 lightStrength, float3 lightVec, float3 normal, float3 toEye, Material mat)
{
	const float m = mat.shininess * 256.0f;
	float3 halfVec = normalize(lightVec + toEye);

	float roughnessFactor = (m + 8.0f) * pow(max(dot(halfVec, normal), 0.0f), m) / 8.0f;
	float3 fresnelFactor =     
	  



}




float3 ComputeDirectionalLight(Light L, Material mat, float3 normal, float3 toEye) {


}
