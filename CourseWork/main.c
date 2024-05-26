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
#include <sys/types.h>
#include <dirent.h>

#include <serviceMacros.h>
#include <gl.h>
#include <road.h>
#include <map.h>
#include <shader.h>
#include <cars.h>
#include <gui.h>
#include <algorithms.h>


int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, keyCallback);

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    genShader();

    srand(time(NULL));

    setMap();
    setCarsToDefault();

    initRoads();
    initLines();
    initCars();

    glm_mat4_identity(identityTrans);
    glm_mat4_identity_array(carTrans, MAX_CARS);

    context = nk_glfw3_init(&glfw, window, NK_GLFW3_INSTALL_CALLBACKS);
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
            showFPS();
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


