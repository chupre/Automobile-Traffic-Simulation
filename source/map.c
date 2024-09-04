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
int MAP_TYPE;
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
    
    MAP_TYPE = map_type;
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

    if (map_type == CROSS) {
        lineVertices = malloc(sizeof(float) * (NUMBER_OF_LINES * NUMBER_OF_ROADS * 5 * 2 + 4 * 5 * 2));
    } else 
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
        DEFAULT_FOV = 39.0f;
        GLfloat crossCenterX = 0.0f;
        GLfloat crossCenterY = 0.0f;
        setRoadsAroundCross(crossCenterX, crossCenterX);
		GLint enterRoadIndexes[] = {0, 1, 2, 3};
		GLint exitRoadIndexes[] = {4, 5, 6, 7};
		addCross(0, crossCenterX, crossCenterY, enterRoadIndexes, exitRoadIndexes);
	}

#ifdef DEBUG
    DEFAULT_FOV = 60.0f;
#endif

	for (int i = 0; i < NUMBER_OF_ROADS; i++)
	{
		setLines(i);
	}
}

GLvoid setRoadsAroundCross(GLfloat start_x, GLfloat start_y)
{   
    GLfloat roadLength = CELL_LENGTH * NUMBER_OF_CELLS;
    GLfloat halfRoadIndent = ((NUMBER_OF_LINES + 1) / 2) * CELL_LENGTH;
    GLfloat crossIndent = (NUMBER_OF_LINES + 1) * CELL_LENGTH;
    addRoad(0, start_x + halfRoadIndent,           start_y - crossIndent - roadLength, NORTH); // N;
    addRoad(1, start_x - halfRoadIndent,           start_y + crossIndent + roadLength, SOUTH); // S
    addRoad(2, start_x - crossIndent - roadLength, start_y - halfRoadIndent,            EAST); // E
    addRoad(3, start_x + crossIndent + roadLength, start_y + halfRoadIndent,            WEST); // W
    addRoad(4, start_x + halfRoadIndent,           start_y + crossIndent,              NORTH); // N
    addRoad(5, start_x - halfRoadIndent,           start_y - crossIndent,              SOUTH); // S
    addRoad(6, start_x + crossIndent,              start_y - halfRoadIndent,            EAST); // E
    addRoad(7, start_x - crossIndent,              start_y + halfRoadIndent,            WEST); // W
}
