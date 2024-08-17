// Standard
#include <stdlib.h>
#include <string.h>

// Custom modules
#include <cars.h>
#include <algorithms.h>
#include <direction.h>

mat3 carTransformMatrixes[MAX_CARS];
car cars[MAX_CARS];

GLuint carVAO, carVBO, carEBO, carInstanceVBO;

GLfloat carVertices[4 * 2] =
{
    0.0f, 0.0f,
    CAR_WIDTH, 0.0f,
    0.0f, CAR_LENGTH,
    CAR_WIDTH, CAR_LENGTH,
};

GLint carIndices[6] =
{
    0, 2, 3,
    0, 1, 3
};

GLint freeCars = MAX_CARS;
GLint innerBornCarsIndex = NO_INNER_INDEX;
GLint bornCarsIndexes[MAX_CARS];


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

			if (freeSpotRLC.road != EMPTY && carIndex != NO_CAR_INDEX)
			{
				addCar(&cars[carIndex], carIndex, freeSpotRLC);
				thoughtsOfOneCar(&cars[carIndex]);

				--freeCars;

				appendInBornCarsIndexes(carIndex);
			}
		}
	}
}

GLvoid addCar(car* Car, GLint carIndex, RLC rlc)
{
	setBornCarProperties(&cars[carIndex], carIndex, rlc);
	setCar(&cars[carIndex], carIndex, rlc);
}

GLvoid setBornCarProperties(car* Car, GLint carIndex, RLC rlc)
{
	Car->ID = carIndex;

	car** ptrCell = getFirstCellPtr(rlc);
	ptrCell[rlc.cell] = Car;// bind a car pointer with a spawn cell

	Car->target = rand() % NUMBER_OF_DIRECTIONS;//NONE can't be as it out of range of number of directions
	Car->velocity = _1_CELL_ ;//* (rand() % (NUMBER_OF_VELOCITY_TYPES));
	Car->isActive = true;
	memcpy(&Car->currCell, &rlc, sizeof(RLC));
	memcpy(&Car->nextCell, &rlc, sizeof(RLC));

	Car->dirOnRoad = getCarDirOnRoad(&roads[rlc.road]);
 }

 GLvoid setCrushedCarProperties(car* Car, GLint carIndex, RLC rlc)
 {
	Car->isCrushed = true;
	Car->ID = carIndex;
	Car->velocity = _0_CELL_;
	Car->isActive = true;
	memcpy(&Car->currCell, &rlc, sizeof(RLC));//what's it for ?
	memcpy(&Car->nextCell, &rlc, sizeof(RLC));

	car** ptrCell = getFirstCellPtr(rlc);
	ptrCell[rlc.cell] = Car;// bind a car pointer with a spawn cell
 }

 GLvoid getFreeSpotAddress(RLC* rlc)
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
		
		rlc->road = freeSpots[randFreeSpotIndex].road;
		rlc->line = freeSpots[randFreeSpotIndex].line;
		rlc->cell = 0;
	}
}

GLvoid setCar(car* Car, GLint carIndex, RLC rlc)
{
	DIRECTION carDir = roads[rlc.road].dir;
	GLfloat x1 = roads[rlc.road].lines[rlc.line].carSpawnCoord - CAR_WIDTH * 0.75f;
	GLfloat x2 = x1 - CAR_WIDTH * 0.75f;
	GLfloat y1 = 0, y2 = 0;

	glm_mat3_identity(carTransformMatrixes[carIndex]);

	switch (carDir)
	{
		case NORTH:
		{
			y1 = -1.0f + (CAR_WIDTH * 0.5f);
			y2 = y1 + CAR_LENGTH + (CAR_WIDTH * 0.5f);
			Car->realPos = y1;

			vec2 carTranslationVector = { x2, y1 };
			glm_translate2d(carTransformMatrixes[carIndex], carTranslationVector);

			break;
		}

		case SOUTH:
		{
			y1 = 1.0f - CAR_LENGTH - (CAR_WIDTH * 0.5f);
			y2 = y1 - CAR_LENGTH - (CAR_WIDTH * 0.5f);
			Car->realPos = y1;

			vec2 carTranslationVector = { x2, y1 };
			glm_translate2d(carTransformMatrixes[carIndex], carTranslationVector);

			break;
		}

		case EAST:
		{
			y1 = x1;
			y2 = x2;
			x1 = -1.0f;
			x2 = -1.0f - CAR_WIDTH;
			Car->realPos = x1;

			vec2 carTranslationVector = { x2, y2 };
			glm_translate2d(carTransformMatrixes[carIndex], carTranslationVector);
			glm_scale2d(carTransformMatrixes[carIndex], (vec2){ 1.7, 0.7 });

			break;
		}

		case WEST:
		{
			y1 = x1;
			y2 = x2;
			x1 = 1.0f;
			x2 = 1.0f - CAR_WIDTH;
			Car->realPos = x1;

			vec2 carTranslationVector = { x2, y2 };
			glm_translate2d(carTransformMatrixes[carIndex], carTranslationVector);
			glm_scale2d(carTransformMatrixes[carIndex], (vec2) { 1.7, 0.7 });

			break;
		}
	}
}

