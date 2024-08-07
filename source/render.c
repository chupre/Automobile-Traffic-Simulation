// Standard
#include "algorithms.h"
#include "direction.h"
#include <time.h>

// External
#include <stb/stb_image.h>

// Custom modules
#include <render.h>
#include <camera.h>
#include <map.h>
#include <road.h>
#include <cars.h>
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

bool paused = false;
bool isSaveMenuActive = false;
bool isLoadMenuActive = false;

GLFWwindow* window;

struct nk_glfw glfw = { 0 };
struct nk_context* context;

GLuint isCarLoc, projectionLoc, modelLoc;

GLdouble limitFPS = 1.0 / FPS;

GLdouble lastTime;
GLdouble deltaTime = 0, currTime = 0, endPauseTime = 0;
GLdouble timer;

#ifdef DEBUG

GLuint cellsVBO, cellsVAO;
GLfloat cellsVertices[(NUMBER_OF_LINES + 1) * NUMBER_OF_ROADS * NUMBER_OF_CELLS * 5 * 8];
bool dbgIsCellsInit = false;

#endif

GLvoid scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (yoffset < 0.0)
    {
        cameraPos[0] = 0.0;
        cameraPos[1] = 0.0;
    }

    cameraFOV -= (float)yoffset;

    if (cameraFOV < 1.0f)
    {
        cameraFOV = 1.0f;
    }

    if (cameraFOV > DEFAULT_FOV)
    {
        cameraFOV = DEFAULT_FOV;
    }
}


GLvoid framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    WINDOW_HEIGHT = height;
    WINDOW_WIDTH = width;

    glViewport(0, 0, width, height);
}

GLvoid keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        if (isSaveMenuActive)
        {
            isSaveMenuActive = false;
        }
        else if (isLoadMenuActive)
        {
            isLoadMenuActive = false;
        }
        else
        {
            paused = !paused;
        }
    }

    if (key == GLFW_KEY_F6 && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

GLvoid processKeyboardInput() {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        moveCamera(UP);
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        moveCamera(DOWN);
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        moveCamera(LEFT);
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        moveCamera(RIGHT);
    }
}


GLvoid initRoads()
{
    glGenVertexArrays(1, &roadVAO);
    glGenBuffers(1, &roadVBO);
    glGenBuffers(1, &roadEBO);
    glBindVertexArray(roadVAO);

    glBindBuffer(GL_ARRAY_BUFFER, roadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * NUMBER_OF_ROADS * 4 * 5, roadVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, roadEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLint) * 6 * NUMBER_OF_ROADS, roadIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


GLvoid initLines()
{
    glGenVertexArrays(1, &lineVAO);
    glGenBuffers(1, &lineVBO);
    glBindVertexArray(lineVAO);

    glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * NUMBER_OF_LINES * NUMBER_OF_ROADS * 5 * 2, lineVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


GLvoid initCars()
{
    glGenBuffers(1, &carInstanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, carInstanceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(mat3) * MAX_CARS, &carTransformMatrixes[0], GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenVertexArrays(1, &carVAO);
    glGenBuffers(1, &carVBO);
    glGenBuffers(1, &carEBO);
    glBindVertexArray(carVAO);

    glBindBuffer(GL_ARRAY_BUFFER, carVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * MAX_CARS * 4 * 2, carVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, carEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLint) * MAX_CARS * 6, carIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, carInstanceVBO);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(mat3), (GLvoid*)(sizeof(float) * 0));
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(mat3), (GLvoid*)(sizeof(float) * 3));
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(mat3), (GLvoid*)(sizeof(float) * 6));
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);
    glVertexAttribDivisor(2, 1);
    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


GLvoid render()
{
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

    for (int i = 0; i < MAX_CARS; i++)
    {
        if (cars[i].isActive)
        {
            moveCarOnScreen(i);
        }
    }

    glBindBuffer(GL_ARRAY_BUFFER, carInstanceVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(carTransformMatrixes), carTransformMatrixes);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, carEBO);
    glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, MAX_CARS);
}


GLvoid quit()
{
    glDeleteVertexArrays(1, &roadVAO);
    glDeleteBuffers(1, &roadVBO);
    glDeleteBuffers(1, &roadEBO);
    glDeleteProgram(shaderProgram);

    nk_glfw3_shutdown(&glfw);
    glfwTerminate();
    exit(0);
}


GLdouble getPauseTime()
{
    GLdouble pauseTime = endPauseTime - lastTime;

    if (pauseTime > 0)
    {
        return pauseTime;
    }
    else
    {
        return 0;
    }
}


GLvoid initGL()
{
    srand(time(NULL));

    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_RESIZABLE, false);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, NULL, NULL);
    glfwMakeContextCurrent(window);


    int iconWidth, iconHeight;
    int channels;
    unsigned char* pixels = stbi_load("../resources/icons/icon.png", &iconWidth, &iconHeight, &channels, 4);

    GLFWimage images[1];
    images[0].width = iconWidth;
    images[0].height = iconHeight;
    images[0].pixels = pixels;

    glfwSetWindowIcon(window, 1, images);

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glEnable(GL_DEPTH_TEST);

    context = nk_glfw3_init(&glfw, window, NK_GLFW3_INSTALL_CALLBACKS);

    glfwSwapInterval(1);

    glfwSetKeyCallback(window, keyCallback);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetScrollCallback(window, scrollCallback);

}


GLvoid moveCarOnScreen(GLint carIndex)
{
    GLfloat screenVelocity = (GLfloat)cars[carIndex].velocity * cars[carIndex].dirOnRoad * VELOCITY_MULTIPLIER / FPS;
    cars[carIndex].realPos += screenVelocity;

    DIRECTION roadDir = roads[cars[carIndex].currCell.road].dir;

    if (roadDir == NORTH || roadDir == SOUTH)
    {
        glm_translate2d_y(carTransformMatrixes[carIndex], screenVelocity);
    }
    else
    {
        glm_translate2d_x(carTransformMatrixes[carIndex], screenVelocity);
    }
}

#ifdef DEBUG

GLvoid dbgInitCells() {
    glGenVertexArrays(1, &cellsVAO);
    glGenBuffers(1, &cellsVBO);
   
    glBindVertexArray(cellsVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cellsVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cellsVertices), cellsVertices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
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
    glDrawArrays(GL_LINES, 0, 2 * 5 * 8 * NUMBER_OF_CELLS * (NUMBER_OF_LINES + 1) * NUMBER_OF_ROADS);
}

#endif