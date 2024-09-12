#include <cross.h>
#include <macros.h>
#include <map.h>

#include <direction.h>

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
int NUMBER_OF_CROSS_CELLS;

cross crosses[NUMBER_OF_CROSSES];
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
        return;
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
        printf("ID: %d\n", tmp->value->ID);
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
    // printf("del ID: %d\n", Car->ID);
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
        // printf("AFTER_DEL _h\n");
        // q_print(q);
        // printf("after_del\n");
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
            // if (prev->next == NULL) printf("tail->next==NULL\n");
            free(tmp);
            --q->qauntity;
            // printf("AFTER_DEL_t\n");
            // q_print(q);
            // printf("after_del\n");
            return;
        }
        prev = tmp;
        tmp = tmp->next;
    }
}
//....................................................................................................................
GLvoid thoughtsOfOneCarOnCross(car* Car)
{
    if (isOnCurvingCell(Car)){
        Car->moveDir = Car->target;
        Car->roadDirMultiplier = getDirMultiplier(Car->target);
    }
    cross_cell c;
    getNextCrossCell(Car, &c);
    // printf("curr:");printCrossCell(Car->crossCurrCell);
    // printf("NEXT:");printCrossCell(c);

    if (isInCrossBoards(c)){
        if (getCarByCrossCell(&c) == NULL){
            Car->crossNextCell = c;
            Car->velocity = CROSS_VELOCITY;
            initCrossCell(&c, OCCUPYING_CAR);
        }else{
            Car->velocity = _0_CELL_;
        }
    }else{
        // printf("else\n");
        RLC rlc;
        transformCrossCellIntoRLC(&rlc, Car);
        // printCrossCell(Car->crossCurrCell);
        if (getCarPtr(&rlc) == NULL){
            Car->nextCell = rlc;
            Car->crossNextCell.crossNum = NEXT_CELL_IS_ON_ROAD;
            Car->velocity = _1_CELL_;
            initRoadCell(&rlc, OCCUPYING_CAR);
            // printRLC(rlc, "rlc");
        }else{
            Car->velocity = _0_CELL_;
        }
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
    if (Car->moveDir == NORTH){
        --c->y;
    }
    else if (Car->moveDir == SOUTH){
        ++c->y;
    }
    else if (Car->moveDir == EAST){
        ++c->x;
    }
    else if (Car->moveDir == WEST){
        --c->x;
    }
}

bool isInCrossBoards(cross_cell c)
{
    return (c.x >= 0 && c.x < CROSS_SIDE && c.y >= 0 && c.y < CROSS_SIDE);
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
    HALF_CROSS_SIDE = CROSS_SIDE / 2;
    CROSS_WIDTH = CELL_WIDTH * CROSS_SIDE;
    NUMBER_OF_CROSS_CELLS = CROSS_SIDE * CROSS_SIDE;

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
    // binding crosses with roads
    for (int i = 0; i < NUMBER_OF_CROSS_ROADS; i++){
        roads[enterRoadIndexes[i]].isEndCross = true;
        roads[enterRoadIndexes[i]].isBeginCross = false;
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




