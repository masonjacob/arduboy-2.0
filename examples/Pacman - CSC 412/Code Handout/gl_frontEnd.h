//
//  gl_frontEnd.h
//  GL threads

#ifndef GL_FRONT_END_H
#define GL_FRONT_END_H

#include <vector>
#include <string>
#include "glPlatform.h"
#include "PacMan.h"
#include "Ghost.h"
#include "Direction.h"
#include "ColorRGB.h"

//-----------------------------------------------------------------------------
//	Data types
//-----------------------------------------------------------------------------


struct RowCol
{
    int row;
    int col;
};

//Possible types for each tile in the grid
enum TileType
{
    Tile_Null = -1, //no grid entry should ever be this value but can be used as a return type of out-of-bounds tiles
    Tile_Floor = 0, //empty floor nothing more
    Tile_Wall, //no entities can go through
    Tile_Dot, //a dot for pacman to collect
    Tile_PowerUp, //a powerup for pacman to collect
    Tile_GhostGate, //A wall that pacman can go through but the ghosts cannot
    Tile_Jail, //A tile that represents the jail where the ghosts start out at
    Tile_Total //this should always be last
};

//everything we draw in the scene is stored in this data structure
struct GameWorld
{
    TileType** grid;
    int numRows;
    int numCols;
    PacMan* pacman;
    Ghost* ghost_array; //all of the ghosts in the game
    int ghost_count;
    std::vector<RowCol> grid_intersections; //all of the positions in the grid where there is an intersection
};

//helper methods for direction control
TileType gw_tileAt(int x, int y);
bool gw_dirIsOpposite(Direction dir1, Direction dir2);
bool gw_isInRange(int x, int y);
bool gw_isFree(int x, int y);
bool gw_isFloorTile(int x, int y);
bool gw_isFreeWithWrap(int x, int y);
bool gw_isIntersection(int x, int y);
void gw_dirToXY(Direction dir, int& x, int& y);
void gw_closetIntersection(int x, int y, int& out_x, int& out_y);
void gw_findNeighborsOfType(int x, int y, TileType desired_type, std::vector<RowCol>& out_pos);
void gw_getValidDirections(int x, int y, TileType desired_type, Direction* dirs, int& dirs_found);
void gw_getValidDirectionsMulti(int x, int y, const std::vector<TileType>& desired_type, Direction* dirs, int& dirs_found);
int distanceSquaredI(int x1, int y1, int x2, int y2);

// functions are in level_io.cpp and are used for reading the grid from a file
TileType io_charToTile(char c);
char io_tileToChar(TileType tt);
int io_loadLevel(GameWorld& game_world, const char* filepath);


//-----------------------------------------------------------------------------
//	Function prototypes
//-----------------------------------------------------------------------------

int initializeGameWorldFile(GameWorld& game_world, const char* filepath); //initialize the grid world by reading a file
void drawState(int numLiveThreads, int score, int dotsCollected, int pacmanLives);
void drawGameWorld(const GameWorld *game_world);
void initializeFrontEnd(int argc, char** argv, void (*gridCB)(void), void (*stateCB)(void));
void handleEntityMovement(int& row, int& row_percentage, int& col, int& col_percentage);
bool keyDown(char key); //poll the state of the keyboard
void updatePacAndGhosts(void);

#endif // GL_FRONT_END_H
