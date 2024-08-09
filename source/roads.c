// Standard
#include <string.h>

// Custom modules
#include <road.h>
#include <cars.h>
#include <algorithms.h>
#include <render.h>

road roads[NUMBER_OF_ROADS];
GLuint roadVAO, roadVBO, roadEBO;
GLfloat roadVertices[NUMBER_OF_ROADS * 4 * 5];
GLint roadIndices[NUMBER_OF_ROADS * 6];
GLuint lineVAO, lineVBO;
GLfloat lineVertices[NUMBER_OF_LINES * NUMBER_OF_ROADS * 5 * 2];

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
    return roads[rlc.road].lines[rlc.line].cells;
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
            start_x + ROAD_WIDTH, start_y, 0.31f, 0.31f, 0.31f,
            start_x - ROAD_WIDTH, start_y, 0.31f, 0.31f, 0.31f,
            start_x + ROAD_WIDTH, start_y + lenght, 0.31f, 0.31f, 0.31f,
            start_x - ROAD_WIDTH, start_y + lenght, 0.31f, 0.31f, 0.31f
        };

        if (start_y + 1.0f < ERROR_LIMIT)
        {
            roads[roadIndex].isEdge = true;
        }
        else
        {
            roads[roadIndex].isEdge = false;
        }

        memcpy(&roadVertices[roadIndex * 4 * 5], verticies, sizeof(GLfloat) * 4 * 5);
    }

    else if (dir == SOUTH)
    {
        GLfloat verticies[] =
        {
            start_x + ROAD_WIDTH, start_y, 0.31f, 0.31f, 0.31f,
            start_x - ROAD_WIDTH, start_y, 0.31f, 0.31f, 0.31f,
            start_x + ROAD_WIDTH, start_y - lenght, 0.31f, 0.31f, 0.31f,
            start_x - ROAD_WIDTH, start_y - lenght, 0.31f, 0.31f, 0.31f
        };

        if (start_y - 1.0f < ERROR_LIMIT)
        {
            roads[roadIndex].isEdge = true;
        }
        else
        {
            roads[roadIndex].isEdge = false;
        }

        memcpy(&roadVertices[roadIndex * 4 * 5], verticies, sizeof(GLfloat) * 4 * 5);
    }

    else if (dir == EAST)
    {
        GLfloat verticies[] =
        {
            start_x, start_y + ROAD_WIDTH, 0.31f, 0.31f, 0.31f,
            start_x, start_y - ROAD_WIDTH, 0.31f, 0.31f, 0.31f,
            start_x + lenght, start_y + ROAD_WIDTH, 0.31f, 0.31f, 0.31f,
            start_x + lenght, start_y - ROAD_WIDTH, 0.31f, 0.31f, 0.31f
        };

        if (start_x + 1.0f < ERROR_LIMIT)
        {
            roads[roadIndex].isEdge = true;
        }
        else
        {
            roads[roadIndex].isEdge = false;
        }

        memcpy(&roadVertices[roadIndex * 4 * 5], verticies, sizeof(GLfloat) * 4 * 5);
    }

    else if (dir == WEST)
    {
        GLfloat vertices[] =
        {
            start_x, start_y + ROAD_WIDTH, 0.31f, 0.31f, 0.31f,
            start_x, start_y - ROAD_WIDTH, 0.31f, 0.31f, 0.31f,
            start_x - lenght, start_y + ROAD_WIDTH, 0.31f, 0.31f, 0.31f,
            start_x - lenght, start_y - ROAD_WIDTH, 0.31f, 0.31f, 0.31f
        };

        if (start_x - 1.0f < ERROR_LIMIT)
        {
            roads[roadIndex].isEdge = true;
        }
        else
        {
            roads[roadIndex].isEdge = false;
        }

        memcpy(&roadVertices[roadIndex * 4 * 5], vertices, sizeof(GLfloat) * 4 * 5);
    }
}

