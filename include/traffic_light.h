#ifndef TRAFFIC_LIGHT_H
#define TRAFFIC_LIGHT_H

#include <macros.h>
#include <map.h>

// External
#include <glad/glad.h>
#include <GLFW/glfw3.h>

///Standart
#include <stdbool.h>

#define TRAFFIC_LIGHTS_TIME_DIGIT (4)

#define RED_TIME 3
#define YELLOW_TIME 1
#define GREEN_TIME 2

typedef struct car car;

typedef enum COLOR COLOR;
enum COLOR {
    GREEN, YELLOW, RED
};

typedef struct traffic_light traffic_light;
struct traffic_light {
    COLOR color;
    GLint changeTimer;
};

extern GLint timer_digit;
extern GLint traffic_light_adding_index;
extern bool yellowRedChange;

GLvoid changeLightsColor();
COLOR getLightColor(GLint lightIndex);
COLOR getLightColorByCar(car* Car);
GLint getFreeTrafficLightIndex();
GLvoid setDefaultTrafficLightProperties();
GLvoid printLights();


#endif