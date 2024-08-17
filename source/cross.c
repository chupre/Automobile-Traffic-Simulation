#include "cross.h"
#include <road.h>
#include <cars.h>
#include <algorithms.h>

GLvoid setCross(GLint crossIndex, GLint* roadIndexes)
{
    int index = -1;
    while (roadIndexes[++index])
    {
        roads[roadIndexes[index]].isCross = true;
        roads[roadIndexes[index]].Cross = crosses[crossIndex];
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
    Car->dirOnRoad = Car->target;
    //target doesn't change as then it and the direction are identical and that's why then target is considered default.
}