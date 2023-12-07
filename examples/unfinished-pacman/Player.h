//  ***************************************************************
//  Player - Creation date: 05/29/2016
//  -------------------------------------------------------------
//  Robinson Technologies Copyright (C) 2016 - All Rights Reserved
//
//  ***************************************************************
//  Programmer(s):  Seth A. Robinson (seth@rtsoft.com)
//  ***************************************************************

#ifndef Player_h__
#define Player_h__

#define CRUMB_COUNT 24
#define LIVES_COUNT 3
#define PLAYER_FAST_SPEED 0.6f
#define PLAYER_SLOW_SPEED 0.48f //when eating pellets
#define TOTAL_PELLETS 244 //arcade game has 240.  Fruit at 70 and 170 
#define C_POWER_PILL_TIME_MS 6000
#define POWER_WEAR_OFF_WARNING_MS 1000
#define GHOST_KILL_POINTS 200
#define PELLET_POINTS 10
#define POWER_PELLET_POINTS 50


#include "Utils.h"

uint8_t player_right_open[] =
{
	0x07, 0x05, 0x05,
};

uint8_t player_right_closed[] =
{
	0x07, 0x07, 0x07,
};

uint8_t player_left_open[] =
{
	0x05, 0x05, 0x07,
};

uint8_t player_up_open[] =
{
	0x07, 0x04, 0x07,
};

//yeah, it's the same as the ghost, so what
uint8_t player_down_open[] =
{
	0x07, 0x01, 0x07,
};

struct Breadcrumb
{
	int16_t m_x;
	int16_t m_y;
	uint8_t m_dir;
};

#define POWER_PILL_COUNT 4

class PowerPill
{
public:

	uint8_t m_x;
	uint8_t m_y;
	bool m_active;
	
	void Render(Console* gameboy)
	{
		if (m_active)
			gameboy->drawBitmap(m_x-1, m_y-1, solid_box, PLAY_W,PLAY_H, ST7735_WHITE, ST7735_BLACK);
	}
	void UnRender(Console* gameboy)
	{
		gameboy->drawBitmap(m_x-1, m_y-1, solid_box, PLAY_W,PLAY_H, ST7735_BLACK, ST7735_BLACK);
	}

	void Setup(uint8_t x, uint8_t y)
	{
		m_x = x;
		m_y = y;
		m_active = true;
	}

};

class CrumbManager
{
public:
	
	CrumbManager()
	{
		Reset();
	}

	void Reset()
	{
		memset(m_crumbArray, 0, sizeof(m_crumbArray));
		m_curCrumb =0;
	}

	uint8_t GetDirToPlayer(int16_t x, int16_t y)
	{
		for (int i=0; i < CRUMB_COUNT; i++)
		{
			if (m_crumbArray[i].m_x == x && m_crumbArray[i].m_y == y)
			{
				//player was here, and we know which way he went!
				return m_crumbArray[i].m_dir;
			}
		}

		return 0;
	}

	void AddCrumb(int16_t x, int16_t y, uint8_t dir)
	{
		if (m_crumbArray[m_curCrumb].m_x == x && m_crumbArray[m_curCrumb].m_y == y)
		{
			//same as before, don't need to add it, but should update the dir
			m_crumbArray[m_curCrumb].m_dir= dir;
			return;
		}

			m_curCrumb = (m_curCrumb+1)%CRUMB_COUNT;
			//LogMsg("Setting crumb id %d to %s", m_curCrumb, DirToText(dir));
			m_crumbArray[m_curCrumb].m_x = x;
			m_crumbArray[m_curCrumb].m_y = y;
			m_crumbArray[m_curCrumb].m_dir= dir;

		
	}
	
	uint8_t m_curCrumb;
	Breadcrumb m_crumbArray[CRUMB_COUNT];
};

extern CrumbManager crumbManager;

class Player
{
public:
	Player();

	void ResetGame();
	void ResetLevel();
	void ResetStatus();
	void OnPowerPillGet();
	bool PowerIsActive();
	long GetPowerTimeLeft();
	bool PowerIsActiveDisplay();
	void OnKilledGhost();
	void AddScore(long score);
	void Update();
	bool LevelPassed();
	void Blit(bool color);
	void UnRender();
	void Render();
	void OnHurt();
	void Melt(uint16_t y);
	void OnEndOfLoop();

	float m_x;
	float m_y;
	uint8_t m_curDir;

	uint32_t m_score; //don't change format
	uint8_t m_inputState;
	const uint8_t *pBitmap;
	float m_speed;
	uint8_t m_wasHurt;
	uint8_t m_livesLeft;
	uint8_t m_noAnimate;
	long m_slowTimer;
	uint16_t m_pelletsEaten;
	PowerPill m_pills[POWER_PILL_COUNT];
	long m_powerTimer;
	uint8_t m_ghostsKilled;
	long m_sfxTimer;
	bool m_bGotExtraLife; //get an extra life at 10k
	

};

#endif // Player_h__