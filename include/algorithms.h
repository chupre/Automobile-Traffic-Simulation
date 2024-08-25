#ifndef ALGO_H
#define ALGO_H

// External
#include <glad/glad.h>
#include <GLFW/glfw3.h>

///Standart
#include <stdbool.h>

// Custom modules
#include <rlc.h>
#include <cars.h>
#include <road.h>
#include <direction.h>

#define SPAWN_FRECUENCY 50
#define ANTI_SPAWN_FRECUENCY (100 - SPAWN_FRECUENCY)

#define _1_VELOCITY (1)
#define _2_VELOCITY (_1_VELOCITY * 2)
#define _3_VELOCITY (_1_VELOCITY * 3)
#define _4_VELOCITY (_1_VELOCITY * 4)
#define _5_VELOCITY (_1_VELOCITY * 5)
#define _6_VELOCITY (_1_VELOCITY * 6)
#define _7_VELOCITY (_1_VELOCITY * 7)

#define MAX_VELOCITY (_3_VELOCITY)
#define NUMBER_OF_VELOCITY_TYPES (3)

#define VELOCITY_MULTIPLIER (CELL_LENGTH)

#define ERROR_LIMIT 0.0000001f
#define EMPTY -1

#define STEP_TIME 1.0f //seconds

#define MAX_CELL_DIGIT (NUMBER_OF_CELLS - 1)
#define MAX_LINE_DIGIT (NUMBER_OF_LINES)
#define MAX_ROAD_DIGIT (NUMBER_OF_ROADS - 1)

typedef enum MOVING_TYPE MOVING_TYPE;
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


#define max_cars 1000

extern GLint bornCarsIndexes[max_cars];
extern GLint innerBornCarsIndex;
extern GLint overtakeCarsIndexes[max_cars]; 
extern GLint innerOvertakeCarsIndex;

extern RLC rouletteRLC;

extern car* ignoredBackCars[max_cars];
extern GLint innerIgnoredBackCarsIndex;

bool isToExclude(car* Car);
GLint getVelocityByRLC(RLC rlc);
GLvoid excludeFromMap(car* Car);
GLvoid thoughtsOfOneCar(car* Car);
GLint distanceToForthCar(RLC rlc);
GLint distanceToBackCar(RLC rlc);
GLint getPureBackDistance(car* Car, RLC rlc, car** backCar);

MOVING_TYPE checkChangeLineAbility(car* Car, RLC* rlc);
MOVING_TYPE isSafetyForthAndBack(car* Car, RLC rlc);
//bool _isSafetyForthAndBack(car* Car, RLC rlc);
GLvoid update();
GLvoid step();
GLvoid _step();
GLvoid spawnCars();

GLvoid appendInBornCarsIndexes(GLint carIndex);
GLvoid clearBornCarsIndedxes();
bool isInBornCars(GLint i);

GLvoid appendInOvertakeCarsIndexes(GLint carIndex);
GLvoid clearOvertakeCarsIndedxes();

GLvoid appendInIgnoredBackCarsPtrs(car* Car);
GLvoid clearIgnoredBackCarsPtrs();
bool isInIgnoredBackCars(car* Car);

GLvoid unbindCarPtrFromCell(car* Car);
GLvoid reinitCurrCellWithNextCell(car* Car);

car* getCarPtr(RLC* rlc);
bool getCarPtrByRoulette(car** Car);
bool rollRouletteRLC();

bool isFurtherThanEndCell(car* Car);


GLvoid printRLC(RLC rlc, char* string);
GLint isRLCbad(RLC rlc);
GLvoid printCarProperties(RLC rlc);
GLvoid printDir(DIRECTION dir);

/*
//turn on the corner
GLint isReadyToTurnRight(void);
//turn through the cross
GLint isReadyToTurnLeft(void);
GLint isReadyToChangeLine(void);
*/

#endif