#include <search_tools.h>

// Standard
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include <road.h>

dot_coord dot = {NO_COORD, NO_COORD};

bool getRLCbyDot(RLC* rlc, dot_coord* dot)
{
    if (dot->x == NO_COORD || dot->y == NO_COORD)
    {
        printf("Given dot is {NO_COORD, NO_COORD}\n");
        return false;
    }

    bool is_in = false;

    GLint roadIndex = getRoadIndex(dot);
    if (roadIndex == NO_ROAD_INDEX)
    {
        return false;
    }

    GLint lineIndex = getLineIndex(dot, roadIndex);
    if (lineIndex == NO_LINE_INDEX)
    {
        return false;
    }

    GLint cellIndex = getCellIndex(dot, roadIndex, lineIndex);
    if (cellIndex == NO_LINE_INDEX)
    {
        return false;
    }

    rlc->road = roadIndex;
    rlc->line = lineIndex;
    rlc->cell = cellIndex;
    return true;
}

GLint getRoadIndex(dot_coord* dot)
{
    GLint roadIndex = NO_ROAD_INDEX;
    bool is_in = false;
    for (roadIndex = 0; roadIndex < NUMBER_OF_ROADS; roadIndex++)
    {
        if (is_in = isInRoadRange(roadIndex, dot))
        {
            break;
        }
    }

    if (is_in)
    {
        return roadIndex;
    }
    else
    {
        return NO_ROAD_INDEX;
    }
}

//If the chosen dot belong to the line confining the cell, then isInRoadRange() returns FALSE.
bool isInRoadRange(GLint roadIndex, dot_coord* dot)
{
    road* Road = &roads[roadIndex];

    if (!(dot->x > Road->stem - HALF_ROAD_WIDTH && dot->x < Road->stem + HALF_ROAD_WIDTH))
    {
        return false;
    }

    switch (Road->dir)
    {
        case NORTH:
        {
            if (dot->y > Road->startLineCoord && dot->y < Road->endLineCoord)
            {
                return true;
            }
            else return false;
        }
        case  SOUTH:
        {
            if (dot->y < Road->startLineCoord && dot->y > Road->endLineCoord)
            {
                return true;
            }
            else return false;
        }
        case EAST:
        {
            if (dot->x > Road->startLineCoord && dot->x < Road->endLineCoord)
            {
                return true;
            }
            else return false;
        }
        case WEST:
        {
            if (dot->x < Road->startLineCoord && dot->x > Road->endLineCoord)
            {
                return true;
            }
            else return false;
        }
        
    }
}

GLint getLineIndex(dot_coord* dot, GLint roadIndex)
{
    GLfloat leftCheek = getRoadLeftCheek(roadIndex);
    GLfloat recess;

    switch (roads[roadIndex].dir)
    {
        case NORTH:
        {
            recess = abs(dot->x - leftCheek);
        }
        case SOUTH:
        {
            recess = abs(dot->x - leftCheek);
        }
        case EAST:
        {
            recess = abs(dot->y - leftCheek);
        }
        case WEST:
        {
            recess = abs(dot->y - leftCheek);
        }
    }

    GLint index = (GLint) recess / CELL_WIDTH;
    if (index >= 0 && index < NUMBER_OF_LINES)
    {
        return index;
    }
    else
    {
        return NO_LINE_INDEX;
    }
}

GLfloat getRoadLeftCheek(GLint roadIndex)
{
    switch (roads[roadIndex].dir)
    {
        case NORTH:
        {
            return roads[roadIndex].stem - HALF_ROAD_WIDTH;
        }
        case SOUTH:
        {
            return roads[roadIndex].stem + HALF_ROAD_WIDTH;
        }
        case EAST:
        {
            return roads[roadIndex].stem - HALF_ROAD_WIDTH;
        }
        case WEST:
        {
            return roads[roadIndex].stem + HALF_ROAD_WIDTH;
        }
    }
}

GLint getCellIndex(dot_coord* dot, GLint roadIndex, GLint lineIndex)
{
    GLfloat recess;

    switch (roads[roadIndex].dir)
    {
        case NORTH:
        {
            recess = abs(dot->y - roads[roadIndex].startLineCoord);
        }
        case SOUTH:
        {
            recess = abs(dot->y - roads[roadIndex].startLineCoord);
        }
        case EAST:
        {
            recess = abs(dot->x - roads[roadIndex].startLineCoord);
        }
        case WEST:
        {
            recess = abs(dot->x - roads[roadIndex].startLineCoord);
        }
    }

    GLint index = (GLint) recess / CELL_LENGTH;

    if (index >= 0 && index < NUMBER_OF_CELLS)
    {
        return index;
    }
    else
    {
        return NO_CELL_INDEX;
    }
}

GLvoid clearDot(dot_coord* dot)
{
    dot->x = NO_COORD;
    dot->y = NO_COORD;
}