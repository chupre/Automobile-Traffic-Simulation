#include <traffic_light.h>
#include <macros.h>
#include <cars.h>
#include <road.h>
#include <map.h>

#include <stdbool.h>
#include <stdio.h>

GLint timer_digit = 0;
GLint traffic_light_adding_index = -1;
traffic_light lights[NUMBER_OF_TRAFFIC_LIGHTS];
bool yellowRedChange = false;

GLvoid changeLightsColor(){
    // return;
    for (GLint i = 0; i <= traffic_light_adding_index; i++){
        if (--lights[i].changeTimer > 0){
            continue;
        }
        if (lights[i].color == GREEN){
            lights[i].color = YELLOW;
            lights[i].changeTimer = YELLOW_TIME;
        }
        else if (lights[i].color == YELLOW){
            lights[i].color = RED;
            lights[i].changeTimer = RED_TIME;
            yellowRedChange = true;
        }
        else if(lights[i].color == RED){
            lights[i].color = GREEN;
            lights[i].changeTimer = GREEN_TIME;
        }
    }
    // printLights();
}

COLOR getLightColor(GLint lightIndex){
    return lights[lightIndex].color;
}

COLOR getLightColorByCar(car* Car){
    return lights[roads[Car->currCell.road].traffic_light_index].color;
}

GLint getFreeTrafficLightIndex(){
    return ++traffic_light_adding_index;
}

GLvoid setDefaultTrafficLightProperties(){
    for (int roadIndex = 0; roadIndex < NUMBER_OF_ROADS; roadIndex++){
        if (!roads[roadIndex].isEndCross){
            continue;
        }
        if (roads[roadIndex].dir == NORTH || roads[roadIndex].dir == SOUTH){
            lights[roads[roadIndex].traffic_light_index].color = GREEN;
            lights[roads[roadIndex].traffic_light_index].changeTimer = GREEN_TIME;
        }
        else{
            lights[roads[roadIndex].traffic_light_index].color = RED;
            lights[roads[roadIndex].traffic_light_index].changeTimer = RED_TIME ;
        }
    }
}

GLvoid printLights(){
    printf("\t\t\t\t");
    for (int i = 0; i < 4; i+=4){
        // printf("light ");
        if (i == 0) printf("NORTH");
        else if (i == 1) printf("SOUTH");
        else if (i == 2) printf("EAST");
        else if (i == 3) printf("WEST");
        printf(":");
        if (lights[i].color == RED){
            printf("RED");
        }
        else if (lights[i].color == YELLOW){
            printf("YELLOW");
        }
        else if (lights[i].color == GREEN){
            printf("GREEN");
        }
        printf("\t");
    }
    printf("\n");
}