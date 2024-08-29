// External
#include <glad/glad.h>

#include <stdbool.h>

// Custom modules
#include <map.h>
#include <direction.h>
#include <road.h>

//a road gets a direction, an edge state
GLvoid setMap()
{
	switch (MAP_TYPE)
	{
	case ONE_ROAD_N:
		addRoad(0, -0.5f, -1.0f, NORTH);
		// roads[0].isEndCross = true;
		break;

	case ONE_ROAD_S:
		addRoad(0, 0.5f, 1.0f, SOUTH);
		break;

	case ONE_ROAD_W:
		addRoad(0, 1.0f, 0.0f, WEST);
		break;

	case ONE_ROAD_E:
		addRoad(0, -1.0f, 0.0f, EAST);
		break;

	case TWO_ROADS_NS:
		addRoad(0, 0.4f, -1.0f, NORTH);
		addRoad(1, -0.4f, 1.0f, SOUTH);
		break;

	case TWO_ROADS_WE:
		addRoad(1, 1.0f, 0.4f, WEST);
		addRoad(0, -1.0f, -0.4f, EAST);
		// roads[1].isEndCross = true;
		break;

	case CROSS:
		addRoad(0, 0.0f, -1.0f, NORTH);
		addRoad(1, 1.0f, 0.0f, WEST);
	}

	for (int i = 0; i < NUMBER_OF_ROADS; i++)
	{
		setLines(i);
	}
}
