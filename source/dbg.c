#include <dbg.h>

#include <cross.h>
#include <cars.h>

GLvoid printGrid(GLint crossIndex){
    printf("\nCROSS %d\n", crossIndex);
    for (int y = 0; y < CROSS_SIDE; y++){
        for (int i = 0; i < CROSS_SIDE; i++) printf("____");
        printf("\n");
        for (int x = 0; x < CROSS_SIDE; x++){
            cross_cell c = {crossIndex, x, y};
            car* Car = getCarByCrossCell(&c);
            if (Car == OCCUPYING_CAR){
                printf("| * ");
            }
            else if (Car == NULL){
                printf("|   ");
            }
            else{
                if (Car->ID < 10) printf("| %d ", Car->ID);
                else if (Car->ID < 100) printf("| %d", Car->ID);
            }
        }
        printf("|\n");
    }
    for (int i = 0; i < CROSS_SIDE; i++) printf("____");
    printf("\n");
}