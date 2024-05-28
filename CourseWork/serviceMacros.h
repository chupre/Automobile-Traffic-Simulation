#pragma once
//..........................................................
#define STEP_TIME 1.0f //seconds
//..........................................................
#define CELL_SIZE 0.1f
//..........................................................
#define MAX_CARS 100

#define CAR_WIDTH (ROAD_WIDTH * 2.0f / ((GLfloat)NUMBER_OF_LINES + 1.0f) / 3.0f)
#define CAR_LENGHT (CAR_WIDTH * 1.7f)
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
#define VELOCITY_MULTIPLIER CELL_SIZE

//..........................................................
#define NUMBER_OF_LINES 5

#define CELL_LENGHT CELL_SIZE
#define ROAD_WIDTH CELL_SIZE 

#define ONE_ROAD_N 1
#define ONE_ROAD_S 2
#define ONE_ROAD_W 3
#define ONE_ROAD_E 4
#define TWO_ROADS_NS 5
#define TWO_ROADS_WE 6
#define MAP_TYPE TWO_ROADS_NS

#if MAP_TYPE == ONE_ROAD_N || MAP_TYPE == ONE_ROAD_S || MAP_TYPE == ONE_ROAD_W || MAP_TYPE == ONE_ROAD_E
    #define NUMBER_OF_ROADS 1
#elif MAP_TYPE == TWO_ROADS_NS || MAP_TYPE == TWO_ROADS_WE
    #define NUMBER_OF_ROADS 2
#endif	

#define ERROR_LIMIT 0.0000001f
#define EMPTY -1

//for some reason when it's 40 program crashes on 1920x1080 resolution 
//and cars spawn in the middle of the screen after window border is dragged
//I have no fucking clue why but it is what it is

#define NUMBER_OF_CELLS 20//(NUMBER_OF_VISIBLE_CELLS + NUMBER_OF_COVERED_CELLS)

#define FPS 60.0f
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
    GLint ID;
    DIRECTION target;
    DIRECTION overtake;
    GLfloat realPos;
    bool isActive;
    bool isAvaria;
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

GLint freeCars = MAX_CARS;
