#pragma once

// External
#include <glad/glad.h>

extern GLuint shaderProgram;

void genShader();
char* getShaderContent(const GLchar* fileName);