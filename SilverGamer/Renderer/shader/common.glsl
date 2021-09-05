#version 450 core

struct MaterialPhong
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct MaterialPBR
{
    sampler2D albedo;
    sampler2D normal;
    sampler2D metallic;
    sampler2D roughness;
    sampler2D ao;
};

struct PointLight
{
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
    vec3 direction;
    vec3 diffuse;
    vec3 specular;
};


#define SG_RENDER_PIPELINE_LIGHT_NUM 4

uniform DirLight dirLight;
uniform PointLight pointLights[SG_RENDER_PIPELINE_LIGHT_NUM];

//Blinn Phone Light Model For DirLight 
//args:
//vec3 lightDir   = normalize(lightPos - FragPos);
//vec3 viewDir    = normalize(viewPos - FragPos);
vec3 CalcDirLight(DirLight light, MaterialPhong mat, vec3 normal, vec3 viewDir, vec2 uv)
{
    vec3 lightDir = normalize(-light.direction);
    float factor_diffuse = max(dot(lightDir, normal), 0);
    vec3 halfDir = normalize(lightDir + viewDir);
    float factor_spec = pow(max(dot(normal, halfDir), 0.0), mat.shininess);
    vec3 ambient = light.ambient * vec3(texture(mat.diffuse, uv));
    vec3 diffuse = light.diffuse * factor_diffuse * vec3(texture(mat.diffuse, uv));
    vec3 specular = light.specular * factor_spec * vec3(texture(mat.specular, uv));
    return vec3(ambient + diffuse + specular);
}


//Blinn Phone Light Model For DirLight 
//args:
//vec3 lightDir   = normalize(lightPos - FragPos);
//vec3 viewDir    = normalize(viewPos - FragPos);
vec3 CalcSpotLight(PointLight light, MaterialPhong mat, vec3 fragPos, vec3 normal, vec3 viewDir, vec2 uv)
{
    //Light Factor
    vec3 lightDir = normalize(light.position - fragPos);
    float factor_diffuse = max(dot(lightDir, normal));
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


