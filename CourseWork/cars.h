#pragma once

#define CAR_WIDTH (ROAD_WIDTH * 2.0f / ((GLfloat)NUMBER_OF_LINES + 1.0f) / 3.0f)
#define CAR_LENGHT (CAR_WIDTH * 1.7f)

//its definition is in algorithms.h
GLvoid christenNewBornCar(RLC rlc, car* Car);

GLvoid setCarsToDefault();
GLint getFreeCarIndex();


GLvoid getFreeSpotAddress(RLC* address)
{
	RLC freeSpots[MAX_CARS * sizeof(RLC)];
	int freeSpotsCounter = 0;
	int randFreeSpotIndex = 0;

	for (int i = 0; i < NUMBER_OF_ROADS; i++)
	{
		if (roads[i].isEdge)
		{
			for (int j = 0; j < NUMBER_OF_LINES + 1; j++)
			{
				if (roads[i].lines[j].cells[0] == NULL)
				{
					freeSpots[freeSpotsCounter].road = i;
					freeSpots[freeSpotsCounter].line = j;
					freeSpotsCounter++;
				}
			}
		}
	}

	if (freeSpotsCounter)
	{
		randFreeSpotIndex = rand() % freeSpotsCounter;
		
		address->road = freeSpots[randFreeSpotIndex].road;
		address->line = freeSpots[randFreeSpotIndex].line;
		address->cell = 0;
	}
}

GLvoid setCar(car* Car, GLint carIndex, RLC rlc)
{
	christenNewBornCar(rlc, Car);

	Car->ID = carIndex;

	DIRECTION carDir = roads[rlc.road].dir;
	GLfloat x1 = roads[rlc.road].lines[rlc.line].carSpawnCoord - CAR_WIDTH;
	GLfloat x2 = x1 - CAR_WIDTH;
	GLfloat y1 = 0, y2 = 0;

	switch (carDir)
	{
		case NORTH:
			y1 = -1.0f;
			y2 = y1 + CAR_LENGHT;
			Car->realPos = y1;
			break;

		case SOUTH:
			y1 = 1.0f;
			y2 = y1 - CAR_LENGHT;
			Car->realPos = y1;
			break;

		case EAST:
			y1 = x1;
			y2 = x2;
			x1 = -1.0f;
			x2 = -1.0f + CAR_WIDTH;
			Car->realPos = x1;
			break;

		case WEST:
			y1 = x1;
			y2 = x2;
			x1 = 1.0f;
			x2 = 1.0f - CAR_WIDTH;
			Car->realPos = x1;
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
	glm_mat4_identity(carTrans[Car->ID]);
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

	return;
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
		cars[i].ID = EMPTY;
		cars[i].realPos = EMPTY;
		cars[i].target = NONE;
		cars[i].velocity = 0;
		cars[i].isActive = false;
		cars[i].overtake = NONE;

		cars[i].dirOnRoad = 0;

		cars[i].isAvaria = 0;
	}
}

GLvoid spawnCars()
{
	if (freeCars)
	{
		int counter = freeCars;
		for (int i = 0; i < counter; i++)
		{
			RLC freeSpotRLC = { EMPTY, EMPTY, EMPTY };
			getFreeSpotAddress(&freeSpotRLC);
			GLint carIndex = getFreeCarIndex();

			if (freeSpotRLC.road != EMPTY)
			{
				setCar(&cars[carIndex], carIndex, freeSpotRLC);
				//printf("car %d spawned:\n", carIndex);
				//printf("vertices: %f %f %f %f\n", carVertices[carIndex * 24], carVertices[carIndex * 24 + 6], carVertices[carIndex * 24 + 1], carVertices[carIndex * 24 + 7]);
				//printf("direction: %d\n", cars[carIndex].dirOnRoad);
				//printf("RLC: Road: %d, Line: %d, Cell: %d\n", cars[carIndex].currCell.road, cars[carIndex].currCell.line, cars[carIndex].currCell.cell);
				//printf("real position: %f\n", cars[carIndex].realPos);
				//printf("velocity: %d\n\n", cars[carIndex].velocity);
				glBindBuffer(GL_ARRAY_BUFFER, carVBO);
				glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(carVertices), carVertices);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, carEBO);
				glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(carIndices), carIndices);
				--freeCars;
			}
		}
	}
}

