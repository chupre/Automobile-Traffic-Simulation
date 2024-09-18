#include <cross.h>
#include <macros.h>
#include <map.h>

#include <direction.h>
#include <map.h>
#include <road.h>
#include <cars.h>
#include <string.h>
#include <traffic_light.h>
#include <algorithms.h>
#include <render.h>

#include <stdlib.h>
#include <stdbool.h>

int CROSS_SIDE;
int HALF_CROSS_SIDE;
float CROSS_WIDTH;

int MAX_CROSS_DIGIT;
int MAX_CROSS_CELL_DIGIT;

cross_roulette rouletteCross = {.crossNum=0, .cellNum=-1};
DIRECTION crossQuaters[8] = {
    NORTH, EAST,
    SOUTH, WEST,
    WEST, NORTH,
    EAST, SOUTH
};

//...................................................................................................................
GLvoid printCrossRoulette(){
    printf("roulette: cross:%d, cell:%d\n", rouletteCross.crossNum, rouletteCross.cellNum);
}

GLvoid printCarCharacter(car* Car){
    if (Car == NULL)
	{
		printf("Car == NULL\n");
	}
	else
	{
		// printf(	"currCell:(%d, %d, %d)\n",
		// 	Car->currCell.road, Car->currCell.line, Car->currCell.cell
        //     );
        printf("ID: %d\n", Car->ID);
        printf(
            "crossCurrCell:(%d, %d, %d)\ncrossNextCell:(%d, %d, %d)\nvelocity:%d\ncurvingCell:  (%d, %d, %d)\n",
            Car->crossCurrCell.crossNum, Car->crossCurrCell.x, Car->crossCurrCell.y,
			Car->crossNextCell.crossNum, Car->crossNextCell.x, Car->crossNextCell.y,
			Car->velocity,
            Car->curvingCell.crossNum, Car->curvingCell.x, Car->curvingCell.y
        );


        if (Car->moveDir == NORTH){
            printf("moveDir: NORTH\n");
        }
        else if (Car->moveDir == SOUTH){
            printf("moveDir: SOUTH\n");
        }
        else if (Car->moveDir == EAST){
            printf("moveDir: EAST\n");
        }
        else if (Car->moveDir == WEST){
            printf("moveDir: WEST\n");
        }
        if (Car->target == NORTH){
            printf("target: NORTH\n");
        }
        else if (Car->target == SOUTH){
            printf("target: SOUTH\n");
        }
        else if (Car->target == EAST){
            printf("target: EAST\n");
        }
        else if (Car->target == WEST){
            printf("target: WEST\n");
        }
	}
}

GLvoid q_print(queue* q){
    q_item* tmp = q->head;
    // printf(".........queue.........: %d\n", q->qauntity);
    GLint count = q->qauntity;
    while (tmp != NULL){
        printf("ID: %d, ", tmp->value->ID);
        // printf("v: %d, ", tmp->value->velocity);
        // printf("act: %d, ", tmp->value->isActive);
        // printf("is: %d, ", getCarByCrossCell(&tmp->value->crossCurrCell) != NULL);
        // printCrossCell(tmp->value->crossCurrCell);
        // printf("!");
        // printCrossCell(tmp->value->crossNextCell);
        printf("\n");
        // printCarCharacter(tmp->value);
        // printf("`\n");
        tmp = tmp->next;
        --count;
    }
    // printf(".........q.............\n");
    
}

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
            c->y = CROSS_SIDE - firstCellOnRoad.x - 1;
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
    if (rouletteCross.cellNum < MAX_CROSS_CELL_DIGIT){
        rouletteCross.cellNum += 1;
    }else{
        rouletteCross.cellNum = -1;
        if (rouletteCross.crossNum < MAX_CROSS_DIGIT){
            rouletteCross.crossNum += 1;
        }
        else{
            rouletteCross.crossNum = 0;
            return false;
        }
    }
    return true;
}

