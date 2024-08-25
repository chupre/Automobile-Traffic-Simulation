#ifndef CARS_H
#define CARS_H

// Standard
#include <stdbool.h>

// External
#include <glad/glad.h>
#include <cglm/cglm.h>

// Custom modules
#include <algorithms.h>
#include <rlc.h>
#include <direction.h>

// #define CAR_WIDTH (ROAD_WIDTH * 2.0f / ((GLfloat)NUMBER_OF_LINES + 1.0f) / 3.0f)
#define CAR_WIDTH (CELL_LENGTH / 1)
#define CAR_LENGTH (CAR_WIDTH)
// #define CAR_LENGHT (CAR_WIDTH * 1.7f)

#define MAX_CARS 1000

#define NO_INNER_INDEX -1
#define NO_CAR_INDEX -1

typedef enum DIRECTION DIRECTION;
typedef enum MOVING_TYPE MOVING_TYPE;
typedef enum VELOCITY VELOCITY;

enum MOVING_TYPE {
    FORWARD, SHIFT, OVERTAKE
};

typedef struct car car;

struct car {
    RLC currCell;
    RLC nextCell;
    RLC crossCurrCell;//struct must be another than RLC
    RLC crossNextCell;
    GLint velocity;
    GLint roadDirMultiplier;
    GLint ID;
    DIRECTION target;
    DIRECTION overtake;
    MOVING_TYPE move;
    DIRECTION moveDir;
    GLfloat realPos;
    bool isActive;
    bool isCrushed;
    bool markRight;
};

extern mat3 carTransformMatrixes[MAX_CARS];
extern car cars[MAX_CARS];
extern GLuint carVAO, carVBO, carEBO, carInstanceVBO;
extern GLfloat carVertices[4 * 2];
extern GLint carIndices[6];
extern GLint freeCars;

extern car occupying_car;
extern car* OCCUPYING_CAR;


GLvoid addCar(car* Car, GLint carIndex, RLC rlc);
GLvoid addCrushedCar(RLC rlc);
GLvoid setCar(car* Car, GLint carIndex, RLC address);
GLvoid setCrushedCar(car* Car, GLint carIndex, RLC rlc);
GLvoid getFreeSpotAddress(RLC* rlc);
GLint getFreeCarIndex();
GLvoid setBornCarProperties(car* Car, GLint carIndex, RLC rlc);
GLvoid setCrushedCarProperties(car* Car, GLint carIndex, RLC rlc);
GLvoid setCarsToDefault();
GLvoid clearCarProperties(car* Car);

GLvoid bindCellAndCar(RLC* rlc, car* Car);
GLvoid setOccupyingCarProperties();
