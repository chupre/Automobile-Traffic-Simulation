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

#define MIN_SPAWN_VELOCITY _4_VELOCITY
#define MAX_VELOCITY (_9_VELOCITY)
#define NUMBER_OF_VELOCITY_TYPES (9)

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

extern bool alghorithmsInit;
extern RLC rouletteRLC;

extern car ** userCarsPtrs;
extern GLint innerUserCarsPtrsIndex;

extern RLC * carAddingQueue;
extern GLint innerCarAddingQueueIndex;

extern RLC * carAddingQueue_CRUSH;
extern GLint innerCarAddingQueueIndex_CRUSH;

extern car** skipCarsFromCross;
extern GLint innerSkipCarsFromCrossIndex;

extern car** checkedCars;
extern GLint innerCheckedCarsIndex;

extern int step_count;

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
GLvoid renewCells();
GLvoid stepRoad();
GLvoid spawnCars();

GLvoid appendInUserCarsPtrs(car* Car);
GLvoid clearUserCarsPtrs();
bool isInUserCarsPtrs(car* Car);

GLvoid appendInSkipCarsFromCross(car* Car);
GLvoid clearSkipCarsFromCross();
bool isInSkipCarsFromCross(car* Car);

GLvoid appendInCheckedCars(car* Car);
GLvoid clearCheckedCars();
bool isInCheckedCars(car* Car);

GLvoid rebindRoadCars(car* Car);

car* getCarPtr(RLC* rlc);
bool getCarByRoulette(car** Car);
bool rollRouletteRLC();

bool isFurtherThanEndCell(car* Car);
bool compareRLCs(RLC* rlc1, RLC* rlc2);

GLvoid processCarAddingQueue();
GLvoid processCarAddingQueue_CRUSH();

GLvoid appendRLCinCarAddingQueue_CRUSH(RLC rlc);
// bool isInCarAddingQueue(RLC rlc);
GLvoid clearCarAddingQueue_CRUSH();

GLvoid appendRLCinCarAddingQueue(RLC rlc);
bool isInCarAddingQueue(RLC rlc);
GLvoid clearCarAddingQueue();

bool isRLCsuitableForSettingCar(RLC rlc);

GLvoid printRLC(RLC rlc, char* string);
GLvoid printCrossCell(cross_cell c);
GLint isRLCbad(RLC rlc);
GLvoid printCarProperties(RLC rlc);
GLvoid printDir(DIRECTION dir);
GLvoid printSkipCars();
GLvoid printCheckedCars();
GLvoid printCar(car* Car);

#endif
