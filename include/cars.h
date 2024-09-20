#ifndef CARS_H
#define CARS_H

#include <macros.h>

// Standard
#include <stdbool.h>

// External
#include <glad/glad.h>
#include <cglm/cglm.h>

// Custom modules
#include <direction.h>
#include <rlc.h>
#include <cross.h>

// #define CAR_WIDTH (ROAD_WIDTH * 2.0f / ((GLfloat)NUMBER_OF_LINES + 1.0f) / 3.0f)
#define CAR_WIDTH (CELL_LENGTH / 1)
#define CAR_LENGTH (CAR_WIDTH)
// #define CAR_LENGHT (CAR_WIDTH * 1.7f)

#define NO_INNER_INDEX 0
#define NO_CAR_INDEX -1

// typedef struct RLC RLC;
typedef enum DIRECTION DIRECTION;
typedef enum MOVING_TYPE MOVING_TYPE;
typedef enum VELOCITY VELOCITY;
typedef struct cross_cell cross_cell;
typedef struct car car;

enum MOVING_TYPE {
    FORWARD, SHIFT, OVERTAKE
};

struct car {
    cross_cell crossCurrCell;
    cross_cell crossNextCell;
    RLC currCell;
    RLC nextCell;
    GLint velocity;
    GLint roadDirMultiplier;
    GLint ID;
    DIRECTION target;
    MOVING_TYPE move;
    DIRECTION moveDir;
    GLfloat realPos;
    bool isActive;
    bool isCrushed;
    bool checked;
    cross_cell curvingCell;//the pole stays uninitializing after GLvoid clearCarProperties(car* Car)
};

extern mat3 carTransformMatrixes[1000];
extern GLuint carVAO, carVBO, carEBO, carInstanceVBO;
extern GLfloat carVertices[4 * 4];
extern GLint carIndices[6];
extern GLint freeCars;

extern car occupying_car;
extern car* OCCUPYING_CAR;

GLvoid addCar(car* Car, GLint carIndex, RLC rlc);
GLvoid addCrushedCar(RLC rlc);
GLvoid setBornCar(car* Car, GLint carIndex, RLC address);
GLvoid setCarByRLC(car* Car, GLint carIndex, RLC rlc);
GLvoid getFreeSpotAddress(RLC* rlc);
GLint getFreeCarIndex();
GLvoid setBornCarProperties(car* Car, GLint carIndex, RLC rlc);
GLvoid setCrushedCarProperties(car* Car, GLint carIndex, RLC rlc);
GLvoid setCarsToDefault();
GLvoid clearCarProperties(car* Car);

GLvoid bindCellAndCar(RLC* rlc, car* Car);
GLvoid setOccupyingCarProperties();
DIRECTION getOppositeDir(DIRECTION dir);
GLvoid setTarget(car* Car);

#endif
