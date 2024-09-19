#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <traffic_density.h>

bool densityDataInit = false;

GLvoid increaseDensityData(GLint roadIndex)
{
    // if (!densityDataInit) {
    //     densityData = (GLint*)calloc(NUMBER_OF_ROADS, sizeof(GLint));

    //     if (densityData == NULL) {
    //         printf("malloc failed with densityData");
    //         exit(1);
    //     } 
    //     printf("yes\n");
    //     // printDensity();
        
    //     densityDataInit = true;
    // }

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