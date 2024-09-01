#ifndef TRAFFIC_LIGHT_H
#define TRAFFIC_LIGHT_H

#include <macros.h>

// External
#include <glad/glad.h>
#include <GLFW/glfw3.h>

///Standart
#include <stdbool.h>

#define NUMBER_OF_TRAFFIC_LIGHTS (4 * NUMBER_OF_CROSSES)

typedef enum COLOR COLOR;
enum COLOR {
    GREEN, YELLOW, RED
};

typedef struct traffic_light traffic_light;
struct traffic_light {
    COLOR color;
};

extern traffic_light lights[NUMBER_OF_TRAFFIC_LIGHTS];
extern bool yellowRedChange;

#endif