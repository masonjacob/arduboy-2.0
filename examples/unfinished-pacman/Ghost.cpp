#include "Ghost.h"
#include "Direction.h"

uint8_t GHOST_COUNT = STARTING_GHOSTS;

Ghost ghosts[GHOST_MAX];

void DontDrawPelletHere(uint8_t x, uint8_t y)
{
	for (int i=0; i < GHOST_COUNT; i++)
	{
		ghosts[i].OnReleasePellet(x,y);
	}
}


uint8_t RandomDirection()
{
	
	int r = random(0,4);

	switch (r)
	{
	case 0: return Direction::West;
	case 1: return Direction::East;
	case 2: return Direction::North;
	case 3: return Direction::South;
	}

	return Direction::West; //error
}

uint8_t GetDirTowardsPlayer(int16_t x, int16_t y, uint8_t curDir)
{
	if (curDir == Direction::West || curDir == Direction::East)
	{
		if (y > player.m_y) return Direction::North; else return Direction::South;
	} 

	if (x < player.m_x) return Direction::East;

	return Direction::West;
}

uint8_t RandomTurn(uint8_t dir)
{

#ifdef _DEBUG
//LogMsg("Rand turn starting with %s", DirToText(dir));
#endif
	int r = random(0,2);


	if (dir == Direction::West || dir == Direction::East)
	{
		switch (r)
		{
		case 0: return Direction::North;
		case 1: return Direction::South;
		}
	} else
	{
		switch (r)
		{
		case 0: return Direction::West;
		case 1: return Direction::East;
		}
	}

	return Direction::West; //error
}

Ghost::Ghost()
{
	m_x = 0;
	m_y = 0;
	m_id = 0;
	m_pauseTimer = 0;
	m_foodX = m_foodY = 0;
}

Ghost::~Ghost()
{
}



void ResetGhostPositions(bool bNewGame, Console* gameboy)
{
	GHOST_COUNT = STARTING_GHOSTS + (GHOSTS_TO_ADD_EACH_LEVEL*(g_level-1));
	for (int i=0; i < GHOST_COUNT; i++)
	{
		if (bNewGame)
			ghosts[i].ResetForNewGame();
		ghosts[i].SetGhostID(i);
		ghosts[i].ResetPosition(gameboy);
		ghosts[i].SetPauseTimer(millis()+500+800*i);

	}
}

void Ghost::ResetForNewGame()
{
	m_foodX = 0;
	m_x = 0;
	m_y = 0;
}
void Ghost::SetPauseTimer(long time)
{
	m_pauseTimer = time;
	m_thinkTimer = m_thinkTimer+= time;
}

float Ghost::GetAdjustedSpeed()
{
	return GHOST_SPEED; //if we wanted them faster each level   +(0.02f*(g_level-1));
}

void Ghost::ResetPosition(Console* gameboy)
{
	UnRender(gameboy);
	m_foodX = 0;
	m_oldY = m_oldX =0;
	m_speed = GetAdjustedSpeed();//make it harder..
	if (m_speed > 1) m_speed = 1;
	m_x = GHOST_CUBBY_HOLE_X;
	int offset;
	
	if (m_id < 4)
	{
		 offset = (m_id*5);
	} else
	{
		offset = random(0, 15);
	}

	m_y = (float)GHOST_CUBBY_HOLE_Y+ offset;
	m_curDir = RandomTurn(Direction::North); //so we only turn left or right at the start
	m_bIsChasing = false;
	m_thinkTimer = random(500, 4000);

}

void Ghost::SetLastTurnedSpot(int16_t x, int16_t y)
{
	m_oldX = x;
	m_oldY = y;
}

bool Ghost::IsNewTurnSpot(int16_t x, int16_t y)
{
	return x != m_oldX || y != m_oldY;
}

bool Ghost::IsChasing()
{
	if (m_thinkTimer < millis())
	{
		if (!m_bIsChasing)
		{
			m_thinkTimer = millis()+random(5000,8000);
		} else
		{
			m_thinkTimer = millis()+random(1000,3000); //let's chase more than we don't chase
		}
		m_bIsChasing = !m_bIsChasing;
	}
	return m_bIsChasing;
}

