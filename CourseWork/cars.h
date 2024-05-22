#pragma once

#define CAR_WIDTH (ROAD_WIDTH * 2.0f / ((GLfloat)NUMBER_OF_LINES + 1.0f) / 3.0f)
#define CAR_LENGHT (CAR_WIDTH * 1.7f)

//its definition is in algorithms.h
GLvoid christenNewBornCar(RLC rlc, car* Car);

GLvoid setCarsToDefault();
GLint getFreeCarIndex();


GLvoid getFreeSpotAddress(RLC* address)
{
	for (int i = 0; i < NUMBER_OF_ROADS; i++)
	{
		if (roads[i].isEdge)
		{
			for (int j = 0; j < NUMBER_OF_LINES + 1; j++)
			{
				if (roads[i].lines[j].cells[0] == NULL)
				{
					address->road = i;
					address->line = j;
					address->cell = 0;

					return;
				}
			}
		}
	}
}

GLvoid setCar(car* Car, GLint carIndex, RLC rlc)
{
	christenNewBornCar(rlc, Car);

	DIRECTION carDir = roads[rlc.road].dir;
	GLfloat x1 = roads[rlc.road].lines[rlc.line].carSpawnCoord - CAR_WIDTH;
	GLfloat x2 = x1 - CAR_WIDTH;
	GLfloat y1 = 0, y2 = 0;

	switch (carDir)
	{
	case NORTH:
		y1 = -1.0f;
		y2 = y1 + CAR_LENGHT;
		break;
	case SOUTH:
		y1 = 1.0f;
		y2 = y1 - CAR_LENGHT;
		break;
	case EAST:
		y1 = x1;
		y2 = x2;
		x1 = -1.0f;
		x2 = -1.0f + CAR_WIDTH;
		break;
	case WEST:
		y1 = x1;
		y2 = x2;
		x1 = 1.0f;
		x2 = 1.0f - CAR_WIDTH;
		break;
	}

	switch (Car->target)
	{

	case 0: 
	{
		GLfloat vertices[] =
		{
			x1, y1, 0.0f, 1.0f, 0.0f, 0.0f,
			x2, y1, 0.0f, 1.0f, 0.0f, 0.0f,
			x1, y2, 0.0f, 1.0f, 0.0f, 0.0f,
			x2, y2 ,0.0f, 1.0f, 0.0f, 0.0f
		};
		memcpy(&carVertices[carIndex * 3 * 4 * 2], vertices, sizeof(GLfloat) * 4 * 3 * 2);
		Car->target = NORTH;
		break;
	}
	case 1:
	{
		GLfloat vertices[] =
		{
			x1, y1, 0.0f, 0.0f, 1.0f, 0.0f,
			x2, y1, 0.0f, 0.0f, 1.0f, 0.0f,
			x1, y2, 0.0f, 0.0f, 1.0f, 0.0f,
			x2, y2 ,0.0f, 0.0f, 1.0f, 0.0f
		};
		memcpy(&carVertices[carIndex * 3 * 4 * 2], vertices, sizeof(GLfloat) * 4 * 3 * 2);
		Car->target = SOUTH;
		break;
	}
	case 2:
	{
		GLfloat vertices[] =
		{
			x1, y1, 0.0f, 0.0f, 0.0f, 1.0f,
			x2, y1, 0.0f, 0.0f, 0.0f, 1.0f,
			x1, y2, 0.0f, 0.0f, 0.0f, 1.0f,
			x2, y2 ,0.0f, 0.0f, 0.0f, 1.0f
		};
		memcpy(&carVertices[carIndex * 3 * 4 * 2], vertices, sizeof(GLfloat) * 4 * 3 * 2);
		Car->target = EAST;
		break;
	}
	case 3:
	{
		GLfloat vertices[] =
		{
			x1, y1, 0.0f, 1.0f, 1.0f, 0.0f,
			x2, y1, 0.0f, 1.0f, 1.0f, 0.0f,
			x1, y2, 0.0f, 1.0f, 1.0f, 0.0f,
			x2, y2 ,0.0f, 1.0f, 1.0f, 0.0f
		};
		memcpy(&carVertices[carIndex * 3 * 4 * 2], vertices, sizeof(GLfloat) * 4 * 3 * 2);
		Car->target = WEST;
		break;
	}

	}

	GLint indeces[] =
	{
		0 + (carIndex * 4), 2 + (carIndex * 4), 3 + (carIndex * 4),
		0 + (carIndex * 4), 1 + (carIndex * 4), 3 + (carIndex * 4)
	};

	memcpy(&carIndices[carIndex * 6], indeces, sizeof(GLint) * 6);
}

GLint getFreeCarIndex()
{
	for (int i = 0; i < MAX_CARS; i++)
	{
		if (!cars[i].isActive)
		{
			return i;
		}
	}
	return 
		;
}

GLvoid setCarsToDefault()
{
	for (int i = 0; i < MAX_CARS; i++)
	{
		cars[i].currCell.road = EMPTY;
		cars[i].currCell.line = EMPTY;
		cars[i].currCell.cell = EMPTY;
		cars[i].nextCell.road = EMPTY;
		cars[i].nextCell.line = EMPTY;
		cars[i].nextCell.cell = EMPTY;
		cars[i].target = NONE;
		cars[i].velocity = 0;
		cars[i].isActive = false;
		cars[i].overtake = NONE;

		cars[i].dirOnRoad = 0;

		cars[i].isAvaria = 0;
	}
}

//void initRoadsToDefault(road* roads)
//{
//	for (int a = 0; a < NUMBER_OF_ROADS; a++)
//	{
//		for (int b = 0; b < NUMBER_OF_LINES + 1; b++)
//		{
//			line* l = ((roads + a)->lines + b);
//			for (int c = 0; c < NUMBER_OF_CELLS; c++)
//			{
//				l->cells[c] = NULL;
//			}
//		}
//	}
//}

GLvoid getCarTranslateVector()
{
	int sasasas = 0;
}

