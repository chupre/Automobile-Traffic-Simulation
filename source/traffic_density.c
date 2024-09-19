#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <traffic_density.h>

int * densityData;
bool densityDataInit = false;

GLvoid increaseDensityData(GLint roadIndex)
{
    if (!densityDataInit) {
        densityData = (int*)calloc(NUMBER_OF_ROADS, sizeof(int));

        if (densityData == NULL) {
            printf("malloc failed with densityData");
            exit(1);
        } 

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
    // printDensity();
    return densityData[roadIndex];
}
// 


GLvoid printDensity(){
    for (GLint i = 0; i < NUMBER_OF_ROADS; i++){
        printf("r%d: %d\n", i, getDensityData(i));
    }
    
}
