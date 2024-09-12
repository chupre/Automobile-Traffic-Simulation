#version 330 core

out vec4 FragColor;

in vec2 texCoord;
in vec3 color;

uniform sampler2D ourTexture;

void main()
{
    vec4 texColor = texture(ourTexture, texCoord);
    if (texColor.a < 0.1)
        discard;
    FragColor = texture(ourTexture, texCoord) * vec4(color, 1.0f);
} 
