#pragma once

GLint getVelocityByRLC(RLC rlc, road* roads);
GLvoid step(car* cars, road* roads);
GLvoid excludeOutMappers(car* cars, road* roads);
GLvoid thoughtsOfOneCar(car* car, road* roads);
GLint distanceToForthCar(RLC rlc, road* roads);
GLint distanceToBackCar(RLC rlc, road* roads);
GLint isAbleToChangeLine(car* car, road* roads, RLC* posOnNewLine);
GLint isSafetyForthAndBack(car* car, road* roads, RLC rlc);
GLvoid countSubVelocity(car* car);
float getAdaptedCarVelocity(car* Car);

/*
//turn on the corner
GLint isReadyToTurnRight(void);
//turn through the cross
GLint isReadyToTurnLeft(void);
GLint isReadyToChangeLine(void);
*/

float getAdaptedCarVelocity(car* Car)
{
	return (float)Car->velocity * Car->dirOnRoad * VELOCITY_MULTIPLIER / NUMBER_OF_SUB_STEPS;
}

GLvoid christenNewBornCar(RLC rlc, car* Car, road* Roads)
{
	car** ptrCell = getFirstCellPtr(rlc, Roads);
	ptrCell[rlc.cell] = Car;

	Car->target = rand() % NUMBER_OF_DIRECTIONS;//NONE can't be as it out of range of number of directions
	Car->velocity = _1_CELL * (rand() % NUMBER_OF_VELOCITY_TYPES);
	Car->isActive = true;
	memcpy(&Car->currCell, &rlc, sizeof(RLC));
	memcpy(&Car->nextCell, &rlc, sizeof(RLC));

	Car->dirOnRoad = getCarDirOnRoad(&Roads[rlc.road]);

	thoughtsOfOneCar(Car, Roads);
 }

//change cras' velocity & rlc
GLvoid thoughtsOfOneCar(car* car, road* roads)
{
	RLC rlc;
	GLint distance = distanceToForthCar(car->currCell, roads);
	GLint forthCarVelocity = getVelocityByRLC(car->currCell, roads);
	if (distance > car->velocity || distance == NO_CAR)
	{
		if (car->velocity < MAX_VELOCITY)
		{
			car->velocity += _1_CELL;
		}
	}
	else if ((forthCarVelocity < car->velocity) && isAbleToChangeLine(car, roads, &rlc))
	{
		DIRECTION roadDir = getRoadDir(car, roads);
		car->overtake = getOvertakeDir(roadDir);

		car->nextCell.road = rlc.road;
		car->nextCell.line = rlc.line;
		car->nextCell.cell = rlc.cell;

		printf("\n\nline changed\a\n");
		return;
	}
	else
	{
		car->velocity = distance - 1;
	}
	car->nextCell.cell += car->velocity;
}



GLvoid step(car* cars, road* roads)
{
	for (int i = 0; i < MAX_CARS; i++)
	{
		if (cars[i].isActive && !cars[i].isAvaria)
		{
			if (cars[i].nextCell.cell >= NUMBER_OF_CELLS/*|| cars->nextCell.cell < 0*/)
			{
				printf("excluded car on r %d, l %d, c %d\n", cars[i].currCell.road, cars[i].currCell.line, cars[i].currCell.cell);
				excludeOutMappers(&cars[i], roads);
				continue;
			}

			unbindCarPtrFromCell(&cars[i], roads);

			reinitCurrCellWithNextCell(&cars[i]);

			thoughtsOfOneCar(&cars[i], roads);
		}
	}
}

GLvoid excludeOutMappers(car* cars, road* roads)
{
	car** ptrCell = getFirstCellPtr(cars->currCell, roads);
	GLint start = cars->currCell.cell;
	GLint end = cars->nextCell.cell;
	ptrCell[start] = NULL;
	ptrCell[end] = NULL;

	cars->currCell.road = EMPTY;
	cars->currCell.line = EMPTY;
	cars->currCell.cell = EMPTY;
	cars->nextCell.road = EMPTY;
	cars->nextCell.line = EMPTY;
	cars->nextCell.cell = EMPTY;
	cars->target = NONE;
	cars->velocity = EMPTY;
	cars->isActive = false;
	cars->overtake = NONE;

	cars->dirOnRoad = 0;

	cars->isAvaria = 0;//though there is no sense to zeroize it because cars in Avaria cannot go out the boards of map

	++__freeCars__;
}

GLint getVelocityByRLC(RLC rlc, road* roads)
{
	car** ptrCell = ((roads + rlc.road)->lines + rlc.line)->cells;
	if (ptrCell[rlc.cell] == NULL)
	{
		return NO_CAR;
	}
	else
	{
		return ptrCell[rlc.cell]->velocity;
	}
}

GLint distanceToForthCar(RLC rlc, road* roads)
{
	car** ptrCell = getFirstCellPtr(rlc, roads);
	for (int i = 1 + rlc.cell; i < NUMBER_OF_CELLS; ++i)
	{
		if (ptrCell[i] == NULL)
		{
			continue;
		}
		else if (ptrCell[i]->isActive == true)
		{
			return (i - rlc.cell);
		}
	}
	return NO_CAR;
}

GLint distanceToBackCar(RLC rlc, road* roads)
{
	car** ptrCell = getFirstCellPtr(rlc, roads);
	for (int i = -1 + rlc.cell; i >= 0; --i)
	{
		if (ptrCell[i] == NULL)
		{
			continue;
		}
		else if (ptrCell[i]->isActive == true)
		{
			return (rlc.cell - i);
		}
	}
	return NO_CAR;
}

GLint isAbleToChangeLine(car* car, road* roads, RLC* posOnNewLine)
{
	GLint newLine = car->currCell.line - 1;
	if (newLine >= 0 && newLine <= NUMBER_OF_CELLS)
	{
		RLC rlc = { car->currCell.road, newLine, car->currCell.line };
		if (isSafetyForthAndBack(car, roads, rlc))
		{
			posOnNewLine->road = rlc.road;
			posOnNewLine->line = rlc.line;
			posOnNewLine->cell = rlc.cell;
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}
}

//you shouldn't fear wether the car chooses a position between CurrCell and NextCell
GLint isSafetyForthAndBack(car* car, road* roads, RLC rlc)
{
	GLint forthDistance = distanceToForthCar(rlc, roads);
	GLint backDistance = distanceToBackCar(rlc, roads);

	return (forthDistance > _5_CELL && backDistance >= car->velocity);
}

GLvoid countSubVelocity(car* car)
{
	GLint mainVelocity = car->velocity;

}