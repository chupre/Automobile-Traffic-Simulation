#pragma once

GLuint shaderProgram;

void genShader();
char* getShaderContent(const GLchar* fileName);

char* getShaderContent(const GLchar* fileName)
{
    FILE* shaderFile;
    long size = 0;
    char* shaderContent;

    shaderFile = fopen(fileName, "r");
    fseek(shaderFile, 0, SEEK_END);
    size = ftell(shaderFile);
    rewind(shaderFile);

    shaderContent = memset(malloc(size + 1), '\0', size + 1);
    fread(shaderContent, 1, size, shaderFile);
    fclose(shaderFile);

    return shaderContent;
}

void genShader()
{
    const GLchar* vertexShaderSource = getShaderContent("shaders/vertex_shader.glsl");
    const GLchar* fragmentShaderSource = getShaderContent("shaders/fragment_shader.glsl");

    GLuint vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}