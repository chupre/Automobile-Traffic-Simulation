// External
#include <glad/glad.h>

// Standard
#include <stdbool.h>
#include <stdlib.h>

// Custom modules
#include <map.h>
#include <direction.h>
#include <road.h>

int NUMBER_OF_ROADS;
int NUMBER_OF_LINES;
int MAX_CARS;
int SPAWN_FREQUENCY;
float DEFAULT_FOV;

road* roads;
unsigned int roadVAO, roadVBO, roadEBO;
float* roadVertices;
int* roadIndices;
unsigned int lineVAO, lineVBO;
float* lineVertices;

//a road gets a direction, an edge state
void setMap(int map_type, int lines, int max_cars, int spawn_frequency)
{
    if (map_type == TWO_ROADS_NS || map_type == TWO_ROADS_WE || map_type == CROSS) {
        NUMBER_OF_ROADS = 2;
    } else {
        NUMBER_OF_ROADS = 1;
    }

    NUMBER_OF_LINES = lines;
    MAX_CARS = max_cars;
    SPAWN_FREQUENCY = spawn_frequency;

    roads = malloc(sizeof(road) * NUMBER_OF_ROADS);
    roadVertices = malloc(sizeof(float) * NUMBER_OF_ROADS * 4 * 5);
    roadIndices = malloc(sizeof(int) * NUMBER_OF_ROADS * 6);
    lineVertices = malloc(sizeof(float) * NUMBER_OF_LINES * NUMBER_OF_ROADS * 5 * 2);

	switch (map_type)
	{
	case ONE_ROAD_N:
        DEFAULT_FOV = 45.0f;
		addRoad(0, -0.5f, -1.0f, NORTH);
		// roads[0].isEndCross = true;
		break;

	case ONE_ROAD_S:
        DEFAULT_FOV = 45.0f;
		addRoad(0, 0.5f, 1.0f, SOUTH);
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
        NUMBER_OF_ROADS = 2;
        DEFAULT_FOV = 45.0f;
		addRoad(0, 0.4f, -1.0f, NORTH);
		addRoad(1, -0.4f, 1.0f, SOUTH);
		break;

	case TWO_ROADS_WE:
        NUMBER_OF_ROADS = 2;
        DEFAULT_FOV = 38.0f;
		addRoad(1, 1.0f, 0.4f, WEST);
		addRoad(0, -1.0f, -0.4f, EAST);
		// roads[1].isEndCross = true;
		break;

	case CROSS:
        NUMBER_OF_ROADS = 2;
        DEFAULT_FOV = 45.0f;
		// addRoad(0, ); // N
		// addRoad(1, ); // S
		// addRoad(2, ); // E
		// addRoad(3, ); // W
		// addRoad(4, ); // N
		// addRoad(5, ); // S
		// addRoad(6, ); // E
		// addRoad(7, ); // W
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
