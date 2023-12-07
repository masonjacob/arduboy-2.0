#include "Utils.h"
#include "Fruit.h"
#include "Player.h"

extern Player player;

const uint16_t FRUIT_WIDTH =5;
const uint16_t FRUIT_HEIGHT =4;
const uint16_t FRUIT_X = 52;
const uint16_t FRUIT_Y = 44;

Fruit fruit;

uint8_t fruit_bmp[] =
{
	0x02, 0x05, 0x0b, 0x05, 0x02,
};

Fruit::Fruit()
{
	m_bActive = false;
	m_killTimer = 0;
}

Fruit::~Fruit()
{
}

void Fruit::ResetForNewLevel()
{
	m_bActive = false;
	m_killTimer= 0;
}

void Fruit::OnPlayerDie()
{
	m_bActive = false;

}
void Fruit::Update()
{
	if (m_bActive)
	{
		if (EntitiesAreTouching(FRUIT_X+1, FRUIT_Y, player.m_x, player.m_y, 2.0f))
		{
			m_bActive = false;
			player.AddScore(1000*g_level);
			return;
		}

		if (m_killTimer < millis())
		{
			m_bActive = false;
		}
	}

	
}

void Fruit::OnPelletEaten()
{
	if (player.m_pelletsEaten == 70 || player.m_pelletsEaten == 170)
	{
		m_bActive = true;
		m_killTimer = millis()+9000;
	}
}
void Fruit::Render(Console* gameboy)
{
	if (m_bActive)
		gameboy->drawBitmap(FRUIT_X, FRUIT_Y, fruit_bmp, FRUIT_WIDTH,FRUIT_HEIGHT, ST7735_GREEN, ST7735_BLACK);
}

void Fruit::UnRender(Console* gameboy)
{
	gameboy->drawBitmap(FRUIT_X, FRUIT_Y, fruit_bmp, FRUIT_WIDTH,FRUIT_HEIGHT, ST7735_BLACK, ST7735_BLACK);
}

void Fruit::RenderSideFruits(Console* gameboy)
{
	uint16_t x = 110;
	uint16_t y = 1;

	for (int i=0; i < g_level; i++)
	{
		gameboy->drawBitmap(x, y, fruit_bmp, FRUIT_WIDTH,FRUIT_HEIGHT, ST7735_GREEN, ST7735_BLACK);
		y += FRUIT_HEIGHT+1;
	}
}
