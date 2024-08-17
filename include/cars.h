#pragma once

// Standard
#include <stdbool.h>

// External
#include <glad/glad.h>
#include <cglm/cglm.h>

// Custom modules
#include <rlc.h>
#include <road.h>
#include <direction.h>

// #define CAR_WIDTH (ROAD_WIDTH * 2.0f / ((GLfloat)NUMBER_OF_LINES + 1.0f) / 3.0f)
#define CAR_WIDTH (CELL_LENGTH / 2)
#define CAR_LENGTH (CAR_WIDTH)
// #define CAR_LENGHT (CAR_WIDTH * 1.7f)

#define MAX_CARS 9

#define NO_INNER_INDEX -1
#define NO_CAR_INDEX -1

typedef struct car car;

struct car {
    RLC currCell;
    RLC nextCell;
    RLC crossCurrCell;
    RLC crossNextCell;
    GLint velocity;
    GLint dirOnRoad;
    GLint ID;
    DIRECTION target;
    DIRECTION overtake;
    GLfloat realPos;
    bool isActive;
    bool isCrushed;
};

extern mat3 carTransformMatrixes[MAX_CARS];
extern car cars[MAX_CARS];
extern GLuint carVAO, carVBO, carEBO, carInstanceVBO;
extern GLfloat carVertices[4 * 2];
extern GLint carIndices[6];
extern GLint freeCars;
extern GLint bornCarsIndexes[MAX_CARS];
extern GLint innerBornCarsIndex;


GLvoid addCar(car* Car, GLint carIndex, RLC rlc);
GLvoid addCrushedCar(RLC rlc);
GLvoid setCar(car* Car, GLint carIndex, RLC address);
GLvoid setCrushedCar(car* Car, GLint carIndex, RLC rlc);
GLvoid getFreeSpotAddress(RLC* rlc);
GLint getFreeCarIndex();
GLvoid spawnCars();
GLvoid setBornCarProperties(car* Car, GLint carIndex, RLC rlc);
GLvoid setCrushedCarProperties(car* Car, GLint carIndex, RLC rlc);
GLvoid setCarsToDefault();
GLvoid appendInBornCarsIndexes(GLint freeCarIndex);
GLvoid clearInBornCarsIndedxes();
GLint isInBornCars(GLint i);