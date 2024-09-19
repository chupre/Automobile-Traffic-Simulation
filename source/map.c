// External
#include <glad/glad.h>

// Standard
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// Custom modules
#include <macros.h>
#include <map.h>
#include <direction.h>
#include <road.h>
#include <cross.h>
#include <cars.h>
#include <traffic_light.h>

int NUMBER_OF_TRAFFIC_LIGHTS;
int NUMBER_OF_CROSSES;
int NUMBER_OF_CROSS_CELLS;
int NUMBER_OF_ROADS;
int NUMBER_OF_CELLS;
int NUMBER_OF_LINES;
int MAX_CARS;
int SPAWN_FREQUENCY;
int MAP_TYPE;
float DEFAULT_FOV;
float ROAD_WIDTH;
float HALF_ROAD_WIDTH;

traffic_light* lights;
cross* crosses;
road* roads;
unsigned int roadVAO, roadVBO, roadEBO;
float* roadVertices;
int* roadIndices;
unsigned int lineVAO, lineVBO;
float* lineVertices;
car * cars;

//a road gets a direction, an edge state
void setMap(int map_type, int lines, int max_cars, int spawn_frequency)
{
    if (map_type == TWO_ROADS_NS || map_type == TWO_ROADS_WE) {
        NUMBER_OF_ROADS = 2;
        NUMBER_OF_CELLS = 40;
    } else if (map_type == CROSS) {
        NUMBER_OF_CROSSES = 1;
        NUMBER_OF_TRAFFIC_LIGHTS = 4 * NUMBER_OF_CROSSES;
        NUMBER_OF_ROADS = 8;
        NUMBER_OF_CELLS = 12;
    }
    else if (map_type == SEVERAL_CROSSES){
        NUMBER_OF_CROSSES = 4;
        NUMBER_OF_TRAFFIC_LIGHTS = 4 * NUMBER_OF_CROSSES;
        NUMBER_OF_ROADS = 24;
        NUMBER_OF_CELLS = 12;
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

    if (map_type == CROSS || map_type == SEVERAL_CROSSES){
        crosses = malloc(sizeof(cross) * NUMBER_OF_CROSSES);
        if (crosses == NULL) {
            printf("malloc failed on crosses");
            exit(1);
        }

        lights = malloc(sizeof(traffic_light) * NUMBER_OF_TRAFFIC_LIGHTS);
        if (lights == NULL) {
            printf("malloc failed on lights");
            exit(1);
        }
    }

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

    cars = malloc(sizeof(car) * MAX_CARS);

    if (cars == NULL) {
        printf("malloc failed on cars");
        exit(1);
    }

    if (map_type == CROSS) 
        lineVertices = malloc(sizeof(float) * (NUMBER_OF_LINES * NUMBER_OF_ROADS * 5 * 2 + 4 * 5 * 2));
    else 
        lineVertices = malloc(sizeof(float) * NUMBER_OF_LINES * NUMBER_OF_ROADS * 5 * 2);

    if (lineVertices == NULL) {
        printf("malloc failed on lineVertices");
        exit(1);
    }

	switch (map_type)
	{
	case ONE_ROAD_N:
        DEFAULT_FOV = 45.0f;
		addRoad(0, HALF_ROAD_WIDTH, -1.0f, NORTH);
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
		addRoad(0, HALF_ROAD_WIDTH + 0.005f, -1.0f, NORTH);
		addRoad(1, -HALF_ROAD_WIDTH - 0.005f, 1.0f, SOUTH);
		break;

	case TWO_ROADS_WE:
        DEFAULT_FOV = 38.0f;
		addRoad(1, 1.0f, HALF_ROAD_WIDTH, WEST);
		addRoad(0, -1.0f, -HALF_ROAD_WIDTH, EAST);
		break;

	case CROSS:
        DEFAULT_FOV = 50.0f + NUMBER_OF_LINES;
        GLfloat crossCenterX = 0.0f;
        GLfloat crossCenterY = 0.0f;
        setRoadsAroundCross(crossCenterX, crossCenterX);
		GLint enterRoadIndexes[] = {0, 1, 2, 3};
		GLint exitRoadIndexes[] = {4, 5, 6, 7};
		addCross(0, crossCenterX, crossCenterY, enterRoadIndexes, exitRoadIndexes);
        break;

    case SEVERAL_CROSSES:
        DEFAULT_FOV = 70.0f;
        GLfloat roadLength = NUMBER_OF_CELLS * CELL_LENGTH;
        GLfloat roadLengthHalf = roadLength / 2;
        GLfloat roadWidth = (NUMBER_OF_LINES + 1) * CELL_LENGTH;
        GLfloat crossIndent = roadLengthHalf + roadWidth;
        GLfloat innerRoadHalf = roadLengthHalf + roadWidth / 2;
        GLfloat outerRoadHalf = roadLengthHalf + roadWidth / 2 + roadWidth;
        GLfloat nearRoadStart = roadLengthHalf + roadWidth * 2;
        GLfloat farRoadStart = roadLengthHalf + roadWidth * 2 + roadLength;

        GLint enterRoadIndexes0[] = {4,  9,  18, 23};
        GLint enterRoadIndexes1[] = {16, 21, 14, 19};
        GLint enterRoadIndexes2[] = {12, 17, 2,   7};
        GLint enterRoadIndexes3[] = {0,  5,  6,  11};
        
        GLint exitRoadIndexes0[] = { 8,  5, 22, 19};
        GLint exitRoadIndexes1[] = {20, 17, 18, 15};
        GLint exitRoadIndexes2[] = {16, 13,  6,  3};
        GLint exitRoadIndexes3[] = { 4,  1, 10,  7};

        
        addRoad(0, outerRoadHalf, -farRoadStart, NORTH);
        addRoad(1, innerRoadHalf, -nearRoadStart, SOUTH);
        addRoad(2, -farRoadStart, -outerRoadHalf, EAST);
        addRoad(3, -nearRoadStart, -innerRoadHalf, WEST);

        addRoad(4, outerRoadHalf, -roadLengthHalf, NORTH);
        addRoad(5, innerRoadHalf, roadLengthHalf, SOUTH);
        addRoad(6, -roadLengthHalf, -innerRoadHalf, EAST);
        addRoad(7, roadLengthHalf, -outerRoadHalf, WEST);

        addRoad(8, outerRoadHalf, nearRoadStart, NORTH);
        addRoad(9, innerRoadHalf, farRoadStart, SOUTH);
        addRoad(10, nearRoadStart, -outerRoadHalf, EAST);
        addRoad(11, farRoadStart, -innerRoadHalf, WEST);

        addRoad(12, -innerRoadHalf, -farRoadStart, NORTH);
        addRoad(13, -outerRoadHalf, -nearRoadStart, SOUTH);
        addRoad(14, -farRoadStart, innerRoadHalf, EAST);
        addRoad(15, -nearRoadStart, outerRoadHalf, WEST);

        addRoad(16, -innerRoadHalf, -roadLengthHalf, NORTH);
        addRoad(17, -outerRoadHalf, roadLengthHalf, SOUTH);
        addRoad(18, -roadLengthHalf, innerRoadHalf, EAST);
        addRoad(19, roadLengthHalf, outerRoadHalf, WEST);

        addRoad(20, -innerRoadHalf, nearRoadStart, NORTH);
        addRoad(21, -outerRoadHalf, farRoadStart, SOUTH);
        addRoad(22, nearRoadStart, innerRoadHalf, EAST);
        addRoad(23, farRoadStart, outerRoadHalf, WEST);


        addCross(0, crossIndent, crossIndent, enterRoadIndexes0, exitRoadIndexes0);
        addCross(1, -crossIndent, crossIndent, enterRoadIndexes1, exitRoadIndexes1);
        addCross(2, -crossIndent, -crossIndent, enterRoadIndexes2, exitRoadIndexes2);
        addCross(3, crossIndent, -crossIndent, enterRoadIndexes3, exitRoadIndexes3);
        break;
	}
    

#ifdef DEBUG
    DEFAULT_FOV = 60.0f;
#endif

	for (int i = 0; i < NUMBER_OF_ROADS; i++)
	{
		setLines(i);
	}
}
//tempt foo. will be used only for the map with one cross.
GLvoid setRoadsAroundCross(GLfloat start_x, GLfloat start_y)
{   
    GLfloat roadLength = CELL_LENGTH * NUMBER_OF_CELLS;
    GLfloat halfRoadIndent = ((GLfloat)(NUMBER_OF_LINES + 1) / 2) * CELL_LENGTH;
    GLfloat crossIndent = (NUMBER_OF_LINES + 1) * CELL_LENGTH;
    addRoad(0, start_x + halfRoadIndent,           start_y - crossIndent - roadLength, NORTH);
    addRoad(1, start_x - halfRoadIndent,           start_y + crossIndent + roadLength, SOUTH);
    addRoad(2, start_x - crossIndent - roadLength, start_y - halfRoadIndent,            EAST);
    addRoad(3, start_x + crossIndent + roadLength, start_y + halfRoadIndent,            WEST);
    addRoad(4, start_x + halfRoadIndent,           start_y + crossIndent,              NORTH);
    addRoad(5, start_x - halfRoadIndent,           start_y - crossIndent,              SOUTH);
    addRoad(6, start_x + crossIndent,              start_y - halfRoadIndent,            EAST);
    addRoad(7, start_x - crossIndent,              start_y + halfRoadIndent,            WEST);
}
