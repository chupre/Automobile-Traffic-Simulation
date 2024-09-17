#include <search_tools.h>

// Standard
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include <road.h>
#include <algorithms.h>
#include <map.h>

dot_coord _DOT_ = {NO_COORD, NO_COORD};

GLfloat absFloat(GLfloat a)
{
    if (a < 0) return (-1) * a;
    else return a;
}

bool getRLCbyDot(RLC* rlc, dot_coord* dot)
{
    if (dot->x == NO_COORD || dot->y == NO_COORD)
    {
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

    if (Road->dir == NORTH || Road->dir == SOUTH) {
        if (!(dot->x > Road->stem - HALF_ROAD_WIDTH && dot->x < Road->stem + HALF_ROAD_WIDTH)) {
            return false;
        }
    } else {
        if (!(dot->y > Road->stem - HALF_ROAD_WIDTH && dot->y < Road->stem + HALF_ROAD_WIDTH)) {
            return false;
        }
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

    DIRECTION dir = roads[roadIndex].dir;
    if (dir == NORTH || dir == SOUTH)
    {
        recess = absFloat(dot->x - leftCheek);
    }
    else
    {
        recess = absFloat(dot->y - leftCheek);
    }

    GLint index = recess / CELL_WIDTH;


    // printf("leftCheek: %f, recess: %f, index: %d\n", leftCheek, recess, index);
    if (index >= 0 && index < NUMBER_OF_LINES + 1)
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
            return roads[roadIndex].stem + HALF_ROAD_WIDTH;
        }
        case WEST:
        {
            return roads[roadIndex].stem - HALF_ROAD_WIDTH;
        }
    }
}

GLint getCellIndex(dot_coord* dot, GLint roadIndex, GLint lineIndex)
{
    GLfloat recess;
    DIRECTION dir = roads[roadIndex].dir;
    if (dir == NORTH || dir == SOUTH)
    {
        recess = absFloat(dot->y - roads[roadIndex].startLineCoord);
    }
    else
    {
        recess = absFloat(dot->x - roads[roadIndex].startLineCoord);
    }

    GLint index = recess / CELL_LENGTH;
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
