#version 450 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aUv;

out vec3 fragPos;
out vec3 normal;
out vec2 uv;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main()
{
    fragPos = vec3(model * vec4(aPos, 1.0));
    normal = vec3(transpose(inverse(model))) * normal;
    uv = aUv;
    gl_Position = projection * view * vec4(fragPos, 1.0);
}