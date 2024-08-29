#ifndef CROSS_H
#define CROSS_H
// External
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Custom modules
#include <algorithms.h>
// #include <cars.h>
#include <road.h>
// #include <rlc.h>
// #include <direction.h>

#define CROSS_SIDE ((NUMBER_OF_LINES + 1) * 2)
#define HALF_CROSS_SIDE (CROSS_SIDE / 2)

#define NUMBER_OF_CROSSES 10
#define NUMBER_OF_CROSS_ROADS 4
#define NUMBER_OF_CROSS_CELLS (CROSS_SIDE * CROSS_SIDE)

#define MAX_CROSS_NUM (NUMBER_OF_CROSSES)
#define MAX_CELL_NUM (CROSS_SIDE * CROSS_SIDE)

#define NEXT_CELL_IS_ON_ROAD -12312

#define CROSS_VELOCITY (_1_VELOCITY)

typedef struct car car;
typedef struct road road;
typedef struct cross cross;

typedef struct cross_roulette cross_roulette;

struct cross_cell {
    GLint x;
    GLint y;
    GLint crossNum;
};
typedef struct cross_cell cross_cell;
struct cross_roulette {
    GLint cellNum;
    GLint crossNum;
};

struct cross {
    road* enterConnectedRoads[NUMBER_OF_CROSS_ROADS]; //DIRECTION type values can be indexes of roads of corresponding directions
    road* exitConnectedRoads[NUMBER_OF_CROSS_ROADS];
    car* cells[NUMBER_OF_CROSS_CELLS];
};

extern cross crosses[NUMBER_OF_CROSSES];
extern cross_roulette rouletteCross;

GLvoid setCross(GLint crossIndex, GLint* roadIndexes);
GLvoid setCarPropertiesOnCross(car* Car);
GLvoid setCarTurningProperties(car* Car);
GLvoid getCrossExitAndRoadFirstCell(GLint enter, DIRECTION dir, DIRECTION target, GLint* exit, GLint* roadFirstCell);


car* getCarByCrossCell(cross_cell c);
GLvoid initCrossCell(cross_cell* c, car* Car);
bool isEndedWithCross(RLC rlc);

bool getCarByRouletteCross(car** Car);
bool rollRouletteCross();
GLvoid stepCross();

GLvoid rebindCrossCars(car* Car);
GLvoid reinitCrossCells(car* Car);


#endif