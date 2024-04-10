#pragma once

//velocities from 40 to 90 in cells
typedef enum {
	NO_CAR = -1, _0_CELL, _1_CELL, _2_CELL, _3_CELL, _4_CELL, _5_CELL
} VELOCITY;

GLint distanceToForthCar(RLC rlc, road* roads);
GLint distanceToBackCar(RLC rlc, road* roads);
GLint isAbleToChangeLine(car* car, road* roads, RLC* posOnNewLine);
GLint isSafetyForthAndBack(car* car, road* roads, RLC rlc);

/*
//есть ли возможность обгона
GLint isAbleToOvertake(void);//при обгоне поле nextCell не будет задано
//turn on the corner
GLint isReadyToTurnRight(void);
//turn through the cross
GLint isReadyToTurnLeft(void);
GLint isReadyToChangeLine(void);
*/

GLint distanceToForthCar(RLC rlc, road* roads)
{
	line* ptrLine = getLinePtr(rlc, roads);
	for (int i = 1 + rlc.cell; i < NUMBER_OF_CELLS; ++i)
	{
		if (ptrLine->cells[i]->isActive == true)
		{
			return (i - rlc.cell);
		}
	}
	return EMPTY;
}

GLint distanceToBackCar(RLC rlc, road* roads)
{
	line* ptrLine = getLinePtr(rlc, roads);
	for (int i = -1 + rlc.cell; i >= 0; --i)
	{
		if (ptrLine->cells[i]->isActive == true)
		{
			return (rlc.cell - i);
		}
	}
	return EMPTY;
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

GLint isSafetyForthAndBack(car* car, road* roads, RLC rlc)
{
	GLint forthDistance = distanceToForthCar(rlc, roads);
	GLint backDistance = distanceToBackCar(rlc, roads);

	return (forthDistance > _5_CELL && backDistance >= car->velocity);
}
