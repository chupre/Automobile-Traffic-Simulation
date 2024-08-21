#ifndef CROSS_H
#define CROSS_H
// External
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Custom modules
#include <map.h>
#include <rlc.h>
#include <direction.h>

#define NUMBER_OF_LI 9
#define CROSS_SIDE ((NUMBER_OF_LI + 1) * 2)
#define HALF_CROSS_SIDE (CROSS_SIDE / 2)

#define NUMBER_OF_CROSSES 10
#define NUMBER_OF_CROSS_ROADS 4

#define CROSS_VELOCITY (1) // _1_VELOCITY

typedef struct car car;
typedef struct road road;
typedef struct cross cross;

struct cross {
    road* enterConnectedRoads[NUMBER_OF_CROSS_ROADS]; //DIRECTION type values can be indexes of roads of corresponding directions
    road* exitConnectedRoads[NUMBER_OF_CROSS_ROADS];
    
};

extern cross crosses[NUMBER_OF_CROSSES];
extern GLint crossTable[4][NUMBER_OF_LI * 2][NUMBER_OF_LI];


GLvoid setCross(GLint crossIndex, GLint* roadIndexes);
GLvoid setCarPropertiesOnCross(car* Car);
GLvoid setCarTurningProperties(car* Car);
GLvoid getCrossExitAndRoadFirstCell(GLint enter, DIRECTION dir, DIRECTION target, GLint* exit, GLint* roadFirstCell);

bool isEndCross(RLC rlc);

#endif