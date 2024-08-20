// Custom modules
#include <algorithms.h>
#include <cross.h>
#include <cars.h>
#include <road.h>
#include <render.h>

GLvoid update()
{
	if (glfwGetTime() - timer > STEP_TIME)
	{
		//printf("Step at time: %lf\n\n", glfwGetTime());

		timer += STEP_TIME;

		step();

		// RLC rlc1 = {0, 2, 0};
		// RLC rlc2 = {0, 5, 0};
		// RLC rlc3 = {0, 8, 0};
		// printCarProperties(rlc1);
		// printCarProperties(rlc2);
		// printCarProperties(rlc3);
	}
}

GLvoid step()
{
	spawnCars();//born cars get velocity and drive according to it at this step
	// if (rand() % 2)
	// {
	// 	spawnCars();
	// }

	for (int i = 0; i < MAX_CARS; i++)
	{    
		if (!isInBornCars(i))//in order not to try it as it needn't be unbind
		{
			if (cars[i].isActive && !cars[i].isCrushed)
			{
				if (isToExclude(&cars[i]))
				{
					excludeFromMap(&cars[i]);
					continue;
				}

				unbindCarPtrFromCell(&cars[i]);
				reinitCurrCellWithNextCell(&cars[i]);// establishes currCell
				thoughtsOfOneCar(&cars[i]);//establishes nextCell
			}
		}
	}

	clearInBornCarsIndedxes();
}

GLvoid spawnCars()
{
	if (freeCars)
	{
		int counter = freeCars;
		for (int i = 0; i < counter; i++)
		{
			RLC freeSpotRLC = { EMPTY, EMPTY, EMPTY };
			getFreeSpotAddress(&freeSpotRLC);
			GLint carIndex = getFreeCarIndex();

			if (freeSpotRLC.road != EMPTY && carIndex != NO_CAR_INDEX)
			{
				addCar(&cars[carIndex], carIndex, freeSpotRLC);
				thoughtsOfOneCar(&cars[carIndex]);

				--freeCars;

				appendInBornCarsIndexes(carIndex);
			}
		}
	}
}

GLvoid appendInBornCarsIndexes(GLint freeCarIndex)
{
	bornCarsIndexes[++innerBornCarsIndex] = freeCarIndex;
}

GLvoid clearInBornCarsIndedxes()
{
	innerBornCarsIndex = NO_INNER_INDEX;
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
	if (rlc.line >= NUMBER_OF_LINES)
		return 2;
	if (rlc.cell >= NUMBER_OF_CELLS)
		return 3;
	return 0;
}

GLvoid printCarProperties(RLC rlc)
{
	if (isRLCbad(rlc))
		return;

	printf("RLC (%d, %d, %d)\n", rlc.road, rlc.line, rlc.cell);
	car* Car = (roads[rlc.road].lines[rlc.line].cells[rlc.cell]);
	if (Car == NULL)
	{
		printf("NULL\n\n\n\n\n\n");
	}
	else
	{
		printf(	"currCell (%d, %d, %d)\nnextCell (%d, %d, %d)\nvelocity %d\nisActive %d\nisCrushed %d\n\n",
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
	if (Car->ableToChangeLine)
	{
		Car->ableToChangeLine = false;
		Car->nextCell.cell += Car->velocity;
		return;
	}

	RLC overtakeRLC;
	GLint distance = distanceToForthCar(Car->currCell);//takes in cognisance that road can have endCross
	
	// RLC q = Car->currCell;
	// if (roads[q.road].lines[q.line].cells[q.cell] != NULL && (q.line == 2 || q.line == 5 || q.line == 8) && q.cell == 0){
	// 	printf("RLC (%d, %d, %d)\n", q.road, q.line, q.cell);
	// 	printf("distance: %d\n\n",  distance);
	// }

	RLC forthCarRLC;
	GLint forthCarVelocity;

	if (distance != _NO_CAR_ /*&& !roads[Car->currCell.road].isEndCross*/){
		forthCarRLC = Car->currCell;
		forthCarRLC.cell += distance;
		forthCarVelocity = getVelocityByRLC(forthCarRLC);
	}

	if ((distance - 1 > Car->velocity && Car->velocity != _0_CELL_) || distance == _NO_CAR_)
	{	/* car gets velocity if this car is newBorn or if this car doesn't stop because of the crushed car a cell before. */
		if (Car->velocity < MAX_VELOCITY)
		{
			Car->velocity += _1_CELL_;
		}
	}
	else if ((forthCarVelocity == _0_CELL_/*forthCarVelocity < Car->velocity*/) && (Car->ableToChangeLine = isAbleToChangeLine(Car, &overtakeRLC)))
	{
		DIRECTION roadDir = getRoadDir(Car);
		Car->overtake = getOvertakeDir(roadDir);
		Car->nextCell.road = overtakeRLC.road;
		Car->nextCell.line = overtakeRLC.line;
		Car->nextCell.cell = overtakeRLC.cell;
		return;
	}
	else
	{
		Car->velocity = distance - 1;
	}
	Car->nextCell.cell += Car->velocity;
}

bool isToExclude(car* Car)
{
	// if (!isOutOfScreenSpace(Car->realPos))
	// {
	// 	return 0;
	// }
	road* Road = &roads[Car->currCell.road];
	if (!isFurhterThanEndLine(Car, Road))
	{
		return false;
	}
	return true;
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
		else if (ptrCell[i]->isActive == true)
		{
			return (rlc.cell - i);
		}
	}
	return _NO_CAR_;
}

bool isAbleToChangeLine(car* Car, RLC* posOnNewLine)
{
	GLint newLine = Car->currCell.line - 1;
	if (newLine >= 0 && newLine < NUMBER_OF_LINES + 1)
	{
		RLC rlc = { Car->currCell.road, newLine, Car->currCell.line };
		if (isSafetyForthAndBack(Car, rlc))
		{
			posOnNewLine->road = rlc.road;
			posOnNewLine->line = rlc.line;
			posOnNewLine->cell = rlc.cell;
			return true;
		}
		else return false;
	}
	else return false;
}

//you shouldn't fear wether the car chooses a position between CurrCell and NextCell
bool isSafetyForthAndBack(car* Car, RLC rlc)
{
	GLint forthDistance = distanceToForthCar(rlc);
	GLint backDistance = distanceToBackCar(rlc);

	return (forthDistance - 1 > MAX_VELOCITY && backDistance - 1 >= Car->velocity);
}

GLvoid countSubVelocity(car* Car)
{
	GLint mainVelocity = Car->velocity;

}