#include <Arduino.h>
#include "audio-test.h"

#define BUZZER_PIN A5

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
}

void loop() {
  play_audio(BUZZER_PIN);
}