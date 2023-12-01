#include <Arduino.h>
#include "Console.h"
#include "Sounds.h"
#include "display-test.h"


#define TFT_RST    8 
#define TFT_CS     9
#define CARD_CS    10
#define TFT_DC     14

//Adafruit_ST7735 *tft = new Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST);

Console* gameboy = new Console();

void setup() {
  Serial.begin(9600);
  gameboy->init();
  //gameboy->test_display();
  //setupDisplay(gameboy->display);
  gameboy->speaker->play(SW_melody, SW_durations, sizeof(SW_durations));
}

void loop() {
  gameboy->test_peripherals();
  delay(100);

  //displayLoop(gameboy->display);
  
}