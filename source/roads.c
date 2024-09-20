// Standard
#include <stdlib.h>
#include <string.h>

// Custom modules
#include <road.h>
#include <map.h>
#include <cars.h>
#include <algorithms.h>
#include <render.h>

int midLinesCounter = 0;

DIRECTION getRoadDir(car* Car)
{
    return ((roads + Car->currCell.road)->dir);
}

GLint getDirMultiplier(DIRECTION dir)
{
    if (dir == NORTH || dir == EAST) return 1;
    else if (dir == SOUTH || dir == WEST) return -1;
    else return 0; 
}

DIRECTION getLeftMoveDir(DIRECTION dir)
{
    if (dir == NORTH) return WEST;
    else if (dir == SOUTH) return EAST;
    else if (dir == WEST) return SOUTH;
    else if (dir == EAST) return NORTH;
    else return NONE;
}

DIRECTION getRightMoveDir(DIRECTION dir)
{
    if (dir == NORTH) return EAST;
    else if (dir == SOUTH) return WEST;
    else if (dir == WEST) return NORTH;
    else if (dir == EAST) return SOUTH;
    else return NONE;
}

car** getFirstCellPtr(RLC rlc) {
    return roads[rlc.road].lines[rlc.line].cells;
}

GLvoid addRoad(GLint roadIndex, GLfloat start_x, GLfloat start_y, DIRECTION dir)
{
    // road properties
    roads[roadIndex].dir = dir;
    roads[roadIndex].isBeginCross = false; // by default
    roads[roadIndex].isEndCross = false; // by default
    setRoadBoards(roadIndex, start_x, start_y);
    setEdgeState(roadIndex, start_x, start_y, dir);

    GLfloat length = CELL_LENGTH * NUMBER_OF_CELLS;

    setRoad(roadIndex, start_x, start_y, length, dir);
}

GLvoid setRoadBoards(GLint roadIndex, GLfloat start_x, GLfloat start_y)
{
    switch (roads[roadIndex].dir)
    {
        case NORTH:
        {
            roads[roadIndex].stem = start_x;
            roads[roadIndex].startLineCoord = start_y;
            roads[roadIndex].endLineCoord = start_y + (NUMBER_OF_CELLS * CELL_LENGTH);
            break;
        }
        case SOUTH:
        {
            roads[roadIndex].stem = start_x;
            roads[roadIndex].startLineCoord = start_y;
            roads[roadIndex].endLineCoord = start_y - (NUMBER_OF_CELLS * CELL_LENGTH);
            break;
        }
        case EAST:
        {
            roads[roadIndex].stem = start_y;
            roads[roadIndex].startLineCoord = start_x;
            roads[roadIndex].endLineCoord = start_x + (NUMBER_OF_CELLS * CELL_LENGTH);
            break;
        }
        case WEST:
        {
            roads[roadIndex].stem = start_y;
            roads[roadIndex].startLineCoord = start_x;
            roads[roadIndex].endLineCoord = start_x - (NUMBER_OF_CELLS * CELL_LENGTH);
            break;
        }
    }
    // printf("dir: %d, endLIneCoord: %f, startLineCoord: %f\n", roads[roadIndex].dir, roads[roadIndex].endLineCoord, roads[roadIndex].startLineCoord);
}

GLvoid setEdgeState(GLint roadIndex, GLfloat start_x, GLfloat start_y, DIRECTION dir)
{
    switch (dir){
        case NORTH:{
            if (start_y + 1.0f < ERROR_LIMIT){
                roads[roadIndex].isEdge = true;
            }else{
                roads[roadIndex].isEdge = false;
            }
            break;
        }
        case SOUTH:{
            if (start_y - 1.0f < ERROR_LIMIT){
                roads[roadIndex].isEdge = true;
            }else{
                roads[roadIndex].isEdge = false;
            }
            break;
        }
        case EAST:{
            if (start_x + 1.0f < ERROR_LIMIT){
                roads[roadIndex].isEdge = true;
            }else{
                roads[roadIndex].isEdge = false;
            }
            break;
        }
        case WEST:{
            if (start_x - 1.0f < ERROR_LIMIT){
                roads[roadIndex].isEdge = true;
            }else{
                roads[roadIndex].isEdge = false;
            }
            break;
        }
    }
}

