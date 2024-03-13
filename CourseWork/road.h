#pragma once
#define NUMBER_OF_ROADS 2
#define NUMBER_OF_LINES 1
#define NUMBER_OF_CELLS 20
#define CELL_LENGHT 0.1f
#define ROAD_WIDTH 0.1f
#define ERROR_LIMIT 0.0000001
#define EMPTY -1
#include <string.h>


enum direction { NORTH, SOUTH, EAST, WEST, NONE = EMPTY };

typedef struct
{
    GLint road;
    GLint line;
    GLint cell;
} RLC;

typedef struct
{
    RLC currCell;
    RLC nextCell;
    GLint velocity;
    enum direction target;
    enum direction overtake;
    bool isActive;
} car;

typedef struct
{
    car* cells[NUMBER_OF_CELLS];
    GLfloat carSpawnCoord;
} line;

typedef struct
{
    bool isEdge;
    enum direction dir;
    line lines[NUMBER_OF_LINES + 1];
} road;


void setRoad(road* Road, GLint roadIndex, GLfloat* roadVerticies, GLint* roadIndices, GLfloat start_x, GLfloat start_y, GLfloat lenght, enum direction dir);
void setLines(road* Road, GLint roadIndex, GLfloat* roadVertices, GLfloat* lineVertices);


void setRoad(road* Road, GLint roadIndex, GLfloat* roadVerticies, GLint* roadIndices, GLfloat start_x, GLfloat start_y, GLfloat lenght, enum direction dir)
{
    Road->dir = dir;

    GLint indeces[] =
    {
        0 + (roadIndex * 4), 2 + (roadIndex * 4), 3 + (roadIndex * 4),
        0 + (roadIndex * 4), 1 + (roadIndex * 4), 3 + (roadIndex * 4)
    };

    memcpy(&roadIndices[roadIndex * 6], indeces, sizeof(GLint) * 6);

    if (dir == NORTH)
    {
        GLfloat verticies[] =
        {
            start_x + ROAD_WIDTH, start_y, 0.0f, 0.31f, 0.31f, 0.31f,
            start_x - ROAD_WIDTH, start_y, 0.0f, 0.31f, 0.31f, 0.31f,
            start_x + ROAD_WIDTH, start_y + lenght, 0.0f, 0.31f, 0.31f, 0.31f,
            start_x - ROAD_WIDTH, start_y + lenght, 0.0f, 0.31f, 0.31f, 0.31f
        };

        if (start_y + 1.0f < ERROR_LIMIT)
        {
            Road->isEdge = true;
        }
        else
        {
            Road->isEdge = false;
        }

        memcpy(&roadVerticies[roadIndex * 3 * 4 * 2], verticies, sizeof(GLfloat) * 4 * 3 * 2);
    }

    else if (dir == SOUTH)
    {
        GLfloat verticies[] =
        {
            start_x + ROAD_WIDTH, start_y, 0.0f, 0.31f, 0.31f, 0.31f,
            start_x - ROAD_WIDTH, start_y, 0.0f, 0.31f, 0.31f, 0.31f,
            start_x + ROAD_WIDTH, start_y - lenght, 0.0f, 0.31f, 0.31f, 0.31f,
            start_x - ROAD_WIDTH, start_y - lenght, 0.0f, 0.31f, 0.31f, 0.31f
        };

        if (start_y - 1.0f < ERROR_LIMIT)
        {
            Road->isEdge = true;
        }
        else
        {
            Road->isEdge = false;
        }

        memcpy(&roadVerticies[roadIndex * 3 * 4 * 2], verticies, sizeof(GLfloat) * 4 * 3 * 2);
    }

    else if (dir == EAST)
    {
        GLfloat verticies[] =
        {
            start_x, start_y + ROAD_WIDTH, 0.0f, 0.31f, 0.31f, 0.31f,
            start_x, start_y - ROAD_WIDTH, 0.0f, 0.31f, 0.31f, 0.31f,
            start_x + lenght, start_y + ROAD_WIDTH, 0.0f, 0.31f, 0.31f, 0.31f,
            start_x + lenght, start_y - ROAD_WIDTH, 0.0f, 0.31f, 0.31f, 0.31f
        };

        if (start_x + 1.0f < ERROR_LIMIT)
        {
            Road->isEdge = true;
        }
        else
        {
            Road->isEdge = false;
        }

        memcpy(&roadVerticies[roadIndex * 3 * 4 * 2], verticies, sizeof(GLfloat) * 4 * 3 * 2);
    }

    else if (dir == WEST)
    {
        GLfloat vertices[] =
        {
            start_x, start_y + ROAD_WIDTH, 0.0f, 0.31f, 0.31f, 0.31f,
            start_x, start_y - ROAD_WIDTH, 0.0f, 0.31f, 0.31f, 0.31f,
            start_x - lenght, start_y + ROAD_WIDTH, 0.0f, 0.31f, 0.31f, 0.31f,
            start_x - lenght, start_y - ROAD_WIDTH, 0.0f, 0.31f, 0.31f, 0.31f
        };

        if (start_x - 1.0f < ERROR_LIMIT)
        {
            Road->isEdge = true;
        }
        else
        {
            Road->isEdge = false;
        }

        memcpy(&roadVerticies[roadIndex * 3 * 4 * 2], vertices, sizeof(GLfloat) * 4 * 3 * 2);
    }
}


