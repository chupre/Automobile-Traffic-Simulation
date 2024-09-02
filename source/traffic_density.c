#include "map.h"
#include <stdlib.h>
#include <string.h>
#include <traffic_density.h>
#include <stdbool.h>

GLint * densityData;
bool isDensityDataInit = false;

GLvoid increaseDensityData(GLint roadIndex)
{
    if (!isDensityDataInit) {
        densityData = malloc(sizeof(GLint) * NUMBER_OF_ROADS);
        memset(densityData, 0, sizeof(densityData));
        isDensityDataInit = true;
    }
    densityData[roadIndex] += 1;
}

GLvoid decreaseDensityData(GLint roadIndex)
{
    densityData[roadIndex] -= 1;
}

GLint getDensityData(GLint roadIndex)
{
    return densityData[roadIndex];
}
