// Custom modules
#include <camera.h>
#include <map.h>
#include <shader.h>
#include <render.h>
#include <road.h>

vec3 cameraPos = { 0.0f, 0.0f, 3.0f };
vec3 cameraFront = { 0.0f, 0.0f, -1.0f };
vec3 cameraUp = { 0.0f, 1.0f, 0.0f };

mat4 projection;
mat4 view;

GLfloat cameraFOV;

bool cameraInit = false;

GLvoid setProjection(int shader)
{
    if (!cameraInit) {
        cameraFOV = DEFAULT_FOV;
        cameraInit = true;
    }

    GLuint projLoc = glGetUniformLocation(shader , "projection");
    glm_perspective(glm_rad(cameraFOV), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 1.0f, 100.0f, projection);
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, (float *)projection);
}


GLvoid setView(int shader)
{
    vec3 temp;
    glm_vec3_add(cameraPos, cameraFront, temp);
    glm_lookat(cameraPos, temp, cameraUp, view);
    GLuint viewLoc = glGetUniformLocation(shader, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, (float *)view);
}


GLvoid moveCamera(camDir dir)
{
    float cameraSpeed = 0.01;

    if (dir == UP && isPossibleToMoveCam(dir, cameraSpeed))
    {
        cameraPos[1] += cameraSpeed;
    }

    if (dir == DOWN && isPossibleToMoveCam(dir, cameraSpeed))
    {
        cameraPos[1] -= cameraSpeed;
    }

    if (dir == RIGHT && isPossibleToMoveCam(dir, cameraSpeed))
    {
        cameraPos[0] += cameraSpeed;

    }

    if (dir == LEFT && isPossibleToMoveCam(dir, cameraSpeed))
    {
        cameraPos[0] -= cameraSpeed;
    }
}


GLint isVertexOnCamBorders(vec4 vertex, camDir dir, GLfloat offset, DIRECTION roadDir)
{
    vec3 newCameraPos;
    glm_vec3_copy(cameraPos, newCameraPos);

    if (dir == UP)
    {
        if (roadDir == WEST || roadDir == EAST)
        {
            return 1;
        }

        newCameraPos[1] += offset;
        setTestVertex(vertex, newCameraPos);

        if (vertex[1] >= 2.0)
        {
            return 1;
        }
    }

    if (dir == DOWN)
    {
        if (roadDir == WEST || roadDir == EAST)
        {
            return 1;
        }

        newCameraPos[1] -= offset;
        setTestVertex(vertex, newCameraPos);

        if (vertex[1] <= -2.0)
        {
            return 1;
        }
    }

    if (dir == RIGHT)
    {
        if (roadDir == NORTH || roadDir == SOUTH)
        {
            return 1;
        }

        newCameraPos[0] += offset;
        setTestVertex(vertex, newCameraPos);

        if (vertex[0] >= 2.0)
        {
            return 1;
        }
    }

    if (dir == LEFT)
    {
        if (roadDir == NORTH || roadDir == SOUTH)
        {
            return 1;
        }

        newCameraPos[0] -= offset;
        setTestVertex(vertex, newCameraPos);

        if (vertex[0] <= -2.0)
        {
            return 1;
        }
    }

    return 0;
}


GLvoid setTestVertex(vec4 vertex, vec3 newCamPos)
{
    mat4 projection;
    glm_perspective(glm_rad(cameraFOV), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.01f, 100.0f, projection);

    mat4 newView;
    vec3 temp;
    glm_vec3_add(newCamPos, cameraFront, temp);
    glm_lookat(newCamPos, temp, cameraUp, newView);

    glm_mat4_mulv(newView, vertex, vertex);
    glm_mat4_mulv(projection, vertex, vertex);
}


