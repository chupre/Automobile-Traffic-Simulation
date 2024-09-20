#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <traffic_density.h>

bool densityDataInit = false;

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


GLvoid printDensity(){
    for (GLint i = 0; i < NUMBER_OF_ROADS; i++){
        printf("r%d: %d\n", i, getDensityData(i));
    } 
}