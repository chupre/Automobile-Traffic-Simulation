#ifndef RENDER_H
#define RENDER_H

// Standard
#include <stdbool.h>

// External
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

#define FPS 60.0f

//#define DEBUG

#define WINDOWS 0
#define UNIX 1
#if defined(_WIN32) || defined(WIN32)
    #define OS WINDOWS
#else
    #define OS UNIX
#endif

typedef struct car car;

extern GLuint WINDOW_WIDTH;
extern GLuint WINDOW_HEIGHT;
extern GLchar WINDOW_NAME[];
extern mat4 view;
extern mat4 projection;
extern bool paused;
extern bool isInitMenuActive;
extern bool isSaveMenuActive;
extern bool isLoadMenuActive;
extern bool isInit;
extern GLFWwindow* window;
extern struct nk_glfw glfw;
extern struct nk_context* context;
extern GLuint isCarLoc, projectionLoc, modelLoc;
extern GLdouble limitFPS;
extern GLdouble lastTime;
extern GLdouble deltaTime, currTime, endPauseTime;
extern GLdouble timer;
extern float mousePosX;
extern float mousePosY;

GLdouble getPauseTime();
GLvoid scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
GLvoid framebufferSizeCallback(GLFWwindow* window, int width, int height);
GLvoid keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
GLvoid cursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
GLvoid initRoads();
GLvoid initLines();
GLvoid initCars();
GLvoid render();
GLvoid quit();
GLvoid initGL();
GLvoid moveCarOnScreen(GLint carIndex);
GLvoid processKeyboardInput();
void showInitMenu();

GLfloat getScreenVelocity(car* Car);
GLfloat getScreenVelocityShift(car* Car);

#ifdef DEBUG

#include <road.h>
GLvoid dbgInitCells();
GLvoid dbgRenderCells();
extern GLuint cellsVBO, cellsVAO;
extern GLfloat cellsVertices[(NUMBER_OF_LINES + 1) * NUMBER_OF_ROADS * NUMBER_OF_CELLS * 5 * 8];
extern bool dbgIsCellsInit;
#define DEFAULT_FOV 60

#endif

#endif
