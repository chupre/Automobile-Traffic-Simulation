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
#include <style.c>

#include <time.h>

#include <serviceMacros.h> 

#include <road.h>
#include <map.h>
#include <shader.h>
#include <cars.h>
#include <gl.h>
#include <gui.h>

#include <algorithms.h>


int main()
{
    printf("clock at begin of main: %d\n", clock());

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

    GLuint carVertexArray, carVertexBuffer, carElementBuffer;
    initCars(&carVertexArray, &carVertexBuffer, &carElementBuffer, carIndices, carVertices);
    
    printf("clock before defining previousTime: %d\n\n", clock());
    clock_t previousTime = clock() / CLOCKS_PER_SEC;

    mat4 identityTrans, carTrans[MAX_CARS];
    glm_mat4_identity(identityTrans);
    glm_mat4_identity_array(carTrans, MAX_CARS);

    struct nk_context* context = nk_glfw3_init(&glfw, window, NK_GLFW3_INSTALL_CALLBACKS);
    initFont(&glfw, context);
    
    GLint k = 0;
    GLint clockTime;

    GLint restOfSubSteps = NUMBER_OF_SUB_STEPS;
    GLint timeOfNewStep = 0;
    GLint middleTimeOfSubStep = 0;

    while (!glfwWindowShouldClose(window))
    {
        initGUI(&glfw, context);

        if (!__paused__)
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

            if (__freeCars__)
            {
                int counter = __freeCars__;
                for (int i = 0; i < counter; i++)
                {
                    RLC freeSpotRLC = { EMPTY, EMPTY, EMPTY };
                    getFreeSpotAddress(roads, &freeSpotRLC);
                    GLint carIndex = getFreeCarIndex(cars);

                    if (freeSpotRLC.road != EMPTY)
                    {
                        setCar(roads, &cars[carIndex], carIndex, freeSpotRLC, carVertices, carIndices);
                        glBindBuffer(GL_ARRAY_BUFFER, carVertexBuffer);
                        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(carVertices), carVertices);
                        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, carElementBuffer);
                        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(carIndices), carIndices);
                        --__freeCars__;

                        printf("settled  car on r %d, l %d, c %d, dirOnRoad %d\n", freeSpotRLC.road, freeSpotRLC.line, freeSpotRLC.cell, cars[carIndex].dirOnRoad);
                    }
                }
            }

            if (clock() >= middleTimeOfSubStep)
            {
                if (restOfSubSteps > 0)
                {
                    //printf("loop enter time: %d\t|", clock());

                    for (int i = 0; i < MAX_CARS; i++)
                    {
                        if (cars[i].isActive)
                        {
                            //printf("\nk %d\n", ++k);
                            glm_translate_y(carTrans[i], getAdaptedCarVelocity(&cars[i]));
                            GLint currCarIndices[6];
                            memcpy(currCarIndices, &carIndices[i * 6], sizeof(GLint) * 6);
                            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, carElementBuffer);
                            glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(currCarIndices), currCarIndices);
                            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, carTrans[i]);
                            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
                        }
                    }

                    //printf("new middleTime : %d\t|restOfSubSteps %d\n", middleTimeOfSubStep, restOfSubSteps);

                    --restOfSubSteps;

                    middleTimeOfSubStep = clock() + SUB_STEP_TIME;
                }
                else
                {
                    printf("_%d_STEPTIME__:  %d\n\n", ++k, clock());

                    step(cars, roads);

                    restOfSubSteps = NUMBER_OF_SUB_STEPS;

                    middleTimeOfSubStep = clock() + SUB_STEP_TIME;
                }
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


