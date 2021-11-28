#version 430 core
#extension GL_ARB_shading_language_include : require
#include "/common.glsl"

in vec3 fragPos;
in vec3 normal;
in vec2 uv;

uniform vec3 camPos;

uniform MaterialPhongFlat material;

uniform int lightsNum;

out vec4 fragcolor;

void main(){
    fragcolor = vec4(1.0);
    return;

    vec3 result = vec3(0.0);
    for(int i=0; i<lightsNum; ++i)
    {
         // Ambient
        float ambientStrength = 0.1f;
        vec3 ambient = vec3(ambientStrength);
  	    
        // Diffuse 
        vec3 norm = normalize(normal);
        vec3 lightDir = normalize(pointLights[i].position - fragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * material.diffuse;
        
        // Specular
        float specularStrength = 0.5f;
        vec3 viewDir = normalize(camPos - fragPos);
        vec3 reflectDir = reflect(-lightDir, norm);  
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular = specularStrength * spec * material.specular;  
        
        result += (ambient + diffuse + specular) * pointLights[i].light_color;
    }

	fragcolor = vec4(result, 1.0f);
}