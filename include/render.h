#ifndef RENDER_H
#define RENDER_H

// Standard
#include <stdbool.h>

// External
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

#include <rlc.h>

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
extern bool isHelpMenuActive;
extern bool isShowInfo;
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
extern unsigned int backgroundVBO, backgroundVAO, backgroundEBO;
extern bool isLinePicked;
extern RLC pickedRLC;

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
void showInfo();
void initBackground();
void errorCallback(int e, const char *d);

GLfloat getScreenVelocity(car* Car);
GLfloat getScreenVelocityShift(car* Car);

#ifdef DEBUG

GLvoid dbgInitCells();
GLvoid dbgRenderCells();
extern GLuint cellsVBO, cellsVAO;
extern GLfloat * cellsVertices;
extern bool dbgIsCellsInit;
extern bool dbgVerticesInit;

#endif

#endif
