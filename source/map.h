#pragma once

GLvoid setMap();

//a road gets a direction, an edge state
GLvoid setMap()
{
	switch (MAP_TYPE)
	{
	case ONE_ROAD_N:
		setRoad(0, 0.0f, -1.0f, 2.0f, NORTH);
		break;

	case ONE_ROAD_S:
		setRoad(0, 0.0f, 1.0f, 2.0f, SOUTH);
		break;

	case ONE_ROAD_W:
		setRoad(0, 1.0f, 0.0f, 2.0f, WEST);
		break;

	case ONE_ROAD_E:
		setRoad(0, -1.0f, 0.0f, 2.0f, EAST);
		break;

	case TWO_ROADS_NS:
		setRoad(0, -0.2f, -1.0f, 2.0f, NORTH);
		setRoad(1, 0.2f, 1.0f, 2.0f, SOUTH);
		break;

	case TWO_ROADS_WE:
		setRoad(1, 1.0f, -0.2f, 2.0f, WEST);
		setRoad(0, -1.0f, 0.2f, 2.0f, EAST);
		break;

	case CROSS:
		setRoad(0, 0.0f, -1.0f, 2.0f, NORTH);
		setRoad(1, 1.0f, 0.0f, 2.0f, WEST);
	}

	for (int i = 0; i < NUMBER_OF_ROADS; i++)
	{
		setLines(i);
	}
}