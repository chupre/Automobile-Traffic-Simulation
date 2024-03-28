#define _CRT_SECURE_NO_WARNINGS

#include <glad/glad.h> 
#include <glfw/glfw3.h>
#include <cglm/cglm.h>

#define MAX_VERTEX_BUFFER 512 * 1024
#define MAX_ELEMENT_BUFFER 128 * 1024
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_GLFW_GL3_IMPLEMENTATION
#include <nuklear\nuklear.h>
#include <nuklear\nuklear_glfw_gl3.h>

#include <road.h>
#include <map.h>
#include <shader.h>
#include <cars.h>
#include <gl.h>
#include <gui.h>
#include <algorithms.h>


int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, NULL, NULL);
    struct nk_glfw glfw = { 0 };
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, keyCallback);

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    GLuint shaderProgram;
    genShader(&shaderProgram);
    
    GLfloat roadVertices[NUMBER_OF_ROADS * 4 * 3 * 2];
    GLfloat lineVertices[NUMBER_OF_LINES * NUMBER_OF_ROADS * 3 * 2 * 2];
    GLint roadIndices[NUMBER_OF_ROADS * 6];

    road roads[NUMBER_OF_ROADS];

    setMap(roads, roadVertices, roadIndices, lineVertices);

    GLuint roadVertexArray, roadVertexBuffer, roadElementBuffer;
    initRoads(&roadVertexArray, &roadVertexBuffer, &roadElementBuffer, roadIndices, roadVertices);

    GLuint lineVertexArray, lineVertexBuffer;
    initLines(&lineVertexArray, &lineVertexBuffer, lineVertices);

    GLfloat carVertices[MAX_CARS * 4 * 3 * 2];
    GLint carIndices[MAX_CARS * 6];
    car cars[MAX_CARS];
    setCarsToDefault(cars);
    GLint freeCars = MAX_CARS;

    GLuint carVertexArray, carVertexBuffer, carElementBuffer;
    initCars(&carVertexArray, &carVertexBuffer, &carElementBuffer, carIndices, carVertices);
    
    clock_t previousTime = clock() / CLOCKS_PER_SEC;

    mat4 identityTrans, carTrans[MAX_CARS];
    glm_mat4_identity(identityTrans);
    glm_mat4_identity_array(carTrans, MAX_CARS);

    struct nk_context* context = nk_glfw3_init(&glfw, window, NK_GLFW3_INSTALL_CALLBACKS);
    initFont(&glfw, context);

    while (!glfwWindowShouldClose(window))
    {
        initGUI(&glfw, context);

        if (!paused)
        {
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

            //current velocities are just demo, car's transform vectors should be got from getCarTranslateVector()
            GLfloat velocities[] = { -0.001f, -0.0015f };
            for (int i = 0; i < MAX_CARS; i++)
            {
                if (cars[i].isActive)
                {
                    //glm_translate_y is just demo version, car transformations should be set by getCarTranslateVector()
                    glm_translate_y(carTrans[i], velocities[i]);
                    GLint currCarIndices[6];
                    memcpy(currCarIndices, &carIndices[i * 6], sizeof(GLint) * 6);
                    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, carElementBuffer);
                    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(currCarIndices), currCarIndices);
                    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, carTrans[i]);
                    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
                }
            }

            int currTime = (clock() - previousTime) / CLOCKS_PER_SEC;

            if (currTime - previousTime == STEP_TIME)
            {
                previousTime = currTime;
                printf("Step: %d seconds\n", currTime);
                step(cars);
            }
        }

        nk_glfw3_new_frame(&glfw);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &roadVertexArray);
    glDeleteBuffers(1, &roadVertexBuffer);
    glDeleteBuffers(1, &roadElementBuffer);
    glDeleteProgram(shaderProgram);

    nk_glfw3_shutdown(&glfw);
    glfwTerminate();
    return 0;
}


