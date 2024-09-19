#include "map.h"
#include <algorithms.h>
#include <macros.h>
#include <stdlib.h>
#include <traffic_density.h>
#include <cars.h>
#include <road.h>
#include <cross.h>
#include <render.h>
#include <log.h>
#include <dbg.h>
#include <cglm/cglm.h>

bool alghorithmsInit = false;
int MAX_LINE_DIGIT;
int MAX_ROAD_DIGIT;
RLC rouletteRLC;

GLint innerUserCarsPtrsIndex = NO_INNER_INDEX;
car ** userCarsPtrs;

RLC * carAddingQueue;
GLint innerCarAddingQueueIndex = NO_INNER_INDEX;

RLC * carAddingQueue_CRUSH;
GLint innerCarAddingQueueIndex_CRUSH = NO_INNER_INDEX;

car** skipCarsFromCross;
GLint innerSkipCarsFromCrossIndex = NO_INNER_INDEX;

car** checkedCars;
GLint innerCheckedCarsIndex = NO_INNER_INDEX;

bool compareRLCs(RLC* rlc1, RLC* rlc2)
{
	return (rlc1->road == rlc2->road && rlc1->line == rlc2->line && rlc1->cell == rlc2->cell);
}

GLvoid printCrossCell(cross_cell c){
	printf("(%d,%d,%d) ", c.crossNum, c.x, c.y);
}

car* getCarPtr(RLC* rlc)
{
	return roads[rlc->road].lines[rlc->line].cells[rlc->cell];
}

GLvoid printDir(DIRECTION dir)
{
	if (dir == NORTH) printf("NORTH\n");
	else if (dir == SOUTH) printf("SOUTH\n");
	else if (dir == WEST) printf("WEST\n");
	else if (dir == EAST) printf("EAST\n");
	else if (dir == NONE) printf("NONE\n");
}

GLvoid update()
{
    if (!alghorithmsInit) {
        MAX_LINE_DIGIT = NUMBER_OF_LINES;
        MAX_ROAD_DIGIT = NUMBER_OF_ROADS - 1;
        rouletteRLC.road = MAX_ROAD_DIGIT;
        rouletteRLC.line = -1;
        rouletteRLC.cell = MAX_CELL_DIGIT;
        userCarsPtrs = (car **)malloc(sizeof(car *) * MAX_CARS);
        innerUserCarsPtrsIndex = NO_INNER_INDEX;
        innerCarAddingQueueIndex = NO_INNER_INDEX;

        if (userCarsPtrs == NULL) {
            printf("malloc failed on userCarsPtrs");
            exit(1);
        }

        carAddingQueue = (RLC *)malloc(sizeof(RLC) * MAX_CARS);

        if (carAddingQueue == NULL) {
            printf("malloc failed on carAddingQueue");
            exit(1);
        }

		carAddingQueue_CRUSH = (RLC *)malloc(sizeof(RLC) * MAX_CARS);

        if (carAddingQueue_CRUSH == NULL) {
            printf("malloc failed on carAddingQueue_CRUSH");
            exit(1);
        }

		skipCarsFromCross = (car **)malloc(sizeof(car*) * MAX_CARS);
		if (skipCarsFromCross == NULL){
			printf("malloc failed on skipCars");
            exit(1);
		}

		checkedCars = (car **)malloc(sizeof(car *) * MAX_CARS);
		if (checkedCars == NULL){
			printf("malloc failed on checkedCars");
            exit(1);
		}

        alghorithmsInit = true;
    }


	if (glfwGetTime() - timer > STEP_TIME)
	{
		// printf("Step: %lf\n", glfwGetTime());
		timer += STEP_TIME;

		if (MAP_TYPE == CROSS || MAP_TYPE == SEVERAL_CROSSES){
			// printLights();
		}
		renewCells();
		if (MAP_TYPE == CROSS || MAP_TYPE == SEVERAL_CROSSES){
			stepCross();
			stepRoad();
			changeLightsColor();
		}
		else{
			stepRoad();
		}
		// printGrid(0);
		// printf("============================================================\n");
	}
}

GLvoid printRLC(RLC rlc, char* string)
{
	printf("%s:(%d, %d, %d)\n", string, rlc.road, rlc.line, rlc.cell);
}

