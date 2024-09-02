#include <cross.h>
#include <macros.h>
#include <map.h>

#include <direction.h>

#include <road.h>
#include <cars.h>
#include <traffic_light.h>
#include <algorithms.h>

#include <stdlib.h>
#include <stdbool.h>

int CROSS_SIDE;
int HALF_CROSS_SIDE;
int CROSS_WIDTH;
int NUMBER_OF_CROSS_CELLS;
int MAX_CELL_NUM;

cross crosses[NUMBER_OF_CROSSES];
cross_roulette rouletteCross = {0, 0};
DIRECTION crossQuaters[8] = {
    NORTH, EAST,
    SOUTH, WEST,
    WEST, NORTH,
    EAST, SOUTH
};

//...................................................................................................................
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

//....................................................................................................................
GLint getCrossEnter(GLint cell, DIRECTION dir)
{
    if (dir == NORTH)   return (cell + HALF_CROSS_SIDE);
    if (dir == SOUTH)   return abs(HALF_CROSS_SIDE - cell) - 1;
    if (dir == EAST)    return (cell + HALF_CROSS_SIDE);
    if (dir == WEST)    return abs(HALF_CROSS_SIDE - cell) - 1;
}

GLint getCrossExit(GLint cell, DIRECTION dir)
{
    if (dir == NORTH)   return abs(HALF_CROSS_SIDE - cell);
    if (dir == SOUTH)   return abs(HALF_CROSS_SIDE - cell) - 1;
    if (dir == EAST)    return abs(HALF_CROSS_SIDE - cell);
    if (dir == WEST)    return abs(HALF_CROSS_SIDE - cell) - 1;
}

GLvoid getCurvingCell(cross_cell* c, car* Car, cross_cell firstCellOnRoad)
{
    c->crossNum = firstCellOnRoad.crossNum;
    DIRECTION dir = Car->moveDir;
    DIRECTION target = Car->target;

    if (dir == NORTH)
    {
        c->x = firstCellOnRoad.x;
        if (target == EAST) {
            c->y = firstCellOnRoad.x;
        }
        else if (target == WEST) {
            c->y = CROSS_SIDE - firstCellOnRoad.x - 1;
        }
    }
    else if (dir == SOUTH)
    {
        c->x = firstCellOnRoad.x;
        if (target == WEST) {
            c->y = firstCellOnRoad.x;
        }
        else if (target == EAST) {
            c->x = CROSS_SIDE - firstCellOnRoad.x - 1;
        }
    }
    else if (dir == EAST)
    {
        c->y = firstCellOnRoad.y;
        if (target == SOUTH) {
            c->x = CROSS_SIDE - firstCellOnRoad.y - 1;
        }
        else if (target == NORTH) {
            c->x = firstCellOnRoad.y;
        }
        
    }
    else if (dir == WEST)
    {
        c->y = firstCellOnRoad.y;
        if (target == NORTH) {
            c->x = CROSS_SIDE - firstCellOnRoad.y - 1;
        }
        else if (target == SOUTH) {
            c->x = firstCellOnRoad.y;
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
        q_item* queue = crosses[crossIndex].carsEndingManeuver.head;
        q_item* item = queue;
        while (item != NULL)
        {
            if (!isAnybodyToDriveBeforeNose(queue, item->value->moveDir)) {
                thoughtsOfOneCar(item->value);
            }
            item = item->next;
        }
    }

    for (GLint crossIndex = 0; crossIndex < NUMBER_OF_CROSSES; crossIndex++)
    {
        q_item* queue = crosses[crossIndex].carsArriving.head;
        q_item* item = queue;
        while (item != NULL)
        {
            if (!isAnybodyToDriveBeforeNose(queue, item->value->moveDir)) {
                thoughtsOfOneCar(item->value);
            }
            item = item->next;
        }
    }
}

bool isAnybodyToDriveBeforeNose(queue* q, DIRECTION ourCarDir)
{
    DIRECTION beforeNoseDir = getRightMoveDir(ourCarDir);
    q_item* item = q->head;
    while (item != NULL) {
        if (item->value->moveDir == beforeNoseDir) {
            return true;
        }
        item = item->next;
    }
    
    return false;
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
    item->next = NULL;

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
    if (isOnCurvingCell(Car))
    {
        Car->moveDir = Car->target;
        Car->roadDirMultiplier = getDirMultiplier(Car->target);
    }

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

bool isOnCurvingCell(car* Car)
{
    return (Car->crossCurrCell.crossNum == Car->curvingCell.crossNum
        && Car->crossCurrCell.x == Car->curvingCell.x
        && Car->crossCurrCell.y == Car->curvingCell.y);
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

//....................................................................................................................
GLvoid addCross(GLint crossIndex, GLfloat start_x, GLfloat start_y, GLint* enterRoadIndexes, GLint* exitRoadIndexes)
{
    
    CROSS_SIDE = (NUMBER_OF_LINES + 1) * 2;
    HALF_CROSS_SIDE = CROSS_SIDE / 2;
    CROSS_WIDTH = CELL_WIDTH * CROSS_SIDE;
    NUMBER_OF_CROSS_CELLS = CROSS_SIDE * CROSS_SIDE;
    MAX_CELL_NUM = CROSS_SIDE * CROSS_SIDE;

    crosses[crossIndex].cells = malloc(sizeof(car*) * NUMBER_OF_CROSS_CELLS);

    setCrossProperties(crossIndex, enterRoadIndexes, exitRoadIndexes);
    // setCross(); // for D.
}

GLvoid setCrossProperties(GLint crossIndex, GLint* enterRoadIndexes, GLint* exitRoadIndexes)
{
    cross* Cross = &crosses[crossIndex];

    for (int i = 0; i < NUMBER_OF_CROSS_ROADS; i++)
    {
        Cross->enterRoadsIndexes[i] = enterRoadIndexes[i];
        Cross->exitRoadsIndexes[i] = exitRoadIndexes[i];
        Cross->enterRoadsPtrs[i] = &roads[enterRoadIndexes[i]];
        Cross->exitRoadsPtrs[i] = &roads[exitRoadIndexes[i]];
    }
    for (GLint i = 0; i < NUMBER_OF_CROSS_CELLS; i++)
    {
        Cross->cells[i] = NULL;
    }
    Cross->carsEndingManeuver.head = NULL;
    Cross->carsEndingManeuver.tail = NULL;
    Cross->carsEndingManeuver.qauntity = 0;
    Cross->carsArriving.head = NULL;
    Cross->carsArriving.tail = NULL;
    Cross->carsArriving.qauntity = 0;


    // binding crosses with roads
    for (int i = 0; i < NUMBER_OF_CROSS_ROADS; i++)
    {
        roads[enterRoadIndexes[i]].isEndCross = true;
        roads[enterRoadIndexes[i]].endCrossNum = crossIndex;
        roads[enterRoadIndexes[i]].endCross = Cross;

        roads[exitRoadIndexes[i]].beginCross = Cross;
        roads[exitRoadIndexes[i]].isBeginCross = true;
    }   
}




