#pragma once
#define NUMBER_OF_ROADS 2
#define NUMBER_OF_LINES 2
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
        float verticies[] =
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
        float verticies[] =
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
        float verticies[] =
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
        float verticies[] =
        {
            start_x, start_y + ROAD_WIDTH, 0.0f,
            start_x, start_y - ROAD_WIDTH, 0.0f,
            start_x - lenght, start_y + ROAD_WIDTH, 0.0f,
            start_x - lenght, start_y - ROAD_WIDTH, 0.0f
        };

        memcpy(&roadVerticies[roadIndex * 3 * 4], verticies, sizeof(GLfloat) * 4 * 3);
    }
}