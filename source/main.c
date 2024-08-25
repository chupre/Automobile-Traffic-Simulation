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
#include <cross.h>
#include <map.h>
#include <cars.h>
#include <gui.h>
#include <algorithms.h>
#include <search_tools.h>

// #define crushhhh3
#define dot_debug 1

int main()
{
    initGL();
    genShader();

    setMap();
    setCarsToDefault();

    initRoads();
    initLines();
    initCars();

    roads[0].isEdge = true;

#ifdef dot_debug
    dot_coord _dot;
    _dot.x = -0.8f;
    _dot.y = -0.55f;
    RLC _rlc_;
    if (getRLCbyDot(&_rlc_, &_dot))
    {
        printRLC(_rlc_, "_rlc_");
    }
    else printf("none\n");
    // RLC crRLC = {0, 1, 2};
    // addCrushedCar(crRLC);

#endif
    

#ifdef crushhhh3
    // RLC crushRLC = {0, 19, 20};
    // addCrushedCar(crushRLC);

    // RLC crushRLC2 = {0, 18, 19};
    // addCrushedCar(crushRLC2);

    // RLC crushRLC3 = {0, 17, 18};
    // addCrushedCar(crushRLC3);

    //  RLC crushRLC4 = {0, 16, 13};
    // addCrushedCar(crushRLC4);

    //  RLC crushRLC5 = {0, 15, 14};
    // addCrushedCar(crushRLC5);

    //  RLC crushRLC6 = {0, 14, 15};
    // addCrushedCar(crushRLC6);

    //  RLC crushRLC7 = {0, 13, 16};
    // addCrushedCar(crushRLC7);

    // RLC crushRLC8 = {0, 12, 17};
    // addCrushedCar(crushRLC8);

    //  RLC crushRLC9 = {0, 11, 18};
    // addCrushedCar(crushRLC9);

    //  RLC crushRLC10 = {0, 10, 19};
    // addCrushedCar(crushRLC10);

    // RLC crushRLC11 = {0, 9, 20};
    // addCrushedCar(crushRLC11);

    for (int line = 10, cell = 19; line < 13 ; line++, cell--)
    {
        RLC cr = {0, line, cell};
        addCrushedCar(cr);
    }


    for (int line = 20, cell = 17; line < 23 ; line++, cell++)
    {
        RLC cr = {0, line, cell};
        addCrushedCar(cr);
    }


#endif


    //setOccupingCarProperties();
    occupying_car.velocity = _0_CELL_;
    occupying_car.isActive = true;

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
            //printf("\rfreeCars: %d ", freeCars);
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


