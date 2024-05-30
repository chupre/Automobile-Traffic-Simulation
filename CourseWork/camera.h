#pragma once

#define DEFAULT_FOV 45.0f

vec3 cameraPos = { 0.0f, 0.0f, 3.0f };
vec3 cameraFront = { 0.0f, 0.0f, -1.0f };
vec3 cameraUp = { 0.0f, 1.0f, 0.0f };

GLfloat cameraSpeedMultiplier;
GLfloat cameraFOV = DEFAULT_FOV;

typedef enum { UP, DOWN, RIGHT, LEFT} camDir;

GLvoid setProjection();
GLvoid setView();
GLvoid moveCamera(camDir dir);


GLvoid setProjection()
{
    mat4 projection;
    GLuint projLoc = glGetUniformLocation(shaderProgram, "projection");
    glm_perspective(glm_rad(cameraFOV), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.01f, 100.0f, projection);
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, projection);
}


GLvoid setView()
{
    mat4 view;
    vec3 temp;
    glm_vec3_add(cameraPos, cameraFront, temp);
    glm_lookat(cameraPos, temp, cameraUp, view);
    GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view);
}


GLvoid moveCamera(camDir dir)
{
    float cameraSpeed = 2.5 * cameraSpeedMultiplier;

    if (dir == UP)
    {
        cameraPos[1] += cameraSpeed;

        
    }

    if (dir == DOWN)
    {
        cameraPos[1] -= cameraSpeed;
    }

    if (dir == RIGHT)
    {
        cameraPos[0] += cameraSpeed;

    }

    if (dir == LEFT)
    {
        cameraPos[0] -= cameraSpeed;
    }
}