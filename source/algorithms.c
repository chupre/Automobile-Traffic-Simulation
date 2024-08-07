// Custom modules
#include <algorithms.h>
#include <cars.h>
#include <road.h>
#include <render.h>

GLvoid update()
{
	if (glfwGetTime() - timer > STEP_TIME)
	{
		printf("Step at time: %lf\n\n", glfwGetTime());

		timer += STEP_TIME;

		step();
	}
}

//change cars' velocity & rlc
GLvoid thoughtsOfOneCar(car* Car)
{
	RLC rlc;
	GLint distance = distanceToForthCar(Car->currCell);
	GLint forthCarVelocity;

	if (Car->currCell.cell < NUMBER_OF_CELLS)
	{
		forthCarVelocity = getVelocityByRLC(Car->currCell);
	}
	else
	{
		forthCarVelocity = 0;
	}

	if (distance > Car->velocity || distance == NO_CAR)
	{
		if (Car->velocity < MAX_VELOCITY)
		{
			Car->velocity += _1_CELL;
		}
	}
	//else if ((forthCarVelocity < Car->velocity) && isAbleToChangeLine(Car, roads, &rlc))
	//{
	//	DIRECTION roadDir = getRoadDir(Car);
	//	Car->overtake = getOvertakeDir(roadDir);

	//	Car->nextCell.road = rlc.road;
	//	Car->nextCell.line = rlc.line;
	//	Car->nextCell.cell = rlc.cell;

	//	return;
	//}
	else
	{
		Car->velocity = distance - 1;
	}
	Car->nextCell.cell += Car->velocity;
}


GLvoid step()
{
	spawnCars();

	for (int i = 0; i < MAX_CARS; i++)
	{
		if (cars[i].isActive && !cars[i].isAvaria)
		{

			if (cars[i].realPos > 1.0f || cars[i].realPos < -1.0f)
			{
				excludeOutMappers(&cars[i]);
				continue;
			}

			unbindCarPtrFromCell(&cars[i]);
			reinitCurrCellWithNextCell(&cars[i]);
			thoughtsOfOneCar(&cars[i]);
		}
	}

}

GLvoid excludeOutMappers(car* Car)
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

	Car->currCell.road = EMPTY;
	Car->currCell.line = EMPTY;
	Car->currCell.cell = EMPTY;
	Car->nextCell.road = EMPTY;
	Car->nextCell.line = EMPTY;
	Car->nextCell.cell = EMPTY;
	Car->realPos = EMPTY;
	Car->ID = EMPTY;
	Car->target = NONE;
	Car->velocity = EMPTY;
	Car->isActive = false;
	Car->overtake = NONE;

	Car->dirOnRoad = 0;

	Car->isAvaria = 0;//though there is no sense to zeroize it because cars in Avaria cannot go out the boards of map
	++freeCars;
}

GLint getVelocityByRLC(RLC rlc)
{
	car** ptrCell = ((roads + rlc.road)->lines + rlc.line)->cells;
	if (ptrCell[rlc.cell] == NULL)
	{
		return NO_CAR;
	}
	else
	{
		return ptrCell[rlc.cell]->velocity;
	}
}

GLint distanceToForthCar(RLC rlc)
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
			return (i - rlc.cell);
		}
	}
	return NO_CAR;
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
	return NO_CAR;
}

GLint isAbleToChangeLine(car* Car, RLC* posOnNewLine)
{
	GLint newLine = Car->currCell.line - 1;
	if (newLine >= 0 && newLine <= NUMBER_OF_CELLS)
	{
		RLC rlc = { Car->currCell.road, newLine, Car->currCell.line };
		if (isSafetyForthAndBack(Car, rlc))
		{
			posOnNewLine->road = rlc.road;
			posOnNewLine->line = rlc.line;
			posOnNewLine->cell = rlc.cell;
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}
}

//you shouldn't fear wether the car chooses a position between CurrCell and NextCell
GLint isSafetyForthAndBack(car* Car, RLC rlc)
{
	GLint forthDistance = distanceToForthCar(rlc);
	GLint backDistance = distanceToBackCar(rlc);

	return (forthDistance > _5_CELL && backDistance >= Car->velocity);
}

GLvoid countSubVelocity(car* Car)
{
	GLint mainVelocity = Car->velocity;

}