GLvoid setLines(GLint roadIndex)
{
    GLfloat stride = ROAD_WIDTH * 2 / (NUMBER_OF_LINES + 1.0f);
    int i;

    if (roads[roadIndex].dir == NORTH || roads[roadIndex].dir == SOUTH)
    {
        GLfloat x, y1, y2;

        x = roadVertices[0 + 4 * 5 * roadIndex] - ROAD_WIDTH * 2;
        y1 = roadVertices[1 + 4 * 5 * roadIndex];
        y2 = roadVertices[11 + 4 * 5 * roadIndex];

        for (i = 0; i < NUMBER_OF_LINES; i++)
        {
            x += stride;
            GLfloat vertices[] =
            {
                x, y1, 1.0f, 1.0f, 1.0f,
                x, y2, 1.0f, 1.0f, 1.0f
            };

            memcpy(&lineVertices[i * 5 * 2 + roadIndex * 5 * NUMBER_OF_LINES * 2], vertices, sizeof(GLfloat) * 5 * 2);
            memset(roads[roadIndex].lines[i].cells, 0, sizeof(roads[roadIndex].lines[i].cells));
            roads[roadIndex].lines[i].carSpawnCoord = x;
        }

        memset(roads[roadIndex].lines[i].cells, 0, sizeof(roads[roadIndex].lines[i].cells));
        roads[roadIndex].lines[i].carSpawnCoord = x + stride;

        #ifdef DEBUG

        x = roadVertices[0 + 4 * 5 * roadIndex] - ROAD_WIDTH * 2;
        y1 = roadVertices[1 + 4 * 5 * roadIndex];
        y2 = roadVertices[11 + 4 * 5 * roadIndex];


        int dir_multiplier;
        if (roads[roadIndex].dir == NORTH)
            dir_multiplier = 1;
        else
            dir_multiplier = -1;

        for (i = 0; i < NUMBER_OF_LINES + 1; i++) {
            x += stride;
            for(int j = 0; j < NUMBER_OF_CELLS; j++) {
                float cellVertices[] = {
                    x, y1 + (j * CELL_LENGHT) * dir_multiplier, 1.0f, 0.0f, 0.0f, 
                    x, y1 + ((j + 1) * CELL_LENGHT) * dir_multiplier, 1.0f, 0.0f, 0.0f, 
                    x - CELL_WIDTH, y1 + (j * CELL_LENGHT) * dir_multiplier, 1.0f, 0.0f, 0.0f, 
                    x - CELL_WIDTH, y1 + ((j + 1) * CELL_LENGHT) * dir_multiplier, 1.0f, 0.0f, 0.0f, 
                    x - CELL_WIDTH, y1 + (j * CELL_LENGHT) * dir_multiplier, 1.0f, 0.0f, 0.0f, 
                    x, y1 + (j * CELL_LENGHT) * dir_multiplier, 1.0f, 0.0f, 0.0f, 
                    x - CELL_WIDTH, y1 + (j * CELL_LENGHT) * dir_multiplier, 1.0f, 0.0f, 0.0f, 
                    x, y1 + (j * CELL_LENGHT) * dir_multiplier, 1.0f, 0.0f, 0.0f 
                };        
                int index = roadIndex * 5 * 8 * (NUMBER_OF_LINES + 1) * NUMBER_OF_CELLS + i * 5 * 8 * NUMBER_OF_CELLS + j * 5 * 8;
                memcpy(&cellsVertices[index], cellVertices, sizeof(GLfloat) * 5 * 8);
            }
        }

        #endif
    }
    
    if (roads[roadIndex].dir == EAST || roads[roadIndex].dir == WEST)
    {
        GLfloat y, x1, x2;

        y = roadVertices[1 + 4 * 5 * roadIndex] - ROAD_WIDTH * 2;
        x1 = roadVertices[0 + 4 * 5 * roadIndex];
        x2 = roadVertices[10 + 4 * 5 * roadIndex];

        for (i = 0; i < NUMBER_OF_LINES; i++)
        {
            y += stride;
            GLfloat vertices[] =
            {
                x1, y, 1.0f, 1.0f, 1.0f,
                x2, y, 1.0f, 1.0f, 1.0f
            };

            memcpy(&lineVertices[i * 5 * 2 + roadIndex * 5 * NUMBER_OF_LINES * 2], vertices, sizeof(GLfloat) * 5 * 2);
            memset(roads[roadIndex].lines[i].cells, 0, sizeof(roads[roadIndex].lines[i].cells));
            roads[roadIndex].lines[i].carSpawnCoord = y;
        }
        memset(roads[roadIndex].lines[i].cells, 0, sizeof(roads[roadIndex].lines[i].cells));
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
