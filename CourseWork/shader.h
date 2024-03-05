#pragma once
#include <glad/glad.h> 
#include <glfw/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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