#ifndef MAP_H
#define MAP_H

#define ONE_ROAD_N 1
#define ONE_ROAD_S 2
#define ONE_ROAD_W 3
#define ONE_ROAD_E 4
#define TWO_ROADS_NS 5
#define TWO_ROADS_WE 6
#define CROSS 7
#define MAP_TYPE (6)

#if MAP_TYPE == ONE_ROAD_N || MAP_TYPE == ONE_ROAD_S || MAP_TYPE == ONE_ROAD_W || MAP_TYPE == ONE_ROAD_E
    #define NUMBER_OF_ROADS 1
#elif MAP_TYPE == TWO_ROADS_NS || MAP_TYPE == TWO_ROADS_WE || MAP_TYPE == CROSS
    #define NUMBER_OF_ROADS 2
#endif

//because debug has its own FOV
#ifndef DEFAULT_POV 
    #if MAP_TYPE == ONE_ROAD_N || MAP_TYPE == ONE_ROAD_S || MAP_TYPE == TWO_ROADS_NS
        #define DEFAULT_FOV 45.0f
    #else
        #define DEFAULT_FOV 40.0f
    #endif
#endif

GLvoid setMap();

#endif