GLvoid setRoad(GLint roadIndex, GLfloat start_x, GLfloat start_y, GLfloat length, DIRECTION dir)
{
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
            start_x + HALF_ROAD_WIDTH, start_y,          0.31f, 0.31f, 0.31f,
            start_x - HALF_ROAD_WIDTH, start_y,          0.31f, 0.31f, 0.31f,
            start_x + HALF_ROAD_WIDTH, start_y + length, 0.31f, 0.31f, 0.31f,
            start_x - HALF_ROAD_WIDTH, start_y + length, 0.31f, 0.31f, 0.31f
        };

        memcpy(&roadVertices[roadIndex * 4 * 5], verticies, sizeof(GLfloat) * 4 * 5);
    }

    else if (dir == SOUTH)
    {
        GLfloat verticies[] =
        {
            start_x + HALF_ROAD_WIDTH, start_y, 0.31f, 0.31f, 0.31f,
            start_x - HALF_ROAD_WIDTH, start_y, 0.31f, 0.31f, 0.31f,
            start_x + HALF_ROAD_WIDTH, start_y - length, 0.31f, 0.31f, 0.31f,
            start_x - HALF_ROAD_WIDTH, start_y - length, 0.31f, 0.31f, 0.31f
        };

        memcpy(&roadVertices[roadIndex * 4 * 5], verticies, sizeof(GLfloat) * 4 * 5);
    }

    else if (dir == EAST)
    {
        GLfloat verticies[] =
        {
            start_x, start_y + HALF_ROAD_WIDTH, 0.31f, 0.31f, 0.31f,
            start_x, start_y - HALF_ROAD_WIDTH, 0.31f, 0.31f, 0.31f,
            start_x + length, start_y + HALF_ROAD_WIDTH, 0.31f, 0.31f, 0.31f,
            start_x + length, start_y - HALF_ROAD_WIDTH, 0.31f, 0.31f, 0.31f
        };

        memcpy(&roadVertices[roadIndex * 4 * 5], verticies, sizeof(GLfloat) * 4 * 5);
    }

    else if (dir == WEST)
    {
        GLfloat vertices[] =
        {
            start_x, start_y + HALF_ROAD_WIDTH, 0.31f, 0.31f, 0.31f,
            start_x, start_y - HALF_ROAD_WIDTH, 0.31f, 0.31f, 0.31f,
            start_x - length, start_y + HALF_ROAD_WIDTH, 0.31f, 0.31f, 0.31f,
            start_x - length, start_y - HALF_ROAD_WIDTH, 0.31f, 0.31f, 0.31f
        };

        memcpy(&roadVertices[roadIndex * 4 * 5], vertices, sizeof(GLfloat) * 4 * 5);
    }
}

