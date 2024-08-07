#pragma once

// External
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Custom modules
#include <rlc.h>

#define CELL_SIZE 0.1f
#define CELL_LENGHT CELL_SIZE
#define NUMBER_OF_CELLS 20

#define MAX_VELOCITY (_5_VELOCITY)
#define NUMBER_OF_VELOCITY_TYPES (5)

#define _1_VELOCITY (1)
#define _2_VELOCITY (_1_VELOCITY * 2)
#define _3_VELOCITY (_1_VELOCITY * 3)
#define _4_VELOCITY (_1_VELOCITY * 4)
#define _5_VELOCITY (_1_VELOCITY * 5)

#define VELOCITY_MULTIPLIER CELL_SIZE

#define ERROR_LIMIT 0.0000001f
#define EMPTY -1

#define STEP_TIME 0.5f //seconds

typedef enum VELOCITY VELOCITY;
typedef struct car car;

enum VELOCITY {
	_0_CELL = 0,
	_1_CELL = _1_VELOCITY,
	_2_CELL = _2_VELOCITY,
	_3_CELL = _3_VELOCITY,
	_4_CELL = _4_VELOCITY,
	_5_CELL = _5_VELOCITY,
    NO_CAR = 666,
};

GLint getVelocityByRLC(RLC rlc);
GLvoid excludeOutMappers(car* Car);
GLvoid thoughtsOfOneCar(car* Car);
GLint distanceToForthCar(RLC rlc);
GLint distanceToBackCar(RLC rlc);
GLint isAbleToChangeLine(car* Car, RLC* posOnNewLine);
GLint isSafetyForthAndBack(car* Car, RLC rlc);
GLvoid countSubVelocity(car* Car);
GLvoid update();
GLvoid step();

/*
//turn on the corner
GLint isReadyToTurnRight(void);
//turn through the cross
GLint isReadyToTurnLeft(void);
GLint isReadyToChangeLine(void);
*/