// External
#include <glad/glad.h>

// Standard
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// Custom modules
#include <map.h>
#include <direction.h>
#include <road.h>
#include <cross.h>

int NUMBER_OF_ROADS;
int NUMBER_OF_CELLS;
int NUMBER_OF_LINES;
int MAX_CARS;
int SPAWN_FREQUENCY;
float DEFAULT_FOV;
float ROAD_WIDTH;
float HALF_ROAD_WIDTH;

road* roads;
unsigned int roadVAO, roadVBO, roadEBO;
float* roadVertices;
int* roadIndices;
unsigned int lineVAO, lineVBO;
float* lineVertices;

//a road gets a direction, an edge state
void setMap(int map_type, int lines, int max_cars, int spawn_frequency)
{
    if (map_type == TWO_ROADS_NS || map_type == TWO_ROADS_WE) {
        NUMBER_OF_ROADS = 2;
        NUMBER_OF_CELLS = 40;
    } else if (map_type == CROSS) {
        NUMBER_OF_CELLS = 10;
        NUMBER_OF_ROADS = 8;
    } else {
        NUMBER_OF_ROADS = 1;
        NUMBER_OF_CELLS = 40;
    }

    NUMBER_OF_LINES = lines;
    MAX_CARS = max_cars;
    SPAWN_FREQUENCY = spawn_frequency;
    ROAD_WIDTH = CELL_LENGTH * (NUMBER_OF_LINES + 1);
    HALF_ROAD_WIDTH = ROAD_WIDTH / 2;

    roads = malloc(sizeof(road) * NUMBER_OF_ROADS);

    if (roads == NULL) {
        printf("malloc failed on roads");
        exit(1);
    }

    roadVertices = malloc(sizeof(float) * NUMBER_OF_ROADS * 4 * 5);

    if (roadVertices == NULL) {
        printf("malloc failed on roadVertices");
        exit(1);
    }

    roadIndices = malloc(sizeof(int) * NUMBER_OF_ROADS * 6);

    if (roadIndices == NULL) {
        printf("malloc failed on roadIndices");
        exit(1);
    }

    lineVertices = malloc(sizeof(float) * NUMBER_OF_LINES * NUMBER_OF_ROADS * 5 * 2);

    if (lineVertices == NULL) {
        printf("malloc failed on lineVertices");
        exit(1);
    }

	switch (map_type)
	{
	case ONE_ROAD_N:
        DEFAULT_FOV = 45.0f;
		addRoad(0, 0, -1.0f, NORTH);
		break;

	case ONE_ROAD_S:
        DEFAULT_FOV = 45.0f;
		addRoad(0, 0, 1.0f, SOUTH);
		break;

	case ONE_ROAD_W:
        DEFAULT_FOV = 38.0f;
		addRoad(0, 1.0f, 0.0f, WEST);
		break;

	case ONE_ROAD_E:
        DEFAULT_FOV = 38.0f;
		addRoad(0, -1.0f, 0.0f, EAST);
		break;

	case TWO_ROADS_NS:
        DEFAULT_FOV = 45.0f;
		addRoad(0, HALF_ROAD_WIDTH, -1.0f, NORTH);
		addRoad(1, -HALF_ROAD_WIDTH - 0.1f, 1.0f, SOUTH);
		break;

	case TWO_ROADS_WE:
        DEFAULT_FOV = 38.0f;
		addRoad(1, 1.0f, CELL_WIDTH * (NUMBER_OF_LINES + 1) / 2, WEST);
		addRoad(0, -1.0f, -CELL_WIDTH * (NUMBER_OF_LINES + 1) / 2 - 0.1f, EAST);
		break;

	case CROSS:
        DEFAULT_FOV = 45.0f;
		addRoad(0, 0.1f, -0.7f, NORTH); // N;
		addRoad(1, -0.1f, 0.7f, SOUTH); // S
		addRoad(2, -0.7f, -0.1f , EAST); // E
		addRoad(3, 0.7f, 0.1f, WEST); // W
		addRoad(4, -0.1f, -0.7f, NORTH); // N
		addRoad(5, 0.1f,  0.7f, SOUTH); // S
		addRoad(6, -0.7f, 0.1f, EAST); // E
		addRoad(7, 0.7f, -0.1f, WEST); // W
		GLint enterRoadIndexes[] = {0, 1, 2, 3};
		GLint exitRoadIndexes[] = {4, 5, 6, 7};
		addCross(0, 0.0f, 0.0f, enterRoadIndexes, exitRoadIndexes);
	}

#ifdef DEBUG
    DEFAULT_FOV = 60.0f;
#endif

	for (int i = 0; i < NUMBER_OF_ROADS; i++)
	{
		setLines(i);
	}
}
