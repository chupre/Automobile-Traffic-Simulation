#pragma once

// External
#include <cglm/cglm.h>
#include <glad/glad.h>

// Custom modules
#include <direction.h>

#define MAX_CAMERA_DISTANCE 1.0f

extern vec3 cameraPos;
extern vec3 cameraFront;
extern vec3 cameraUp;
extern GLfloat cameraFOV;

typedef enum camDir camDir;

enum camDir { 
    UP, DOWN, RIGHT, LEFT
}; 

GLvoid setProjection();
GLvoid setView();
GLvoid moveCamera(camDir dir);
GLint isPossibleToMoveCam(camDir dir, GLfloat offset);
GLvoid getTestVertex(GLint roadIndex, camDir dir, vec4 dest);
GLint isVertexOnCamBorders(vec4 vertex, camDir dir, GLfloat offset, DIRECTION roadDir);
GLvoid setTestVertex(vec4 vertex, vec3 newCamPos);