GLint isPossibleToMoveCam(camDir dir, GLfloat offset)
{
    int roadsOnEdge = 0, roadsOnCamBorder = 0;

    if (MAP_TYPE == CROSS) {
        if (dir == UP) {
            vec4 upNorthRoadEdge = { roadVertices[4 * 5 * 4 + 10], roadVertices[4 * 5 * 4 + 11], 1.0f, 1.0f };
            return isVertexOnCamBorders(upNorthRoadEdge, dir, offset, NORTH);
        }
        
        if (dir == DOWN) {
            vec4 downNorthRoadEdge = { roadVertices[0 * 5 * 4 + 0], roadVertices[0 * 5 * 4 + 1], 1.0f, 1.0f };
            return isVertexOnCamBorders(downNorthRoadEdge, dir, offset, NORTH);
        }
        
        if (dir == RIGHT) {
            vec4 rightWestRoadEdge = { roadVertices[3 * 5 * 4 + 0], roadVertices[3 * 5 * 4 + 1], 1.0f, 1.0f };
            return isVertexOnCamBorders(rightWestRoadEdge, dir, offset, WEST);
        }

        if (dir == LEFT) {
            vec4 leftWestRoadEdge = { roadVertices[7 * 5 * 4 + 10], roadVertices[7 * 5 * 4 + 11], 1.0f, 1.0f };
            return isVertexOnCamBorders(leftWestRoadEdge, dir, offset, WEST);
        }
    }

    if (MAP_TYPE == SEVERAL_CROSSES)
        return true;

    for (int i = 0; i < NUMBER_OF_ROADS; i++)
    {
        if (roads[i].isEdge)
        {
            roadsOnEdge++;

            vec4 testVertex = { 0.0, 0.0, 1.0, 1.0 };
            getTestVertex(i, dir, testVertex);

            if (isVertexOnCamBorders(testVertex, dir, offset, roads[i].dir))
            {
                roadsOnCamBorder++;
            }
        }
    }

    if (roadsOnCamBorder == roadsOnEdge)
    {
        return 1;
    }

    return 0;
}


GLvoid getTestVertex(GLint roadIndex, camDir dir, vec4 dest)
{
    if (dir == UP)
    {
        if (roads[roadIndex].dir == NORTH)
        {
            dest[0] = roadVertices[10 + roadIndex * 4 * 5];
            dest[1] = roadVertices[11 + roadIndex * 4 * 5];
        }

        if (roads[roadIndex].dir == SOUTH)
        {
            dest[0] = roadVertices[0 + roadIndex * 4 * 5];
            dest[1] = roadVertices[1 + roadIndex * 4 * 5];
        }
    }

    if (dir == DOWN)
    {
        if (roads[roadIndex].dir == NORTH)
        {
            dest[0] = roadVertices[0 + roadIndex * 4 * 5];
            dest[1] = roadVertices[1 + roadIndex * 4 * 5];
        }

        if (roads[roadIndex].dir == SOUTH)
        {
            dest[0] = roadVertices[10 + roadIndex * 4 * 5];
            dest[1] = roadVertices[11 + roadIndex * 4 * 5];
        }
    }

    if (dir == RIGHT)
    {
        if (roads[roadIndex].dir == WEST)
        {
            dest[0] = roadVertices[0 + roadIndex * 4 * 5];
            dest[1] = roadVertices[1 + roadIndex * 4 * 5];
        }

        if (roads[roadIndex].dir == EAST)
        {
            dest[0] = roadVertices[10 + roadIndex * 4 * 5];
            dest[1] = roadVertices[11 + roadIndex * 4 * 5];
        }
    }

    if (dir == LEFT)
    {
        if (roads[roadIndex].dir == WEST)
        {
            dest[0] = roadVertices[10 + roadIndex * 4 * 5];
            dest[1] = roadVertices[11 + roadIndex * 4 * 5];
        }

        if (roads[roadIndex].dir == EAST)
        {
            dest[0] = roadVertices[0 + roadIndex * 4 * 5];
            dest[1] = roadVertices[1 + roadIndex * 4 * 5];
        }
    }
}
