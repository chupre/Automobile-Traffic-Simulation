#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in mat3 aTransform;

out vec3 color;

uniform bool isCar;

uniform mat4 projection;
uniform mat4 view;

float random(float seed) 
{
    return fract(sin(seed) * 43758.5453);
}

void main()
{
    color = aColor;

    if (isCar)
    {
        color = vec3(random(gl_InstanceID), random(cos(gl_InstanceID)), random(gl_InstanceID * sin(gl_InstanceID)));
        gl_Position = projection * view * vec4(aTransform * vec3(aPos, 1.0), 1.0);
    }
    else
    {
        gl_Position =  projection * view * vec4(aPos, 1.0, 1.0);
    }

}
