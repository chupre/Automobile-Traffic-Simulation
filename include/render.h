#pragma once

// Standard
#include <stdbool.h>

// External
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define FPS 60.0f

#define WINDOWS 0
#define UNIX 1
#if defined(_WIN32) || defined(WIN32)
    #define OS WINDOWS
#else
    #define OS UNIX
#endif

extern GLuint WINDOW_WIDTH;
extern GLuint WINDOW_HEIGHT;
extern GLchar WINDOW_NAME[];
extern bool paused;
extern bool isSaveMenuActive;
extern bool isLoadMenuActive;
extern GLFWwindow* window;
extern struct nk_glfw glfw;
extern struct nk_context* context;
extern GLuint isCarLoc, projectionLoc, modelLoc;
extern GLdouble limitFPS;
extern GLdouble lastTime;
extern GLdouble deltaTime, currTime, endPauseTime;
extern GLdouble timer;

GLdouble getPauseTime();
GLvoid scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
GLvoid framebufferSizeCallback(GLFWwindow* window, int width, int height);
GLvoid keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
GLvoid initRoads();
GLvoid initLines();
GLvoid initCars();
GLvoid render();
GLvoid quit();
GLvoid initGL();
GLvoid moveCarOnScreen(GLint carIndex);
GLvoid processKeyboardInput();