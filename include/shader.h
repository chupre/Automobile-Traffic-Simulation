#ifndef SHADER_H
#define SHADER_H

// External
#include <glad/glad.h>

extern GLuint shaderProgram;

void genShader();
char* getShaderContent(const GLchar* fileName);

#endif