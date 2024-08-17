#define _CRT_SECURE_NO_WARNINGS

// External
#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#define NK_ASSERT(a)
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
#include <nuklear/nuklear.h>
#include <nuklear/nuklear_glfw_gl3.h>
#include <stb/stb_image.h>

// Custom modules
#include <shader.h>
#include <render.h>
#include <road.h>
#include <map.h>
#include <cars.h>
#include <gui.h>
#include <algorithms.h>

int main()
{
    initGL();
    genShader();

    setMap();
    setCarsToDefault();

    initRoads();
    initLines();
    initCars();

    RLC crushRLC = {0, 3, 2};

    for (int k = 19; k < NUMBER_OF_CELLS; k+=6)
    {
        crushRLC.cell = k;
        for (int i = 0; i <= NUMBER_OF_LINES; i++)
        {
            crushRLC.line = i;
            addCrushedCar(crushRLC);
        }
    }

        

    
    // if (roads[crushRLC.road].lines[crushRLC.line].cells[crushRLC.cell]->isCrushed == true)
    // {
    //     printf("crushed car is bound with the cell\n");
    // }






    initFont();

    glfwSetTime(0.0f);
    lastTime = glfwGetTime();
    timer = lastTime;

    while (!glfwWindowShouldClose(window))
    {
        initGUI();

        if (!paused)
        {
            glfwSetTime(glfwGetTime() - getPauseTime());

            currTime = glfwGetTime();
            deltaTime += (currTime - lastTime) / limitFPS;
            lastTime = currTime;

            endPauseTime = 0;

            while (deltaTime >= 1.0)
            {
                update();
                deltaTime--;
            }

            render();
        }
        else
        {
            endPauseTime = glfwGetTime();
        }

        nk_glfw3_new_frame(&glfw);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    quit();
}


