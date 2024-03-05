#define _CRT_SECURE_NO_WARNINGS
#include <glad/glad.h> 
#include <glfw/glfw3.h>
#include <cglm/cglm.h>
#include <road.h>
#include <shader.h>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

GLuint WINDOW_WIDTH = 800;
GLuint WINDOW_HEIGHT = 600;
GLchar WINDOW_NAME[] = "Simulation of automobile traffic BETA";

int main()
{
    const GLchar* vertexShaderSource = getShaderContent("vertex_shader.glsl");
    const GLchar* fragmentShaderSource = getShaderContent("fragment_shader.glsl");

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, NULL, NULL);
    glfwMakeContextCurrent(window);

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    GLuint vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    

    GLuint fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    GLuint shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    GLfloat roadVerticies[NUMBER_OF_ROADS * 4 * 3];
    GLint roadIndices[NUMBER_OF_ROADS * 6];

    road roads[NUMBER_OF_ROADS];
    setRoad(&roads[0], 0, roadVerticies, roadIndices, -0.2f, -1.0f, 2.0f, NORTH);
    setRoad(&roads[1], 1, roadVerticies, roadIndices, 0.2f, 1.0f, 2.0f, SOUTH);

    GLuint RoadVertexArray, RoadVertexBuffer, RoadElementBuffer;
    glGenVertexArrays(1, &RoadVertexArray);
    glGenBuffers(1, &RoadVertexBuffer);
    glGenBuffers(1, &RoadElementBuffer);
    glBindVertexArray(RoadVertexArray);

    glBindBuffer(GL_ARRAY_BUFFER, RoadVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(roadVerticies), roadVerticies, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, RoadElementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(roadIndices), roadIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.28f, 0.55f, 0.24f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(RoadVertexArray);
        glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &RoadVertexArray);
    glDeleteBuffers(1, &RoadVertexBuffer);
    glDeleteBuffers(1, &RoadElementBuffer);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}


void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

