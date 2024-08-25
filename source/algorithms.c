#include <algorithms.h>
#include <cars.h>
#include <road.h>
#include <render.h>



GLint innerBornCarsIndex = NO_INNER_INDEX;
GLint bornCarsIndexes[max_cars];
GLint innerOvertakeCarsIndex = NO_INNER_INDEX;
GLint overtakeCarsIndexes[max_cars];
RLC rouletteRLC = {MAX_ROAD_DIGIT, -1, MAX_CELL_DIGIT};
car* ignoredBackCars[max_cars];
GLint innerIgnoredBackCarsIndex = NO_INNER_INDEX;

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

		step();
		// printf("\n..........................................................................\n");
	}
}

GLvoid printRLC(RLC rlc, char* string)
{
	printf("%s:(%d, %d, %d)\n", string, rlc.road, rlc.line, rlc.cell);
}

GLvoid step()
{
	car* Car;
	while (getCarPtrByRoulette(&Car))
	{
		if (Car->isCrushed)
		{
			// printf("rrr\n");
			continue;
		}
		if (Car->markRight)
		{
			// printf("hhh\n");
			Car->markRight = false;
			continue;
		}
		if (Car == OCCUPYING_CAR)
		{
			printf("OCCUPYING CAR\n");
			printRLC(rouletteRLC, "occupying RLC"); printf("\n");
			roads[rouletteRLC.road].lines[rouletteRLC.line].cells[rouletteRLC.cell] = NULL;
			continue;
		}

		//printf("(%d, %d, %d)\n", rouletteRLC.road, rouletteRLC.line, rouletteRLC.cell);
		// printCarProperties(Car->currCell);

		if (isToExclude(Car))
		{
			// printCarProperties(Car->currCell);
			// printf("excluded\n\n");
			excludeFromMap(Car);
			continue;
		}

		printRLC(rouletteRLC, "roulette"); car* rouletteCar = getCarPtr(&rouletteRLC); printf("V: %d\n", rouletteCar->velocity);
		
		unbindCarPtrFromCell(Car);
		reinitCurrCellWithNextCell(Car);
		thoughtsOfOneCar(Car);
		
		printRLC(Car->currCell, "currCell");
		printf("\n");
	}
	
	// printf("_______________SPAWN_CARS______________\n");
	if (rand() % 100 > ANTI_SPAWN_FRECUENCY)
	{
		spawnCars();
	}
	
}

