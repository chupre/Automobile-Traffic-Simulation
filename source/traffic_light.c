#include <traffic_light.h>
#include <macros.h>

#include <stdbool.h>

traffic_light lights[NUMBER_OF_TRAFFIC_LIGHTS];
bool yellowRedChange = false;

GLvoid changeLightsColor()
{
    for (GLint i = 0; i < NUMBER_OF_TRAFFIC_LIGHTS; i++)
    {
        if (lights[i].color == GREEN)
        {
            lights[i].color == YELLOW;
        }
        else if (lights[i].color == YELLOW)
        {
            lights[i].color == RED;
            yellowRedChange = true;
        }
        else
        {
            lights[i].color = GREEN;
        }
    }
}
