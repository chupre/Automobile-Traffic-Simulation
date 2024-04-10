#include <time.h>
#define MAX_CARS 4
#define CAR_WIDTH (ROAD_WIDTH * 2.0f / ((GLfloat)NUMBER_OF_LINES + 1.0f) / 3.0f)
#define CAR_LENGHT (CAR_WIDTH * 1.7f)
#define STEP_TIME 3


void setCar(road* roads, car* Car, GLint carIndex, RLC address, GLfloat* carVertices, GLint* carIndices);
void setCarsToDefault(car* cars);
void getFreeSpotAddress(road* roads, RLC* address);
void getCarRealPos();
void getCarRealVelocity();
void getCarTransformation();
void step(car* cars, road* roads);
void thoughtsOfOneCar(car* car, road* roads);
GLint getFreeCarIndex(car* cars);
GLint getVelocityByRLC(RLC rlc, road* roads);


void getFreeSpotAddress(road* roads, RLC* address)
{
	for (int i = 0; i < NUMBER_OF_ROADS; i++)
	{
		if (roads[i].isEdge)
		{
			for (int j = 0; j < NUMBER_OF_LINES + 1; j++)
			{
				if (roads[i].lines[j].cells[0] == EMPTY)
				{
					address->road = i;
					address->line = j;
					address->cell = 0;
				}
			}
		}
	}
}


void setCar(road* roads, car* Car, GLint carIndex, RLC address, GLfloat* carVertices, GLint* carIndices)
{
	DIRECTION carDir = roads[address.road].dir;
	GLfloat x1 = roads[address.road].lines[address.line].carSpawnCoord - CAR_WIDTH;
	GLfloat x2 = x1 - CAR_WIDTH;
	GLfloat y1, y2;

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

	roads[address.road].lines[address.line].cells[address.cell] = Car;
	Car->target = rand() % 4;
	Car->velocity = 1;
	Car->isActive = true;
	memcpy(&Car->currCell, &address, sizeof(RLC));
	memset(&Car->nextCell, EMPTY, sizeof(RLC));

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


GLint getFreeCarIndex(car* cars)
{
	for (int i = 0; i < MAX_CARS; i++)
	{
		if (!cars[i].isActive)
		{
			return i;
		}
	}

	return EMPTY;
}


void setCarsToDefault(car* cars)
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
		cars[i].velocity = EMPTY;
		cars[i].isActive = false;
		cars[i].overtake = NONE;
	}
}


GLint getVelocityByRLC(RLC rlc, road* roads)
{
	line* ptrLine = ((roads + rlc.road)->lines + rlc.line)->cells;
	return ptrLine->cells[rlc.cell]->velocity;
}

//change cras' velocity & rlc
void thoughtsOfOneCar(car* car, road* roads)
{
	RLC rlc;
	GLint distance = distanceToForthCar(car->currCell, roads);
	GLint forthCarVelocity = getVelocityByRLC(car->currCell, roads);
	if (distance > car->velocity)
	{
		car->velocity += 1;
	}
	else if ((forthCarVelocity < car->velocity) && isAbleToChangeLine(car, roads, &rlc))
	{
		DIRECTION roadDir = getRoadDir(car, roads);
		car->overtake = getOvertakeDir(roadDir);
		
		car->nextCell.road = rlc.road;
		car->nextCell.line = rlc.line;
		car->nextCell.cell = rlc.cell;

		return;
	}
	else
	{
		car->velocity = distance - 1;
	}
	car->nextCell.cell += car->velocity;
}


void getCarTranslateVector()
{
	int sasasas = 0;
}


//Updates cars' positions in cells and their velocities.
void step(car* cars, road* roads)
{
	for (int i = 0; i < MAX_CARS; i++)
	{
		if (cars[i].isActive)
		{
			unbindCarPtrFromCell(cars, roads);

			reinitCurrCellWithNextCell(cars);

			thoughtsOfOneCar(cars, roads);
		}
	}
}