void Ghost::Update(Console* gameboy)
{
	bool bIgnoreNextTurn = false;
	uint8_t tempDir = m_curDir;

	if (m_pauseTimer > millis()) 
	{
		//do nothing	
	} else
	{

		if (m_x == GHOST_CUBBY_HOLE_X && m_y > GHOST_START_Y && m_y < GHOST_CUBBY_HOLE_Y+(4*4))
		{
			//they are still in the cubby!

			if (m_y > GHOST_START_Y)
			{
				//float up
				m_y = int(m_y-1);
				return;
			}

			m_y = GHOST_START_Y;

			bIgnoreNextTurn = true;
		}
		
		//get new position
		float x = m_x;
		float y = m_y;
		if (EntitiesAreTouching(m_x, m_y, player.m_x, player.m_y, 2.0f))
		{
			if (player.PowerIsActive())
			{
				Render(true, gameboy);
				delay(GHOST_KILL_DELAY_MS);
				// arduboy.display();
				ResetPosition(gameboy);
				SetPauseTimer(millis()+player.GetPowerTimeLeft()+(500*m_id));
				player.OnKilledGhost();
				return;
			} else
			{
				player.OnHurt();
			}
		}
		uint8_t crumbDir =crumbManager.GetDirToPlayer((int16_t)x, (int16_t)y);

		/*
		if (crumbDir)
		LogMsg("Enemy located player to the %s", DirToText(crumbDir));
		*/

		if (!bIgnoreNextTurn && IsNewTurnSpot((int16_t)x,(int16_t)y) && CanTurn((int16_t)x,(int16_t)y, m_curDir) && 
			(random(3) == 0||crumbDir || IsChasing()) && m_y > 5 && m_y < 60)
		{
		
				if (crumbDir != 0)
				{
					tempDir = crumbDir;
				} else
				{
					if (IsChasing())
					{
						tempDir = GetDirTowardsPlayer((int16_t)m_x, (int16_t)m_y, m_curDir);
						
					} else
					{
						tempDir = RandomTurn(m_curDir);
					}
				}
#ifdef _DEBUG
//LogMsg("Chose dir %s", DirToText(tempDir));
#endif
			SetLastTurnedSpot((int16_t)x,(int16_t)y);

			MovePosition(&x,&y, tempDir, 1);

			while (HitWall((int16_t)x, (int16_t)y))
			{
				//bad turn, undo it
				x = m_x;
				y = m_y;
				tempDir = RandomTurn(m_curDir);
#ifdef _DEBUG
		//		LogMsg("Chose dir2 %s", DirToText(tempDir));
#endif
				MovePosition(&x,&y, tempDir, 1);
			}
			
		} else
		{
			float speed;
			if (player.PowerIsActive() || IsInTunnel((int16_t)x,(int16_t)y))
			{
				speed = m_speed*GHOST_TUNNEL_SPEED_MOD;
			} else
			{
				speed = m_speed;
			}
			MovePosition(&x,&y, m_curDir, speed);
		
			while(HitWall((int16_t)x, (int16_t)y) && m_y > 0 && m_y < 60)
			{
				uint8_t crumbDir =crumbManager.GetDirToPlayer((int16_t)m_x, (int16_t)m_y);

				if (crumbDir)
				{
					tempDir = crumbDir;
					//LogMsg("Enemy located player to the %s (wall bump)", DirToText(crumbDir));
				} else
				{
					tempDir = RandomTurn(m_curDir);
				}
				x = m_x;
				y = m_y;
#ifdef _DEBUG
			//	LogMsg("Chose dir3 %s", DirToText(tempDir));
#endif
				MovePosition(&x,&y, tempDir, 1);
				SetLastTurnedSpot((int16_t)x,(int16_t)y);
			}

		}

		//valid move
		m_curDir = tempDir;
		m_x = x;
		m_y = y;
		WorldWrapY(m_y);
	}

	m_foodX = m_foodY = 0;

	if (GetFoodWeAreOn(&m_foodX, &m_foodY, (int16_t)m_x, (int16_t)m_y))
	{
	}

}

void Ghost::Blit(bool color, bool bDying, Console* gameboy)
{
	if (m_x == 0) return;

	if (!player.PowerIsActiveDisplay() || millis()%100 > 50 || bDying)
		gameboy->drawBitmap((int16_t)m_x, (int16_t)m_y, ghost_bmp, PLAY_W,PLAY_H, color, ST7735_BLACK);
}

void Ghost::UnRender(Console* gameboy)
{
	if (m_x == 0) return;

	gameboy->drawBitmap((int16_t)m_x, (int16_t)m_y, solid_box, PLAY_W,PLAY_H, ST7735_WHITE, ST7735_BLACK);
	//put back any food pellets
	if (m_foodX != 0)
	{
		gameboy->drawPixel(m_foodX, m_foodY, 1);
	}
	
}

void Ghost::Render(bool bDying, Console* gameboy)
{
	Blit(1, bDying, gameboy);
}

void Ghost::OnReleasePellet(uint8_t foodX, uint8_t foodY)
{
	if (foodX == m_foodX && m_foodY == foodY)
	{
		m_foodX = m_foodY = 0;
	}
}

