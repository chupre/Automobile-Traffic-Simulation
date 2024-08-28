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
#include <spawn_logs.h>

// #define crushhhh3 1
// #define dot_debug 1
#define crus 1

int main()
{
    initGL();
    genShader();

    setMap();
    setCarsToDefault();

    initRoads();
    initLines();
    initCars();

    // openFile();

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
#ifdef crus
    RLC cr = {0, 2, 10};
    addCrushedCar(cr);
    RLC cr1 = {0, 4, 12};
    addCrushedCar(cr1);
    RLC cr2 = {0, 6, 14};
    addCrushedCar(cr2);


    // for (int line = 2, cell = 10; line < 5 ; line++)
    // {
    //     RLC cr = {0, line, cell};
    //     addCrushedCar(cr);
    // }
#endif

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

    // closeFile();
    quit();
}


