#pragma once

// Standard
#include <stdbool.h>

// External
#include <glad/glad.h>

// Custom modules
#include <rlc.h>
#include <direction.h>
#include <map.h>
#include <algorithms.h>

#define ROAD_WIDTH CELL_SIZE * 1.5
#define NUMBER_OF_LINES 15

typedef enum DIRECTION DIRECTION;
typedef struct line line;
typedef struct road road;
typedef struct car car;

struct line {
    car* cells[NUMBER_OF_CELLS];
    GLfloat carSpawnCoord;
};

struct road {
    bool isEdge;
    DIRECTION dir;
    line lines[NUMBER_OF_LINES + 1];
};

extern road roads[NUMBER_OF_ROADS];
extern GLuint roadVAO, roadVBO, roadEBO;
extern GLfloat roadVertices[NUMBER_OF_ROADS * 4 * 5];
extern GLint roadIndices[NUMBER_OF_ROADS * 6];
extern GLuint lineVAO, lineVBO;
extern GLfloat lineVertices[NUMBER_OF_LINES * NUMBER_OF_ROADS * 5 * 2];

GLvoid setRoad(GLint roadIndex, GLfloat start_x, GLfloat start_y, GLfloat lenght, DIRECTION dir);
GLvoid setLines(GLint roadIndex);
DIRECTION getRoadDir(car* Сar);
DIRECTION getOvertakeDir(DIRECTION roadDir);
car** getFirstCellPtr(RLC rlc);
GLint getCarDirOnRoad(road* Road);
GLvoid unbindCarPtrFromCell(car* Сar);
GLvoid reinitCurrCellWithNextCell(car* Сar);
GLvoid setRoadsToDefault();

