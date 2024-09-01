#include <cross.h>
#include <macros.h>

#include <direction.h>

#include <road.h>
#include <cars.h>
#include <traffic_light.h>
#include <algorithms.h>

#include <stdlib.h>
#include <stdbool.h>

cross crosses[NUMBER_OF_CROSSES];
cross_roulette rouletteCross = {0, 0};
DIRECTION crossQuaters[8] = {
    NORTH, EAST,
    SOUTH, WEST,
    WEST, NORTH,
    EAST, SOUTH
};

//..................................................................................................................
car* getCarByCrossCell(cross_cell* c)
{
    return crosses[c->crossNum].cells[c->x + c->y * CROSS_SIDE];
}

GLvoid initCrossCell(cross_cell* c, car* Car)
{
    crosses[c->crossNum].cells[c->x + c->y * CROSS_SIDE] = Car;
}

bool isEndedWithCross(RLC* rlc)
{
    return (roads[rlc->road].isEndCross);
}

//...................................................................................................................
GLvoid setCrossProperties(GLint crossIndex, GLint* roadIndexes)
{
    //binding crosses with roads
    int index = -1;
    while (roadIndexes[++index]) /* roads' indexes correspond roads' directions (NORTH == 0, SOUTH == 1, EAST == 2, WEST == 3)*/
    {
        roads[roadIndexes[index]].isBeginCross = true;
        roads[roadIndexes[index]].beginCross = &crosses[crossIndex];
        roads[roadIndexes[index]].endCrossNum = crossIndex;
    }
    //
    cross* Cross = &crosses[crossIndex];
    for (int i = 0; i < NUMBER_OF_CROSS_ROADS; i++)
    {
        Cross->enterRoadsIndexes[i] = roadIndexes[i];
    }
    

    Cross->carsEndingManeuver.head = NULL;
    Cross->carsEndingManeuver.tail = NULL;
    Cross->carsEndingManeuver.qauntity = 0;
    Cross->carsArriving.head = NULL;
    Cross->carsArriving.tail = NULL;
    Cross->carsArriving.qauntity = 0;
    //nullify car* pointers
    for (GLint i = 0; i < NUMBER_OF_CROSS_CELLS; i++)
    {
        Cross->cells[i] = NULL;
        
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

GLint getCrossExit(GLint cell, DIRECTION dir)
{
    if (dir == NORTH)   return abs(HALF_CROSS_SIDE - cell);
    if (dir == SOUTH)   return abs(HALF_CROSS_SIDE - cell) - 1;
    if (dir == EAST)    return abs(HALF_CROSS_SIDE - cell);
    if (dir == WEST)    return abs(HALF_CROSS_SIDE - cell) - 1;
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

//....................................................................................................................
GLint getQuaterNum(cross_cell c)
{
    if (c.x < HALF_CROSS_SIDE) {
        if (c.y < HALF_CROSS_SIDE) {
            return 2;
        }
        else return 4;
    }
    else {
        if (c.y < HALF_CROSS_SIDE){
            return 3;
        }
        else return 1;
    }
}

cross_flood getCodirectional(DIRECTION carDir, GLint quaterNum)
{
    if (carDir == crossQuaters[quaterNum * TWO /*+ ENTER*/])
    {
        return ENTER;
    }
    if (carDir == crossQuaters[quaterNum * TWO + EXIT])
    {
        return EXIT;
    }
    exit(-182);
}


//....................................................................................................................
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

    if (yellowRedChange)
    {
        yellowRedChange = false;
        for (GLint crossIndex = 0; crossIndex < NUMBER_OF_CROSSES; crossIndex++)
        {
            q_fill(&crosses[crossIndex].carsEndingManeuver, &crosses[crossIndex].carsArriving);
        }
    }
 
    for (GLint crossIndex = 0; crossIndex < NUMBER_OF_CROSSES; crossIndex++)
    {
        q_item* item = crosses[crossIndex].carsEndingManeuver.head;
        while (item != NULL)
        {
            thoughtsOfOneCar(item->value);
            item = item->next;
        }
    }

    for (GLint crossIndex = 0; crossIndex < NUMBER_OF_CROSSES; crossIndex++)
    {
        q_item* item = crosses[crossIndex].carsArriving.head;
        while (item != NULL)
        {
            thoughtsOfOneCar(item->value);
            item = item->next;
        }
    }
}

//....................................................................................................................
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
    if (Car->crossNextCell.crossNum != NEXT_CELL_IS_ON_ROAD)
    {
        Car->crossCurrCell = Car->crossNextCell;
    }
}

//....................................................................................................................
GLvoid q_append(car* Car, queue* q)
{
    q_item* item = (q_item*) malloc(sizeof(q_item*));
    if (item == NULL) exit(-1);
    item->value = Car;

    if (q->qauntity > 0) {
        q->tail->next = item;
    }
    else {
        q->head = item;
    }
    q->tail = item;

    ++q->qauntity;
}

car* q_pop(queue* q)
{
    car* Car = q->head->value;
    if (q->qauntity > 1) {
        q->head = q->head->next;
    }
    else {
        q->head = NULL;
        q->tail = NULL;
    }
    --q->qauntity;
    return Car;
}

GLvoid q_fill(queue* dest, queue* src)
{
    q_delete(dest);
    dest->head = src->head;
    dest->tail = src->tail;
    dest->qauntity = dest->qauntity;
}

GLvoid q_delete(queue* q)
{
    q_item* tmp;
    GLint quantity = q->qauntity;
    while (--quantity >= 0)
    {
        tmp = q->head;
        q->head = q->head->next;
        free(tmp);
    }
    q->head = NULL;
    q->tail = NULL;
    q->qauntity = 0;
}

//....................................................................................................................
GLvoid thoughtsOfOneCarOnCross(car* Car)
{
    bool frontCheck, sideCheck;
    cross_flood flood = getCodirectional(Car->moveDir, getQuaterNum(Car->crossCurrCell));
    
    cross_cell c;
    getNextCrossCell(Car, &c);

    if (isInCrossBoards(c))
    {
        frontCheck = isSafetyFront(c);
        if (flood == ENTER) {
            sideCheck = isSafetyLeft(c, Car);
        }
        else {
            sideCheck = isSafetyRight(c, Car);
        }

        if (frontCheck && sideCheck) {
            Car->crossNextCell = c;
            Car->velocity = CROSS_VELOCITY;
        }
        else {
            Car->velocity = _0_CELL_;
        }
    }
    else
    {
        RLC rlc;
        transformCrossCellIntoRLC(&rlc, Car);
        if (getCarPtr(&rlc) == NULL) {
            Car->nextCell = rlc;
            Car->velocity = _1_CELL_;
        }
        else {
            Car->velocity = _0_CELL_;
        }
        /*else Car->crossNextCell = Car->crossCurrCell; is useless as before thoughtsOfOneCarOnCross the pointers were rebinded.*/ 
    }
}

GLvoid getNextCrossCell(car* Car, cross_cell* c)
{
    *c = Car->crossCurrCell;
    if (Car->moveDir == NORTH) {
        --c->y;
    }
    else if (Car->moveDir == SOUTH) {
        ++c->y;
    }
    else if (Car->moveDir == EAST) {
        ++c->x;
    }
    else if (Car->moveDir == WEST) {
        --c->x;
    }
}

bool isInCrossBoards(cross_cell c)
{
    return (c.x >= 0 && c.x < CROSS_SIDE && c.y >= 0 && c.y < CROSS_SIDE);
}

/*the foo is applied after rebinding pointers. by the momemnt the foo is carried cars have identical next and current cells*/
/*cars the foo is applied are false-cheked, has identical next and cureent cells*/
bool isSafetyLeft(cross_cell cell, car* Car)
{
    cross_cell c = cell;
    car* leftCar = NULL;
    GLint leftCarCoord = - 1;
    if (Car->moveDir == NORTH) //y is the same
    {
        while (--(c.x) >= 0)
        {
            if ((leftCar = getCarByCrossCell(&c)) != NULL)
            {
                leftCarCoord = leftCar->crossNextCell.x;
                return (cell.x - leftCarCoord >= 1);
            }
        }
    }
    else if (Car->moveDir == SOUTH) //y is the same
    {
        while (++(c.x) < CROSS_SIDE)
        {
            if ((leftCar = getCarByCrossCell(&c)) != NULL)
            {
                leftCarCoord = leftCar->crossNextCell.x;
                return (leftCarCoord - cell.x >= 1);
            }
        }
    }
    else if (Car->moveDir == EAST) //x is the same
    {
        while (--(c.y) >= 0)
        {
            if ((leftCar = getCarByCrossCell(&c)) != NULL)
            {
                leftCarCoord = leftCar->crossNextCell.y;
                return (cell.y - leftCarCoord);
            }
        }
    }
    else if (Car->moveDir == WEST) //x is the same
    {
        while (++(c.y) < CROSS_SIDE)
        {
            if ((leftCar = getCarByCrossCell(&c)) != NULL)
            {
                leftCarCoord = leftCar->crossNextCell.y;
                return (leftCarCoord - cell.y);
            }
        }
    }
    return false;
}

bool isSafetyRight(cross_cell cell, car* Car)
{
    cross_cell c = cell;
    car* leftCar = NULL;
    GLint leftCarCoord = - 1;
    if (Car->moveDir == NORTH) //y is the same
    {
        while (++(c.x) < CROSS_SIDE)
        {
            if ((leftCar = getCarByCrossCell(&c)) != NULL)
            {
                leftCarCoord = leftCar->crossNextCell.x;
                return (leftCarCoord - cell.x >= 1);
            }
        }
    }
    else if (Car->moveDir == SOUTH) //y is the same
    {
        while (--(c.x) >= 0)
        {
            if ((leftCar = getCarByCrossCell(&c)) != NULL)
            {
                leftCarCoord = leftCar->crossNextCell.x;
                return (cell.x - leftCarCoord >= 1);
            }
        }
    }
    else if (Car->moveDir == EAST) //x is the same
    {
        while (++(c.y) < CROSS_SIDE)
        {
            if ((leftCar = getCarByCrossCell(&c)) != NULL)
            {
                leftCarCoord = leftCar->crossNextCell.y;
                return (leftCarCoord - cell.y);
            }
        }
    }
    else if (Car->moveDir == WEST) //x is the same
    {
        while (--(c.y) >= 0)
        {
            if ((leftCar = getCarByCrossCell(&c)) != NULL)
            {
                leftCarCoord = leftCar->crossNextCell.y;
                return (cell.y - leftCarCoord);
            }
        }
    }
    return false;
}

bool isSafetyFront(cross_cell cell)
{
    return (getCarByCrossCell(&cell) == NULL);
}

GLvoid transformCrossCellIntoRLC(RLC* rlc, car* Car)
{
    rlc->road = crosses[Car->crossCurrCell.crossNum].exitRoadsIndexes[Car->moveDir];
    rlc->line = getLineOfAppearingOnRoadFromCross(Car);
    rlc->cell = 0;
}

GLint getLineOfAppearingOnRoadFromCross(car* Car)
{
    if (Car->moveDir == NORTH) return getCrossExit(Car->crossCurrCell.x, Car->moveDir);
    if (Car->moveDir == SOUTH) return getCrossExit(Car->crossCurrCell.x, Car->moveDir);
    if (Car->moveDir == EAST) return getCrossExit(Car->crossCurrCell.y, Car->moveDir);
    if (Car->moveDir == WEST) return getCrossExit(Car->crossCurrCell.y, Car->moveDir);
}

//....................................................................................................................
GLvoid transformRLCIntoCrossCell(cross_cell* c, car* Car)
{
    Car->crossNextCell.crossNum = roads[Car->currCell.road].endCrossNum;
    if (Car->moveDir == NORTH)
    {
        c->x = getCrossEnter(Car->currCell.line, Car->moveDir);
        c->y = CROSS_SIDE - 1;
    }
    else if (Car->moveDir == SOUTH)
    {
        c->x = getCrossEnter(Car->currCell.line, Car->moveDir);
        c->y = 0;
    }
    else if (Car->moveDir == EAST)
    {
        c->y = getCrossEnter(Car->currCell.line, Car->moveDir);
        c->x = 0;
    }
    else
    {
        c->y = getCrossEnter(Car->currCell.line, Car->moveDir);
        c->x = CROSS_SIDE - 1;
    }
}





