#pragma once
//..........................................................
#define STEP_TIME 1000 //seconds
#define NUMBER_OF_SUB_STEPS 1000 //milliseconds
#define SUB_STEP_TIME (STEP_TIME / NUMBER_OF_SUB_STEPS) //250 milli seconds
//..........................................................
#define CELL_SIZE 0.05f
//..........................................................
#define MAX_CARS 4
//..........................................................
#define _1_VELOCITY (1)
#define _2_VELOCITY (_1_VELOCITY * 2)
#define _3_VELOCITY (_1_VELOCITY * 3)
#define _4_VELOCITY (_1_VELOCITY * 4)
#define _5_VELOCITY (_1_VELOCITY * 5)

#define MAX_VELOCITY (_5_VELOCITY)
#define NUMBER_OF_VELOCITY_TYPES (5)

//#define DIMINISHER 0.01f //was 0.001f
//#define VELOCITY_MULTIPLIER (DIMINISHER * (CELL_SIZE / _1_VELOCITY)) /*-0.0005f*/
#define VELOCITY_MULTIPLIER (CELL_SIZE)

//..........................................................
#define NUMBER_OF_LINES 1

#define NUMBER_OF_VISIBLE_CELLS 20
#define NUMBER_OF_COVERED_CELLS (MAX_VELOCITY)
#define NUMBER_OF_CELLS 40//(NUMBER_OF_VISIBLE_CELLS + NUMBER_OF_COVERED_CELLS)
//..........................................................

typedef enum {
	_0_CELL = 0,
	_1_CELL = _1_VELOCITY,
	_2_CELL = _2_VELOCITY,
	_3_CELL = _3_VELOCITY,
	_4_CELL = _4_VELOCITY,
	_5_CELL = _5_VELOCITY,
    NO_CAR = 666,
} VELOCITY;//macros NO_CAR can give an error while passing into "glm_translate_y" in main

//...........................................................
#define NUMBER_OF_DIRECTIONS 4
typedef enum
{
    NORTH, SOUTH, EAST, WEST, NONE
} DIRECTION;

typedef struct
{
    GLint road;
    GLint line;
    GLint cell;
} RLC;

typedef struct
{
    RLC currCell;
    RLC nextCell;
    GLint velocity;
    GLint dirOnRoad;
    DIRECTION target;
    DIRECTION overtake;
    bool isActive;
    GLint isAvaria;
} car;

typedef struct
{
    car* cells[NUMBER_OF_CELLS];
    GLfloat carSpawnCoord;
} line;

typedef struct
{
    bool isEdge;
    DIRECTION dir;
    line lines[NUMBER_OF_LINES + 1];
} road;
//..................................................................
//global variable
GLint __freeCars__ = MAX_CARS;