bool getCarPtrByRoulette(car** Car)
{
	//printf("getCarPtrByRoulette\n");
	car* tmpCar;
	while (rollRouletteRLC())
	{
		//printRLC(rouletteRLC, "dedug_roulette");
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
	printf("FREE__cARS: %d\n", freeCars);
	if (freeCars > 0)
	{
		int counter = freeCars;
		for (int i = 0; i < counter; i++)
		{
			GLint carIndex = getFreeCarIndex();
			printf("CAR_INDEX: %d\n", carIndex);
			if (carIndex == NO_CAR_INDEX)
			{
				return;
			}
			RLC freeSpotRLC = { EMPTY, EMPTY, EMPTY };
			getFreeSpotAddress(&freeSpotRLC);
			

			printRLC(freeSpotRLC, "FREE-SPOT-RLC");
			
			if (freeSpotRLC.road != EMPTY && carIndex != NO_CAR_INDEX)
			{
				printf("HURAAA\n");
				addCar(&cars[carIndex], carIndex, freeSpotRLC);
				thoughtsOfOneCar(&cars[carIndex]);

				--freeCars;

				printCarProperties(freeSpotRLC);
				printf("\n");
				//appendInBornCarsIndexes(carIndex);
			}
		}
	}
}

GLvoid appendInBornCarsIndexes(GLint carIndex)
{
	bornCarsIndexes[++innerBornCarsIndex] = carIndex;
}

GLvoid clearBornCarsIndedxes()
{
	innerBornCarsIndex = NO_INNER_INDEX;
}

GLvoid appendInOvertakeCarsIndexes(GLint carIndex)
{
	overtakeCarsIndexes[++innerOvertakeCarsIndex] = carIndex;
}

GLvoid clearOvertakeCarsIndedxes()
{
	innerOvertakeCarsIndex = NO_INNER_INDEX;
}

bool isInBornCars(GLint i)
{
	for (int k = 0; k <= innerBornCarsIndex; k++)
	{
		if (i == bornCarsIndexes[k])
		{
			return true;
		}
	}
	return false;
}

GLvoid unbindCarPtrFromCell(car* Car)
{
    car** ptrCell = getFirstCellPtr(Car->currCell);
    GLint start = Car->currCell.cell;
    if (start < NUMBER_OF_CELLS) ptrCell[start] = NULL;

    ptrCell = getFirstCellPtr(Car->nextCell);
    GLint newStart = Car->nextCell.cell;
    if (newStart < NUMBER_OF_CELLS) ptrCell[newStart] = Car;
    //else // what would be ohterwise ?
    /*
    a car won't belong to the road, but will be still drawning
    then a single way to delete from screen is done by using cars massive.
    */
}

GLvoid reinitCurrCellWithNextCell(car* Car)
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

//changes car's velocity and rlc
GLvoid thoughtsOfOneCar(car* Car)
{
	if (Car->move == OVERTAKE)
	{	//as end fo line changing which is just done
		Car->move = FORWARD;
		Car->moveDir = getRoadDir(Car);
		Car->nextCell.cell += Car->velocity;
		return;
	}
	else if (Car->move == SHIFT)
	{
		Car->move = FORWARD;
		Car->moveDir = getRoadDir(Car);
		// Car->velocity = _0_CELL_;
	}

	RLC overtakeRLC;
	GLint distance = distanceToForthCar(Car->currCell);//the foo takes in cognisance that road can have endCross
	RLC forthCarRLC;
	GLint forthCarVelocity;
	car* forthCar;

	if (distance != _NO_CAR_ /*&& !roads[Car->currCell.road].isEndCross*/)
	{
		forthCarRLC = Car->currCell;
		forthCarRLC.cell += distance;
		forthCarVelocity = getVelocityByRLC(forthCarRLC);
		forthCar = getCarPtr(&forthCarRLC);

		printRLC(forthCarRLC, "forthCarRLC==========================");
	}

	// condition "distance - 1 > Car->velocity" will stay strickt because there considered is the faculty of mounting the car velocity a one more
	if ((distance - 1 > Car->velocity && Car->velocity != _0_CELL_) || /*(Car->velocity == _0_CELL_ && distance == 1) ||*/ distance == _NO_CAR_)
	{	/* car gets velocity if this car is newBorn or if this car doesn't stop because of the crushed car a cell before. */
		if (Car->velocity < MAX_VELOCITY)
		{
			Car->velocity += _1_CELL_;
		}
		Car->nextCell.cell += Car->velocity;
		return;
	}
	/*
	condition "forthCarVelocity < Car->velocity" will stay strickt
	because the conceding the unstrickt condition means additional condition
	that the driver is sure that forth-car-on-the-near-line velocity is greater than tha driver's one.
	this additional condition is not done.  
	moreover, including the additional condition deprives the driver of ability to escape from the case when there is an row of less-velocity cars before him.
	*/
	if ((forthCarVelocity <= Car->velocity /*|| (forthCarVelocity == _0_CELL_ && Car->velocity == _0_CELL_)*/) && forthCar != OCCUPYING_CAR)
	{
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
				printf("_0_CELL_ -> _1_CELL_\n");
			}
			if (Car->move == SHIFT)
			{
				printf("SHIFT\n");
				printDir(Car->moveDir);
			}
			if (Car->move == OVERTAKE)
			{
				printf("OVERTAKE\n");
				printDir(Car->moveDir);
			}

			return;
		}
	}

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
	//printf("!\n");
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
	GLint newLine = Car->currCell.line - 1;
	if (newLine >= 0 && newLine < NUMBER_OF_LINES + 1)//the utter right line is excluded
	{
		rlc->road = Car->currCell.road;
		rlc->line = newLine;
		rlc->cell = Car->currCell.cell;

		printRLC(*rlc, "safeRLC");
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

		printRLC(*rlc, "safeRLC");
		if(getCarPtr(rlc) == NULL)
		{
			MOVING_TYPE res = isSafetyForthAndBack(Car, *rlc);
			if (res != FORWARD)
			{
				printf("RIGHT         RIGHT          RIGHT\n");
				Car->moveDir = getRightMoveDir(getRoadDir(Car));
				return res;
			}
		}
	}

	return FORWARD;
}

