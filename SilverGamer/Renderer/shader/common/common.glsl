
struct MaterialPhongFlat
{
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct MaterialPhong
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct MaterialGBuffer
{
    sampler2D position;
    sampler2D normal;
    sampler2D diffuse;
    sampler2D mra;
    sampler2D emit;
};

struct MaterialPBR
{
    sampler2D diffuse;
    sampler2D normal;
    sampler2D metallic;
    sampler2D roughness;
    sampler2D ao;
};

struct MaterialPBREmit
{
    sampler2D diffuse;
    sampler2D normal;
    sampler2D metallic;
    sampler2D roughness;
    sampler2D ao;
    sampler2D emit;
};

struct PointLight
{   
    vec3 light_color;

    vec3 position; 

    vec3 ambient;
    vec3 diffuse;
    vec3 specular; 
    
    float constant; 
    float linear;
    float quadratic;

};

struct DirLight
{
    vec3 light_color;
    vec3 direction;
    vec3 diffuse;
    vec3 specular;
};

#define PI 3.14159265
#define SG_RENDER_PIPELINE_LIGHT_NUM 4

#define METAL_WORKFLOW_F0 vec3(0.04)

uniform DirLight dirLight;
uniform PointLight pointLights[SG_RENDER_PIPELINE_LIGHT_NUM];


//Blinn Phone Light Model For DirLight and MaterialPhongFlat
//args:
//vec3 lightDir   = normalize(lightPos - FragPos);
//vec3 viewDir    = normalize(viewPos - FragPos);
vec3 CalcDirLight(DirLight light, MaterialPhongFlat mat, vec3 normal, vec3 viewDir, vec2 uv)
{
    vec3 lightDir = normalize(-light.direction);
    float factor_diffuse = max(dot(lightDir, normal), 0);
    vec3 halfDir = normalize(lightDir + viewDir);
    float factor_spec = pow(max(dot(normal, halfDir), 0.0), mat.shininess);
    vec3 ambient = light.diffuse * mat.diffuse;
    vec3 diffuse = light.diffuse * factor_diffuse * mat.diffuse;
    vec3 specular = light.specular * factor_spec * mat.diffuse;
    return vec3(ambient + diffuse + specular);
}


//Blinn Phone Light Model For DirLight MaterialPhong
//args:
//vec3 lightDir   = normalize(lightPos - FragPos);
//vec3 viewDir    = normalize(viewPos - FragPos);
vec3 CalcSpotLight(PointLight light, MaterialPhongFlat mat, vec3 fragPos, vec3 normal, vec3 viewDir, vec2 uv)
{
    //Light Factor
    vec3 lightDir = normalize(light.position - fragPos);
    float factor_diffuse = max(dot(lightDir, normal), 0.0);
    vec3 halfDir = normalize(lightDir + viewDir);
    float factor_spec = pow(max(dot(normal, halfDir), 0.0), mat.shininess);
    //Attenuation Factor
    float distance = length(light.position - fragPos);
    float factor_attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);
    //result
    vec3 ambient = light.ambient * mat.diffuse * factor_attenuation;
    vec3 diffuse = light.diffuse * factor_diffuse * mat.diffuse * factor_attenuation;
    vec3 specular = light.specular * factor_spec * mat.diffuse * factor_attenuation;
    return ambient + diffuse + specular;
}

//Blinn Phone Light Model For DirLight and MaterialPhong
//args:
//vec3 lightDir   = normalize(lightPos - FragPos);
//vec3 viewDir    = normalize(viewPos - FragPos);
vec3 CalcDirLight(DirLight light, MaterialPhong mat, vec3 normal, vec3 viewDir, vec2 uv)
{
    vec3 lightDir = normalize(-light.direction);
    float factor_diffuse = max(dot(lightDir, normal), 0);
    vec3 halfDir = normalize(lightDir + viewDir);
    float factor_spec = pow(max(dot(normal, halfDir), 0.0), mat.shininess);
    vec3 ambient = light.diffuse * vec3(texture(mat.diffuse, uv));
    vec3 diffuse = light.diffuse * factor_diffuse * vec3(texture(mat.diffuse, uv));
    vec3 specular = light.specular * factor_spec * vec3(texture(mat.specular, uv));
    return vec3(ambient + diffuse + specular);
}


//Blinn Phone Light Model For DirLight MaterialPhong
//args:
//vec3 lightDir   = normalize(lightPos - FragPos);
//vec3 viewDir    = normalize(viewPos - FragPos);
vec3 CalcSpotLight(PointLight light, MaterialPhong mat, vec3 fragPos, vec3 normal, vec3 viewDir, vec2 uv)
{
    //Light Factor
    vec3 lightDir = normalize(light.position - fragPos);
    float factor_diffuse = max(dot(lightDir, normal), 0.0);
    vec3 halfDir = normalize(lightDir + viewDir);
    float factor_spec = pow(max(dot(normal, halfDir), 0.0), mat.shininess);
    //Attenuation Factor
    float distance = length(light.position - fragPos);
    float factor_attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);
    //result
    vec3 ambient = light.ambient * vec3(texture(mat.diffuse, uv)) * factor_attenuation;
    vec3 diffuse = light.diffuse * factor_diffuse * vec3(texture(mat.diffuse, uv)) * factor_attenuation;
    vec3 specular = light.specular * factor_spec * vec3(texture(mat.specular, uv)) * factor_attenuation;
    return ambient + diffuse + specular;
}


//Fresnel Equation
//reflection ratio = reflection ray / ray
vec3 FresnelEquation(float cosTheta, vec3 F0)
{
    return F0 + (1 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

//Distribution function D
//Considering surface roughness, get the micro surface ratio 
//the micro surfaces which have similar direction with normal-vec
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float alpha = roughness * roughness;
    float alpha2 = alpha * alpha;
    float NdotH =  max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float num = alpha2;
    float denom = (NdotH2 * (alpha2 - 1.0) + 1.0);
    denom = PI * denom * denom;
    return num / denom;
}

//Geometry function G
//Micro Plane Occlusion: the ratio of planes be occlusion
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;
    
    float num = NdotV;
    float denom = NdotV * (1.0 - k) + k;
    
    return num / denom;
}
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);
    return ggx1 * ggx2;
}


