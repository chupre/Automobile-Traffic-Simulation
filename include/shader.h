#ifndef SHADER_H
#define SHADER_H

// External
#include <glad/glad.h>

extern int shaderProgram, carShader, backgroundShader;

void genShader(int * shader, char * vertexShaderName, char * fragmentShaderName);
char* getShaderContent(const GLchar* fileName);

#endif
