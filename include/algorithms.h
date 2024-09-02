#ifndef ALGO_H
#define ALGO_H

#include <macros.h>

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

#define MAX_VELOCITY (_7_VELOCITY)
#define NUMBER_OF_VELOCITY_TYPES (7)

#define VELOCITY_MULTIPLIER (CELL_LENGTH)

#define ERROR_LIMIT 0.0000001f
#define EMPTY -1

#define STEP_TIME 1.0f //seconds

#define MAX_CELL_DIGIT (NUMBER_OF_CELLS - 1)

#define NEXT_CELL_IS_ON_CROSS -45737

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

extern int MAX_LINE_DIGIT;
extern int MAX_ROAD_DIGIT;
extern car ** userCarsPtrs;
extern GLint innerUserCarsPtrsIndex;
extern GLint * overtakeCarsIndexes; 
extern GLint innerOvertakeCarsIndex;

extern RLC rouletteRLC;

extern car** ignoredBackCars;
extern GLint innerIgnoredBackCarsIndex;

extern RLC * carAddingQueue;
extern GLint innerCarAddingQueueIndex;

GLvoid initRoadCell(RLC *rlc, car* Car);

bool isToExcludeFormRoad(car* Car);
GLint getVelocityByRLC(RLC rlc);
GLvoid excludeFromMap(car* Car);
GLvoid thoughtsOfOneCar(car* Car);
GLint distanceToForthCar(RLC rlc, car** forthCar);
GLint distanceToBackCar(RLC rlc, car** backCar);

MOVING_TYPE checkChangeLineAbility(car* Car, RLC* rlc);
MOVING_TYPE isSafetyForthAndBack(car* Car, RLC rlc);
GLvoid update();
GLvoid stepRoad();
GLvoid spawnCars();

GLvoid appendInUserCarsPtrs(car* Car);
GLvoid clearUserCarsPtrs();
bool isInUserCarsPtrs(car* Car);

GLvoid appendInOvertakeCarsIndexes(GLint carIndex);
GLvoid clearOvertakeCarsIndedxes();

GLvoid appendInIgnoredBackCarsPtrs(car* Car);
GLvoid clearIgnoredBackCarsPtrs();
bool isInIgnoredBackCars(car* Car);

GLvoid rebindRoadCars(car* Car);
GLvoid reinitRoadCells(car* Car);

car* getCarPtr(RLC* rlc);
bool getCarByRoulette(car** Car);
bool rollRouletteRLC();

bool isFurtherThanEndCell(car* Car);
bool compareRLCs(RLC* rlc1, RLC* rlc2);

GLvoid processCarAddingQueue();
GLvoid addInRLCcarAddingQueue(RLC rlc);
bool isInCarAddingQueue(RLC rlc);
GLvoid clearCarAddingQueue();
bool isRLCsuitableForSettingCar(RLC rlc);

GLvoid printRLC(RLC rlc, char* string);
GLint isRLCbad(RLC rlc);
GLvoid printCarProperties(RLC rlc);
GLvoid printDir(DIRECTION dir);

#endif
