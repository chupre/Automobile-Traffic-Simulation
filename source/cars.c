#include <road.h>
#include <macros.h>

#include <stdlib.h>
#include <string.h>

// Custom modules
#include <algorithms.h>
#include <direction.h>
#include <map.h>
#include <cars.h>
#include <rlc.h>

car occupying_car = {.isActive = true, .velocity = _0_CELL_};
car* OCCUPYING_CAR = &occupying_car;

mat3 carTransformMatrixes[1000];
car * cars;

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

GLint freeCars;

GLvoid setOccupyingCarProperties()
{
	occupying_car.velocity = _0_CELL_;

}

GLvoid bindCellAndCar(RLC* rlc, car* Car)
{
	roads[rlc->road].lines[rlc->line].cells[rlc->cell] = Car;
}

GLvoid addCar(car* Car, GLint carIndex, RLC rlc)
{
	setBornCarProperties(&cars[carIndex], carIndex, rlc);
	setCar(&cars[carIndex], carIndex, rlc);
}

GLvoid setBornCarProperties(car* Car, GLint carIndex, RLC rlc)
{
	Car->move = FORWARD;
	Car->moveDir = roads[rlc.road].dir;
	Car->ID = carIndex;
	Car->overtake = NONE;
	Car->target = rand() % NUMBER_OF_DIRECTIONS;//NONE can't be as it out of range of number of directions
	//Car->velocity = _3_CELL_ + _1_CELL_ + (rand() % (NUMBER_OF_VELOCITY_TYPES - 3));
	//Car->velocity = _1_CELL_;
	Car->velocity = _1_CELL_ + rand() % NUMBER_OF_VELOCITY_TYPES;
	Car->isActive = true;
	Car->roadDirMultiplier = getDirMultiplier(Car->moveDir);
	memcpy(&Car->currCell, &rlc, sizeof(RLC));
	memcpy(&Car->nextCell, &rlc, sizeof(RLC));
	
	bindCellAndCar(&rlc, Car);
 }

 GLvoid setCrushedCarProperties(car* Car, GLint carIndex, RLC rlc)
 {
	Car->isCrushed = true;
	Car->ID = carIndex;
	Car->velocity = _0_CELL_;
	Car->isActive = true;
	memcpy(&Car->currCell, &rlc, sizeof(RLC));//what's it for ?
	memcpy(&Car->nextCell, &rlc, sizeof(RLC));

	bindCellAndCar(&rlc, Car);
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
			y1 = roads[rlc.road].startLineCoord + rlc.cell * CELL_LENGTH + 0 * margin;
			y2 = y1 + rlc.cell * CELL_LENGTH + CAR_LENGTH + 0 * margin;
			Car->realPos = y1;

			vec2 carTranslationVector = { x2, y1 };
			glm_translate2d(carTransformMatrixes[carIndex], carTranslationVector);

			break;
		}

		case SOUTH:
		{
	        x1 = roads[rlc.road].lines[rlc.line].carSpawnCoord;
	        x2 = x1;
			y1 = roads[rlc.road].startLineCoord - CAR_LENGTH - rlc.cell * CELL_LENGTH - 0 * margin;
			y2 = 1.0f;//y1 - CAR_LENGTH - rlc.cell * CELL_LENGTH - 0 * margin;
			Car->realPos = y1;

			vec2 carTranslationVector = { x2, y1 };
			glm_translate2d(carTransformMatrixes[carIndex], carTranslationVector);

			break;
		}

		case EAST:
		{
	        x1 = roads[rlc.road].lines[rlc.line].carSpawnCoord;
	        x2 = x1;
			y1 = x1;
			y2 = x2;
			x1 = roads[rlc.road].startLineCoord + rlc.cell * CELL_LENGTH;
			x2 = roads[rlc.road].startLineCoord + rlc.cell * CELL_LENGTH;
			Car->realPos = x1;

			vec2 carTranslationVector = { x2, y2 };
			glm_translate2d(carTransformMatrixes[carIndex], carTranslationVector);

			break;
		}

		case WEST:
		{
	        x1 = roads[rlc.road].lines[rlc.line].carSpawnCoord - margin * 2.0f;
	        x2 = x1 - margin * 2.0f;
			y1 = x1;
			y2 = x2;
			x1 = roads[rlc.road].startLineCoord - rlc.cell * CELL_WIDTH;
			x2 = roads[rlc.road].startLineCoord - rlc.cell * CELL_WIDTH - CAR_WIDTH;
			Car->realPos = x1;

			vec2 carTranslationVector = { x2, y2 };
			glm_translate2d(carTransformMatrixes[carIndex], carTranslationVector);

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
	switch (roads[rlc.road].dir)
	{
		case NORTH:
		{
			return (roads[rlc.road].startLineCoord + CELL_LENGTH * rlc.cell);
		}
		case SOUTH:
		{
			return (roads[rlc.road].startLineCoord - CELL_LENGTH * rlc.cell);
		}
		case WEST:
		{
			return (roads[rlc.road].startLineCoord - CELL_LENGTH * rlc.cell);
		}
		case EAST:
		{
			return (roads[rlc.road].startLineCoord + CELL_LENGTH * rlc.cell);
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
	        x1 = roads[rlc.road].lines[rlc.line].carSpawnCoord - (CAR_WIDTH + 0 * margin);
	        x2 = x1 - 0 * margin;
			y1 = cellWall + 0 * margin;
			y2 = y1 + CAR_LENGTH + 0 * margin;
			Car->realPos = y1;

			vec2 carTranslationVector = { x2, y1 };
			glm_translate2d(carTransformMatrixes[carIndex], carTranslationVector);

			break;
		}

		case SOUTH:
		{
	        x1 = roads[rlc.road].lines[rlc.line].carSpawnCoord - 0 * margin;
	        x2 = x1;
			y1 = cellWall - CAR_LENGTH - 0 * margin;
			y2 = y1 - CAR_LENGTH - 0 * margin;
			Car->realPos = y1;

			vec2 carTranslationVector = { x2, y1 };
			glm_translate2d(carTransformMatrixes[carIndex], carTranslationVector);

			break;
		}

		case EAST:
		{
	        x1 = roads[rlc.road].lines[rlc.line].carSpawnCoord + margin * 2.0f;
	        x2 = x1 + margin * 2.0f;
			
			y1 = x1;
			y2 = x2 - CAR_WIDTH;
			x1 = cellWall;
			x2 = cellWall;
			Car->realPos = x1;

			vec2 carTranslationVector = { x2, y2 };
			glm_translate2d(carTransformMatrixes[carIndex], carTranslationVector);

			break;
		}

		case WEST:
		{
	        x1 = roads[rlc.road].lines[rlc.line].carSpawnCoord - margin * 2.0f;
	        x2 = x1 - margin * 2.0f;

			y1 = x1;
			y2 = x2;
			x1 = cellWall;
			x2 = cellWall - CAR_WIDTH;
			Car->realPos = x1;

			vec2 carTranslationVector = { x2, y2 };
			glm_translate2d(carTransformMatrixes[carIndex], carTranslationVector);

			break;
		}
	}
}

