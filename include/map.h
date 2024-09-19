#ifndef MAP_H
#define MAP_H

#include <glad/glad.h>


#define ONE_ROAD_N 1
#define ONE_ROAD_S 2
#define ONE_ROAD_W 3
#define ONE_ROAD_E 4
#define TWO_ROADS_NS 5
#define TWO_ROADS_WE 6
#define CROSS 7
#define SEVERAL_CROSSES 8

#define CELL_LENGTH 0.05f
#define CELL_WIDTH (CELL_LENGTH)

typedef struct cross cross;
typedef struct traffic_light traffic_light;
typedef struct road road;
typedef struct car car;

extern int NUMBER_OF_TRAFFIC_LIGHTS;
extern int NUMBER_OF_CROSSES;
extern int NUMBER_OF_ROADS;
extern int NUMBER_OF_LINES;
extern int NUMBER_OF_CELLS;
extern int MAP_TYPE;
extern int MAX_CARS;
extern int SPAWN_FREQUENCY;
extern float DEFAULT_FOV;
extern float ROAD_WIDTH;
extern float HALF_ROAD_WIDTH;

extern traffic_light* lights;
extern cross* crosses;
extern road* roads;
extern unsigned int roadVAO, roadVBO, roadEBO;
extern float* roadVertices;
extern int* roadIndices;
extern unsigned int lineVAO, lineVBO;
extern float* lineVertices;
extern car* cars;
extern GLint * densityData;

GLvoid setMap(int map_type, int lines, int max_cars, int spawn_frequency);
GLvoid setRoadsAroundCross(GLfloat start_x, GLfloat start_y);

#endif