void setLines(road *Road, GLint roadIndex, GLfloat* roadVertices, GLfloat* lineVertices)
{
    GLfloat stride = ROAD_WIDTH * 2 / (NUMBER_OF_LINES + 1.0f);
    int i;

    if (Road->dir == NORTH || Road->dir == SOUTH)
    {
        GLfloat x, y1, y2;

        x = roadVertices[0 + 4 * 3 * 2 * roadIndex] - ROAD_WIDTH * 2;
        y1 = roadVertices[1 + 4 * 3 * 2 * roadIndex];
        y2 = roadVertices[13 + 4 * 3 * 2 * roadIndex];

        for (i = 0; i < NUMBER_OF_LINES; i++)
        {
            x += stride;
            GLfloat vertices[] =
            {
                x, y1, 0.0f, 1.0f, 1.0f, 1.0f,
                x, y2, 0.0f, 1.0f, 1.0f, 1.0f
            };
            
            memcpy(&lineVertices[i * 6 * 2 + roadIndex * 6 * NUMBER_OF_LINES * 2], vertices, sizeof(GLfloat) * 6 * 2);
            memset(Road->lines[i].cells, EMPTY, sizeof(GLint) * NUMBER_OF_CELLS);
            Road->lines[i].carSpawnCoord = x; // косвенное значение.Уточняется в setCar()
        }
        memset(Road->lines[i].cells, EMPTY, sizeof(GLint) * NUMBER_OF_CELLS);
        Road->lines[i].carSpawnCoord = x + stride; 
    }
    
    if (Road->dir == EAST || Road->dir == WEST)
    {
        GLfloat y, x1, x2;

        y = roadVertices[1 + 4 * 3 * 2 * roadIndex] - ROAD_WIDTH * 2;
        x1 = roadVertices[0 + 4 * 3 * 2 * roadIndex];
        x2 = roadVertices[12 + 4 * 3 * 2 * roadIndex];

        for (i = 0; i < NUMBER_OF_LINES; i++)
        {
            y += stride;
            GLfloat vertices[] =
            {
                x1, y, 0.0f, 1.0f, 1.0f, 1.0f,
                x2, y, 0.0f, 1.0f, 1.0f, 1.0f
            };

            memcpy(&lineVertices[i * 6 * 2 + roadIndex * 6 * NUMBER_OF_LINES * 2], vertices, sizeof(GLfloat) * 6 * 2);
            memset(Road->lines[i].cells, EMPTY, sizeof(GLint) * NUMBER_OF_CELLS);
            Road->lines[i].carSpawnCoord = y;
        }
        memset(Road->lines[i].cells, EMPTY, sizeof(GLint) * NUMBER_OF_CELLS);
        Road->lines[i].carSpawnCoord = y + stride;
    }
}
