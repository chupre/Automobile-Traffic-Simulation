#include <algorithms.h>
#include <traffic_density.h>
#include <cars.h>
#include <road.h>
#include <cross.h>
#include <render.h>

RLC rouletteRLC = {MAX_ROAD_DIGIT, -1, MAX_CELL_DIGIT};

GLint innerUserCarsPtrsIndex = NO_INNER_INDEX;
car* userCarsPtrs[max_cars];

GLint innerOvertakeCarsIndex = NO_INNER_INDEX;
GLint overtakeCarsIndexes[max_cars];

car* ignoredBackCars[max_cars];
GLint innerIgnoredBackCarsIndex = NO_INNER_INDEX;

RLC carAddingQueue[MAX_CARS];
GLint innerCarAddingQueueIndex = NO_INNER_INDEX;

bool compareRLCs(RLC* rlc1, RLC* rlc2)
{
	return (rlc1->road == rlc2->road && rlc1->line == rlc2->line && rlc1->cell == rlc2->cell);
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
	if (glfwGetTime() - timer > STEP_TIME)
	{
		//printf("Step: %lf\n", glfwGetTime());		

		timer += STEP_TIME;

		stepRoad();
		// printf("\n..........................................................................\n");
	}
}

GLvoid printRLC(RLC rlc, char* string)
{
	printf("%s:(%d, %d, %d)\n", string, rlc.road, rlc.line, rlc.cell);
}

GLvoid stepRoad()
{
	car* Car;

	while (getCarByRoulette(&Car))
	{
		if (Car->isCrushed || Car == OCCUPYING_CAR)
		{
			continue;
		}
		if (isToExclude(Car))
		{
			// printCarProperties(Car->currCell);
			// printf("excluded\n\n");
			excludeFromMap(Car);
			continue;
		}
		rebindRoadCars(Car);
		reinitRoadCells(Car);
	}

	processCarAddingQueue();

	while (getCarByRoulette(&Car))
	{
		if (Car->isCrushed)
		{
			continue;
		}
		if (Car == OCCUPYING_CAR)
		{
			// printRLC(rouletteRLC, "OCCUPYING_CAR RLC");
			continue;
		}
		if (isInUserCarsPtrs(Car))
		{
			continue;
		}

		// printRLC(rouletteRLC, "roulette"); car* rouletteCar = getCarPtr(&rouletteRLC); printf("V: %d\n", rouletteCar->velocity);
		// printf("\n");

		thoughtsOfOneCar(Car);
	}
	clearUserCarsPtrs();
	
	// printf("_______________SPAWN_CARS______________\n");
	if (rand() % 100 < SPAWN_FREQUENCY)
	{
		spawnCars();
	}
	
}

bool getCarByRoulette(car** Car)
{
	car* tmpCar;
	while (rollRouletteRLC())
	{
		tmpCar = roads[rouletteRLC.road].lines[rouletteRLC.line].cells[rouletteRLC.cell];
		if (tmpCar != NULL)
		{
			*Car = tmpCar;
			return true;
		}
	}
	return false;
}

