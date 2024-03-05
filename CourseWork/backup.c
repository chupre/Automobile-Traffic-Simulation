//#define _CRT_SECURE_NO_WARNINGS
//#include <glad/glad.h> 
//#include <glfw/glfw3.h>
//#include <cglm/cglm.h>
//#include <stdio.h>
//#include <stdbool.h>
//#include <string.h>
//#define NUMBER_OF_ROADS 2
//#define NUMBER_OF_LINES 2
//#define DEFAULT_CELLS_LENGHT 16
//#define ROAD_WIDTH 0.1f
//
//enum direction { NORTH, SOUTH, EAST, WEST };
//
//typedef struct
//{
//    int velocity;
//    enum direction target;
//} car;
//
//typedef struct
//{
//    int* cells[DEFAULT_CELLS_LENGHT];
//    bool isEdge;
//} line;
//
//typedef struct
//{
//    enum direction dir;
//    line lines[NUMBER_OF_LINES];
//} road;
//
//void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//void processInput(GLFWwindow* window);
//void setRoad(road* Road, GLint roadIndex, GLfloat* roadVerticies, GLint* roadIndeces, GLfloat start_x, GLfloat start_y, GLfloat lenght, enum direction dir);
//char* getShaderContent(const GLchar* fileName);
//
//GLuint WINDOW_WIDTH = 800;
//GLuint WINDOW_HEIGHT = 600;
//GLchar WINDOW_NAME[] = "Simulation of automobile traffic BETA";
//
//int main()
//{
//    const GLchar* vertexShaderSource = getShaderContent("vertex_shader.glsl");
//    const GLchar* fragmentShaderSource = getShaderContent("fragment_shader.glsl");
//
//    glfwInit();
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, NULL, NULL);
//    glfwMakeContextCurrent(window);
//
//    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
//    glViewport(0, 0, 800, 600);
//    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
//
//    GLuint vertexShader;
//    vertexShader = glCreateShader(GL_VERTEX_SHADER);
//    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
//    glCompileShader(vertexShader);
//
//
//    GLuint fragmentShader;
//    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
//    glCompileShader(fragmentShader);
//
//    GLuint shaderProgram;
//    shaderProgram = glCreateProgram();
//    glAttachShader(shaderProgram, vertexShader);
//    glAttachShader(shaderProgram, fragmentShader);
//    glLinkProgram(shaderProgram);
//    glDeleteShader(vertexShader);
//    glDeleteShader(fragmentShader);
//
//    GLfloat roadVerticies[NUMBER_OF_ROADS * 4 * 3];
//    GLint roadIndices[NUMBER_OF_ROADS * 6];
//
//    road roads[NUMBER_OF_ROADS];
//    setRoad(&roads[0], 0, roadVerticies, roadIndices, -0.2f, -1.0f, 2.0f, NORTH);
//    setRoad(&roads[1], 1, roadVerticies, roadIndices, 0.2f, 1.0f, 2.0f, SOUTH);
//
//    GLuint RoadVertexArray, RoadVertexBuffer, RoadElementBuffer;
//    glGenVertexArrays(1, &RoadVertexArray);
//    glGenBuffers(1, &RoadVertexBuffer);
//    glGenBuffers(1, &RoadElementBuffer);
//    glBindVertexArray(RoadVertexArray);
//
//    glBindBuffer(GL_ARRAY_BUFFER, RoadVertexBuffer);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(roadVerticies), roadVerticies, GL_STATIC_DRAW);
//
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, RoadElementBuffer);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(roadIndices), roadIndices, GL_STATIC_DRAW);
//
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
//    glEnableVertexAttribArray(0);
//
//    glBindBuffer(GL_ARRAY_BUFFER, 0);
//    glBindVertexArray(0);
//
//    while (!glfwWindowShouldClose(window))
//    {
//        processInput(window);
//
//        glClearColor(0.28f, 0.55f, 0.24f, 1.0f);
//        glClear(GL_COLOR_BUFFER_BIT);
//
//        glUseProgram(shaderProgram);
//        glBindVertexArray(RoadVertexArray);
//        glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
//
//        glfwSwapBuffers(window);
//        glfwPollEvents();
//    }
//
//    glDeleteVertexArrays(1, &RoadVertexArray);
//    glDeleteBuffers(1, &RoadVertexBuffer);
//    glDeleteBuffers(1, &RoadElementBuffer);
//    glDeleteProgram(shaderProgram);
//
//    glfwTerminate();
//    return 0;
//}
//
//void framebuffer_size_callback(GLFWwindow* window, int width, int height)
//{
//    glViewport(0, 0, width, height);
//}
//
//
//void processInput(GLFWwindow* window)
//{
//    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//        glfwSetWindowShouldClose(window, true);
//}
//
//
//void setRoad(road* Road, GLint roadIndex, GLfloat* roadVerticies, GLint* roadIndices, GLfloat start_x, GLfloat start_y, GLfloat lenght, enum direction dir)
//{
//    (*Road).dir = dir;
//
//    int indeces[] =
//    {
//        0 + (roadIndex * 4), 2 + (roadIndex * 4), 3 + (roadIndex * 4),
//        0 + (roadIndex * 4), 1 + (roadIndex * 4), 3 + (roadIndex * 4)
//    };
//
//    memcpy(&roadIndices[roadIndex * 6], indeces, sizeof(GLint) * 6);
//
//    if (dir == NORTH)
//    {
//        float verticies[] =
//        {
//            start_x + ROAD_WIDTH, start_y, 0.0f,
//            start_x - ROAD_WIDTH, start_y, 0.0f,
//            start_x + ROAD_WIDTH, start_y + lenght, 0.0f,
//            start_x - ROAD_WIDTH, start_y + lenght, 0.0f
//        };
//
//        memcpy(&roadVerticies[roadIndex * 3 * 4], verticies, sizeof(GLfloat) * 4 * 3);
//    }
//
//    else if (dir == SOUTH)
//    {
//        float verticies[] =
//        {
//            start_x + ROAD_WIDTH, start_y, 0.0f,
//            start_x - ROAD_WIDTH, start_y, 0.0f,
//            start_x + ROAD_WIDTH, start_y - lenght, 0.0f,
//            start_x - ROAD_WIDTH, start_y - lenght, 0.0f
//        };
//
//        memcpy(&roadVerticies[roadIndex * 3 * 4], verticies, sizeof(GLfloat) * 4 * 3);
//    }
//
//    else if (dir == EAST)
//    {
//        float verticies[] =
//        {
//            start_x, start_y + ROAD_WIDTH, 0.0f,
//            start_x, start_y - ROAD_WIDTH, 0.0f,
//            start_x + lenght, start_y + ROAD_WIDTH, 0.0f,
//            start_x + lenght, start_y - ROAD_WIDTH, 0.0f
//        };
//
//        memcpy(&roadVerticies[roadIndex * 3 * 4], verticies, sizeof(GLfloat) * 4 * 3);
//    }
//
//    else if (dir == WEST)
//    {
//        float verticies[] =
//        {
//            start_x, start_y + ROAD_WIDTH, 0.0f,
//            start_x, start_y - ROAD_WIDTH, 0.0f,
//            start_x - lenght, start_y + ROAD_WIDTH, 0.0f,
//            start_x - lenght, start_y - ROAD_WIDTH, 0.0f
//        };
//
//        memcpy(&roadVerticies[roadIndex * 3 * 4], verticies, sizeof(GLfloat) * 4 * 3);
//    }
//}
//
//char* getShaderContent(const GLchar* fileName)
//{
//    FILE* shaderFile;
//    long size = 0;
//    char* shaderContent;
//
//    shaderFile = fopen(fileName, "r");
//    fseek(shaderFile, 0, SEEK_END);
//    size = ftell(shaderFile);
//    rewind(shaderFile);
//
//    shaderContent = memset(malloc(size + 1), '\0', size + 1);
//    fread(shaderContent, 1, size, shaderFile);
//    fclose(shaderFile);
//
//    return shaderContent;
//}