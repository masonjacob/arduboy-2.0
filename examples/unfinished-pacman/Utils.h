#ifndef Utils_h__
#define Utils_h__
#include "Arduino.h"
#include "Console.h"
#include "Direction.h"

#define WIDTH 160
#define HEIGHT 128
#define TUNNEL_X 53 //top left of where the entity touches

extern uint8_t g_level;


#define PLAY_W 3
#define PLAY_H 3
uint8_t solid_box[] =
{
	0x07, 0x07, 0x07,
};

char * DirToText(uint8_t dir);
void MovePosition(float *pXInOut, float *pYInOut, uint8_t dir, float amount);
bool HitWall(int16_t x, int16_t y);
bool CanGoRight(int16_t x, int16_t y);
bool CanGoUp(int16_t x, int16_t y);
bool CanGoLeft(int16_t x, int16_t y);
bool CanGoDown(int16_t x, int16_t y);
bool CanTurn(int16_t x, int16_t y, uint8_t dir);
bool IsInTunnel(int16_t x, int16_t y);
float EntitiesAreTouching(float ax, float ay, float bx, float by, float width );
void WorldWrapY(float &yInOut);
bool GetFoodWeAreOn(uint8_t *pFoodOutX,uint8_t *pFoodOutY, int16_t posX, int16_t posY);
// size_t printNumberFancy(uint8_t x, uint8_t y, long n, bool bCentered, bool bVertical);
#endif // Utils_h__