GLvoid stepCross()
{
    car* Car = NULL;

    if (yellowRedChange == true)
    {
        yellowRedChange = false;
        for (GLint crossIndex = 0; crossIndex < NUMBER_OF_CROSSES; crossIndex++)
        {
            // printf("q_fill\n");
            q_fill(&crosses[crossIndex].carsEndingManeuver, &crosses[crossIndex].carsArriving);
            // if (crosses[crossIndex].carsEndingManeuver.head != NULL) q_print(&crosses[crossIndex].carsEndingManeuver);
        }
    }
    // printf("MANEUVER: %d\n", crosses[0].carsEndingManeuver.qauntity);// q_print(&crosses[0].carsEndingManeuver);
    // printf("ARRIVE: %d\n", crosses[0].carsArriving.qauntity);// q_print(&crosses[0].carsArriving);

    for (GLint crossIndex = 0; crossIndex < NUMBER_OF_CROSSES; crossIndex++)
    {
        q_item* item = crosses[crossIndex].carsEndingManeuver.head;
        // printf(">> m <<\n");
        while (item != NULL)
        {
            // printf("ID: %d\n", item->value->ID);
            thoughtsOfOneCarOnCross(item->value);
            // printf("CROSS\n");
            // printCarCharacter(item->value);
            item = item->next;
        }
        // printf("<< m >>\n");
    }

    for (GLint crossIndex = 0; crossIndex < NUMBER_OF_CROSSES; crossIndex++)
    {
        queue* q = &crosses[crossIndex].carsEndingManeuver;
        q_item* item = crosses[crossIndex].carsArriving.head;
        // printf(">> a <<\n");
        while (item != NULL)
        {
            if (!isAnybodyToDriveBeforeNose(q, item->value->moveDir)) {
                // printf("ID: %d\n", item->value->ID);
                thoughtsOfOneCarOnCross(item->value);
                // printf("CROSS\n");
                // printCarCharacter(item->value);
            }else{
                item->value->velocity = _0_CELL_;
            }
            item = item->next;
        }
        // printf("<< a >>\n");
    }
    // printf("MANEUVER\n"); q_print(&crosses[0].carsEndingManeuver);
    // printf("ARRIVE\n"); q_print(&crosses[0].carsArriving);
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
    initCrossCell(&Car->crossCurrCell, NULL);

    if (Car->crossNextCell.crossNum != NEXT_CELL_IS_ON_ROAD){
        initCrossCell(&Car->crossNextCell, Car);
        Car->crossCurrCell = Car->crossNextCell;
        // printf(">> INIT CROSS   << ID: %d\n", Car->ID);
    }
    else{
        initRoadCell(&Car->nextCell, Car);
        Car->currCell = Car->nextCell;
        q_del_item(&crosses[Car->crossCurrCell.crossNum].carsEndingManeuver, Car);
        q_del_item(&crosses[Car->crossCurrCell.crossNum].carsArriving, Car);
        // printf(">> NEXT ON ROAD << ID: %d\n", Car->ID);
    }   
}

//....................................................................................................................
GLvoid q_append(car* Car, queue* q)
{
    // printf("APPEND ID: %d\n", Car->ID); //printCarCharacter(Car);
    q_item* item = (q_item*) malloc(sizeof(q_item));
    if (item == NULL) exit(-1);
    item->value = Car;
    item->next = NULL;
    // printf("***\n");
    // q_print(q);
    // printf("***\n");

    // if (q->head) printf("head ID: %d\n", q->head->value->ID);
    // if (q->tail) printf("tail ID: %d\n", q->tail->value->ID);

    if (q->head && q->tail) {
        q->tail->next = item;
        q->tail = item;
    }else{
        q->head = item;
        q->head->next = NULL;
        q->tail = item;
    }
    ++q->qauntity;

    // printf("======= arrive queue =======\n");
    // q_print(q);
    // printf("============================\n");

}

car* q_pop(queue* q)
{
    if (q->head == NULL){
        exit(-2);
    }
    car* Car = q->head->value;
    if (q->qauntity > 1) {
        q_item* tmp = q->head;
        q->head = q->head->next;
        free(tmp);
    }
    else {
        free(q->head);
        q->head = NULL;
        q->tail = NULL;
    }
    --q->qauntity;
    return Car;
}

GLvoid q_fill(queue* dest, queue* src)
{
    if (src->qauntity == 0){
        return;
    }
    if (dest->qauntity == 0){
        dest->head = src->head;
    }
    else{
        dest->tail->next = src->head;
    }
    
    dest->tail = src->tail;
    dest->qauntity += src->qauntity;

    src->head = NULL;
    src->tail = NULL;
    src->qauntity = 0;
}

