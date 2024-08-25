#include <cross.h>
#include <road.h>
#include <cars.h>
#include <algorithms.h>

#include <stdlib.h>
#include <stdbool.h>

cross crosses[NUMBER_OF_CROSSES];

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
    RLC rlc = Car->currCell;
    roads[rlc.road].lines[rlc.line].cells[rlc.cell] = NULL;
    
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

bool isEndCross(RLC rlc)
{
    return (roads[rlc.road].isEndCross);
}