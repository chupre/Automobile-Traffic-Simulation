#ifndef CROSS_H
#define CROSS_H

#include <macros.h>

// External
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Custom modules
#include <algorithms.h>
#include <traffic_light.h>

#define CROSS_SIDE ((NUMBER_OF_LINES + 1) * 2)
#define HALF_CROSS_SIDE (CROSS_SIDE / 2)

#define NUMBER_OF_CROSS_ROADS 4
#define NUMBER_OF_CROSS_CELLS (CROSS_SIDE * CROSS_SIDE)

#define MAX_CROSS_NUM (NUMBER_OF_CROSSES)
#define MAX_CELL_NUM (CROSS_SIDE * CROSS_SIDE)

#define NEXT_CELL_IS_ON_ROAD -12312

#define CROSS_VELOCITY (_1_VELOCITY)

#define NOTHING -1

typedef struct car car;
typedef struct road road;

typedef enum DIRECTION DIRECTION;
typedef struct cross_cell cross_cell;
typedef struct cross_roulette cross_roulette;
typedef struct cross cross;
typedef struct q_item q_item;
typedef struct queue queue;
typedef enum cross_flood cross_flood;

struct q_item {
    car* value;
    q_item* next;
};

struct queue {
    q_item* head;
    q_item* tail;
    GLint qauntity;
};

struct cross_cell {
    GLint x;
    GLint y;
    GLint crossNum;
};
struct cross_roulette {
    GLint cellNum;
    GLint crossNum;
};

struct cross {
    GLint enterRoadsIndexes[NUMBER_OF_CROSS_ROADS];
    GLint exitRoadsIndexes[NUMBER_OF_CROSS_ROADS];
    road* enterConnectedRoads[NUMBER_OF_CROSS_ROADS]; //DIRECTION type values can be indexes of roads of corresponding directions
    road* exitConnectedRoads[NUMBER_OF_CROSS_ROADS];
    car* cells[NUMBER_OF_CROSS_CELLS];
    queue carsEndingManeuver;
    queue carsArriving;
    GLint maneuverNumber;
    GLint arrivingNumber;
};

extern cross crosses[NUMBER_OF_CROSSES];
extern cross_roulette rouletteCross;

#define TWO 2
extern DIRECTION crossQuaters[NUMBER_OF_DIRECTIONS * TWO];
enum cross_flood {
    ENTER, EXIT
};

GLvoid q_append(car* Car, queue* q);
car* q_pop(queue* q);
GLvoid q_delete(queue* q);
GLvoid q_fill(queue* dest, queue* src);

GLvoid setCrossProperties(GLint crossIndex, GLint* roadIndexes);
GLvoid setCarPropertiesOnCross(car* Car);
GLvoid setCarTurningProperties(car* Car);

GLint getCrossEnter(GLint cell, DIRECTION dir);
GLint getCrossExit(GLint cell, DIRECTION dir);
GLvoid getCrossExitAndRoadFirstCell(GLint enter, DIRECTION dir, DIRECTION target, GLint* exit, GLint* roadFirstCell);

GLint getQuaterNum(cross_cell c);
cross_flood getCodirectional(DIRECTION carDir, GLint quaterNum);

car* getCarByCrossCell(cross_cell* c);
GLvoid initCrossCell(cross_cell* c, car* Car);
bool isEndedWithCross(RLC* rlc);

bool getCarByRouletteCross(car** Car);
bool rollRouletteCross();
GLvoid stepCross();

GLvoid rebindCrossCars(car* Car);
GLvoid reinitCrossCells(car* Car);



GLvoid thoughtsOfOneCarOnCross(car* Car);
GLvoid getNextCrossCell(car* Car, cross_cell* c);
bool isInCrossBoards(cross_cell c);
bool isSafetyLeft(cross_cell c, car* Car);
bool isSafetyRight(cross_cell c, car* Car);
bool isSafetyFront(cross_cell cell);
GLvoid transformCrossCellIntoRLC(RLC* rlc, car* Car);
GLvoid transformRLCIntoCrossCell(cross_cell* c, car* Car);
GLint getLineOfAppearingOnRoadFromCross(car* Car);

#endif