GLvoid q_delete(queue* q)
{
    if (q->head == NULL){
        return;
    }
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

GLvoid q_del_item(queue* q, car* Car){
    if (q->head == NULL){
        return;
    }
    if (q->head->value == Car){
        q_item* del_item = q->head;
        if (q->head == q->tail){
            q->head = NULL;
            q->tail = NULL;
        }
        else{
            q->head = q->head->next;
        }
        free(del_item);
        --q->qauntity;
        return;
    }
    q_item* prev = q->head;
    q_item* tmp = prev->next;
    while (tmp != NULL){
        if (tmp->value == Car){
            prev->next = tmp->next;
            if (q->tail == tmp){
                q->tail = prev;
            }
            free(tmp);
            --q->qauntity;
            return;
        }
        prev = tmp;
        tmp = tmp->next;
    }
}
//....................................................................................................................
GLvoid thoughtsOfOneCarOnCross(car* Car)
{
    if (isItCurvingCell(&Car->crossCurrCell, &Car->curvingCell)){

                if (Car->moveDir == NORTH) {
            if (Car->target == WEST) {
                glm_rotate2d(carTransformMatrixes[Car->ID], glm_rad(90.0f));
            }
            if (Car->target == EAST) {
                glm_translate2d_y(carTransformMatrixes[Car->ID], CELL_LENGTH);
                glm_rotate2d(carTransformMatrixes[Car->ID], glm_rad(-90.0f));
            }
        }
        if (Car->moveDir == SOUTH) {
            if (Car->target == WEST) {
                glm_translate2d_y(carTransformMatrixes[Car->ID], CELL_LENGTH);
                glm_rotate2d(carTransformMatrixes[Car->ID], glm_rad(-90.0f));
            }
            if (Car->target == EAST) {
                glm_rotate2d(carTransformMatrixes[Car->ID], glm_rad(90.0f));
            }
        }
        if (Car->moveDir == WEST) {
            if (Car->target == NORTH) {
                glm_translate2d_y(carTransformMatrixes[Car->ID], CELL_LENGTH);
                glm_rotate2d(carTransformMatrixes[Car->ID], glm_rad(-90.0f));
            }
            if (Car->target == SOUTH) {
                glm_rotate2d(carTransformMatrixes[Car->ID], glm_rad(90.0f));
            }
        }
        if (Car->moveDir == EAST) {
            if (Car->target == NORTH) {
                glm_rotate2d(carTransformMatrixes[Car->ID], glm_rad(90.0f));
            }
            if (Car->target == SOUTH) {
                glm_translate2d_y(carTransformMatrixes[Car->ID], CELL_LENGTH);
                glm_rotate2d(carTransformMatrixes[Car->ID], glm_rad(-90.0f));
            }
        }



        Car->moveDir = Car->target;
        Car->roadDirMultiplier = getDirMultiplier(Car->target);
    }
    cross_cell c;
    getNextCrossCell(Car, &c);

    if (isInCrossBoards(&c)){

        countCarVelocityAndNextCell(Car, &c);
        if (Car->velocity != _0_CELL_/*!isCrossCellsEqual(&Car->crossCurrCell, &Car->crossNextCell)*/){
            initCrossCell(&Car->crossNextCell, OCCUPYING_CAR);
            // printf("occupy is set\n");
        }
    }else{
        RLC rlc;
        transformCrossCellIntoRLC(&rlc, Car);
        if (getCarPtr(&rlc) == NULL){
            Car->nextCell = rlc;
            Car->crossNextCell.crossNum = NEXT_CELL_IS_ON_ROAD;
            Car->velocity = _1_CELL_;
            initRoadCell(&rlc, OCCUPYING_CAR);
        }else{
            Car->velocity = _0_CELL_;
        }
    }    
}

bool isItCurvingCell(cross_cell* crossCurrCell,  cross_cell* curvingCell)
{
    // printf("curr\n");printCrossCell(*crossCurrCell);
    // printf("curving\n");printCrossCell(*curvingCell);
    return (crossCurrCell->crossNum == curvingCell->crossNum
        && crossCurrCell->x == curvingCell->x
        && crossCurrCell->y == curvingCell->y);
}

GLvoid getNextCrossCell(car* Car, cross_cell* c)
{
    // printf("==========[ID: %d]=========\n", Car->ID);
    *c = Car->crossCurrCell;
    cross_cell cell = *c;
    if (Car->moveDir == NORTH){
        if (c->y - 1 < 0){
            c->y -= 1;//in order to be processed in foo isInCrossBoards()
            return;
        }
        for (cell.y -= 1; cell.y >= 0; cell.y--){
            if (!checkCrossCellSafety(&cell, Car->moveDir)){
                break;
            }
            if (isItCurvingCell(&cell, &Car->curvingCell)){
                *c = cell;
                return;
            }
        }
        c->y = cell.y + 1;
    }
    else if (Car->moveDir == SOUTH){
        if (c->y + 1 >= CROSS_SIDE){
            c->y += 1;
            return;
        }
        for (cell.y += 1; cell.y < CROSS_SIDE; cell.y++){
            if (!checkCrossCellSafety(&cell, Car->moveDir)){
                break;
            }
            if (isItCurvingCell(&cell, &Car->curvingCell)){
                *c = cell;
                return;
            }
        }
        c->y = cell.y - 1;
    }
    else if (Car->moveDir == EAST){
        if (c->x + 1 >= CROSS_SIDE){
            c->x += 1;
            return;
        }
        for (cell.x += 1; cell.x < CROSS_SIDE; cell.x++){
            if (!checkCrossCellSafety(&cell, Car->moveDir)){
                break;
            }
            if (isItCurvingCell(&cell, &Car->curvingCell)){
                *c = cell;
                return;
            }
        }
        c->x = cell.x - 1;
    }
    else if (Car->moveDir == WEST){
        if (c->x - 1 < 0){
            c->x -= 1;
            return;
        }
        for (cell.x -= 1; cell.x >= 0; cell.x--){
            if (!checkCrossCellSafety(&cell, Car->moveDir)){
                break;
            }
            if (isItCurvingCell(&cell, &Car->curvingCell)){
                *c = cell;
                return;
            }
        }
        c->x = cell.x + 1;
    }
}

bool checkCrossCellSafety(cross_cell* c, DIRECTION dir){
    // printf("  f: ");
    // printCrossCell(*c);
    car* Car = getCarByCrossCell(c);
    if (Car != NULL){
        // printf("problem ID: %d: ", Car->ID);
        // printf("!=null\n");
        return false;
    }
    bool RESULT_1 = true, RESULT_2 = true;
    // printf("\n");
    cross_cell tmp = *c;
    if (dir == NORTH || dir == SOUTH){
        // printf("--x:\n");
        for (tmp.x -= 1; tmp.x >= 0; tmp.x--){
            Car = getCarByCrossCell(&tmp);
            if (Car == NULL/* || Car == OCCUPYING_CAR*/){
                // printf("null car\n");
                continue;
            }
            if (Car == OCCUPYING_CAR){
                // printf("occupy car\n");
                continue;
            }
            if (Car->moveDir == getLeftMoveDir(dir) || Car->moveDir == getRightMoveDir(dir)){
                // printf("problem ID: %d: ", Car->ID); printCrossCell(tmp);
                if (c->x - Car->crossNextCell.x > 0){
                    // printf("true\n");
                    RESULT_1 = true;
                    break;
                }else{
                    // printf("false\n");
                    RESULT_1 = false;
                    break;
                }
            }
        }
        tmp = *c;
        // printf("++x:\n");
        for (tmp.x += 1; tmp.x < CROSS_SIDE; tmp.x++){
            Car = getCarByCrossCell(&tmp);
            if (Car == NULL/* || Car == OCCUPYING_CAR*/){
                // printf("null car\n");
                continue;
            }
            if (Car == OCCUPYING_CAR){
                // printf("occupy car\n");
                continue;
            }
            if (Car->moveDir == getLeftMoveDir(dir) || Car->moveDir == getRightMoveDir(dir)){
                // printf("problem ID: %d: ", Car->ID); printCrossCell(tmp);
                if (Car->crossNextCell.x - c->x > 0){
                    // printf("true\n");
                    RESULT_2 = true;
                    break;
                }else{
                    // printf("false\n");
                    RESULT_2 = false;
                    break;
                }
            }
        }

    }else{
        // printf("--y:\n");
        for (tmp.y -= 1; tmp.y >= 0; tmp.y--){
            Car = getCarByCrossCell(&tmp);
            if (Car == NULL/* || Car == OCCUPYING_CAR*/){
                // printf("null car\n");
                continue;
            }
            if (Car == OCCUPYING_CAR){
                // printf("occupy car\n");
                continue;
            }
            if (Car->moveDir == getLeftMoveDir(dir) || Car->moveDir == getRightMoveDir(dir)){
                // printf("problem ID: %d: ", Car->ID); printCrossCell(tmp);
                if (c->y - Car->crossNextCell.y > 0){
                    // printf("true\n");
                    RESULT_1 = true;
                    break;
                }else{
                    // printf("false\n");
                    RESULT_1 = false;
                    break;
                }
            }
        }
        tmp = *c;
        // printf("++y:\n");
        for (tmp.y += 1; tmp.y < CROSS_SIDE; tmp.y++){
            Car = getCarByCrossCell(&tmp);
            if (Car == NULL/* || Car == OCCUPYING_CAR*/){
                // printf("null car\n");
                continue;
            }
            if (Car == OCCUPYING_CAR){
                // printf("occupy car\n");
                continue;
            }
            if (Car->moveDir == getLeftMoveDir(dir) || Car->moveDir == getRightMoveDir(dir)){
                // printf("problem ID: %d, ", Car->ID); printCrossCell(tmp);
                if (Car->crossNextCell.y - c->y > 0){
                    // printf("true\n");
                    RESULT_2 = true;
                    break;
                }else{
                    // printf("false\n");
                    RESULT_2 = false;
                    break;
                }
            }
        }
    }
    return (RESULT_1 && RESULT_2);//in case when a car touches the board of cross
}

GLvoid countCarVelocityAndNextCell(car* Car, cross_cell* c){
    if (Car->moveDir == NORTH){
        Car->velocity = Car->crossCurrCell.y - c->y;
        if (Car->velocity > MAX_CROSS_VELOCITY){
            Car->velocity = MAX_CROSS_VELOCITY;
        }
        Car->crossNextCell = *c;
        Car->crossNextCell.y = Car->crossCurrCell.y - Car->velocity;
    }
    else if (Car->moveDir == SOUTH){
        Car->velocity = c->y - Car->crossCurrCell.y;
        if (Car->velocity > MAX_CROSS_VELOCITY){
            Car->velocity = MAX_CROSS_VELOCITY;
        }
        Car->crossNextCell = *c;
        Car->crossNextCell.y = Car->crossCurrCell.y + Car->velocity;
    }
    else if (Car->moveDir == EAST){
        Car->velocity = c->x - Car->crossCurrCell.x;
        if (Car->velocity > MAX_CROSS_VELOCITY){
            Car->velocity = MAX_CROSS_VELOCITY;
        }
        Car->crossNextCell = *c;
        Car->crossNextCell.x = Car->crossCurrCell.x + Car->velocity;
    }
    else if (Car->moveDir == WEST){
        Car->velocity = Car->crossCurrCell.x - c->x;
        if (Car->velocity > MAX_CROSS_VELOCITY){
            Car->velocity = MAX_CROSS_VELOCITY;
        }
        Car->crossNextCell = *c;
        Car->crossNextCell.x = Car->crossCurrCell.x - Car->velocity;
    }
    
    // if (Car->velocity == _0_CELL_){
    //     printf("_0_CELL_");
    //     printf("ID: %d, ", Car->ID);
    //     printCrossCell(Car->crossCurrCell);
    //     printCarCharacter(Car);
    // }

}

bool isCrossCellsEqual(cross_cell* c, cross_cell* d){
    return (c->crossNum == d->crossNum && c->x == c->x && c->y == d->y);
}

bool isInCrossBoards(cross_cell* c)
{
    return (c->x >= 0 && c->x < CROSS_SIDE && c->y >= 0 && c->y < CROSS_SIDE);
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
    c->crossNum = roads[Car->currCell.road].endCrossNum;
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
    NUMBER_OF_CROSS_CELLS = CROSS_SIDE * CROSS_SIDE;
    HALF_CROSS_SIDE = CROSS_SIDE / 2;
    MAX_CROSS_DIGIT = (NUMBER_OF_CROSSES - 1);
    MAX_CROSS_CELL_DIGIT = (NUMBER_OF_CROSS_CELLS - 1);


#ifdef DEBUG
    DEFAULT_FOV = 60;

    vec2 top_left = { roads[enterRoadIndexes[2]].startLineCoord, roads[enterRoadIndexes[1]].startLineCoord };  
    vec2 bottom_right = { roads[enterRoadIndexes[3]].startLineCoord, roads[enterRoadIndexes[0]].startLineCoord };

    cellsVertices = malloc(sizeof(float) * 2 * 5 * (CROSS_SIDE + NUMBER_OF_CELLS * 2 + 1) * 2);
    
    for (int i = 0; i < CROSS_SIDE + NUMBER_OF_CELLS * 2 + 1; i++) {
        float line_vertices[10] = {
            top_left[0], top_left[1] - CELL_WIDTH * i, 1.0f, 0.0f, 0.0f,
            bottom_right[0], top_left[1] - CELL_WIDTH * i, 1.0f, 0.0f, 0.0f
        };
        memcpy(&cellsVertices[2 * 5 * i], line_vertices, 2 * 5 * sizeof(float));
    }

    for (int i = 0; i < CROSS_SIDE + NUMBER_OF_CELLS * 2 + 1; i++) {
        float line_vertices[10] = {
            top_left[0] + CELL_WIDTH * i, top_left[1], 1.0f, 0.0f, 0.0f,
            top_left[0] + CELL_WIDTH * i, bottom_right[1], 1.0f, 0.0f, 0.0f
        };
        memcpy(&cellsVertices[2 * 5 * (CROSS_SIDE + NUMBER_OF_CELLS * 2 + 1) + 2 * 5 * i], line_vertices, 2 * 5 * sizeof(float));
    }
#endif

    crosses[crossIndex].cells = malloc(sizeof(car*) * NUMBER_OF_CROSS_CELLS);

    if (crosses[crossIndex].cells == NULL) {
        printf("malloc failed on crosses cells on index %d", crossIndex);
        exit(1);
    }

    setCrossProperties(crossIndex, enterRoadIndexes, exitRoadIndexes);
    // setCross(); // for D.
}

GLvoid setCrossProperties(GLint crossIndex, GLint* enterRoadIndexes, GLint* exitRoadIndexes)
{
    cross* Cross = &crosses[crossIndex];

    for (int i = 0; i < NUMBER_OF_CROSS_ROADS; i++){
        Cross->enterRoadsIndexes[i] = enterRoadIndexes[i];
        Cross->exitRoadsIndexes[i] = exitRoadIndexes[i];
        Cross->enterRoadsPtrs[i] = &roads[enterRoadIndexes[i]];
        Cross->exitRoadsPtrs[i] = &roads[exitRoadIndexes[i]];
    }
    for (GLint i = 0; i < NUMBER_OF_CROSS_CELLS; i++){
        Cross->cells[i] = NULL;
    }
    Cross->carsEndingManeuver.head = NULL;
    Cross->carsEndingManeuver.tail = NULL;
    Cross->carsEndingManeuver.qauntity = 0;
    Cross->carsArriving.head = NULL;
    Cross->carsArriving.tail = NULL;
    Cross->carsArriving.qauntity = 0;
    // filling roads with data about cross
    for (int i = 0; i < NUMBER_OF_CROSS_ROADS; i++){
        roads[enterRoadIndexes[i]].isEndCross = true;
        roads[enterRoadIndexes[i]].isBeginCross = false;//this property is to be set outside the foo
        roads[enterRoadIndexes[i]].endCrossNum = crossIndex;
        roads[enterRoadIndexes[i]].endCross = Cross;

        roads[exitRoadIndexes[i]].beginCross = Cross;
        roads[exitRoadIndexes[i]].isBeginCross = true;
        roads[exitRoadIndexes[i]].isEndCross = false;
    }
    for (int i = 0; i < NUMBER_OF_CROSS_ROADS; i++){
        GLint index = getFreeTrafficLightIndex();
        roads[enterRoadIndexes[i]].traffic_light_index = index;
        roads[enterRoadIndexes[i]].traffic_light_ptr = &lights[index];
        // printf("dir: %d, enter: %d, light_index: %d\n", roads[enterRoadIndexes[i]].dir, enterRoadIndexes[i], index);
    }
    setDefaultTrafficLightProperties();
}