MOVING_TYPE isSafetyForthAndBack(car* Car, RLC rlc)
{
	car* backCar = NULL;
	GLint forth = distanceToForthCar(rlc);
	GLint back = getPureBackDistance(Car, rlc, &backCar);
	
	while (1)
	{
		if (back == 0)
		{
			printf("_____back == 0\n");
			return FORWARD;
		}
		else if (back < 0 && abs(back) < forth)
		{
			printf("_____back < 0\n");
			appendInIgnoredBackCarsPtrs(backCar);
			printf("innerIgnorINDex: %d\n", innerIgnoredBackCarsIndex);

			forth = abs(back);
			back = getPureBackDistance(Car, rlc, &backCar);
			printf("_____getPure\n");
		}
		else
		{
			printf("_____else\n");
			break;
		}
	}
	
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	for (int i = 0; i <= innerIgnoredBackCarsIndex; i++)
	{
		printCarProperties(ignoredBackCars[i]->currCell);
		printf("``````````````````\n");	
	}
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	clearIgnoredBackCarsPtrs();


	printf("forth: %d\nback: %d\n", forth, back);

	car* forthCar = NULL;
	if (forth != _NO_CAR_)
	{
		forthCar = roads[rlc.road].lines[rlc.line].cells[rlc.cell + forth];
	}

	/*
	that ">=" is there and four times below means
	that these cars can drive up to the further one with no space between them left.
	it was bad idea as condition ">=" allows standing-a-step-before cars to drive upon the crushed cars while overtaking.
	*/

	if (1 || !(forthCar && forthCar->velocity == _0_CELL_))
	{
		if (backCar == NULL)
		{
			printf("**NULL\n");
			if (forth - 1 > Car->velocity)
			{
				return OVERTAKE;
			}
		}
		else
		{
			printf("** != null\n");
			if (forth - 1 > Car->velocity && back - 1 > backCar->velocity)
			{
				return OVERTAKE;
			}
		}
	}
	else 
	{
		if (forth == 1 && Car->velocity == _0_CELL_)
		{
			if (backCar == NULL)
			{
				return SHIFT;
			}
			else if (back - 1 > backCar->velocity)
			{
				return SHIFT;
			}
		}
	}
	

	return FORWARD;
}

GLint getPureBackDistance(car* Car, RLC rlc, car** backCar)
{
	*backCar == NULL;
	RLC backCarNextCell;
	GLint distance = distanceToBackCar(rlc);
	printf("dist   : %d\n", distance);
	if (distance != _NO_CAR_)
	{
		*backCar = roads[rlc.road].lines[rlc.line].cells[rlc.cell - distance];
		backCarNextCell = (*backCar)->nextCell;
		//printCarProperties((*backCar)->currCell);
		if (backCarNextCell.line == rlc.line)
		{
			distance = rlc.cell - backCarNextCell.cell;
			printf("dist_!_: %d\n", distance);
		}
		else printf("\n$$\n");
	}

	return distance;
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

GLint distanceToForthCar(RLC rlc)
{
	car** ptrCell = getFirstCellPtr(rlc);
	//int checkedCells = 0;
	for (int i = 1 + rlc.cell; i < NUMBER_OF_CELLS; ++i)
	{
		//checkedCells += 1;
		if (ptrCell[i] == NULL)
		{
			continue;
		}
		else if (ptrCell[i]->isActive == true)
		{
			return (i - rlc.cell);
		}
	}

	// if (roads[rlc.road].isEndCross)
	// {
	// 	return NUMBER_OF_CELLS - rlc.cell;
	// }

	return _NO_CAR_;
}

GLint distanceToBackCar(RLC rlc)
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
			return (rlc.cell - i);
		}
	}
	return _NO_CAR_;
}