GLvoid getFreeSpotAddress(RLC* rlc)
{
	RLC * freeSpots = malloc(sizeof(RLC) * MAX_CARS);

	if (freeSpots == NULL) {
		printf("malloc failed on freeSpots");
		exit(1);
	}

	int freeSpotsCounter = 0;
	int randFreeSpotIndex = 0;

	for (int i = 0; i < NUMBER_OF_ROADS; i++)
	{
		if (/*roads[i].isEdge ||*/ !roads[i].isBeginCross)
		{
			if (i >= 4 && i <= 7)
			{
				printf("i:%d, begin:%d, end:%d\n", i, roads[i].isBeginCross, roads[i].isEndCross);
			}
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

	free(freeSpots);
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
    cars = malloc(sizeof(car) * MAX_CARS);

    if (cars == NULL) {
        printf("malloc failed on cars");
        exit(1);
    }

    freeCars = MAX_CARS;

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
	Car->crossCurrCell.crossNum = EMPTY;
	Car->crossCurrCell.x = EMPTY;
	Car->crossCurrCell.y = EMPTY;
	Car->crossNextCell.crossNum = EMPTY;
	Car->crossNextCell.x = EMPTY;
	Car->crossNextCell.y = EMPTY;
	Car->roadDirMultiplier = 0;
	Car->ID = EMPTY;
	Car->realPos = EMPTY;
	Car->target = NONE;
	Car->velocity = 0;
	Car->isActive = false;
	Car->isCrushed = false;
	Car->overtake = NONE;
	Car->move = FORWARD;
	Car->moveDir = NONE;
	Car->maneuverPtr = NULL;
	Car->arrivingPtr = NULL;
	Car->checked = false;// used only on the cross
}
