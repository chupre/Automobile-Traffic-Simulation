#pragma once

// External
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Custom modules
#include <map.h>
#include <rlc.h>
#include <cars.h>
#include <road.h>
#include <direction.h>

#define NUMBER_OF_CROSSES 10
#define NUMBER_OF_CROSS_ROADS 4

typedef struct car car;
typedef struct road road;
typedef struct cross cross;

struct cross {
    road* connectedRoads[NUMBER_OF_CROSS_ROADS]; //DIRECTION type values can be indexes of roads of corresponding directions
    
};

extern cross crosses[NUMBER_OF_CROSSES];

GLvoid setCross(GLint crossIndex, GLint* roadIndexes);
GLvoid setCarPropertiesOnCross(car* Car);
GLvoid setCarTurningProperties(car* Car);