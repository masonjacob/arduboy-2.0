#include "Ghost.h"
#include "gl_frontEnd.h"

const std::vector<TileType> desired_types({Tile_Floor, Tile_Dot, Tile_PowerUp});

Ghost::Ghost()
{

}

void determineTarget(Ghost& ghost, GameWorld& gw, int& x, int& y)
{
    x = y = 0;

    //AI of Blinky (go to pacman)
    if (ghost.ai == AI_ChasePacman && gw.pacman != nullptr)
    {
        x = gw.pacman->col;
        y = gw.pacman->row;
    }
    else if (ghost.ai == AI_InterceptPacman)
    {
        int tx = 0, ty = 0;
        gw_dirToXY(gw.pacman->dir, tx, ty);
        x = gw.pacman->col + (tx * 4);
        y = gw.pacman->row + (ty * 4);
    }
    else if (ghost.ai == AI_RandomAndDumb)
    {
        Direction free_dirs[NUM_DIRECTIONS];
        int dircount = 0, tx = 0, ty = 0;;

        gw_getValidDirectionsMulti(ghost.col, ghost.row, desired_types, free_dirs, dircount);
        if (dircount > 0)
            gw_dirToXY(free_dirs[rand() % dircount], tx, ty);
        x = ghost.col + tx;
        y = ghost.row + ty;
    }
}
bool aiActive(GameWorld& gw, GhostAI& ai)
{
    bool a = false;
    if (gw.pacman != nullptr)
    {
        switch(ai)
        {
        case AI_ChasePacman: a = true; break;
        case AI_InterceptPacman: a = (gw.pacman->squares_traveled >= 32); break;
        case AI_ChaseGhost: a = (gw.pacman->squares_traveled >= 64); break;
        case AI_RandomAndDumb: a = (gw.pacman->squares_traveled >= 90); break;
        default: break;
        }
    }
    return a;
}

Direction getDirection(Ghost& ghost, GameWorld& gw)
{
    Direction requested_dir = NUM_DIRECTIONS;
    const TileType current_tile = gw.grid[ghost.row][ghost.col];

    if (!aiActive(gw, ghost.ai))
        return requested_dir;

    //determine the cell we are standing on
    if (current_tile == Tile_Jail)
    {
        if (gw_tileAt(ghost.col, ghost.row + 1) == Tile_GhostGate) requested_dir = NORTH;
        else if (gw_tileAt(ghost.col + 1, ghost.row) == Tile_Jail) requested_dir = EAST;
        else if (gw_tileAt(ghost.col - 1, ghost.row) == Tile_Jail) requested_dir = WEST;
    }
    else if (current_tile == Tile_GhostGate)
        requested_dir = NORTH;
    else if (current_tile != Tile_Wall)
    {
        Direction free_dirs[NUM_DIRECTIONS];
        Direction good_dirs[NUM_DIRECTIONS];
        int dircount = 0, goodir_count = 0, iter = 0;

        //choose a direction that is non-opposite of the current direction
        gw_getValidDirectionsMulti(ghost.col, ghost.row, desired_types, free_dirs, dircount);
        for (int i = 0; i < dircount; i++)
        {
            if (ghost.dir == NUM_DIRECTIONS || !gw_dirIsOpposite(ghost.dir, free_dirs[i]))
            {
                good_dirs[iter] = free_dirs[i];
                iter++;
                goodir_count++;
            }
        }

        //now determine which direction is best
        if (goodir_count <= 0)
            requested_dir = NUM_DIRECTIONS;
        else if (goodir_count == 1)
            requested_dir = good_dirs[0];
        else
        {
            int target_x = 0, target_y = 0;
            int closest_distance, distance, i, tx, ty;

            closest_distance = ((gw.numCols * gw.numCols) + (gw.numRows * gw.numRows));
            determineTarget(ghost, gw, target_x, target_y);

            //Find the closest direction to the desired target
            for (i = 0; i < goodir_count; i++)
            {
                gw_dirToXY(good_dirs[i], tx, ty); //determine the change in X and Y based on the direction
                distance = distanceSquaredI(ghost.col + tx, ghost.row + ty, target_x, target_y);
                if (distance <= closest_distance)
                {
                    closest_distance = distance;
                    requested_dir = good_dirs[i];
                }
            }

        }
    }

    return requested_dir;
}

void Ghost::ghostAI(GameWorld& gw)
{
    const int prev_row = row, prev_col = col;

    //move the ghost if we found a direction
    if (dir != NUM_DIRECTIONS)
    {
        int x, y;
        x = y = 0;
        gw_dirToXY(dir, x, y); //determine the change in X and Y based on the direction

        //determine if the desired position is a free space if not find a new direction to go
        if (gw_isFreeWithWrap(col + x, row + y) || gw_tileAt(col, row + 1) == Tile_GhostGate)
        {
            //move column or X position
            col_percentage += x * speed;
            row_percentage += y * speed;
		}
        else
            dir = getDirection(*this, gw);
    }
    else
        dir = getDirection(*this, gw);

    //keep moving the ghost if he is stuck between two grid cells and handles grid wrapping
    handleEntityMovement(row, row_percentage, col, col_percentage);

    //if the position has changed checked to see if we reached an intersection and change directions if this is the case
    if (row != prev_row || col != prev_col)
    {
        const TileType tt = gw_tileAt(col, row);
        if (gw_isIntersection(col, row) || tt == Tile_Jail || tt == Tile_GhostGate)
            dir = getDirection(*this, gw);
    }
}
