#pragma once
#define NUMBER_OF_ROADS 2
#define NUMBER_OF_LINES 1
#define DEFAULT_CELLS_LENGHT 16
#define ROAD_WIDTH 0.1f
#include <string.h>


enum direction { NORTH, SOUTH, EAST, WEST };

typedef struct
{
    int velocity;
    enum direction target;
} car;

typedef struct
{
    int* cells[DEFAULT_CELLS_LENGHT];
    bool isEdge;
} line;

typedef struct
{
    enum direction dir;
    line lines[NUMBER_OF_LINES];
} road;

void setRoad(road* Road, GLint roadIndex, GLfloat* roadVerticies, GLint* roadIndices, GLfloat start_x, GLfloat start_y, GLfloat lenght, enum direction dir)
{
    (*Road).dir = dir;

    int indeces[] =
    {
        0 + (roadIndex * 4), 2 + (roadIndex * 4), 3 + (roadIndex * 4),
        0 + (roadIndex * 4), 1 + (roadIndex * 4), 3 + (roadIndex * 4)
    };

    memcpy(&roadIndices[roadIndex * 6], indeces, sizeof(GLint) * 6);

    if (dir == NORTH)
    {
        GLfloat verticies[] =
        {
            start_x + ROAD_WIDTH, start_y, 0.0f,
            start_x - ROAD_WIDTH, start_y, 0.0f,
            start_x + ROAD_WIDTH, start_y + lenght, 0.0f,
            start_x - ROAD_WIDTH, start_y + lenght, 0.0f
        };

        memcpy(&roadVerticies[roadIndex * 3 * 4], verticies, sizeof(GLfloat) * 4 * 3);
    }

    else if (dir == SOUTH)
    {
        GLfloat verticies[] =
        {
            start_x + ROAD_WIDTH, start_y, 0.0f,
            start_x - ROAD_WIDTH, start_y, 0.0f,
            start_x + ROAD_WIDTH, start_y - lenght, 0.0f,
            start_x - ROAD_WIDTH, start_y - lenght, 0.0f
        };

        memcpy(&roadVerticies[roadIndex * 3 * 4], verticies, sizeof(GLfloat) * 4 * 3);
    }

    else if (dir == EAST)
    {
        GLfloat verticies[] =
        {
            start_x, start_y + ROAD_WIDTH, 0.0f,
            start_x, start_y - ROAD_WIDTH, 0.0f,
            start_x + lenght, start_y + ROAD_WIDTH, 0.0f,
            start_x + lenght, start_y - ROAD_WIDTH, 0.0f
        };

        memcpy(&roadVerticies[roadIndex * 3 * 4], verticies, sizeof(GLfloat) * 4 * 3);
    }

    else if (dir == WEST)
    {
        GLfloat vertices[] =
        {
            start_x, start_y + ROAD_WIDTH, 0.0f,
            start_x, start_y - ROAD_WIDTH, 0.0f,
            start_x - lenght, start_y + ROAD_WIDTH, 0.0f,
            start_x - lenght, start_y - ROAD_WIDTH, 0.0f
        };

        memcpy(&roadVerticies[roadIndex * 3 * 4], vertices, sizeof(GLfloat) * 4 * 3);
    }
}


void setLine(road Road, GLint roadIndex, GLfloat* roadVertices, GLfloat* lineVertices)
{
    GLfloat x, y1, y2;
    GLfloat stride = ROAD_WIDTH * 2 / ((GLfloat)NUMBER_OF_LINES + 1.0f);

    if (Road.dir == NORTH || Road.dir == SOUTH)
    {
        x = roadVertices[0 + 4 * 3 * roadIndex] - ROAD_WIDTH * 2;
        y1 = roadVertices[1 + 4 * 3 * roadIndex];
        y2 = roadVertices[7 + 4 * 3 * roadIndex];

        for (int i = 0; i < NUMBER_OF_LINES; i++)
        {
            x += stride;
            GLfloat vertices[] =
            {
                x, y1, 0.0f,
                x, y2, 0.0f
            };
            
            memcpy(&lineVertices[i * 3 * 2 + roadIndex * 6 * NUMBER_OF_LINES], vertices, sizeof(GLfloat) * 3 * 2);
        }
    }

}