GLvoid stepRoad()
{
	car* Car;
	processCarAddingQueue_CRUSH();
	processCarAddingQueue();

	while (getCarByRoulette(&Car)){
		if (Car->isCrushed){
			continue;
		}
		if (Car == OCCUPYING_CAR){
			continue;
		}
		if (isInUserCarsPtrs(Car)){
			continue;
		}
		thoughtsOfOneCar(Car);
	}

	clearUserCarsPtrs();
	if (_LOG_KEY_ == PLAY){
		log_data data;
		while (readFile(&data)){
			RLC freeSpotRLC;
			freeSpotRLC.road = data.road;
			freeSpotRLC.line = data.line;
			freeSpotRLC.cell = data.cell;

			GLint carIndex = getFreeCarIndex();
			if (carIndex == NO_CAR_INDEX){
				return;
			}
			addCar(&cars[carIndex], carIndex, freeSpotRLC);
			cars[carIndex].velocity = data.velocity;
			thoughtsOfOneCar(&cars[carIndex]);
			--freeCars;
			increaseDensityData(freeSpotRLC.road);
		}
		return;
	}
	if (rand() % 100 < SPAWN_FREQUENCY){
		spawnCars();
	}
	if (_LOG_KEY_ == RECORD){
		printFileStepEnd();
	}
}

bool getCarByRoulette(car** Car)
{
	car* tmpCar;
	while (rollRouletteRLC()){
		tmpCar = getCarPtr(&rouletteRLC);
		if (tmpCar != NULL){
			*Car = tmpCar;
			return true;
		}
	}
	return false;
}

bool rollRouletteRLC()
{
	if (rouletteRLC.line < MAX_LINE_DIGIT){
		rouletteRLC.line += 1;
	}else{
		rouletteRLC.line = 0;
		if (rouletteRLC.cell > 0){
			rouletteRLC.cell -= 1;
		}else{
			rouletteRLC.cell = MAX_CELL_DIGIT;
			if (rouletteRLC.road > 0){
				rouletteRLC.road -= 1;
			}else{
				rouletteRLC.road = MAX_ROAD_DIGIT;
				rouletteRLC.line = -1;
				return false;
			}
		}
	}
	return true;
}

GLvoid spawnCars()
{
	GLint carsToSpawn = rand() % NUMBER_OF_LINES;
	GLint spawnedCars = 0;
	if (freeCars > 0){
		int counter = freeCars;
		for (int i = 0; i < counter; i++){
			if (spawnedCars > carsToSpawn){
				return;
			}
			GLint carIndex = getFreeCarIndex();
			if (carIndex == NO_CAR_INDEX){
				return;
			}
			RLC freeSpotRLC = { EMPTY, EMPTY, EMPTY };
			getFreeSpotAddress(&freeSpotRLC);
			// if (roads[freeSpotRLC.road].dir != NORTH){
			// 	return;
			// }
			
			if (freeSpotRLC.road != EMPTY && carIndex != NO_CAR_INDEX){
				addCar(&cars[carIndex], carIndex, freeSpotRLC);
				if (_LOG_KEY_ == RECORD){
					log_data data = {
						.road = freeSpotRLC.road, 
						.line = freeSpotRLC.line,
						.cell = freeSpotRLC.cell,
						.velocity = cars[carIndex].velocity};
					writeFile(&data);
				}
				thoughtsOfOneCar(&cars[carIndex]);
				// printf("www\n");
				// printCar(&cars[carIndex]);
				--freeCars;
				++spawnedCars;
				increaseDensityData(freeSpotRLC.road);
			}
		}
	}
}

GLvoid appendInUserCarsPtrs(car* Car)
{
	if (innerUserCarsPtrsIndex == MAX_CARS){
		printf("extra car is trying to appned in userCarsPtrs\n");
		exit(-1);
	}
	userCarsPtrs[innerUserCarsPtrsIndex] = Car;
	++innerUserCarsPtrsIndex;
}

bool isInUserCarsPtrs(car* Car)
{
	for (int i = 0; i < innerUserCarsPtrsIndex; i++){
		if (Car == userCarsPtrs[i]){
			return true;
		}
	}
	return false;
}

GLvoid clearUserCarsPtrs()
{
	innerUserCarsPtrsIndex = NO_INNER_INDEX;
}

