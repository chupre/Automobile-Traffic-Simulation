#define _CRT_SECURE_NO_WARNINGS
#include <glad/glad.h> 
#include <glfw/glfw3.h>
#include <cglm/cglm.h>
#include <road.h>
#include <shader.h>
#include <cars.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

GLuint WINDOW_WIDTH = 800;
GLuint WINDOW_HEIGHT = 600;
GLchar WINDOW_NAME[] = "Simulation of automobile traffic BETA";
GLuint carID = 0;

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
    
    GLfloat roadVertices[NUMBER_OF_ROADS * 4 * 3];
    GLint roadIndices[NUMBER_OF_ROADS * 6];
    road roads[NUMBER_OF_ROADS];

    setRoad(&roads[0], 0, roadVertices, roadIndices, -0.2f, -1.0f, 2.0f, NORTH);
    //setRoad(&roads[1], 1, roadVertices, roadIndices, 0.2f, 1.0f, 2.0f, SOUTH);

    GLuint roadVertexArray, roadVertexBuffer, roadElementBuffer;
    glGenVertexArrays(1, &roadVertexArray);
    glGenBuffers(1, &roadVertexBuffer);
    glGenBuffers(1, &roadElementBuffer);
    glBindVertexArray(roadVertexArray);

    glBindBuffer(GL_ARRAY_BUFFER, roadVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(roadVertices), roadVertices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, roadElementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(roadIndices), roadIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    GLfloat lineVertices[NUMBER_OF_LINES * NUMBER_OF_ROADS * 3 * 2];

    for (int i = 0; i < NUMBER_OF_ROADS; i++)
    {
        setLine(&roads[i], i, roadVertices, lineVertices);
    }

    GLuint lineVertexArray, lineVertexBuffer;
    glGenVertexArrays(1, &lineVertexArray);
    glGenBuffers(1, &lineVertexBuffer);
    glBindVertexArray(lineVertexArray);

    glBindBuffer(GL_ARRAY_BUFFER, lineVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(lineVertices), lineVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    GLfloat carVertices[MAX_CARS * 4 * 3];
    GLint carIndices[MAX_CARS * 6];
    car cars[MAX_CARS];
    setCarsToDefault(cars);
    GLint freeCars = MAX_CARS;

    GLuint carVertexArray, carVertexBuffer, carElementBuffer;
    glGenVertexArrays(1, &carVertexArray);
    glGenBuffers(1, &carVertexBuffer);
    glGenBuffers(1, &carElementBuffer);
    glBindVertexArray(carVertexArray);

    glBindBuffer(GL_ARRAY_BUFFER, carVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(carVertices), carVertices, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, carElementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(carIndices), carIndices, GL_DYNAMIC_DRAW);

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
        int vertexColorLocation = glGetUniformLocation(shaderProgram, "color");
        glUniform4f(vertexColorLocation, 0.31f, 0.31f, 0.31f, 1.0f);
        glBindVertexArray(roadVertexArray);
        glDrawElements(GL_TRIANGLES, NUMBER_OF_ROADS * 6, GL_UNSIGNED_INT, 0);
        glUniform4f(vertexColorLocation, 1.0f, 1.0f, 1.0f, 1.0f);
        glBindVertexArray(lineVertexArray);
        glDrawArrays(GL_LINES, 0, NUMBER_OF_LINES * NUMBER_OF_ROADS * 2);

        glBindVertexArray(carVertexArray);
        //roads[0].lines[0].cells[0] = 3;
        if (freeCars)
        {
            int counter = freeCars;
            for (int i = 0; i < counter; i++)
            {
                GLint lineIndex, roadIndex;
                GLint carIndex = getFreeCarIndex(cars);
                bool freeSpot = getFreeSpotAddress(roads, &lineIndex, &roadIndex);

                if (freeSpot)
                {
                    setCar(&roads[roadIndex], carID, &cars[carIndex], carIndex, lineIndex, carVertices, carIndices);
                    glBindBuffer(GL_ARRAY_BUFFER, carVertexBuffer);
                    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(carVertices), carVertices);
                    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, carElementBuffer);
                    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(carIndices), carIndices);
                    freeCars--;
                    carID++;
                }
            }
        }

        //car functions
        //step()
        glUniform4f(vertexColorLocation, 1.0f, 0.0f, 0.0f, 1.0f);
        glDrawElements(GL_TRIANGLES, MAX_CARS * 6, GL_UNSIGNED_INT, 0);
        //sleep (?) -> animation

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &roadVertexArray);
    glDeleteBuffers(1, &roadVertexBuffer);
    glDeleteBuffers(1, &roadElementBuffer);
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

