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
	//Car->velocity = _3_CELL_ + _1_CELL_ + (rand() % (NUMBER_OF_VELOCITY_TYPES - 3));
	//Car->velocity = _1_CELL_;
	Car->velocity = _1_CELL_ + rand() % NUMBER_OF_VELOCITY_TYPES;
	Car->isActive = true;
	memcpy(&Car->currCell, &rlc, sizeof(RLC));
	memcpy(&Car->nextCell, &rlc, sizeof(RLC));

	Car->dirOnRoad = getRoadDirForVelocity(&roads[rlc.road]);
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

GLvoid setCar(car* Car, GLint carIndex, RLC rlc)
{
	GLfloat margin = CELL_WIDTH / 4;

	DIRECTION carDir = roads[rlc.road].dir;
	GLfloat x1;
	GLfloat x2;
	GLfloat y1 = 0, y2 = 0;

	glm_mat3_identity(carTransformMatrixes[carIndex]);

	switch (carDir)
	{
		case NORTH:
		{
	        x1 = roads[rlc.road].lines[rlc.line].carSpawnCoord - margin * 2.0f;
	        x2 = x1 - margin * 2.0f;
			y1 = -1.0f + 0 * margin;
			y2 = y1 + CAR_LENGTH + margin;
			Car->realPos = y1;

			vec2 carTranslationVector = { x2, y1 };
			glm_translate2d(carTransformMatrixes[carIndex], carTranslationVector);

			break;
		}

		case SOUTH:
		{
	        x1 = roads[rlc.road].lines[rlc.line].carSpawnCoord;
	        x2 = x1;
			y1 = 1.0f - CAR_LENGTH - 0 * margin;
			y2 = y1 - CAR_LENGTH - margin;
			Car->realPos = y1;

			vec2 carTranslationVector = { x2, y1 };
			glm_translate2d(carTransformMatrixes[carIndex], carTranslationVector);

			break;
		}

		case EAST:
		{
	        x1 = roads[rlc.road].lines[rlc.line].carSpawnCoord - margin * 2.0f;
	        x2 = x1 - margin * 2.0f;
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
	        x1 = roads[rlc.road].lines[rlc.line].carSpawnCoord;
	        x2 = x1;
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
	GLfloat margin = CELL_WIDTH / 4;

	DIRECTION carDir = roads[rlc.road].dir;
	GLfloat x1;
	GLfloat x2;
	GLfloat y1 = 0, y2 = 0;

	glm_mat3_identity(carTransformMatrixes[carIndex]);


	GLfloat cellWall = getCellWall(rlc);


	switch (carDir)
	{
		case NORTH:
		{
	        x1 = roads[rlc.road].lines[rlc.line].carSpawnCoord - (2.0f * margin);
	        x2 = x1 - (2.0f * margin);
			y1 = cellWall + 0 * margin;
			y2 = y1 + CAR_LENGTH + 0 * margin;
			Car->realPos = y1;

			vec2 carTranslationVector = { x2, y1 };
			glm_translate2d(carTransformMatrixes[carIndex], carTranslationVector);

			break;
		}

		case SOUTH:
		{
	        x1 = roads[rlc.road].lines[rlc.line].carSpawnCoord;
	        x2 = x1;
			y1 = cellWall - CAR_LENGTH - 0 * margin;
			y2 = y1 - CAR_LENGTH - margin;
			Car->realPos = y1;

			vec2 carTranslationVector = { x2, y1 };
			glm_translate2d(carTransformMatrixes[carIndex], carTranslationVector);

			break;
		}

		case EAST:
		{
	        x1 = roads[rlc.road].lines[rlc.line].carSpawnCoord - (2.0f * margin);
	        x2 = x1 - (2.0f * margin);
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
	        x1 = roads[rlc.road].lines[rlc.line].carSpawnCoord;
	        x2 = x1;
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
		clearCarProperties(&cars[i]);
	}
}

GLvoid clearCarProperties(car* Car)
{
	Car->currCell.road = EMPTY;
	Car->currCell.line = EMPTY;
	Car->currCell.cell = EMPTY;
	Car->nextCell.road = EMPTY;
	Car->nextCell.line = EMPTY;
	Car->nextCell.cell = EMPTY;
	Car->dirOnRoad = 0;
	Car->ID = EMPTY;
	Car->realPos = EMPTY;
	Car->target = NONE;
	Car->velocity = 0;
	Car->isActive = false;
	Car->isCrushed = false;
	Car->overtake = NONE;
	Car->ableToChangeLine = false;
}
