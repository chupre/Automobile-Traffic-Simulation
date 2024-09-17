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
#include <macros.h>
#include <shader.h>
#include <render.h>
#include <camera.h>
#include <road.h>
#include <rlc.h>
#include <direction.h>
#include <cross.h>
#include <map.h>
#include <cars.h>
#include <gui.h>
#include <algorithms.h>
#include <search_tools.h>
#include <traffic_light.h>
#include <traffic_density.h>
#include <log.h>
#include <texture.h>

#include <sys/resource.h>

int main()
{
    openFile();

    const rlim_t kStackSize = 64L * 1024L * 1024L; 
    struct rlimit rl;
    int result;

    result = getrlimit(RLIMIT_STACK, &rl);
    if (result == 0)
    {
        if (rl.rlim_cur < kStackSize)
        {
            rl.rlim_cur = kStackSize;
            result = setrlimit(RLIMIT_STACK, &rl);
            if (result != 0)
            {
                fprintf(stderr, "setrlimit returned result = %d\n", result);
            }
        }
    }

    initGL();
    genShader();


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

    initFont();

    glfwSetTime(0.0f);
    lastTime = glfwGetTime();
    timer = lastTime;

    while (!glfwWindowShouldClose(window))
    {
        if (paused || !isInit){
            initGUI();
        }

        if (!paused) {
            if (isInit) 
                glfwSetTime(glfwGetTime() - getPauseTime());
            else 
                glfwSetTime(0.0f);

            currTime = glfwGetTime();
            deltaTime += (currTime - lastTime) / limitFPS;
            lastTime = currTime;

            endPauseTime = 0;

            if (isInit) {
                    while (deltaTime >= 1.0) {
                        update();
                        deltaTime--;
                    }

                render();
            }
        }
        else
            endPauseTime = glfwGetTime();

        nk_glfw3_new_frame(&glfw);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    closeFile();
    quit();
}


