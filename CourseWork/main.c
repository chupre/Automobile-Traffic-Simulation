#define _CRT_SECURE_NO_WARNINGS
#include <glad/glad.h> 
#include <glfw/glfw3.h>
#include <cglm/cglm.h>
#include <road.h>
#include <shader.h>
#include <cars.h>
#include <map.h>
#include <gl.h>

GLuint WINDOW_WIDTH = 800;
GLuint WINDOW_HEIGHT = 600;
GLchar WINDOW_NAME[] = "Auto Traffic Simulator";

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, NULL, NULL);
    glfwMakeContextCurrent(window);

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    GLuint shaderProgram;
    genShader(&shaderProgram);
    
    GLfloat roadVertices[NUMBER_OF_ROADS * 4 * 3 * 2];
    GLfloat lineVertices[NUMBER_OF_LINES * NUMBER_OF_ROADS * 3 * 2 * 2];
    GLint roadIndices[NUMBER_OF_ROADS * 6];

    road roads[NUMBER_OF_ROADS];

    setMap(roads, roadVertices, roadIndices, lineVertices);

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
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLint) * 6, (GLvoid*)0, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    clock_t before = clock() / CLOCKS_PER_SEC;

    mat4 identityTrans, carTrans[MAX_CARS];
    glm_mat4_identity(identityTrans);
    glm_mat4_identity_array(carTrans, MAX_CARS);

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

        //current velocities are just demo, car's velocities should be got from getCarRealVelocity()
        GLfloat velocities[] = { -0.001f, -0.0015f };
        for (int i = 0; i < MAX_CARS; i++)
        {
            if (cars[i].isActive)
            {
                //glm_translate_y is just demo version, car transformations should be set by getCarTransformations()
                glm_translate_y(carTrans[i], velocities[i]);
                GLint currCarIndices[6];
                memcpy(currCarIndices, &carIndices[i * 6], sizeof(GLint) * 6);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, carElementBuffer);
                glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(currCarIndices), currCarIndices);
                glUniformMatrix4fv(transformLoc, 1, GL_FALSE, carTrans[i]);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            }
        }

        int sec = (clock() - before) / CLOCKS_PER_SEC;

        if (sec - before == STEP_TIME)
        {
            before = sec;
            printf("Step: %d seconds\n", sec);
            step(cars);
        }

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


