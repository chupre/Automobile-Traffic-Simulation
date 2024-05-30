#pragma once

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

	glm_mat3_identity(carTransformMatrixes[carIndex]);

	switch (carDir)
	{
		case NORTH:
		{
			y1 = -1.0f;
			y2 = y1 + CAR_LENGHT;
			Car->realPos = y1;

			GLfloat carTranslationCoords[3] = { x2, y1 };
			vec3 carTranslationVector;
			glm_vec2_make(carTranslationCoords, carTranslationVector);
			glm_translate2d(carTransformMatrixes[carIndex], carTranslationVector);

			break;
		}

		case SOUTH:
		{
			y1 = 1.0f;
			y2 = y1 - CAR_LENGHT;
			Car->realPos = y1;

			GLfloat carTranslationCoords[3] = { x2, y1 };
			vec3 carTranslationVector;
			glm_vec2_make(carTranslationCoords, carTranslationVector);
			glm_translate2d(carTransformMatrixes[carIndex], carTranslationVector);

			break;
		}

		case EAST:
		{
			y1 = x1;
			y2 = x2;
			x1 = -1.0f;
			x2 = -1.0f + CAR_WIDTH;
			Car->realPos = x1;

			GLfloat carTranslationCoords[3] = { x2, y1 };
			vec3 carTranslationVector;
			glm_vec2_make(carTranslationCoords, carTranslationVector);
			glm_translate2d(carTransformMatrixes[carIndex], carTranslationVector);

			break;
		}

		case WEST:
		{
			y1 = x1;
			y2 = x2;
			x1 = 1.0f;
			x2 = 1.0f - CAR_WIDTH;
			Car->realPos = x1;

			GLfloat carTranslationCoords[3] = { x2, y1 };
			vec3 carTranslationVector;
			glm_vec2_make(carTranslationCoords, carTranslationVector);
			glm_translate2d(carTransformMatrixes[carIndex], carTranslationVector);

			break;
		}
	}
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
				--freeCars;
			}
		}
	}
}

