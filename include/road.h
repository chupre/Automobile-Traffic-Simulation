#ifndef ROAD_H
#define ROAD_H

// Standard
#include <stdbool.h>

// External
#include <glad/glad.h>

// Custom modules
#include <rlc.h>
#include <direction.h>

#define NO_ROAD_INDEX -1
#define NO_LINE_INDEX -1
#define NO_CELL_INDEX -1


typedef enum DIRECTION DIRECTION;
typedef struct cross cross;
typedef struct line line;
typedef struct road road;
typedef struct car car;

struct line {
    car** cells;
    GLfloat carSpawnCoord;
};

struct road {
    bool isEdge;
    bool isBeginCross;
    bool isEndCross;
    GLint endCrossNum;
    cross* beginCross;
    cross* endCross;
    GLfloat stem;
    GLfloat startLineCoord;
    GLfloat endLineCoord;
    DIRECTION dir;
    line* lines;
};

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

#endif
