#ifndef SEARCH_TOOLS_H
#define SEARCH_TOOLS_H

// Standard
#include <stdbool.h>

// External
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define NO_COORD 666.0f

typedef struct dot_coord dot_coord;

struct dot_coord {
    GLfloat x, y
};

extern dot_coord dot;


bool getRLCbyDot(RLC* rlc, dot_coord* dot);
GLint getRoadIndex(dot_coord* dot);
bool isInRoadRange(GLint roadIndex, dot_coord* dot);
GLint getLineIndex(dot_coord* dot, GLint roadIndex);
GLfloat getRoadLeftCheek(GLint roadIndex);
GLint getCellIndex(dot_coord* dot, GLint roadIndex, GLint lineIndex);
GLvoid clearDot(dot_coord* dot);

#endif