#define TEST

#if defined(TEST)
#include <Arduino.h>
#include "Console.h"
#include "Sounds.h"
#include "display-test.h"

Console* gameboy = new Console();

void setup() {
  Serial.begin(9600);
  gameboy->init();
  //gameboy->test_display();
  //setupDisplay(gameboy->display);	
  //gameboy->speaker->play(SW_melody, SW_durations, sizeof(SW_durations));
}

void loop() {
  gameboy->test_peripherals();
//   Serial.println(gameboy->music_state);
//   if (gameboy->music_state) {
// 	anyrtttl::nonblocking::play();
//   } else {
// 	if (anyrtttl::nonblocking::isPlaying()) {
// 		anyrtttl::nonblocking::stop();
// 	}
//   }
  delay(50);
  
  //displayLoop(gameboy->display);
  
}
#elif defined(PACMAN)
#include <Arduino.h>

#include "level_bitmaps.h"
#include "Player.h"
#include "Ghost.h"
#include "Fruit.h"

#include "Console.h"
#include "Sounds.h"

#define WIDTH 160
#define HEIGHT 128

#define EEPROM_HIGH_SCORE_SAVE_SLOT 8







enum GAME_MODE
{
	MODE_NONE,
	MODE_MENU,
	MODE_START,
	MODE_PLAYING,
	MODE_DYING,
	MODE_HIGHSCORES

};

Player player;
uint8_t g_level=1;

GAME_MODE g_mode = MODE_MENU;

Console* gameboy = new Console();

void DrawScore()
{
	// text.setCursor(40,6);
	// text.print("Score:");
	// printNumberFancy(55, 16, player.m_score, true, false);
}

void StartGame()
{
	ClearAndRedrawLevel();
	ResetGhostPositions(true);
	g_mode = GAME_MODE::MODE_PLAYING;
  // arduboy.display();
}

void ClearAndRedrawLevel()
{
	gameboy->clear();
	gameboy->drawBitmap(0, 0, level, WIDTH, HEIGHT, ST7735_WHITE, ST7735_BLACK);
	player.Render();
	fruit.RenderSideFruits(gameboy);

}

void setup()
{

	gameboy->init();
	
}

void PlayingGame()
{
	player.UnRender();
	fruit.UnRender(gameboy);

	for (int i=0; i < GHOST_COUNT; i++)
	{
		ghosts[i].UnRender(gameboy);
	}

	player.Update();
	fruit.Update();

	for (int i=0; i < GHOST_COUNT; i++)
	{
		ghosts[i].Update(gameboy);
	}

	player.Render();
	fruit.Render(gameboy);

	for (int i=0; i < GHOST_COUNT; i++)
	{
		ghosts[i].Render(false, gameboy);
	}

	player.OnEndOfLoop();
	// arduboy.display();

	if (player.LevelPassed())
	{
		g_level++;
		delay(3500);
		player.ResetLevel();
		StartGame();
	}
}
int8_t g_selection = 0;


void loop()
{
  // pause render until it's time for the next frame
//  if (!(arduboy.nextFrame()))
//     return;

 switch(g_mode)
 {
 case MODE_START:
	StartGame();
	 break;
 case MODE_PLAYING:
	 PlayingGame();
	 break;
}

#endif