bool rollRouletteRLC()
{
	if (rouletteRLC.line < MAX_LINE_DIGIT)
	{
		rouletteRLC.line += 1;
	}
	else
	{
		rouletteRLC.line = 0;
		if (rouletteRLC.cell > 0)
		{
			rouletteRLC.cell -= 1;
		}
		else
		{
			rouletteRLC.cell = MAX_CELL_DIGIT;
			if (rouletteRLC.road > 0)
			{
				rouletteRLC.road -= 1;
			}
			else
			{
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
	if (freeCars > 0)
	{
		int counter = freeCars;
		for (int i = 0; i < counter; i++)
		{
			if (spawnedCars > carsToSpawn)
			{
				return;
			}
			GLint carIndex = getFreeCarIndex();
			if (carIndex == NO_CAR_INDEX)
			{
				return;
			}
			RLC freeSpotRLC = { EMPTY, EMPTY, EMPTY };
			getFreeSpotAddress(&freeSpotRLC);
			
			
			if (freeSpotRLC.road != EMPTY && carIndex != NO_CAR_INDEX)
			{
				addCar(&cars[carIndex], carIndex, freeSpotRLC);
				thoughtsOfOneCar(&cars[carIndex]);

				--freeCars;
				++spawnedCars;
				increaseDensityData(freeSpotRLC.road);

				// logCar(&cars[carIndex]);
				// printCarProperties(freeSpotRLC);
				// printf("\n");
			}
		}
	}
}

GLvoid appendInUserCarsPtrs(car* Car)
{
	userCarsPtrs[++innerUserCarsPtrsIndex] = Car;
}

bool isInUserCarsPtrs(car* Car)
{
	for (int i = 0; i <= innerUserCarsPtrsIndex; i++)
	{
		if (Car == userCarsPtrs[i])
		{
			return true;
		}
	}
	return false;
}

GLvoid clearUserCarsPtrs()
{
	innerUserCarsPtrsIndex = NO_INNER_INDEX;
}

GLvoid appendInOvertakeCarsIndexes(GLint carIndex)
{
	overtakeCarsIndexes[++innerOvertakeCarsIndex] = carIndex;
}

GLvoid clearOvertakeCarsIndedxes()
{
	innerOvertakeCarsIndex = NO_INNER_INDEX;
}

GLvoid initRoadCell(RLC *rlc, car* Car)
{
	roads[rlc->road].lines[rlc->line].cells[rlc->cell] = Car;
}

GLvoid rebindRoadCars(car* Car)
{
    if (Car->currCell.cell < NUMBER_OF_CELLS)
	{
		initRoadCell(&Car->nextCell, NULL);
	}

	if (Car->nextCell.road != NEXT_CELL_IS_ON_CROSS)
	{
		if (Car->nextCell.cell < NUMBER_OF_CELLS)
		{
			initRoadCell(&Car->nextCell, Car);
		}
	}
	else
	{
		initCrossCell(&Car->crossNextCell, Car);
	}
    
    //else // what would be ohterwise ?
    /*
    a car won't belong to the road, but will be still drawning
    then a single way to delete from screen is done by using cars massive.
    */
}

GLvoid reinitRoadCells(car* Car)
{
    Car->currCell.road = Car->nextCell.road;
    Car->currCell.line = Car->nextCell.line;
    Car->currCell.cell = Car->nextCell.cell;
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
		printf(	"currCell (%d, %d, %d)\nnextCell (%d, %d, %d)\nvelocity %d\nisActive %d\nisCrushed %d\n",
			Car->currCell.road, Car->currCell.line, Car->currCell.cell,
			Car->nextCell.road, Car->nextCell.line, Car->nextCell.cell,
			Car->velocity,
			Car->isActive,
			Car->isCrushed);
	}
}

GLvoid thoughtsOfOneCar(car* Car)
{
	bool isAllowedToOvertake = true;
	if (Car->move == OVERTAKE)
	{	//as end fo line changing which is just done
		Car->move = FORWARD;
		Car->moveDir = getRoadDir(Car);
		// Car->nextCell.cell += Car->velocity;
		// return;
	}
	else if (Car->move == SHIFT)
	{
		Car->move = FORWARD;
		Car->moveDir = getRoadDir(Car);
	}

	RLC overtakeRLC;
	car* forthCar = NULL;
	GLint distance = distanceToForthCar(Car->currCell, &forthCar);//the foo takes in cognisance that road can have endCross


	if (distance == _NO_CAR_ && isEndedWithCross(Car->currCell))
	{
		distance = NUMBER_OF_CELLS - Car->currCell.cell;
		isAllowedToOvertake = false;
	}
	// printf("distance: %d\n", distance);

	// if (forthCar && forthCar->isCrushed)
	// {
	// 	printf("crushedCar is met\n");
	// }

	// condition "distance - 1 > Car->velocity" will stay strickt because there considered is the faculty of mounting the car velocity a one more
	if (Car->velocity != _0_CELL_)
	{
		if (distance - 1 > Car->velocity || distance == _NO_CAR_)
		{/* car gets velocity if this car is newBorn or if this car doesn't stop because of the crushed car a cell before. */
			if (rand() % 100 < DROP_VELOCITY_FREQUENCY)
			{
				Car->velocity = rand() % Car->velocity;
			}
			
			if (Car->velocity < MAX_VELOCITY)
			{
				Car->velocity += _1_CELL_;
			}
			Car->nextCell.cell += Car->velocity;
			// printf("1\n");
			return;
		}
	}
	else
	{
		if (distance > 1)
		{
			// printf("2\n");
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
		// printf("3\n");
		RLC rlc = {NO_ROAD_INDEX, NO_LINE_INDEX, NO_CELL_INDEX};
		MOVING_TYPE move = checkChangeLineAbility(Car, &rlc);
		if (move != FORWARD)
		{
			Car->move = move;
			Car->nextCell.road = rlc.road;
			Car->nextCell.line = rlc.line;
			Car->nextCell.cell = rlc.cell;
			bindCellAndCar(&rlc, OCCUPYING_CAR);


			if (Car->move == OVERTAKE && Car->velocity == _0_CELL_)
			{
				Car->velocity = _1_CELL_;
				// printf("_0_CELL_ -> _1_CELL_\n");
			}
			if (Car->move == SHIFT)
			{
				// printf("SHIFT\n");
				// printDir(Car->moveDir);
			}
			if (Car->move == OVERTAKE)
			{
				// printf("OVERTAKE\n");
				// printDir(Car->moveDir);
			}

			return;
		}
	}
	
	// printf("4\n");
	//according to the so-called all-excluded condition
	Car->velocity = distance - 1;
	Car->nextCell.cell += Car->velocity;
	// printf("_____distance - 1\n");
}

bool isToExclude(car* Car)
{
	// if (isOutOfScreenSpace(Car->realPos))
	// {
	// 	return true;
	// }
	road* roadPtr = &roads[Car->currCell.road];
	if (isFurhterThanEndLine(Car, roadPtr) || isFurtherThanEndCell(Car))
	{
		//printf("isToExclude\n");
		return true;
	}
	return false;
}

bool isFurtherThanEndCell(car* Car)
{
	return (Car->nextCell.cell >= NUMBER_OF_CELLS);
}

GLvoid excludeFromMap(car* Car)
{
	car** ptrCell = getFirstCellPtr(Car->currCell);
	GLint start = Car->currCell.cell;
	GLint end = Car->nextCell.cell;
	
	if (start < NUMBER_OF_CELLS)
	{
		ptrCell[start] = NULL;
	}
	if (end < NUMBER_OF_CELLS)
	{
		ptrCell[end] = NULL;
	}

	clearCarProperties(Car);
	++freeCars;
	decreaseDensityData(Car->currCell.road);
}

GLint getVelocityByRLC(RLC rlc)
{
	car** ptrCell = ((roads + rlc.road)->lines + rlc.line)->cells;
	if (ptrCell[rlc.cell] == NULL)
	{
		return _NO_CAR_;
	}
	else
	{
		return ptrCell[rlc.cell]->velocity;
	}
}

MOVING_TYPE checkChangeLineAbility(car* Car, RLC* rlc)
{
	GLint newLine;

	switch (rand() % 100 < TURN_LEFT_FREQUENCY)
	{
		case 0:
		{
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
		}
		case 1:
		{
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
		if (Car->velocity != _0_CELL_)
		{
			if (forth - 1 > Car->velocity)
			{
				return OVERTAKE;
			}
		}
		else
		{
			if (forth - 1 > _1_CELL_)
			{
				return OVERTAKE;
			}
			if (forth != 0) //this is that on the neighbour cell there is no cacr
			{
				return SHIFT;
			}
		}			
	}

	return FORWARD;
}

GLvoid appendInIgnoredBackCarsPtrs(car* Car)
{
	ignoredBackCars[++innerIgnoredBackCarsIndex] = Car;
}

GLvoid clearIgnoredBackCarsPtrs()
{
	innerIgnoredBackCarsIndex = NO_INNER_INDEX;
}

bool isInIgnoredBackCars(car* Car)
{
	for (GLint i = 0; i <= innerIgnoredBackCarsIndex; i++)
	{
		if (ignoredBackCars[i] == Car)
			return true;
	}
	return false;
}

GLint distanceToForthCar(RLC rlc, car** Car)
{
	car** ptrCell = getFirstCellPtr(rlc);
	for (int i = 1 + rlc.cell; i < NUMBER_OF_CELLS; ++i)
	{
		if (ptrCell[i] == NULL)
		{
			continue;
		}
		else if (ptrCell[i]->isActive == true)
		{
			*Car = ptrCell[i];
			return (i - rlc.cell);
		}
	}
	//this condition will be checked only if no car is found on the road before
	// if (roads[rlc.road].isEndCross)
	// {
	// 	return NUMBER_OF_CELLS - rlc.cell;
	// }

	return _NO_CAR_;
}

GLint distanceToBackCar(RLC rlc, car** Car)
{
	car** ptrCell = getFirstCellPtr(rlc);
	for (int i = -1 + rlc.cell; i >= 0; --i)
	{
		if (ptrCell[i] == NULL)
		{
			continue;
		}
		if (isInIgnoredBackCars(ptrCell[i]))
		{
			continue;
		}
		else if (ptrCell[i]->isActive == true)
		{
			*Car = ptrCell[i];
			return (rlc.cell - i);
		}
	}
	return _NO_CAR_;
}

bool isRLCsuitableForSettingCar(RLC rlc)
{
	if (getCarPtr(&rlc) != NULL)
	{
		return false;
	}
	car* forthCar = NULL;
	GLint forth = distanceToForthCar(rlc, &forthCar);
	car* backCar = NULL;
	GLint back = distanceToBackCar(rlc, &backCar);
	if (forth > 1 && back > 1)
	{
		return true;
	}
	return false;
}

GLvoid addInRLCCarAddingQueue(RLC rlc)
{
	carAddingQueue[++innerCarAddingQueueIndex] = rlc;
}

GLvoid clearCarAddingQueue()
{
	innerCarAddingQueueIndex = NO_INNER_INDEX;
}

GLvoid processCarAddingQueue()
{
	for (int i = 0; i <= innerCarAddingQueueIndex; i++)
	{
		if (isRLCsuitableForSettingCar(carAddingQueue[i]))
		{
			GLint carIndex = getFreeCarIndex();
			if (carIndex == NO_CAR_INDEX)
			{
				return;
			}
			car* Car = &cars[carIndex];
			addCar(Car, carIndex, carAddingQueue[i]);
			thoughtsOfOneCar(Car);
			--freeCars;

			appendInUserCarsPtrs(Car);
		}
	}
	clearCarAddingQueue();
}

bool isInCarAddingQueue(RLC rlc)
{
	for (int i = 0; i < innerCarAddingQueueIndex; i++)
	{
		if (compareRLCs(&rlc, &carAddingQueue[i]))
		{
			return true;
		}
	}
	return false;
}