GLvoid initRoadCell(RLC *rlc, car* Car)
{
	roads[rlc->road].lines[rlc->line].cells[rlc->cell] = Car;
}
//................................................................................................
GLvoid renewCells(){
	car* Car;
	// printCarCharacter(crosses[0].cells[0]);
	if (MAP_TYPE == CROSS || MAP_TYPE == SEVERAL_CROSSES){
		while (getCarByRouletteCross(&Car))
		{
			// printCrossRoulette();
			if (Car == OCCUPYING_CAR){
				// printf("is occupying car\n");
				continue;
			}
			if (isInCheckedCars(Car)){
				// printf("checked\n");
				continue;
			}
			rebindCrossCars(Car);
			appendInCheckedCars(Car);
			appendInSkipCarsFromCross(Car);
		}
		// printCheckedCars();
		clearCheckedCars();
		// printSkipCars();
		while (getCarByRoulette(&Car)){
			if (Car->isCrushed || Car == OCCUPYING_CAR){
				continue;
			}
			if (isInSkipCarsFromCross(Car)){
				// printf("in skip cars\n");
				continue;
			}
			if (isToExcludeFormRoad(Car)){
				excludeFromMap(Car);
				continue;
			}
			rebindRoadCars(Car);
		}
		clearSkipCarsFromCross();

	}else{
		while (getCarByRoulette(&Car)){
			if (Car->isCrushed || Car == OCCUPYING_CAR){
				continue;
			}
			if (isToExcludeFormRoad(Car)){
				excludeFromMap(Car);
				continue;
			}
			rebindRoadCars(Car);
		}
	}
}	

GLvoid appendInSkipCarsFromCross(car* Car){
	if (innerSkipCarsFromCrossIndex == MAX_CARS){
		printf("extra car is trying to appned in skipCarsFromCross\n");
		exit(-1);
	}
	skipCarsFromCross[innerSkipCarsFromCrossIndex] = Car;
	++innerSkipCarsFromCrossIndex;
}

GLvoid printSkipCars(){
	printf("============skip============\n");
	for (int i = 0; i < innerSkipCarsFromCrossIndex; i++){
		printCarCharacter(skipCarsFromCross[i]);
		printf("`\n");
	}
	printf("============================\n");
}

GLvoid clearSkipCarsFromCross(){
	innerSkipCarsFromCrossIndex = NO_INNER_INDEX;
}

bool isInSkipCarsFromCross(car* Car)
{
	for (int i = 0; i < innerSkipCarsFromCrossIndex; i++){
		if (Car == skipCarsFromCross[i]){
			return true;
		}
	}
	return false;
}

GLvoid appendInCheckedCars(car* Car){
	if (innerCheckedCarsIndex == MAX_CARS){
		printf("extra car is trying to appned in checkedCars\n");
		exit(-1);
	}
	checkedCars[innerCheckedCarsIndex] = Car;
	++innerCheckedCarsIndex;
}

GLvoid printCheckedCars(){
	printf("============check===========\n");
	for (int i = 0; i < innerCheckedCarsIndex; i++){
		printCarCharacter(checkedCars[i]);
		printf("`\n");
	}
	printf("============================\n");
}

GLvoid clearCheckedCars(){
	innerCheckedCarsIndex = NO_INNER_INDEX;
}

bool isInCheckedCars(car* Car)
{
	for (int i = 0; i < innerCheckedCarsIndex; i++){
		if (Car == checkedCars[i]){
			return true;
		}
	}
	return false;
}

//...............................................................................................
GLvoid rebindRoadCars(car* Car)
{
    if (Car->currCell.cell < NUMBER_OF_CELLS){
		initRoadCell(&Car->currCell, NULL);
	}

	if (Car->nextCell.road != NEXT_CELL_IS_ON_CROSS){
		if (Car->nextCell.cell < NUMBER_OF_CELLS){
			initRoadCell(&Car->nextCell, Car);
		}
		Car->currCell = Car->nextCell;
	}else{
		// printf("NEXT_CELL_IS_ON_CROSS\n");
		// printRLC(Car->currCell, "rlc");
		// printCrossCell(Car->crossNextCell);

		initCrossCell(&Car->crossNextCell, Car);
		Car->crossCurrCell = Car->crossNextCell;
		q_append(Car, &crosses[Car->crossNextCell.crossNum].carsArriving);
		// printf(">> NEXT ON CROSS << ID: %d\n", Car->ID);
	}
    
    //else // what would be ohterwise ?
    /*
    a car won't belong to the road, but will be still drawning
    then a single way to delete from screen is done by using cars massive.
    */
}

GLint isRLCbad(RLC rlc)
{
	if (rlc.road >= NUMBER_OF_ROADS)
		return 1;
	if (rlc.line >= NUMBER_OF_LINES + 1)
		return 1;
	if (rlc.cell >= NUMBER_OF_CELLS)
		return 1;
	return 0;
}

