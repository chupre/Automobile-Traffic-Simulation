#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <traffic_density.h>

int * densityData;
bool densityDataInit = false;

GLvoid increaseDensityData(GLint roadIndex)
{
    if (!densityDataInit) {
        densityData = malloc(sizeof(int) * NUMBER_OF_ROADS);

        if (densityData == NULL) {
            printf("malloc failed with densityData");
            exit(1);
        } 

        memset(densityData, 0, sizeof(int) * NUMBER_OF_ROADS);
        // printDensity();
        
        densityDataInit = true;
    }

    densityData[roadIndex] += 1;
    // printDensity();
}

GLvoid decreaseDensityData(GLint roadIndex)
{
    densityData[roadIndex] -= 1;
}

GLint getDensityData(GLint roadIndex)
{
    printf("density on %d road: %d\n", roadIndex, densityData[roadIndex]);
    // printDensity();
    return densityData[roadIndex];
}
// 


GLvoid printDensity(){
    for (GLint i = 0; i < NUMBER_OF_ROADS; i++){
        printf("r%d: %d\n", i, getDensityData(i));
    }
    
}