// Standard
#include "cglm/mat4.h"
#include <time.h>

// External
#include <stb/stb_image.h>

// Custom modules
#include <algorithms.h>
#include <camera.h>
#include <cars.h>
#include <map.h>
#include <render.h>
#include <road.h>
#include <search_tools.h>
#include <shader.h>

#define NK_ASSERT(a)
#define MAX_VERTEX_BUFFER 512 * 1024
#define MAX_ELEMENT_BUFFER 128 * 1024
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#include <nuklear/nuklear.h>
#include <nuklear/nuklear_glfw_gl3.h>

GLuint WINDOW_WIDTH = 1024;
GLuint WINDOW_HEIGHT = 768;
GLchar WINDOW_NAME[] = "Auto Traffic Simulator";

bool paused = true;
bool isInit = false;
bool isInitMenuActive = false;
bool isSaveMenuActive = false;
bool isLoadMenuActive = false;

float mousePosX = 0.0;
float mousePosY = 0.0;

GLFWwindow *window;

struct nk_glfw glfw = {0};
struct nk_context *context;

GLuint isCarLoc, projectionLoc, modelLoc;

GLdouble limitFPS = 1.0 / FPS;

GLdouble lastTime;
GLdouble deltaTime = 0, currTime = 0, endPauseTime = 0;
GLdouble timer;

#ifdef DEBUG

GLuint cellsVBO, cellsVAO;
GLfloat *cellsVertices;
bool dbgIsCellsInit = false;
bool dbgVerticesInit = false;

#endif

GLvoid scrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
  if (yoffset < 0.0) {
    cameraPos[0] = 0.0;
    cameraPos[1] = 0.0;
  }

  cameraFOV -= (float)yoffset;

  if (cameraFOV < 1.0f) {
    cameraFOV = 1.0f;
  }

  if (cameraFOV > DEFAULT_FOV) {
    cameraFOV = DEFAULT_FOV;
  }
}

GLvoid framebufferSizeCallback(GLFWwindow *window, int width, int height) {
  WINDOW_HEIGHT = height;
  WINDOW_WIDTH = width;

  glViewport(0, 0, width, height);
}

GLvoid keyCallback(GLFWwindow *window, int key, int scancode, int action,
                   int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    if (isSaveMenuActive) {
      isSaveMenuActive = false;
    } else if (isLoadMenuActive) {
      isLoadMenuActive = false;
    } else {
      paused = !paused;
    }
  }

  if (key == GLFW_KEY_F6 && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}

GLvoid cursorPositionCallback(GLFWwindow *window, double xpos, double ypos) {
  mousePosX = xpos;
  mousePosY = ypos;
}

void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && !paused) {
    float normalizedX = (2.0f * mousePosX / WINDOW_WIDTH) - 1.0f;
    float normalizedY = 1.0f - (2.0f * mousePosY / WINDOW_HEIGHT);

    setProjection();
    setView();

    mat4 inverseProjectionView;
    glm_mat4_mul(projection, view, inverseProjectionView);
    glm_mat4_inv(inverseProjectionView, inverseProjectionView);

    vec4 clipPos = {normalizedX, normalizedY, 0.0f, 1.0f};
    vec4 viewPos;
    glm_mat4_mulv(inverseProjectionView, clipPos, viewPos);

    vec4 worldPos;
    glm_vec4_divs(viewPos, viewPos[3], worldPos);

    dot_coord mousePos;
    mousePos.x = worldPos[0];
    mousePos.y = worldPos[1];

    RLC rlc;
    if (!getRLCbyDot(&rlc, &mousePos)) {
      glm_vec4_print(worldPos, stdout);
      return;
    }

    addInRLCcarAddingQueue(rlc);
  }
}

GLvoid processKeyboardInput() {
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    moveCamera(UP);
  }

  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    moveCamera(DOWN);
  }

  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    moveCamera(LEFT);
  }

  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    moveCamera(RIGHT);
  }
}

GLvoid initRoads() {
  glGenVertexArrays(1, &roadVAO);
  glGenBuffers(1, &roadVBO);
  glGenBuffers(1, &roadEBO);
  glBindVertexArray(roadVAO);

  glBindBuffer(GL_ARRAY_BUFFER, roadVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * NUMBER_OF_ROADS * 4 * 5,
               roadVertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, roadEBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLint) * 6 * NUMBER_OF_ROADS,
               roadIndices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat),
                        (GLvoid *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat),
                        (GLvoid *)(2 * sizeof(GLfloat)));
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

GLvoid initLines() {
  glGenVertexArrays(1, &lineVAO);
  glGenBuffers(1, &lineVBO);
  glBindVertexArray(lineVAO);

  glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
  glBufferData(GL_ARRAY_BUFFER,
               sizeof(GLfloat) * NUMBER_OF_LINES * NUMBER_OF_ROADS * 5 * 2,
               lineVertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat),
                        (GLvoid *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat),
                        (GLvoid *)(2 * sizeof(GLfloat)));
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

