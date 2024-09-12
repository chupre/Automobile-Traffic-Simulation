#ifndef SHADER_H
#define SHADER_H

// External
#include <glad/glad.h>

extern GLuint shaderProgram, carShader;

void genShader(int * shader, char * vertexShaderName, char * fragmentShaderName);
char* getShaderContent(const GLchar* fileName);

#endif
