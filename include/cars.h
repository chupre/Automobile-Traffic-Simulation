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

#define CAR_WIDTH (ROAD_WIDTH * 2.0f / ((GLfloat)NUMBER_OF_LINES + 1.0f) / 3.0f)
#define CAR_LENGHT (CAR_WIDTH * 1.7f)

#define MAX_CARS 500

typedef struct car car;

struct car {
    RLC currCell;
    RLC nextCell;
    GLint velocity;
    GLint dirOnRoad;
    GLint ID;
    DIRECTION target;
    DIRECTION overtake;
    GLfloat realPos;
    bool isActive;
    bool isAvaria;
};

extern mat3 carTransformMatrixes[MAX_CARS];
extern car cars[MAX_CARS];
extern GLuint carVAO, carVBO, carEBO, carInstanceVBO;
extern GLfloat carVertices[4 * 2];
extern GLint carIndices[6];
extern GLint freeCars;

GLvoid christenNewBornCar(RLC rlc, car* Car);
GLvoid setCarsToDefault();
GLint getFreeCarIndex();
GLvoid setCar(car* Car, GLint carIndex, RLC address);
GLvoid getFreeSpotAddress(RLC* address);
GLvoid spawnCars();

