#ifndef ROAD_H
#define ROAD_H

// Standard
#include <stdbool.h>

// External
#include <glad/glad.h>

// Custom modules
#include <rlc.h>
#include <direction.h>
#include <map.h>

#define CELL_LENGTH 0.05f
// #define CELL_WIDTH ROAD_WIDTH * 2 / (NUMBER_OF_LINES + 1.0f)
#define CELL_WIDTH (CELL_LENGTH)
#define NUMBER_OF_CELLS 10

#define NUMBER_OF_LINES 4
#define ROAD_WIDTH (CELL_LENGTH * (NUMBER_OF_LINES + 1))
#define HALF_ROAD_WIDTH (ROAD_WIDTH / 2)

#define NO_ROAD_INDEX -1
#define NO_LINE_INDEX -1
#define NO_CELL_INDEX -1

// #define NUMBER_OF_ROADS 1

typedef enum DIRECTION DIRECTION;
typedef struct cross cross;
typedef struct line line;
typedef struct road road;
typedef struct car car;

struct line {
    car* cells[NUMBER_OF_CELLS];
    GLfloat carSpawnCoord;
};

struct road {
    bool isEdge;
    bool isBeginCross;
    bool isEndCross;
    cross* beginCross;
    cross* endCross;
    GLfloat stem;
    GLfloat startLineCoord;
    GLfloat endLineCoord;
    DIRECTION dir;
    line lines[NUMBER_OF_LINES + 1];
};

extern road roads[NUMBER_OF_ROADS];
extern GLuint roadVAO, roadVBO, roadEBO;
extern GLfloat roadVertices[NUMBER_OF_ROADS * 4 * 5];
extern GLint roadIndices[NUMBER_OF_ROADS * 6];
extern GLuint lineVAO, lineVBO;
extern GLfloat lineVertices[NUMBER_OF_LINES * NUMBER_OF_ROADS * 5 * 2];

GLvoid addRoad(GLint roadIndex, GLfloat start_x, GLfloat start_y, DIRECTION dir);
GLvoid setRoad(GLint roadIndex, GLfloat start_x, GLfloat start_y, GLfloat lenght, DIRECTION dir);
GLvoid setLines(GLint roadIndex);
GLint getDirMultiplier(DIRECTION dir);
DIRECTION getRoadDir(car* Car);
DIRECTION getLeftMoveDir(DIRECTION dir);
DIRECTION getRightMoveDir(DIRECTION dir);
car** getFirstCellPtr(RLC rlc);
GLvoid setRoadsToDefault();

GLvoid setRoadBoards(GLint roadIndex, GLfloat start_x, GLfloat start_y);
GLvoid setEdgeState(GLint roadIndex, GLfloat start_x, GLfloat start_y, DIRECTION dir);
GLint isFurhterThanEndLine(car* Car, road* Road);
GLint isOutOfScreenSpace(GLfloat realPos);
GLint getOvertakeDirForVelocity(DIRECTION overtake);

#endif