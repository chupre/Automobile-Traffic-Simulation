#pragma once
bool __paused__ = false;

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void initRoads(GLuint* VAO, GLuint* VBO, GLuint* EBO, GLint* indices, GLfloat* vertices);
void initCars(GLuint* VAO, GLuint* VBO, GLuint* EBO, GLint* indices, GLfloat* vertices);
void initLines(GLuint* VAO, GLuint* VBO, GLfloat* vertices);

GLuint WINDOW_WIDTH = 800;
GLuint WINDOW_HEIGHT = 600;
GLchar WINDOW_NAME[] = "Auto Traffic Simulator";

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
        __paused__ = !__paused__;
    }

    if (key == GLFW_KEY_F6 && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}


void initRoads(GLuint* VAO, GLuint* VBO, GLuint* EBO, GLint* indices, GLfloat* vertices)
{
    glGenVertexArrays(1, VAO);
    glGenBuffers(1, VBO);
    glGenBuffers(1, EBO);
    glBindVertexArray(*VAO);

    glBindBuffer(GL_ARRAY_BUFFER, *VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * NUMBER_OF_ROADS * 4 * 3 * 2, vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLint) * 6 * NUMBER_OF_ROADS, indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


void initLines(GLuint* VAO, GLuint* VBO, GLfloat* vertices)
{
    glGenVertexArrays(1, VAO);
    glGenBuffers(1, VBO);
    glBindVertexArray(*VAO);

    glBindBuffer(GL_ARRAY_BUFFER, *VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * NUMBER_OF_LINES * NUMBER_OF_ROADS * 3 * 2 * 2, vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


void initCars(GLuint* VAO, GLuint* VBO, GLuint* EBO, GLint* indices, GLfloat* vertices)
{
    glGenVertexArrays(1, VAO);
    glGenBuffers(1, VBO);
    glGenBuffers(1, EBO);
    glBindVertexArray(*VAO);

    glBindBuffer(GL_ARRAY_BUFFER, *VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * MAX_CARS * 4 * 3 * 2, vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLint) * MAX_CARS * 6, indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

