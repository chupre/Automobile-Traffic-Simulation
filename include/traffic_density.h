#ifndef TRAFFIC_DENSITY
#define TRAFFIC_DENSITY


// External
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <map.h>

extern GLint * densityData;

GLvoid increaseDensityData(GLint roadIndex);
GLvoid decreaseDensityData(GLint roadIndex);
GLint getDensityData(GLint roadIndex);

#endif
