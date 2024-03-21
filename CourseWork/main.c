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
    
    GLfloat roadVertices[NUMBER_OF_ROADS * 4 * 3 * 2];
    GLint roadIndices[NUMBER_OF_ROADS * 6];
    road roads[NUMBER_OF_ROADS];

    setRoad(&roads[0], 0, roadVertices, roadIndices, -0.2f, -1.0f, 2.0f, NORTH);
    //setRoad(&roads[1], 1, roadVertices, roadIndices, 0.2f, 1.0f, 2.0f, SOUTH);
    //setRoad(&roads[0], 0, roadVertices, roadIndices, -1.0f, 0.2f, 2.0f, EAST);
    //setRoad(&roads[1], 1, roadVertices, roadIndices, 1.0f, -0.2f, 2.0f, WEST);

    GLuint roadVertexArray, roadVertexBuffer, roadElementBuffer;
    glGenVertexArrays(1, &roadVertexArray);
    glGenBuffers(1, &roadVertexBuffer);
    glGenBuffers(1, &roadElementBuffer);
    glBindVertexArray(roadVertexArray);

    glBindBuffer(GL_ARRAY_BUFFER, roadVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(roadVertices), roadVertices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, roadElementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(roadIndices), roadIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    GLfloat lineVertices[NUMBER_OF_LINES * NUMBER_OF_ROADS * 3 * 2 * 2];

    for (int i = 0; i < NUMBER_OF_ROADS; i++)
    {
        setLines(&roads[i], i, roadVertices, lineVertices);
    }

    GLuint lineVertexArray, lineVertexBuffer;
    glGenVertexArrays(1, &lineVertexArray);
    glGenBuffers(1, &lineVertexBuffer);
    glBindVertexArray(lineVertexArray);

    glBindBuffer(GL_ARRAY_BUFFER, lineVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(lineVertices), lineVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    GLfloat carVertices[MAX_CARS * 4 * 3 * 2];
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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    clock_t before = clock() / CLOCKS_PER_SEC;

    mat4 carTrans;
    glm_mat4_identity(carTrans);
    mat4 identityTrans;
    glm_mat4_identity(identityTrans);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.28f, 0.55f, 0.24f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        
        GLuint transformLoc = glGetUniformLocation(shaderProgram, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, identityTrans);

        glBindVertexArray(roadVertexArray);
        glDrawElements(GL_TRIANGLES, NUMBER_OF_ROADS * 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(lineVertexArray); 
        glDrawArrays(GL_LINES, 0, NUMBER_OF_LINES * NUMBER_OF_ROADS * 2);
        glBindVertexArray(carVertexArray);
        
        glm_translate_y(carTrans, 0.0001f);
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, carTrans);

        if (freeCars)
        {
            int counter = freeCars;
            for (int i = 0; i < counter; i++)
            {
                RLC freeSpot;
                getFreeSpotAddress(roads, &freeSpot);
                GLint carIndex = getFreeCarIndex(cars);

                if (freeSpot.road != EMPTY)
                {
                    setCar(roads, &cars[carIndex], carIndex, freeSpot, carVertices, carIndices);
                    glBindBuffer(GL_ARRAY_BUFFER, carVertexBuffer);
                    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(carVertices), carVertices);
                    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, carElementBuffer);
                    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(carIndices), carIndices);
                    freeCars--;
                }
            }
        }

        int sec = (clock() - before) / CLOCKS_PER_SEC;

        if (sec - before == STEP_TIME)
        {
            before = sec;
            step(cars);
        }

        //TO-DO: 
        // 1. create transformation matrix array (set identity matrix by default)
        // 2. car EBO should store indices only for one car. We render cars separately and for each car we BufferSubData it's indices to EBO, apply transformation matrix to uniform and draw the car.

        glDrawElements(GL_TRIANGLES, MAX_CARS * 6, GL_UNSIGNED_INT, 0);

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

