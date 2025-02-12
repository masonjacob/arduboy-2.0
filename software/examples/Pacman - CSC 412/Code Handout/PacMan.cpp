#include "PacMan.h"
#include "gl_frontEnd.h"
#include <cstdio>
#include <cmath>

#define SOME_EPSILON 0.00001f

PacMan::PacMan()
{
    for (int i = 0; i < NUM_DIRECTIONS; i++)
        keys[i] = false;
    speed = 20;
    row_percentage = col_percentage = 0;
    squares_traveled = 0;
    dir = EAST;
    requested_dir = NO_DIRECTION;
}
void PacMan::key_update(unsigned char key, bool keydown)
{
    if (keydown)
    {
        switch (key) //this code handles capital letters and lower case letters in case caps lock is unknowingly on
        {
        case 'A':
        case 'a': requested_dir = WEST; break;
        case 'S':
        case 's': requested_dir = SOUTH; break;
        case 'D':
        case 'd': requested_dir = EAST; break;
        case 'W':
        case 'w': requested_dir = NORTH; break;
        }
    }
}

void PacMan::checkUserInput()
{
    if (dir != requested_dir && requested_dir != NO_DIRECTION)
    {
        int x = 0, y = 0;
        const int dir_leeway = 10;

        gw_dirToXY(requested_dir, x, y);
        if (gw_isFreeWithWrap(col + x, row + y))
        {
            if ((requested_dir == NORTH || requested_dir == SOUTH) && abs(col_percentage) <= dir_leeway)
            {
                 col_percentage = 0;
                 dir = requested_dir;
            }
            if ((requested_dir == EAST || requested_dir == WEST) && abs(row_percentage) <= dir_leeway)
            {
                 row_percentage = 0;
                 dir = requested_dir;
            }
        }
    }
}
void PacMan::update(GameWorld& gw)
{
    const int prev_x = col, prev_y = row;
    int x, y;

    checkUserInput();

    //move pacman
    x = y = 0;
    gw_dirToXY(dir, x, y); //determine the change in X and Y based on the direction

    //determine if the desired position is a free space
    if (gw_isFreeWithWrap(col + x, row + y))
    {
        isAnimated = true;

        //move column or X position
        col_percentage += x * speed;
        row_percentage += y * speed;
    }
    else
    {
        isAnimated = false;
    }

    //keep moving pacman if he is stuck between two grid cells and handles grid wrapping
    handleEntityMovement(row, row_percentage, col, col_percentage);

    //update the squares traveled counter if the position has changed
    if (prev_x != col || prev_y != row)
        squares_traveled++;
}
