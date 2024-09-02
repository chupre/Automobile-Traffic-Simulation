#ifndef MAP_H
#define MAP_H

#define ONE_ROAD_N 1
#define ONE_ROAD_S 2
#define ONE_ROAD_W 3
#define ONE_ROAD_E 4
#define TWO_ROADS_NS 5
#define TWO_ROADS_WE 6
#define CROSS 7

typedef struct road road;
extern int NUMBER_OF_ROADS;
extern int NUMBER_OF_LINES;
extern int MAX_CARS;
extern int SPAWN_FREQUENCY;
extern float DEFAULT_FOV;

extern road* roads;
extern unsigned int roadVAO, roadVBO, roadEBO;
extern float* roadVertices;
extern int* roadIndices;
extern unsigned int lineVAO, lineVBO;
extern float* lineVertices;

void setMap(int map_type, int lines, int max_cars, int spawn_frequency);

#endif
