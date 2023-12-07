//  ***************************************************************
//  Ghost - Creation date: 05/29/2016
//  -------------------------------------------------------------
//  Robinson Technologies Copyright (C) 2016 - All Rights Reserved
//
//  ***************************************************************
//  Programmer(s):  Seth A. Robinson (seth@rtsoft.com)
//  ***************************************************************

#ifndef Ghost_h__
#define Ghost_h__

#include "Utils.h"
#include "Player.h"
#include "Console.h"

#define GHOST_MAX 4
#define STARTING_GHOSTS 4
#define GHOSTS_TO_ADD_EACH_LEVEL 0 //if not 0, we add to the total ghost count each level, allowing crazy bonuses
#define GHOST_KILL_DELAY_MS 380 //the freeze when you "pop" a ghost
#define GHOST_SPEED 0.5f
#define GHOST_CUBBY_HOLE_X 53
#define GHOST_CUBBY_HOLE_Y 23
#define GHOST_TUNNEL_SPEED_MOD 0.6f; //ghost speed is multiplied by this when in tunnel
#define GHOST_START_Y 17

extern Player player;

extern uint8_t GHOST_COUNT;

uint8_t ghost_bmp[]  =
{
	0x07, 0x01, 0x07,
};


class Ghost
{
public:
	Ghost();
	~Ghost();

	void SetPauseTimer(long time);
	float GetAdjustedSpeed();
	void ResetPosition(Console* gameboy);
	void SetLastTurnedSpot(int16_t x, int16_t y);
	bool IsNewTurnSpot(int16_t x, int16_t y);
	bool IsChasing();
	void Update(Console* gameboy);
	void Blit(bool color, bool bDying, Console* gameboy);
	void UnRender(Console* gameboy);
	void Render(bool bDying, Console* gameboy);
	void SetGhostID(int8_t id){m_id = id;}
	void OnReleasePellet(uint8_t foodX, uint8_t foodY);
	void ResetForNewGame();

	float m_x;
	float m_y;
	uint8_t m_curDir;
	float m_speed;
	uint8_t m_foodX,m_foodY;
	long m_pauseTimer;
	uint8_t m_id;
	int16_t m_oldX, m_oldY;
	bool m_bIsChasing;
	unsigned long m_thinkTimer;
};

extern Ghost ghosts[GHOST_MAX];

void DontDrawPelletHere(uint8_t x, uint8_t y);
uint8_t RandomTurn(uint8_t dir);

void ResetGhostPositions(bool bNewGame);
#endif // Ghost_h__