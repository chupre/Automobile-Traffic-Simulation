#pragma once

bool paused = false;
bool isSaveMenuActive = false;
bool isLoadMenuActive = false;

GLFWwindow* window;

struct nk_glfw glfw = { 0 };
struct nk_context* context;

GLuint isCarLoc, projectionLoc, modelLoc;

road roads[NUMBER_OF_ROADS];
GLuint roadVAO, roadVBO, roadEBO;
GLfloat roadVertices[NUMBER_OF_ROADS * 4 * 5];
GLint roadIndices[NUMBER_OF_ROADS * 6];

GLuint lineVAO, lineVBO;
GLfloat lineVertices[NUMBER_OF_LINES * NUMBER_OF_ROADS * 5 * 2];

car cars[MAX_CARS];
GLuint carVAO, carVBO, carEBO, carInstanceVBO;

GLfloat carVertices[4 * 2] =
{
    0.0f, 0.0f,
    CAR_WIDTH, 0.0f,
    0.0f, CAR_LENGHT,
    CAR_WIDTH, CAR_LENGHT,
};

GLint carIndices[6] =
{
    0, 2, 3,
    0, 1, 3
};

mat3 carTransformMatrixes[MAX_CARS];

GLdouble limitFPS = 1.0 / FPS;

GLdouble lastTime;
GLdouble deltaTime = 0, currTime = 0, endPauseTime = 0;
GLdouble timer;

GLint frames = 0, updates = 0;
GLint framesPerSec = 0, updatesPerSec = 0;

GLdouble getPauseTime();
GLvoid scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
GLvoid framebufferSizeCallback(GLFWwindow* window, int width, int height);
GLvoid keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
GLvoid initRoads();
GLvoid initLines();
GLvoid initCars();
GLvoid render();
GLvoid quit();
GLvoid showFPS();
GLvoid initOpenGL();


//Definiton is in algorithms.h
GLvoid step();
//Definition is in cars.h 
GLvoid setCar(car* Car, GLint carIndex, RLC address);
GLvoid getFreeSpotAddress(RLC* address);
GLvoid spawnCars();

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

    setProjection();
    setView();

    glBindVertexArray(roadVAO);
    glDrawElements(GL_TRIANGLES, NUMBER_OF_ROADS * 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(lineVAO);
    glDrawArrays(GL_LINES, 0, NUMBER_OF_LINES * NUMBER_OF_ROADS * 2);
    
    glUniform1i(isCarLoc, true);
    glBindVertexArray(carVAO);

    for (int i = 0; i < MAX_CARS; i++)
    {
        if (cars[i].isActive)
        {
            //speed adaptation
            GLfloat screenVelocity = (GLfloat)cars[i].velocity * cars[i].dirOnRoad * VELOCITY_MULTIPLIER / FPS;
            cars[i].realPos += screenVelocity;
            glm_translate2d_y(carTransformMatrixes[i], screenVelocity);
        }
    }

    glBindBuffer(GL_ARRAY_BUFFER, carInstanceVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(carTransformMatrixes), carTransformMatrixes);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, carEBO);
    glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, MAX_CARS);

    frames++;
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


GLvoid showFPS()
{
    printf("FPS: %d      Updates: %d\r", framesPerSec, updatesPerSec);
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


GLvoid initOpenGL()
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_RESIZABLE, FALSE);
    glfwWindowHint(GLFW_SCALE_TO_MONITOR, TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, NULL, NULL);
    glfwMakeContextCurrent(window);


    int iconWidth, iconHeight;
    int channels;
    unsigned char* pixels = stbi_load("resources/icons/icon.png", &iconWidth, &iconHeight, &channels, 4);

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


GLint isPossibleToMoveCam(camDir dir, GLfloat offset)
{
    int roadsOnEdge = 0, roadsOnCamBorder = 0;

    for (int i = 0; i < NUMBER_OF_ROADS; i++)
    {
        if (roads[i].isEdge)
        {
            roadsOnEdge++;

            vec4 testVertex = { 0.0, 0.0, 1.0, 1.0 };
            getTestVertex(i, dir, testVertex);

            if (isVertexOnCamBorders(testVertex, dir, offset))
            {
                roadsOnCamBorder++;
            }
        }
    }

    if (roadsOnCamBorder == roadsOnEdge)
    {
        return 1;
    }

    return 0;
}


GLvoid getTestVertex(GLint roadIndex, camDir dir, vec4 dest)
{
    if (dir == UP)
    {
        if (roads[roadIndex].dir == NORTH)
        {
            dest[0] = roadVertices[10 + roadIndex * 4 * 5];
            dest[1] = roadVertices[11 + roadIndex * 4 * 5];
        }

        if (roads[roadIndex].dir == SOUTH)
        {
            dest[0] = roadVertices[0 + roadIndex * 4 * 5];
            dest[1] = roadVertices[1 + roadIndex * 4 * 5];
        }
    }

    if (dir == DOWN)
    {
        if (roads[roadIndex].dir == NORTH)
        {
            dest[0] = roadVertices[0 + roadIndex * 4 * 5];
            dest[1] = roadVertices[1 + roadIndex * 4 * 5];
        }

        if (roads[roadIndex].dir == SOUTH)
        {
            dest[0] = roadVertices[10 + roadIndex * 4 * 5];
            dest[1] = roadVertices[11 + roadIndex * 4 * 5];
        }
    }

    if (dir == RIGHT)
    {
        if (roads[roadIndex].dir == WEST)
        {
            dest[0] = roadVertices[0 + roadIndex * 4 * 5];
            dest[1] = roadVertices[1 + roadIndex * 4 * 5];
        }

        if (roads[roadIndex].dir == EAST)
        {
            dest[0] = roadVertices[10 + roadIndex * 4 * 5];
            dest[1] = roadVertices[11 + roadIndex * 4 * 5];
        }
    }

    if (dir == LEFT)
    {
        if (roads[roadIndex].dir == WEST)
        {
            dest[0] = roadVertices[10 + roadIndex * 4 * 5];
            dest[1] = roadVertices[11 + roadIndex * 4 * 5];
        }

        if (roads[roadIndex].dir == EAST)
        {
            dest[0] = roadVertices[0 + roadIndex * 4 * 5];
            dest[1] = roadVertices[1 + roadIndex * 4 * 5];
        }
    }
}

