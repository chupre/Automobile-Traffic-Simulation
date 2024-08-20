#pragma once

// External
#include <glad/glad.h>
#include <GLFW/glfw3.h>

///Standart
#include <stdbool.h>

// Custom modules
#include <rlc.h>
#include <road.h>

#define _1_VELOCITY (1)
#define _2_VELOCITY (_1_VELOCITY * 2)
#define _3_VELOCITY (_1_VELOCITY * 3)
#define _4_VELOCITY (_1_VELOCITY * 4)
#define _5_VELOCITY (_1_VELOCITY * 5)
#define _6_VELOCITY (_1_VELOCITY * 6)
#define _7_VELOCITY (_1_VELOCITY * 7)

#define MAX_VELOCITY (_6_VELOCITY)
#define NUMBER_OF_VELOCITY_TYPES (6)

#define VELOCITY_MULTIPLIER (CELL_LENGTH)

#define ERROR_LIMIT 0.0000001f
#define EMPTY -1

#define STEP_TIME 1.0f //seconds

typedef enum VELOCITY VELOCITY;
typedef struct car car;

enum VELOCITY {
	_0_CELL_ = 0,
	_1_CELL_ = _1_VELOCITY,
	_2_CELL_ = _2_VELOCITY,
	_3_CELL_ = _3_VELOCITY,
	_4_CELL_ = _4_VELOCITY,
	_5_CELL_ = _5_VELOCITY,
	_6_CELL_ = _6_VELOCITY,
	_7_CELL_ = _7_VELOCITY,
    _NO_CAR_ = 666
};

bool isToExclude(car* Car);
GLint getVelocityByRLC(RLC rlc);
GLvoid excludeFromMap(car* Car);
GLvoid thoughtsOfOneCar(car* Car);
GLint distanceToForthCar(RLC rlc);
GLint distanceToBackCar(RLC rlc);
bool isAbleToChangeLine(car* Car, RLC* posOnNewLine);
bool isSafetyForthAndBack(car* Car, RLC rlc);
GLvoid countSubVelocity(car* Car);
GLvoid update();
GLvoid step();
GLvoid spawnCars();
GLvoid appendInBornCarsIndexes(GLint freeCarIndex);
GLvoid clearInBornCarsIndedxes();
bool isInBornCars(GLint i);
GLvoid unbindCarPtrFromCell(car* Car);
GLvoid reinitCurrCellWithNextCell(car* Car);


GLint isRLCbad(RLC rlc);
GLvoid printCarProperties(RLC rlc);
/*
//turn on the corner
GLint isReadyToTurnRight(void);
//turn through the cross
GLint isReadyToTurnLeft(void);
GLint isReadyToChangeLine(void);
*/