GLvoid setLines(GLint roadIndex)
{
    roads[roadIndex].lines = (line *)malloc(sizeof(line) * (NUMBER_OF_LINES + 1));

    if (roads[roadIndex].lines == NULL) {
        printf("malloc failed on lines on road %d", roadIndex);
        exit(1);
    }

    for (int i = 0; i < NUMBER_OF_LINES + 1; i++) {
        roads[roadIndex].lines[i].cells = (car **)malloc(sizeof(car *) * NUMBER_OF_CELLS);
        
        if (roads[roadIndex].lines[i].cells == NULL) {
            printf("malloc failed on cells on road %d line %d", roadIndex, i);
            exit(1);
        }
    }

    GLfloat stride = CELL_WIDTH;
    int i;

    if (roads[roadIndex].dir == NORTH || roads[roadIndex].dir == SOUTH)
    {
        GLfloat x, y1, y2;
        
        if (roads[roadIndex].dir == SOUTH) {
            stride *= -1;
            x = roadVertices[0 + 4 * 5 * roadIndex];
        }
        else 
            x = roadVertices[0 + 4 * 5 * roadIndex] - ROAD_WIDTH;

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

            memcpy(&lineVertices[i * 5 * 2 + roadIndex * 5 * NUMBER_OF_LINES * 2], vertices, sizeof(float) * 5 * 2);

            for (int cell = 0; cell < NUMBER_OF_CELLS; cell++) {
                roads[roadIndex].lines[i].cells[cell] = NULL;
            }

            roads[roadIndex].lines[i].carSpawnCoord = x;
        }

        if (MAP_TYPE == VERTICAL && midLinesCounter == 0) {
            float vertices[] =
            {
                x - stride * NUMBER_OF_LINES, y1, 0.0f, 0.0f, 0.0f,
                x - stride * NUMBER_OF_LINES, y2, 0.0f, 0.0f, 0.0f
            };

            memcpy(&lineVertices[NUMBER_OF_LINES * NUMBER_OF_ROADS * 5 * 2], vertices, sizeof(float) * 5 * 2);
            midLinesCounter++;
        }

        if (MAP_TYPE == CROSS && roadIndex < 4) {
            float vertices[] =
            {
                x - stride * NUMBER_OF_LINES, y1, 0.0f, 0.0f, 0.0f,
                x - stride * NUMBER_OF_LINES, y2, 0.0f, 0.0f, 0.0f
            };

            memcpy(&lineVertices[NUMBER_OF_LINES * NUMBER_OF_ROADS * 5 * 2 + midLinesCounter * 5 * 2], vertices, sizeof(float) * 5 * 2);
            midLinesCounter++;
        }

        if (MAP_TYPE == SEVERAL_CROSSES && (
                    roadIndex == 1 ||
                    roadIndex == 5 ||
                    roadIndex == 9 ||
                    roadIndex == 13 ||
                    roadIndex == 17 ||
                    roadIndex == 21
                    )) {
            float vertices[] =
            {
                x - stride * NUMBER_OF_LINES, y1, 0.0f, 0.0f, 0.0f,
                x - stride * NUMBER_OF_LINES, y2, 0.0f, 0.0f, 0.0f
            };

            memcpy(&lineVertices[NUMBER_OF_LINES * NUMBER_OF_ROADS * 5 * 2 + midLinesCounter * 5 * 2], vertices, sizeof(float) * 5 * 2);
            midLinesCounter++;
        }

        for (int cell = 0; cell < NUMBER_OF_CELLS; cell++) {
            roads[roadIndex].lines[i].cells[cell] = NULL;
        }

        roads[roadIndex].lines[i].carSpawnCoord = x + stride;    

        #ifdef DEBUG
        if (MAP_TYPE != CROSS) { 
            if (!dbgVerticesInit) {
                cellsVertices = malloc(sizeof(float) * (NUMBER_OF_LINES + 1) * NUMBER_OF_ROADS * NUMBER_OF_CELLS * 5 * 8);
                dbgVerticesInit = true;
            }

            DEFAULT_FOV = 60;

            stride = fabs(stride);
            x = roadVertices[0 + 4 * 5 * roadIndex] - ROAD_WIDTH;
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
                        x,              y1 + (j * CELL_LENGTH) * dir_multiplier,        1.0f, 0.0f, 0.0f, 
                        x,              y1 + ((j + 1) * CELL_LENGTH) * dir_multiplier,  1.0f, 0.0f, 0.0f, 
                        x - CELL_WIDTH, y1 + (j * CELL_LENGTH) * dir_multiplier,        1.0f, 0.0f, 0.0f, 
                        x - CELL_WIDTH, y1 + ((j + 1) * CELL_LENGTH) * dir_multiplier,  1.0f, 0.0f, 0.0f, 
                        x - CELL_WIDTH, y1 + (j * CELL_LENGTH) * dir_multiplier,        1.0f, 0.0f, 0.0f, 
                        x,              y1 + (j * CELL_LENGTH) * dir_multiplier,        1.0f, 0.0f, 0.0f, 
                        x - CELL_WIDTH, y1 + (j * CELL_LENGTH) * dir_multiplier,        1.0f, 0.0f, 0.0f, 
                        x,              y1 + (j * CELL_LENGTH) * dir_multiplier,        1.0f, 0.0f, 0.0f 
                    };        
                    int index = roadIndex * 5 * 8 * (NUMBER_OF_LINES + 1) * NUMBER_OF_CELLS + i * 5 * 8 * NUMBER_OF_CELLS + j * 5 * 8;
                    memcpy(&cellsVertices[index], cellVertices, sizeof(GLfloat) * 5 * 8);
                }
            }
        }
        #endif
    }
    
    if (roads[roadIndex].dir == EAST || roads[roadIndex].dir == WEST)
    {
        GLfloat y, x1, x2;

        if (roads[roadIndex].dir == EAST) {
            y = roadVertices[1 + 4 * 5 * roadIndex];
            stride *= -1;
        }
        else 
            y = roadVertices[1 + 4 * 5 * roadIndex] - ROAD_WIDTH;

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

            memcpy(&lineVertices[i * 5 * 2 + roadIndex * 5 * NUMBER_OF_LINES * 2], vertices, sizeof(float) * 5 * 2);

            for (int cell = 0; cell < NUMBER_OF_CELLS; cell++) {
                roads[roadIndex].lines[i].cells[cell] = NULL;
            }

            roads[roadIndex].lines[i].carSpawnCoord = y;
        }

        if (MAP_TYPE == HORIZONTAL && midLinesCounter == 0) {
            GLfloat vertices[] =
            {
                x1, y - stride * NUMBER_OF_LINES, 0.0f, 0.0f, 0.0f,
                x2, y - stride * NUMBER_OF_LINES, 0.0f, 0.0f, 0.0f
            };

            memcpy(&lineVertices[NUMBER_OF_LINES * NUMBER_OF_ROADS * 5 * 2], vertices, sizeof(float) * 5 * 2);
            midLinesCounter++;
        }

        if (MAP_TYPE == CROSS && roadIndex < 4) {
            GLfloat vertices[] =
            {
                x1, y - stride * NUMBER_OF_LINES, 0.0f, 0.0f, 0.0f,
                x2, y - stride * NUMBER_OF_LINES, 0.0f, 0.0f, 0.0f
            };

            memcpy(&lineVertices[NUMBER_OF_LINES * NUMBER_OF_ROADS * 5 * 2 + midLinesCounter * 5 * 2], vertices, sizeof(float) * 5 * 2);
            midLinesCounter++;
        }

        if (MAP_TYPE == SEVERAL_CROSSES && (
                    roadIndex == 2 ||
                    roadIndex == 6 ||
                    roadIndex == 10 ||
                    roadIndex == 14 ||
                    roadIndex == 18 ||
                    roadIndex == 22
                    )) {
            if (roadIndex == 6) {
                GLfloat vertices[] =
                {
                    x1, y + stride, 0.0f, 0.0f, 0.0f,
                    x2, y + stride, 0.0f, 0.0f, 0.0f
                };
                memcpy(&lineVertices[NUMBER_OF_LINES * NUMBER_OF_ROADS * 5 * 2 + midLinesCounter* 5 * 2], vertices, sizeof(float) * 5 * 2);
            } else {
                GLfloat vertices[] =
                {
                    x1, y - stride * NUMBER_OF_LINES, 0.0f, 0.0f, 0.0f,
                    x2, y - stride * NUMBER_OF_LINES, 0.0f, 0.0f, 0.0f
                };
                memcpy(&lineVertices[NUMBER_OF_LINES * NUMBER_OF_ROADS * 5 * 2 + midLinesCounter* 5 * 2], vertices, sizeof(float) * 5 * 2);
            }

            midLinesCounter++;
        }

        for (int cell = 0; cell < NUMBER_OF_CELLS; cell++) {
            roads[roadIndex].lines[i].cells[cell] = NULL;
        }

        roads[roadIndex].lines[i].carSpawnCoord = y + stride;
        
        #ifdef DEBUG
        if (MAP_TYPE != CROSS) {
            if (!dbgVerticesInit) {
                cellsVertices = malloc(sizeof(float) * (NUMBER_OF_LINES + 1) * NUMBER_OF_ROADS * NUMBER_OF_CELLS * 5 * 8);
                dbgVerticesInit = true;
            }

            DEFAULT_FOV = 60;
            stride = fabs(stride);
            y = roadVertices[1 + 4 * 5 * roadIndex] - ROAD_WIDTH;
            x1 = roadVertices[0 + 4 * 5 * roadIndex];
            x2 = roadVertices[10 + 4 * 5 * roadIndex];


            int dir_multiplier;
            if (roads[roadIndex].dir == EAST)
                dir_multiplier = 1;
            else
                dir_multiplier = -1;

            for (i = 0; i < NUMBER_OF_LINES + 1; i++) {
                y += stride;
                for(int j = 0; j < NUMBER_OF_CELLS; j++) {
                    float cellVertices[] = {
                        x1 + (j * CELL_LENGTH) * dir_multiplier,       y,                1.0f, 0.0f, 0.0f, 
                        x1 + ((j + 1) * CELL_LENGTH) * dir_multiplier, y,                1.0f, 0.0f, 0.0f, 
                        x1 + (j * CELL_LENGTH) * dir_multiplier,       y - CELL_WIDTH,   1.0f, 0.0f, 0.0f, 
                        x1 + ((j + 1) * CELL_LENGTH) * dir_multiplier, y - CELL_WIDTH,   1.0f, 0.0f, 0.0f, 
                        x1 + (j * CELL_LENGTH) * dir_multiplier,       y - CELL_WIDTH,   1.0f, 0.0f, 0.0f, 
                        x1 + (j * CELL_LENGTH) * dir_multiplier,       y,                1.0f, 0.0f, 0.0f, 
                        x1 + (j * CELL_LENGTH) * dir_multiplier,       y - CELL_WIDTH,   1.0f, 0.0f, 0.0f, 
                        x1 + (j * CELL_LENGTH) * dir_multiplier,       y,                1.0f, 0.0f, 0.0f 
                    };        
                    int index = roadIndex * 5 * 8 * (NUMBER_OF_LINES + 1) * NUMBER_OF_CELLS + i * 5 * 8 * NUMBER_OF_CELLS + j * 5 * 8;
                    memcpy(&cellsVertices[index], cellVertices, sizeof(GLfloat) * 5 * 8);
                }
            }
        }
        #endif
    }
}

GLint isFurhterThanEndLine(car* Car, road* Road)
{
    if (Road->dir == NORTH || Road->dir == EAST){
        if (Car->realPos >= Road->endLineCoord){
            //printf("isFurther\n");
            return 1;
        }else{
            // printf("realpos: %f, endLine: %f\n", Car->realPos, Road->endLineCoord);
            // printCarProperties(Car->currCell);
        }
    }else{
        if (Car->realPos <= Road->endLineCoord)
            return 1;
    }
    return 0;
}

GLint isOutOfScreenSpace(GLfloat realPos)
{
    if (realPos <= -1.0f || realPos >= 1.0f)
        return 1;
    return 0;
}

int getCarsNumOnLine(int roadIndex, int lineIndex) {
    int counter = 0;

    for (int i = 0; i < NUMBER_OF_CELLS; i++) {
        car * cellPtr = roads[roadIndex].lines[lineIndex].cells[i];

        if (cellPtr && cellPtr->isActive)
            counter++;
    }

    return counter;
}



GLvoid printRoadProperties(GLint i){

    road* r = &roads[i];
    printf("isBeginCross: %d\n isEndCross: %d\n nedCrossNum: %d\n\n",
    r->isBeginCross, r->isEndCross, r->endCrossNum);
}
