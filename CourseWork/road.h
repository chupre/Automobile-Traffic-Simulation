#pragma once

#define CELL_LENGHT CELL_SIZE
#define ROAD_WIDTH CELL_SIZE

#include <string.h>

GLvoid setRoad(GLint roadIndex, GLfloat start_x, GLfloat start_y, GLfloat lenght, DIRECTION dir);
GLvoid setLines(GLint roadIndex);
DIRECTION getRoadDir(car* Сar);
DIRECTION getOvertakeDir(DIRECTION roadDir);
car** getFirstCellPtr(RLC rlc);
GLint getCarDirOnRoad(road* Road);
GLvoid unbindCarPtrFromCell(car* Сar);
GLvoid reinitCurrCellWithNextCell(car* Сar);
GLvoid setRoadsToDefault();


DIRECTION getRoadDir(car* Car)
{
    return ((roads + Car->currCell.road)->dir);
}

GLint getCarDirOnRoad(road* Road)
{
    if (Road->dir == NORTH || Road->dir == EAST) return 1;
    else if (Road->dir == SOUTH || Road->dir == WEST) return -1;
    else return 0;
}

DIRECTION getOvertakeDir(DIRECTION roadDir)
{
    if (roadDir == NORTH) return EAST;
    else if (roadDir == SOUTH) return WEST;
    else if (roadDir == WEST) return NORTH;
    else if (roadDir == EAST) return SOUTH;
    else return NONE;
}

car** getFirstCellPtr(RLC rlc) {
    return (((roads + rlc.road)->lines + rlc.line)->cells);
}

GLvoid unbindCarPtrFromCell(car* Car)
{
    car** ptrCell = getFirstCellPtr(Car->currCell);
    GLint start = Car->currCell.cell;
    if (start < NUMBER_OF_CELLS) ptrCell[start] = NULL;

    ptrCell = getFirstCellPtr(Car->nextCell);
    GLint newStart = Car->nextCell.cell;
    if (newStart < NUMBER_OF_CELLS) ptrCell[newStart] = Car;
}

GLvoid reinitCurrCellWithNextCell(car* Car)
{
    Car->currCell.road = Car->nextCell.road;
    Car->currCell.line = Car->nextCell.line;
    Car->currCell.cell = Car->nextCell.cell;
}

//it changes road->dir = dir, road->Edge = true/false
GLvoid setRoad(GLint roadIndex, GLfloat start_x, GLfloat start_y, GLfloat lenght, DIRECTION dir)
{
    roads[roadIndex].dir = dir;

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
            roads[roadIndex].isEdge = true;
        }
        else
        {
            roads[roadIndex].isEdge = false;
        }

        memcpy(&roadVertices[roadIndex * 3 * 4 * 2], verticies, sizeof(GLfloat) * 4 * 3 * 2);
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
            roads[roadIndex].isEdge = true;
        }
        else
        {
            roads[roadIndex].isEdge = false;
        }

        memcpy(&roadVertices[roadIndex * 3 * 4 * 2], verticies, sizeof(GLfloat) * 4 * 3 * 2);
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
            roads[roadIndex].isEdge = true;
        }
        else
        {
            roads[roadIndex].isEdge = false;
        }

        memcpy(&roadVertices[roadIndex * 3 * 4 * 2], verticies, sizeof(GLfloat) * 4 * 3 * 2);
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
            roads[roadIndex].isEdge = true;
        }
        else
        {
            roads[roadIndex].isEdge = false;
        }

        memcpy(&roadVertices[roadIndex * 3 * 4 * 2], vertices, sizeof(GLfloat) * 4 * 3 * 2);
    }
}

GLvoid setLines(GLint roadIndex)
{
    GLfloat stride = ROAD_WIDTH * 2 / (NUMBER_OF_LINES + 1.0f);
    int i;

    if (roads[roadIndex].dir == NORTH || roads[roadIndex].dir == SOUTH)
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
            //in memset shoudn't be NULL !!!
            memcpy(&lineVertices[i * 6 * 2 + roadIndex * 6 * NUMBER_OF_LINES * 2], vertices, sizeof(GLfloat) * 6 * 2);
            memset(roads[roadIndex].lines[i].cells, 0, sizeof(car*) * NUMBER_OF_CELLS);
            roads[roadIndex].lines[i].carSpawnCoord = x;
        }
        memset(roads[roadIndex].lines[i].cells, 0, sizeof(car*) * NUMBER_OF_CELLS);
        roads[roadIndex].lines[i].carSpawnCoord = x + stride;
    }
    
    if (roads[roadIndex].dir == EAST || roads[roadIndex].dir == WEST)
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
            memset(roads[roadIndex].lines[i].cells, EMPTY, sizeof(GLint) * NUMBER_OF_CELLS);
            roads[roadIndex].lines[i].carSpawnCoord = y;
        }
        memset(roads[roadIndex].lines[i].cells, EMPTY, sizeof(GLint) * NUMBER_OF_CELLS);
        roads[roadIndex].lines[i].carSpawnCoord = y + stride;
    }
}


GLvoid setRoadsToDefault()
{
    for (int i = 0; i < NUMBER_OF_ROADS; i++)
    {
        for (int j = 0; j < NUMBER_OF_LINES; j++)
        {
            memset(roads[i].lines[j].cells, 0, sizeof(car*) * NUMBER_OF_CELLS);
        }
    }
}