GLvoid addCrushedCar(RLC rlc)
{
	if (roads[rlc.road].lines[rlc.line].cells[rlc.cell] != NULL)
	{
		printf("The crushed car is tried to be set on the occupied cell rlc(%d, %d, %d).\n", rlc.road, rlc.line, rlc.cell);
		return;
	}	

	GLint carIndex = getFreeCarIndex();
	if (carIndex == NO_CAR_INDEX)
	{
		printf("No free carIndex. It is unable to set a crushed car.\n");
		return;
	}

	--freeCars;
	setCrushedCarProperties(&cars[carIndex], carIndex, rlc);
	setCrushedCar(&cars[carIndex], carIndex, rlc);
}

GLfloat getCellWall(RLC rlc)
{
	GLfloat whereRoadStarts = roads[rlc.road].startLineCoord;
	switch (roads[rlc.road].dir)
	{
		case NORTH:
		{
			return (whereRoadStarts + CELL_LENGTH * rlc.cell);
		}
		case SOUTH:
		{
			return (whereRoadStarts - CELL_LENGTH * rlc.cell);
		}
		case WEST:
		{
			
		}
		case EAST:
		{

		}
	}
}

GLvoid setCrushedCar(car* Car, GLint carIndex, RLC rlc)
{
	DIRECTION carDir = roads[rlc.road].dir;
	GLfloat x1 = roads[rlc.road].lines[rlc.line].carSpawnCoord - CAR_WIDTH * 0.75f;
	GLfloat x2 = x1 - CAR_WIDTH * 0.75f;
	GLfloat y1 = 0, y2 = 0;

	glm_mat3_identity(carTransformMatrixes[carIndex]);


	GLfloat cellWall = getCellWall(rlc);


	switch (carDir)
	{
		case NORTH:
		{
			y1 = cellWall + (CAR_WIDTH * 0.5f);
			y2 = y1 + CAR_LENGTH + (CAR_WIDTH * 0.5f);
			Car->realPos = y1;

			vec2 carTranslationVector = { x2, y1 };
			glm_translate2d(carTransformMatrixes[carIndex], carTranslationVector);

			break;
		}

		case SOUTH:
		{
			y1 = cellWall - CAR_LENGTH - (CAR_WIDTH * 0.5f);
			y2 = y1 - CAR_LENGTH - (CAR_WIDTH * 0.5f);
			Car->realPos = y1;

			vec2 carTranslationVector = { x2, y1 };
			glm_translate2d(carTransformMatrixes[carIndex], carTranslationVector);

			break;
		}

		case EAST:
		{
			y1 = x1;
			y2 = x2;
			x1 = -1.0f;
			x2 = -1.0f - CAR_WIDTH;
			Car->realPos = x1;

			vec2 carTranslationVector = { x2, y2 };
			glm_translate2d(carTransformMatrixes[carIndex], carTranslationVector);
			glm_scale2d(carTransformMatrixes[carIndex], (vec2){ 1.7, 0.7 });

			break;
		}

		case WEST:
		{
			y1 = x1;
			y2 = x2;
			x1 = 1.0f;
			x2 = 1.0f - CAR_WIDTH;
			Car->realPos = x1;

			vec2 carTranslationVector = { x2, y2 };
			glm_translate2d(carTransformMatrixes[carIndex], carTranslationVector);
			glm_scale2d(carTransformMatrixes[carIndex], (vec2) { 1.7, 0.7 });

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

	return NO_CAR_INDEX;
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
		cars[i].dirOnRoad = 0;
		cars[i].ID = EMPTY;
		cars[i].realPos = EMPTY;
		cars[i].target = NONE;
		cars[i].velocity = 0;
		cars[i].isActive = false;
		cars[i].isCrushed = false;
		cars[i].overtake = NONE;
	}
}

GLvoid appendInBornCarsIndexes(GLint freeCarIndex)
{
	bornCarsIndexes[++innerBornCarsIndex] = freeCarIndex;
}

GLvoid clearInBornCarsIndedxes()
{
	innerBornCarsIndex = NO_INNER_INDEX;
}

GLint isInBornCars(GLint i)
{
	for (int k = 0; k <= innerBornCarsIndex; k++)
	{
		if (i == bornCarsIndexes[k])
		{
			return 1;
		}
	}
	return 0;
}