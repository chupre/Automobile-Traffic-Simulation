// Standard
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Custom modules
#include <shader.h>

int shaderProgram, carShader, backgroundShader;

char* getShaderContent(const GLchar* fileName)
{
    FILE* shaderFile;
    long size = 0;
    char* shaderContent;

    shaderFile = fopen(fileName, "r");

    if (shaderFile == NULL)
        printf("Failed to open %s\n", fileName);

    fseek(shaderFile, 0, SEEK_END);
    size = ftell(shaderFile);
    rewind(shaderFile);

    shaderContent = memset(malloc(size + 1), '\0', size + 1);

    if (shaderContent == NULL) {
        printf("malloc failed with shaderContent");
        exit(1);
    }

    fread(shaderContent, 1, size, shaderFile);
    fclose(shaderFile);

    return shaderContent;
}

void genShader(int * shader, char * vertexShaderName, char * fragmentShaderName)
{
    char vertexShaderFullName[128] = "../source/shaders/";
    char fragmentShaderFullName[128] = "../source/shaders/";
    strcat(vertexShaderFullName, vertexShaderName);
    strcat(fragmentShaderFullName, fragmentShaderName);

    const char* vertexShaderSource = getShaderContent(vertexShaderFullName);
    const char* fragmentShaderSource = getShaderContent(fragmentShaderFullName);

    GLuint vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    *shader = glCreateProgram();
    glAttachShader(*shader, vertexShader);
    glAttachShader(*shader, fragmentShader);
    glLinkProgram(*shader);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}
