#include <traffic_density.h>

#include <road.h>
#include <cars.h>

GLint densityData[NUMBER_OF_ROADS] = {0};

GLvoid increaseDensityData(GLint roadIndex)
{
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