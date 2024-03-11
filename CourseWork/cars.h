#include <road.h>
#include <time.h>
#define MAX_CARS 4
#define CAR_WIDTH ROAD_WIDTH * 2.0f / ((GLfloat)NUMBER_OF_LINES + 1.0f) / 3.0f
#define CAR_LENGHT CAR_WIDTH * 1.5f
#define STEP_TIME 3

typedef struct
{
	GLuint* position;
	GLuint ID;
	GLint velocity;
	enum direction target;
	bool isActive;
} car;


void setCar(road* Road, GLint ID, car* Car, GLint carIndex, GLint lineIndex, GLfloat* carVertices, GLint* carIndices);
void setCarsToDefault(car* cars);
void step();
bool getFreeSpotAddress(road* roads, GLint* lineIndex, GLint* roadIndex);
GLint getFreeCarIndex(car* cars);


bool getFreeSpotAddress(road* roads, GLint* lineIndex, GLint* roadIndex)
{
	for (int i = 0; i < NUMBER_OF_ROADS; i++)
	{
		if (roads[i].isEdge)
		{
			for (int j = 0; j < NUMBER_OF_LINES + 1; j++)
			{
				if (roads[i].lines[j].cells[0] == EMPTY)
				{
					*lineIndex = j;
					*roadIndex = i;
					return true;
				}
			}
		}
	}

	return false;
}


void setCar(road* Road, GLint ID, car* Car, GLint carIndex, GLint lineIndex, GLfloat* carVertices, GLint* carIndices)
{
	enum direction carDir = Road->dir;
	GLfloat x1 = Road->lines[lineIndex].carSpawnCoord - CAR_WIDTH;
	GLfloat x2 = x1 - CAR_WIDTH;
	GLfloat y1, y2;

	switch (carDir)
	{
	case NORTH:
		y1 = -1.0f;
		y2 = y1 + CAR_LENGHT;
		break;
	case SOUTH:
		y1 = 1.0f;
		y2 = y1 - CAR_LENGHT;
		break;
	case EAST:
		y1 = x1;
		y2 = x2;
		x1 = -1.0f;
		x2 = -1.0f + CAR_LENGHT;
		break;
	case WEST:
		y1 = x1;
		y2 = x2;
		x1 = 1.0f;
		x2 = 1.0f - CAR_LENGHT;
		break;
	}

	Road->lines[lineIndex].cells[0] = ID;
	Car->target = rand() % 4;
	Car->velocity = 1;
	Car->ID = ID;
	Car->position = &Road->lines[lineIndex].cells[0];
	Car->isActive = true;

	switch (Car->target)
	{

	case 0: 
	{
		GLfloat vertices[] =
		{
			x1, y1, 0.0f, 1.0f, 0.0f, 0.0f,
			x2, y1, 0.0f, 1.0f, 0.0f, 0.0f,
			x1, y2, 0.0f, 1.0f, 0.0f, 0.0f,
			x2, y2 ,0.0f, 1.0f, 0.0f, 0.0f
		};
		memcpy(&carVertices[carIndex * 3 * 4 * 2], vertices, sizeof(GLfloat) * 4 * 3 * 2);
		Car->target = NORTH;
		break;
	}
	case 1:
	{
		GLfloat vertices[] =
		{
			x1, y1, 0.0f, 0.0f, 1.0f, 0.0f,
			x2, y1, 0.0f, 0.0f, 1.0f, 0.0f,
			x1, y2, 0.0f, 0.0f, 1.0f, 0.0f,
			x2, y2 ,0.0f, 0.0f, 1.0f, 0.0f
		};
		memcpy(&carVertices[carIndex * 3 * 4 * 2], vertices, sizeof(GLfloat) * 4 * 3 * 2);
		Car->target = SOUTH;
		break;
	}
	case 2:
	{
		GLfloat vertices[] =
		{
			x1, y1, 0.0f, 0.0f, 0.0f, 1.0f,
			x2, y1, 0.0f, 0.0f, 0.0f, 1.0f,
			x1, y2, 0.0f, 0.0f, 0.0f, 1.0f,
			x2, y2 ,0.0f, 0.0f, 0.0f, 1.0f
		};
		memcpy(&carVertices[carIndex * 3 * 4 * 2], vertices, sizeof(GLfloat) * 4 * 3 * 2);
		Car->target = EAST;
		break;
	}
	case 3:
	{
		GLfloat vertices[] =
		{
			x1, y1, 0.0f, 1.0f, 1.0f, 0.0f,
			x2, y1, 0.0f, 1.0f, 1.0f, 0.0f,
			x1, y2, 0.0f, 1.0f, 1.0f, 0.0f,
			x2, y2 ,0.0f, 1.0f, 1.0f, 0.0f
		};
		memcpy(&carVertices[carIndex * 3 * 4 * 2], vertices, sizeof(GLfloat) * 4 * 3 * 2);
		Car->target = WEST;
		break;
	}

	}

	GLint indeces[] =
	{
		0 + (carIndex * 4), 2 + (carIndex * 4), 3 + (carIndex * 4),
		0 + (carIndex * 4), 1 + (carIndex * 4), 3 + (carIndex * 4)
	};

	memcpy(&carIndices[carIndex * 6], indeces, sizeof(GLint) * 6);
}


GLint getFreeCarIndex(car* cars)
{
	for (int i = 0; i < MAX_CARS; i++)
	{
		if (!cars[i].isActive)
		{
			return i;
		}
	}

	return EMPTY;
}


void setCarsToDefault(car* cars)
{
	for (int i = 0; i < MAX_CARS; i++)
	{
		cars[i].ID = EMPTY;
		cars[i].position = EMPTY;
		cars[i].target = EMPTY;
		cars[i].velocity = EMPTY;
		cars[i].isActive = false;
	}
}


void step()
{

}
