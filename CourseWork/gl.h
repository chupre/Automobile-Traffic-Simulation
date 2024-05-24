#pragma once

bool paused = false;

GLuint WINDOW_WIDTH = 800;
GLuint WINDOW_HEIGHT = 600;
GLchar WINDOW_NAME[] = "Auto Traffic Simulator";

GLFWwindow* window;

struct nk_glfw glfw = { 0 };
struct nk_context* context;

GLuint shaderProgram;
GLuint transformLoc;

GLfloat roadVertices[NUMBER_OF_ROADS * 4 * 3 * 2];
GLfloat lineVertices[NUMBER_OF_LINES * NUMBER_OF_ROADS * 3 * 2 * 2];
GLfloat carVertices[MAX_CARS * 4 * 3 * 2];

GLint roadIndices[NUMBER_OF_ROADS * 6];
GLint carIndices[MAX_CARS * 6];
GLint currCarIndices[6];

road roads[NUMBER_OF_ROADS];
car cars[MAX_CARS];

GLuint roadVAO, roadVBO, roadEBO;
GLuint lineVAO, lineVBO;
GLuint carVAO, carVBO, carEBO;

mat4 identityTrans, carTrans[MAX_CARS];

GLdouble limitFPS = 1.0 / FPS;

GLdouble lastTime;
GLdouble deltaTime = 0, currTime = 0;
GLdouble timer;

GLint frames = 0, updates = 0;
GLint framesPerSec = 0, updatesPerSec = 0;

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void initRoads();
void initLines();
void initCars();
void render();
void update();
void quit();
void showFPS();

//Definiton is in algorithms.h
GLvoid step();

//Definition is in cars.h 
GLvoid setCar(car* Car, GLint carIndex, RLC address);
GLvoid getFreeSpotAddress(RLC* address);


void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    WINDOW_HEIGHT = height;
    WINDOW_WIDTH = width;
    glViewport(0, 0, width, height);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        paused = !paused;
    }

    if (key == GLFW_KEY_F6 && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}


void initRoads()
{
    glGenVertexArrays(1, &roadVAO);
    glGenBuffers(1, &roadVBO);
    glGenBuffers(1, &roadEBO);
    glBindVertexArray(roadVAO);

    glBindBuffer(GL_ARRAY_BUFFER, roadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * NUMBER_OF_ROADS * 4 * 3 * 2, roadVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, roadEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLint) * 6 * NUMBER_OF_ROADS, roadIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


void initLines()
{
    glGenVertexArrays(1, &lineVAO);
    glGenBuffers(1, &lineVBO);
    glBindVertexArray(lineVAO);

    glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * NUMBER_OF_LINES * NUMBER_OF_ROADS * 3 * 2 * 2, lineVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


void initCars()
{
    glGenVertexArrays(1, &carVAO);
    glGenBuffers(1, &carVBO);
    glGenBuffers(1, &carEBO);
    glBindVertexArray(carVAO);

    glBindBuffer(GL_ARRAY_BUFFER, carVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * MAX_CARS * 4 * 3 * 2, roadVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, carEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLint) * MAX_CARS * 6, carIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


void render()
{
    glClearColor(0.28f, 0.55f, 0.24f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);

    GLuint transformLoc = glGetUniformLocation(shaderProgram, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, identityTrans);

    glBindVertexArray(roadVAO);
    glDrawElements(GL_TRIANGLES, NUMBER_OF_ROADS * 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(lineVAO);
    glDrawArrays(GL_LINES, 0, NUMBER_OF_LINES * NUMBER_OF_ROADS * 2);
    glBindVertexArray(carVAO);

    for (int i = 0; i < MAX_CARS; i++)
    {
        if (cars[i].isActive)
        {
            GLfloat screenVelocity = (GLfloat)cars[i].velocity * cars[i].dirOnRoad * VELOCITY_MULTIPLIER / 1000;
            cars[i].realPos += screenVelocity;
            glm_translate_y(carTrans[i], screenVelocity);
            GLint currCarIndices[6];
            memcpy(currCarIndices, &carIndices[i * 6], sizeof(GLint) * 6);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, carEBO);
            glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(currCarIndices), currCarIndices);
            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, carTrans[i]);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }
    }

    frames++;
}


void update()
{
    if (freeCars)
    {
        int counter = freeCars;
        for (int i = 0; i < counter; i++)
        {
            RLC freeSpotRLC = { EMPTY, EMPTY, EMPTY };
            getFreeSpotAddress(&freeSpotRLC);
            GLint carIndex = getFreeCarIndex();

            if (freeSpotRLC.road != EMPTY)
            {
                setCar(&cars[carIndex], carIndex, freeSpotRLC);
                printf("car %d spawned:\n", carIndex);
                printf("vertices: %f %f %f %f\n", carVertices[carIndex * 24], carVertices[carIndex * 24 + 6], carVertices[carIndex * 24 + 1], carVertices[carIndex * 24 + 7]);
                printf("direction: %d\n", cars[carIndex].dirOnRoad);
                printf("RLC: Road: %d, Line: %d, Cell: %d\n", cars[carIndex].currCell.road, cars[carIndex].currCell.line, cars[carIndex].currCell.cell);
                printf("real position: %f\n", cars[carIndex].realPos);
                printf("velocity: %d\n\n", cars[carIndex].velocity);
                glm_mat4_print(carTrans[carIndex], stdout);
                glBindBuffer(GL_ARRAY_BUFFER, carVBO);
                glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(carVertices), carVertices);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, carEBO);
                glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(carIndices), carIndices);
                --freeCars;
            }
        }
    }

    if (glfwGetTime() - timer > STEP_TIME)
    {
        printf("Step at time: %lf\n\n", glfwGetTime());

        timer += STEP_TIME;

        framesPerSec = frames;
        updatesPerSec = updates;
        frames = 0;
        updates = 0;

        step();
    }

    updates++;
}


void quit()
{
    glDeleteVertexArrays(1, &roadVAO);
    glDeleteBuffers(1, &roadVBO);
    glDeleteBuffers(1, &roadEBO);
    glDeleteProgram(shaderProgram);

    nk_glfw3_shutdown(&glfw);
    glfwTerminate();
    exit(0);
}


void showFPS()
{
    printf("FPS: %d      Updates: %d\r", framesPerSec, updatesPerSec);
}