GLvoid printCarProperties(RLC rlc)
{
	if (isRLCbad(rlc))
		return;

	car* Car = getCarPtr(&rlc);
	if (Car == NULL)
	{
		printf("NULL\n\n\n\n\n\n");
	}
	else
	{
		printf("ID: %d\n", Car->ID);
		printf(	"currCell (%d, %d, %d)\nnextCell (%d, %d, %d)\nvelocity %d\nisActive %d\nisCrushed %d\n",
			Car->currCell.road, Car->currCell.line, Car->currCell.cell,
			Car->nextCell.road, Car->nextCell.line, Car->nextCell.cell,
			Car->velocity,
			Car->isActive,
			Car->isCrushed);
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

GLvoid printCar(car* Car){
	if (Car == NULL)
	{
		printf("NULL\n\n\n\n\n\n");
	}
	else
	{
		printf("ID: %d\n", Car->ID);
		printf(	"currCell (%d, %d, %d)\nnextCell (%d, %d, %d)\nvelocity %d\nisActive %d\nisCrushed %d\n",
			Car->currCell.road, Car->currCell.line, Car->currCell.cell,
			Car->nextCell.road, Car->nextCell.line, Car->nextCell.cell,
			Car->velocity,
			Car->isActive,
			Car->isCrushed);
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

GLvoid thoughtsOfOneCar(car* Car)
{
	bool isAllowedToOvertake = true;
	if (Car->move == OVERTAKE){	//as end fo line changing which is just done
		Car->move = FORWARD;
		Car->moveDir = getRoadDir(Car);
	}
	else if (Car->move == SHIFT){
		Car->move = FORWARD;
		Car->moveDir = getRoadDir(Car);
	}

	RLC overtakeRLC;
	car* forthCar = NULL;
	GLint distance = distanceToForthCar(Car->currCell, &forthCar);//the foo takes in cognisance that road can have endCross

	if (distance == _NO_CAR_ && isEndedWithCross(&Car->currCell)){
		distance = NUMBER_OF_CELLS - Car->currCell.cell;
		isAllowedToOvertake = false;

		if (distance == 1) {
			cross_cell c;
			transformRLCIntoCrossCell(&c, Car);

			// printRLC(Car->currCell, "TO CROSS");
			// printCrossCell(c);

			if (getLightColorByCar(Car) == GREEN && checkCrossCellSafety(&c, Car->moveDir)){
				// printf("ID: %d, jump:", Car->ID); printCrossCell(c);printf("\n");
				Car->crossNextCell = c;
				Car->nextCell.road = NEXT_CELL_IS_ON_CROSS;
				Car->velocity = CROSS_VELOCITY;
				initCrossCell(&c, OCCUPYING_CAR);
				getCurvingCell(&Car->curvingCell, Car, c);
			}
			else{
				Car->velocity = _0_CELL_;
			}
			
			return;
		}
	}
	// condition "distance - 1 > Car->velocity" will stay strickt because there considered is the faculty of mounting the car velocity a one more
	if (Car->velocity != _0_CELL_){
		if (distance - 1 > Car->velocity || distance == _NO_CAR_){
		/* car gets velocity if this car is newBorn or if this car doesn't stop because of the crushed car a cell before. */
			if (rand() % 100 < DROP_VELOCITY_FREQUENCY){
				Car->velocity = rand() % Car->velocity;
			}
			else{
				if (Car->velocity < MAX_VELOCITY){
					Car->velocity += _1_CELL_;
				}
			}
			Car->nextCell.cell += Car->velocity;
			return;
		}
	}else{
		if (distance > 1){
			Car->velocity = _1_CELL_;
			Car->nextCell.cell += Car->velocity;
			return;
		}
	}

	/*
	condition "forthCarVelocity < Car->velocity" will stay strickt
	because the conceding the unstrickt condition means additional condition
	that the driver is sure that forth-car-on-the-near-line velocity is greater than tha driver's one.
	this additional condition is not done.  
	moreover, including the additional condition deprives the driver of ability to escape from the case when there is an row of less-velocity cars before him.
	*/
	if (isAllowedToOvertake &&
			(
				Car->velocity != _0_CELL_ && forthCar->velocity < Car->velocity /*&& forthCar != OCCUPYING_CAR*/
			|| 	Car->velocity == _0_CELL_ && distance == 1
			)
		)
	{
		RLC rlc = {NO_ROAD_INDEX, NO_LINE_INDEX, NO_CELL_INDEX};
		MOVING_TYPE move = checkChangeLineAbility(Car, &rlc);
		if (move != FORWARD){
			Car->move = move;
			Car->nextCell = rlc;
			bindCellAndCar(&rlc, OCCUPYING_CAR);
			if (Car->move == OVERTAKE && Car->velocity == _0_CELL_){
				Car->velocity = _1_CELL_;
			}
			return;
		}
	}
	//according to the so-called all-excluded condition
	Car->velocity = distance - 1;
	Car->nextCell.cell += Car->velocity;
}

bool isToExcludeFormRoad(car* Car)
{
	return (Car->nextCell.cell >= NUMBER_OF_CELLS);
	// if (isOutOfScreenSpace(Car->realPos))
	// {
	// 	return true;
	// }
	// road* roadPtr = &roads[Car->nextCell.road];
	// if (isFurtherThanEndCell(Car)){
	// 	return true;
	// }
	// return false;
}

bool isFurtherThanEndCell(car* Car)
{
	return (Car->nextCell.cell >= NUMBER_OF_CELLS);
}

// road ends up to the cross then Car is exluded only from the road, otherwise from road and MAP
GLvoid excludeFromMap(car* Car)
{	
	if (Car->currCell.cell < NUMBER_OF_CELLS){
		initRoadCell(&Car->currCell, NULL);
	}
	decreaseDensityData(Car->currCell.road);
	if (!isEndedWithCross(&Car->currCell)){
		clearCarProperties(Car);
		++freeCars;
	}
	// as cars' amount on the road is decreasing
	
}

GLint getVelocityByRLC(RLC rlc)
{
	car** ptrCell = ((roads + rlc.road)->lines + rlc.line)->cells;
	if (ptrCell[rlc.cell] == NULL){
		return _NO_CAR_;
	}else{
		return ptrCell[rlc.cell]->velocity;
	}
}

MOVING_TYPE checkChangeLineAbility(car* Car, RLC* rlc)
{
	GLint newLine;

	switch (rand() % 100 < TURN_LEFT_FREQUENCY){
		case 0:{
			newLine = Car->currCell.line - 1;
			if (newLine >= 0 && newLine < NUMBER_OF_LINES + 1)//the utter right line is excluded
			{
				rlc->road = Car->currCell.road;
				rlc->line = newLine;
				rlc->cell = Car->currCell.cell;

				// printRLC(*rlc, "safeRLC left");
				if(getCarPtr(rlc) == NULL)
				{
					MOVING_TYPE res = isSafetyForthAndBack(Car, *rlc);
					if (res != FORWARD)
					{
						Car->moveDir = getLeftMoveDir(getRoadDir(Car));
						return res;
					}
				}
			}

			newLine = Car->currCell.line + 1;
			if (newLine >= 0 && newLine < NUMBER_OF_LINES + 1)//the utter right line is excluded
			{
				rlc->road = Car->currCell.road;
				rlc->line = newLine;
				rlc->cell = Car->currCell.cell;

				// printRLC(*rlc, "safeRLC right");
				if(getCarPtr(rlc) == NULL)
				{
					MOVING_TYPE res = isSafetyForthAndBack(Car, *rlc);
					if (res != FORWARD)
					{
						Car->moveDir = getRightMoveDir(getRoadDir(Car));
						return res;
					}
				}
			}

			break;
		}case 1:{
			newLine = Car->currCell.line + 1;
			if (newLine >= 0 && newLine < NUMBER_OF_LINES + 1)//the utter right line is excluded
			{
				rlc->road = Car->currCell.road;
				rlc->line = newLine;
				rlc->cell = Car->currCell.cell;

				// printRLC(*rlc, "safeRLC right");
				if(getCarPtr(rlc) == NULL)
				{
					MOVING_TYPE res = isSafetyForthAndBack(Car, *rlc);
					if (res != FORWARD)
					{
						Car->moveDir = getRightMoveDir(getRoadDir(Car));
						return res;
					}
				}
			}
			newLine = Car->currCell.line - 1;
			if (newLine >= 0 && newLine < NUMBER_OF_LINES + 1)//the utter left line is excluded
			{
				rlc->road = Car->currCell.road;
				rlc->line = newLine;
				rlc->cell = Car->currCell.cell;

				// printRLC(*rlc, "safeRLC left");
				if(getCarPtr(rlc) == NULL)
				{
					MOVING_TYPE res = isSafetyForthAndBack(Car, *rlc);
					if (res != FORWARD)
					{
						Car->moveDir = getLeftMoveDir(getRoadDir(Car));
						return res;
					}
				}
			}

			break;
		}
	}

	return FORWARD;
}

MOVING_TYPE isSafetyForthAndBack(car* Car, RLC rlc)
{
	car* forthCar = NULL;
	GLint forth = distanceToForthCar(rlc, &forthCar);
	car* backCar = NULL;
	GLint back = distanceToBackCar(rlc, &backCar);

	if (back ==_NO_CAR_ || (backCar && back - 1 >= backCar->velocity)){
		if (Car->velocity != _0_CELL_){
			if (forth - 1 > Car->velocity){
				return OVERTAKE;
			}
		}else{
			if (forth - 1 > _1_CELL_){
				return OVERTAKE;
			}
			if (forth != 0){ //this is that on the neighbour cell there is no cacr
				return SHIFT;
			}
		}			
	}

	return FORWARD;
}

GLint distanceToForthCar(RLC rlc, car** Car)
{
	car** ptrCell = getFirstCellPtr(rlc);
	for (int i = 1 + rlc.cell; i < NUMBER_OF_CELLS; ++i){
		if (ptrCell[i] == NULL){
			continue;
		}
		else if (ptrCell[i]->isActive == true){
			*Car = ptrCell[i];
			return (i - rlc.cell);
		}
	}

	return _NO_CAR_;
}

GLint distanceToBackCar(RLC rlc, car** Car)
{
	car** ptrCell = getFirstCellPtr(rlc);
	for (int i = -1 + rlc.cell; i >= 0; --i){
		if (ptrCell[i] == NULL){
			continue;
		}
		if (ptrCell[i]->isActive == true){
			*Car = ptrCell[i];
			return (rlc.cell - i);
		}
	}
	return _NO_CAR_;
}

bool isRLCsuitableForSettingCar(RLC rlc)
{
	if (getCarPtr(&rlc) != NULL){
		return false;
	}
	car* forthCar = NULL;
	GLint forth = distanceToForthCar(rlc, &forthCar);
	car* backCar = NULL;
	GLint back = distanceToBackCar(rlc, &backCar);
	if (forth > 1 && back > 1){
		return true;
	}
	return false;
}

GLvoid appendRLCinCarAddingQueue(RLC rlc)
{
	if (innerCarAddingQueueIndex >= 1){
		return;
	}
	carAddingQueue[innerCarAddingQueueIndex] = rlc;
	innerCarAddingQueueIndex += 1;
}

GLvoid clearCarAddingQueue()
{
	innerCarAddingQueueIndex = NO_INNER_INDEX;
}

GLvoid appendRLCinCarAddingQueue_CRUSH(RLC rlc)
{
	if (innerCarAddingQueueIndex_CRUSH == 1){
		return;
	}
	carAddingQueue_CRUSH[innerCarAddingQueueIndex_CRUSH] = rlc;
	++innerCarAddingQueueIndex_CRUSH;
}

GLvoid clearCarAddingQueue_CRUSH()
{
	innerCarAddingQueueIndex_CRUSH = NO_INNER_INDEX;
}

GLvoid processCarAddingQueue()
{
	for (GLint i = 0; i < innerCarAddingQueueIndex; i++){
		if (isRLCsuitableForSettingCar(carAddingQueue[i])){
			GLint carIndex = getFreeCarIndex();
			if (carIndex == NO_CAR_INDEX){
				return;
			}
			car* Car = &cars[carIndex];
			addCar(Car, carIndex, carAddingQueue[i]);
			thoughtsOfOneCar(Car);
			--freeCars;
			increaseDensityData(carAddingQueue[i].road);

			appendInUserCarsPtrs(Car);
		}
	}
	clearCarAddingQueue();
}

GLvoid processCarAddingQueue_CRUSH()
{
	for (int i = 0; i < innerCarAddingQueueIndex_CRUSH; i++){
		addCrushedCar(carAddingQueue_CRUSH[i]);
	}
	clearCarAddingQueue_CRUSH();
} 

bool isInCarAddingQueue(RLC rlc)
{
	for (int i = 0; i < innerCarAddingQueueIndex; i++){
		if (compareRLCs(&rlc, &carAddingQueue[i])){
			return true;
		}
	}
	return false;
}


