#include <cross.h>
#include <road.h>
#include <cars.h>
#include <algorithms.h>

#include <stdlib.h>
#include <stdbool.h>

cross crosses[NUMBER_OF_CROSSES];
cross_roulette rouletteCross = {0, 0};

car* getCarByCrossCell(cross_cell c)
{
    return crosses[c.crossNum].cells[c.x + c.y * CROSS_SIDE];
}

GLvoid initCrossCell(cross_cell* c, car* Car)
{
    crosses[c->crossNum].cells[c->x + c->y * CROSS_SIDE] = Car;
}

GLvoid setCross(GLint crossIndex, GLint* roadIndexes)
{
    int index = -1;
    while (roadIndexes[++index])
    {
        roads[roadIndexes[index]].isBeginCross = true;
        roads[roadIndexes[index]].beginCross = &crosses[crossIndex];
    }
    
}

GLvoid setCarPropertiesOnCross(car* Car)
{
    /*
    dangerous foo as nulifying of cell is done in rebind foo.
    */
    initRoadCell(&Car->currCell, NULL);
    
    Car->currCell.road = NO_ROAD_INDEX;
    Car->currCell.line = NO_LINE_INDEX;
    Car->currCell.cell = NO_CELL_INDEX;

    Car->nextCell.road = NO_ROAD_INDEX;
    Car->nextCell.line = NO_LINE_INDEX;
    Car->nextCell.cell = NO_CELL_INDEX;

    Car->overtake = NONE;
    Car->velocity = _1_CELL_;

}

GLvoid setCarTurningProperties(car* Car)
{
    Car->roadDirMultiplier = Car->target;
    Car->moveDir = Car->target;
    //target doesn't change as then it and the direction are identical and that's why then target is considered default.
}

GLint getCrossEnter(GLint cell, DIRECTION dir)
{
    if (dir == NORTH)   return (cell + HALF_CROSS_SIDE);
    if (dir == SOUTH)   return abs(HALF_CROSS_SIDE - cell);
    if (dir == EAST)    return (cell + HALF_CROSS_SIDE);
    if (dir == WEST)    return abs(HALF_CROSS_SIDE - cell);
}

GLvoid getCrossExitAndRoadFirstCell(GLint enter, DIRECTION dir, DIRECTION target, GLint* exit, GLint* roadFirstCell)
{
    if (dir == NORTH)
    {
        if (target == NORTH || target == EAST)
        {
            *exit = enter;
            *roadFirstCell = HALF_CROSS_SIDE - *exit;
        }
        else if (target == WEST)
        {
            *exit = CROSS_SIDE - enter - 1;
            *roadFirstCell = abs(HALF_CROSS_SIDE - *exit);
        }
    }
    else if (dir == SOUTH)
    {
        if (target == SOUTH ||target == WEST)
        {
            *exit = enter;
            *roadFirstCell = abs(HALF_CROSS_SIDE - *exit);
        }
        else if (target == EAST)
        {
            *exit = CROSS_SIDE - enter - 1;
            *roadFirstCell = *exit - HALF_CROSS_SIDE;
        }
    }
    else if (dir == EAST)
    {
        if (target == EAST || target == NORTH)
        {
            *exit = enter;
            *roadFirstCell = *exit - HALF_CROSS_SIDE;
        }
        if (target == SOUTH)
        {
            *exit = CROSS_SIDE - enter - 1;
            *roadFirstCell = abs(HALF_CROSS_SIDE - *exit);
        }
    }
    else if (dir == WEST)
    {
        if (target == WEST || target == SOUTH)
        {
            *exit = enter;
            *roadFirstCell = abs(HALF_CROSS_SIDE - *exit);
        }
        if (target == NORTH)
        {
            *exit = CROSS_SIDE - enter - 1;
            *roadFirstCell = *exit - HALF_CROSS_SIDE;
        }
    }
}

bool isEndedWithCross(RLC rlc)
{
    return (roads[rlc.road].isEndCross);
}

bool getCarByRouletteCross(car** Car)
{
    car* tmpCar;
	while (rollRouletteCross())
	{
        tmpCar = crosses[rouletteCross.crossNum].cells[rouletteCross.cellNum];
		if (tmpCar != NULL)
		{
			*Car = tmpCar;
			return true;
		}
	}
	return false;
}

bool rollRouletteCross()
{
    if (rouletteCross.cellNum < MAX_CELL_NUM)
    {
        rouletteCross.cellNum += 1;
    }
    else
    {
        rouletteCross.cellNum = 0;
        if (rouletteCross.crossNum < MAX_CROSS_NUM)
        {
            rouletteCross.crossNum += 1;
        }
        else
        {
            rouletteCross.crossNum = 0;
            return false;
        }
    }
    return true;
}

GLvoid stepCross()
{
    car* Car = NULL;
    while (getCarByRouletteCross(&Car))
    {
        rebindCrossCars(Car);
        reinitCrossCells(Car);
    }
    
}

GLvoid rebindCrossCars(car* Car)
{
    cross_cell cell = Car->crossCurrCell;
    initCrossCell(&Car->crossCurrCell, NULL);

    cell = Car->crossNextCell;
    if (cell.crossNum != NEXT_CELL_IS_ON_ROAD)
    {
        initCrossCell(&Car->crossNextCell, Car);
    }
    else
    {
        initRoadCell(&Car->nextCell, Car);
    }
    
}

GLvoid reinitCrossCells(car* Car)
{
    Car->crossCurrCell.crossNum = Car->crossNextCell.crossNum;
    Car->crossCurrCell.x = Car->crossNextCell.x;
    Car->crossCurrCell.y = Car->crossNextCell.y;
}