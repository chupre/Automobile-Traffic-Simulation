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

#define crushhh 1
//#define crushh 2

int main()
{
    initGL();
    genShader();

    setMap();
    setCarsToDefault();

    initRoads();
    initLines();
    initCars();



#ifdef crushhh
    RLC crushRLC = {0, 5, 17};
    addCrushedCar(crushRLC);

    // RLC crushRLC2 = {0, 2, 17};
    // addCrushedCar(crushRLC2);

    // RLC crushRLC3 = {0, 8, 17};
    // addCrushedCar(crushRLC3);
 #endif


 #ifdef crushh
    RLC crushRLC0 = {0, 0, 29};
    addCrushedCar(crushRLC0);
    RLC crushRLC1 = {0, 1, 29};
    addCrushedCar(crushRLC1);
    RLC crushRLC2 = {0, 2, 29};
    addCrushedCar(crushRLC2);
    RLC crushRLC3 = {0, 3, 29};
    addCrushedCar(crushRLC3);
    RLC crushRLC4 = {0, 4, 29};
    addCrushedCar(crushRLC4);
    // RLC crushRLC5 = {0, 5, 29};
    // addCrushedCar(crushRLC5);
    RLC crushRLC6 = {0, 7, 29};
    addCrushedCar(crushRLC6);
    RLC crushRLC7 = {0, 8, 29};
    addCrushedCar(crushRLC7);
    RLC crushRLC8 = {0, 9, 29};
    addCrushedCar(crushRLC8);
 #endif      

    // for (int k = 19; k < NUMBER_OF_CELLS; k+=6)
    // {
    //     crushRLC.cell = k;
    //     for (int i = 0; i <= NUMBER_OF_LINES; i++)
    //     {
    //         crushRLC.line = i;
    //         addCrushedCar(crushRLC);
    //     }
    // }

        

    
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
            printf("freeCars: %d \r", freeCars);
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


