#pragma once
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

GLvoid setMap(road* roads, GLfloat* roadVertices, GLint* roadIndices, GLfloat* lineVertices);

//a road gets a direction, an edge state
GLvoid setMap(road* roads, GLfloat* roadVertices, GLint* roadIndices, GLfloat* lineVertices)
{
	switch (MAP_TYPE)
	{
	case ONE_ROAD_N:
		setRoad(&roads[0], 0, roadVertices, roadIndices, 0.0f, -1.0f, 2.0f, NORTH);
		break;
	case ONE_ROAD_S:
		setRoad(&roads[0], 0, roadVertices, roadIndices, 0.0f, 1.0f, 2.0f, SOUTH);
		break;
	case ONE_ROAD_W:
		setRoad(&roads[0], 0, roadVertices, roadIndices, 1.0f, 0.0f, 2.0f, WEST);
		break;
	case ONE_ROAD_E:
		setRoad(&roads[0], 0, roadVertices, roadIndices, -1.0f, 0.0f, 2.0f, EAST);
		break;
	case TWO_ROADS_NS:
		setRoad(&roads[0], 0, roadVertices, roadIndices, -0.2f, -1.0f, 2.0f, NORTH);
		setRoad(&roads[1], 1, roadVertices, roadIndices, 0.2f, 1.0f, 2.0f, SOUTH);
		break;
	case TWO_ROADS_WE:
		setRoad(&roads[0], 1, roadVertices, roadIndices, 1.0f, -0.2f, 2.0f, WEST);
		setRoad(&roads[1], 0, roadVertices, roadIndices, -1.0f, 0.2f, 2.0f, EAST);
		break;
	}

	for (int i = 0; i < NUMBER_OF_ROADS; i++)
	{
		setLines(&roads[i], i, roadVertices, lineVertices);
	}
}