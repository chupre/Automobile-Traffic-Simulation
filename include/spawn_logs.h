#ifndef SPAWN_LOGS_H
#define SPAWN_LOGS_H

// External
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cars.h>

extern int fileNumber;
extern char fileName[100];
extern FILE* filePtr;

GLvoid logCar(car* Car);
GLvoid changeFileName();
GLvoid openFile();
GLvoid closeFile();


#endif