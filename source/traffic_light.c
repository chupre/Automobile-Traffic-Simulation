#include <traffic_light.h>
#include <macros.h>
#include <cars.h>
#include <road.h>
#include <map.h>

#include <stdbool.h>
#include <stdio.h>

GLint timer_digit = 0;
GLint traffic_light_adding_index = -1;
bool yellowRedChange = false;

const int RED_TIME = 6;
const int YELLOW_TIME = 2;
const int GREEN_TIME = 6;

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

        // Render
        glGenVertexArrays(1, &lights[roads[roadIndex].traffic_light_index].VAO);
        glGenBuffers(1, &lights[roads[roadIndex].traffic_light_index].VBO);
        glGenBuffers(1, &lights[roads[roadIndex].traffic_light_index].EBO);

        glBindVertexArray(lights[roads[roadIndex].traffic_light_index].VAO);

        glBindBuffer(GL_ARRAY_BUFFER, lights[roads[roadIndex].traffic_light_index].VBO);

        float size = CELL_LENGTH * 1.5;
        float height = size * 2;
        float width = size * 0.7;

        float x = roads[roadIndex].stem + HALF_ROAD_WIDTH;
        float y = roads[roadIndex].endLineCoord; 
    
        DIRECTION dir = roads[roadIndex].dir;
        if (dir == NORTH) {
            y -= height;
            float vertices[] = {
                // positions                    // texture coords
                x + width,  y + height, 1.0f,   1.0f, 1.0f, 
                x + width,  y,          1.0f,   1.0f, 0.0f, 
                x,          y,          1.0f,   0.0f, 0.0f, 
                x,          y + height, 1.0f,   0.0f, 1.0f  
            };
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        }
        if (dir == SOUTH) {
            x -= HALF_ROAD_WIDTH * 2 + CELL_LENGTH;
            float vertices[] = {
                // positions                    // texture coords
                x + width,  y,          1.0f,   1.0f, 1.0f, 
                x + width,  y + height, 1.0f,   1.0f, 0.0f, 
                x,          y + height, 1.0f,   0.0f, 0.0f, 
                x,          y,          1.0f,   0.0f, 1.0f  
            };
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        }
        if (dir == EAST) {
            x -= HALF_ROAD_WIDTH * 2 + CELL_LENGTH;
            y -= height;
            float vertices[] = {
                // positions                    // texture coords
                y + height, x + width, 1.0f,    1.0f, 1.0f, 
                y + height, x,         1.0f,    0.0f, 1.0f, 
                y,          x,         1.0f,    0.0f, 0.0f, 
                y,          x + width, 1.0f,    1.0f, 0.0f  
            };
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        }
        if (dir == WEST) {
            float vertices[] = {
                // positions                    // texture coords
                y,          x + width,  1.0f,   1.0f, 1.0f, 
                y,          x,          1.0f,   0.0f, 1.0f, 
                y + height, x,          1.0f,   0.0f, 0.0f, 
                y + height, x + width,  1.0f,   1.0f, 0.0f  
            };
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        }

        unsigned int indices[3 * 2] = {  
            0, 1, 3,   
            1, 2, 3    
        };  

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lights[roads[roadIndex].traffic_light_index].EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
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