GLvoid initCars() {
  glGenBuffers(1, &carInstanceVBO);
  glBindBuffer(GL_ARRAY_BUFFER, carInstanceVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(mat3) * 1000, carTransformMatrixes,
               GL_DYNAMIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glGenVertexArrays(1, &carVAO);
  glGenBuffers(1, &carVBO);
  glGenBuffers(1, &carEBO);
  glBindVertexArray(carVAO);

  glBindBuffer(GL_ARRAY_BUFFER, carVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * MAX_CARS * 4 * 2, carVertices,
               GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, carEBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLint) * MAX_CARS * 6,
               carIndices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat),
                        (GLvoid *)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, carInstanceVBO);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(mat3),
                        (GLvoid *)(sizeof(float) * 0));
  glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(mat3),
                        (GLvoid *)(sizeof(float) * 3));
  glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(mat3),
                        (GLvoid *)(sizeof(float) * 6));
  glEnableVertexAttribArray(2);
  glEnableVertexAttribArray(3);
  glEnableVertexAttribArray(4);
  glVertexAttribDivisor(2, 1);
  glVertexAttribDivisor(3, 1);
  glVertexAttribDivisor(4, 1);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

GLvoid render() {
  glClearColor(0.28f, 0.55f, 0.24f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUseProgram(shaderProgram);

  isCarLoc = glGetUniformLocation(shaderProgram, "isCar");
  glUniform1i(isCarLoc, false);

  processKeyboardInput();

  setProjection();
  setView();

  glBindVertexArray(roadVAO);
  glDrawElements(GL_TRIANGLES, NUMBER_OF_ROADS * 6, GL_UNSIGNED_INT, 0);
  glBindVertexArray(lineVAO);
  glDrawArrays(GL_LINES, 0, NUMBER_OF_LINES * NUMBER_OF_ROADS * 2);

#ifdef DEBUG
  dbgRenderCells();
#endif

  glUniform1i(isCarLoc, true);
  glBindVertexArray(carVAO);

  for (int i = 0; i < MAX_CARS; i++) {
    if (cars[i].isActive && !cars[i].isCrushed) {
      moveCarOnScreen(i);
    }
  }

  glBindBuffer(GL_ARRAY_BUFFER, carInstanceVBO);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(mat3) * MAX_CARS,
                  carTransformMatrixes);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, carEBO);
  glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, MAX_CARS);
}

GLvoid quit() {
  glDeleteVertexArrays(1, &roadVAO);
  glDeleteBuffers(1, &roadVBO);
  glDeleteBuffers(1, &roadEBO);
  glDeleteProgram(shaderProgram);

  nk_glfw3_shutdown(&glfw);
  glfwTerminate();
  exit(0);
}

GLdouble getPauseTime() {
  GLdouble pauseTime = endPauseTime - lastTime;

  if (pauseTime > 0) {
    return pauseTime;
  } else {
    return 0;
  }
}

GLvoid initGL() {
  srand(time(NULL));

  glfwInit();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_RESIZABLE, false);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_SAMPLES, 4);

  window =
      glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, NULL, NULL);
  glfwMakeContextCurrent(window);

  int iconWidth, iconHeight;
  int channels;
  unsigned char *pixels = stbi_load("../resources/icons/icon.png", &iconWidth,
                                    &iconHeight, &channels, 4);

  GLFWimage images[1];
  images[0].width = iconWidth;
  images[0].height = iconHeight;
  images[0].pixels = pixels;

  glfwSetWindowIcon(window, 1, images);

  gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
  glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_MULTISAMPLE);

  context = nk_glfw3_init(&glfw, window, NK_GLFW3_INSTALL_CALLBACKS);

  glfwSwapInterval(1);

  glfwSetKeyCallback(window, keyCallback);
  glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
  glfwSetScrollCallback(window, scrollCallback);
  glfwSetCursorPosCallback(window, cursorPositionCallback);
  glfwSetMouseButtonCallback(window, mouseButtonCallback);
}

GLfloat getScreenVelocity(car *Car) {
  return (GLfloat)(Car->velocity * Car->roadDirMultiplier *
                   VELOCITY_MULTIPLIER) /
         FPS;
}

GLfloat getScreenVelocityShift(car *Car) {
  return (GLfloat)(_1_CELL_ * getDirMultiplier(Car->moveDir) *
                   VELOCITY_MULTIPLIER) /
         FPS;
}

GLvoid moveCarOnScreen(GLint carIndex) {
  car *Car = &cars[carIndex];
  GLfloat screenVelocity;

  if (Car->move == FORWARD) {
    screenVelocity = getScreenVelocity(Car);
    Car->realPos += screenVelocity;
  } else {
    screenVelocity = getScreenVelocityShift(Car);
  }

  if (Car->moveDir == NORTH || Car->moveDir == SOUTH) {
    glm_translate2d_y(carTransformMatrixes[carIndex], screenVelocity);
  } else {
    glm_translate2d_x(carTransformMatrixes[carIndex], screenVelocity);
    // printf("X shift\n");
  }
}

#ifdef DEBUG

GLvoid dbgInitCells() {
  glGenVertexArrays(1, &cellsVAO);
  glGenBuffers(1, &cellsVBO);

  glBindVertexArray(cellsVAO);
  glBindBuffer(GL_ARRAY_BUFFER, cellsVBO);
  glBufferData(GL_ARRAY_BUFFER,
               sizeof(float) * (NUMBER_OF_LINES + 1) * NUMBER_OF_ROADS *
                   NUMBER_OF_CELLS * 5 * 8,
               cellsVertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat),
                        (GLvoid *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat),
                        (GLvoid *)(2 * sizeof(GLfloat)));
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

GLvoid dbgRenderCells() {
  if (!dbgIsCellsInit) {
    dbgInitCells();
    dbgIsCellsInit = true;
  }

  glBindVertexArray(cellsVAO);
  glDrawArrays(
      GL_LINES, 0,
      (2 * 5 * 8 * NUMBER_OF_CELLS * (NUMBER_OF_LINES + 1) * NUMBER_OF_ROADS));
}

#endif
