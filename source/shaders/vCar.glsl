#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in mat3 aTransform;

out vec2 texCoord;
out vec3 color;

uniform mat4 projection;
uniform mat4 view;

float random(float seed) 
{
    return fract(sin(seed) * 238.5453);
}

void main()
{
    gl_Position = projection * view * vec4(aTransform * vec3(aPos, 1.0), 1.0);
    texCoord = aTexCoord;
    color = vec3(random(gl_InstanceID), random(cos(gl_InstanceID)), random(gl_InstanceID * sin(gl_InstanceID)));
}
