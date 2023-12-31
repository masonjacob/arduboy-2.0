#ifndef __CONSOLE__
#define __CONSOLE__

#include "Arduino.h"
#include "Button.h"
#include "Direction.h"
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <Fonts/FreeSerifBold18pt7b.h>
#include <SPI.h>
#include <SDFat.h>
#include <Adafruit_ImageReader.h> 
#include <anyrtttl.h>
#include <binrtttl.h>
#include <pitches.h>
#include "display-test.h"


// Define Joystick Pins
#define JS_X A6
#define JS_Y A7
#define JS_BUTTON 7

// Define A & B Button Pins
#define A_BUTTON   5
#define B_BUTTON   6

// Define TFT + MicroSD Card Breakout Board Pins
#define TFT_RST    8 
#define TFT_CS     9
#define CARD_CS    10
#define TFT_DC     14

// Define Audio Output Pin
#define AUDIO_OUT  A5

const char* frosty_the_snowman = "frosty:d=4,o=5,b=160:2g,e.,8f,g,2c6,8h,8c6,d6,c6,h,a,2g.,8h,8c6,d6,c6,h,8a,8a,g,c6,e,8g,8a,g,f,e,f,1g";

class Joystick {
    private:
        const uint8_t x_pin;
        const uint8_t y_pin;
        int range = 1024;
        int midpoint = range/2 - 1;
        int tolerance = 100;
    public:
        Button* button;

        Joystick(const uint8_t x_pin, const uint8_t y_pin, const uint8_t button_pin) 
        : x_pin(x_pin), y_pin(y_pin), button(new Button(button_pin)) {
        }
        void init() {
            pinMode(x_pin, INPUT);
            pinMode(y_pin, INPUT);
            button->begin();
        }
        int x_pos() {
            return analogRead(x_pin);
        }
        int y_pos() {
            return analogRead(y_pin);
        }
        JoyStick_Position current_direction() {
            // Store position to ensure a direction value is always returned
            int xpos = this->x_pos();
            int ypos = this->y_pos();

            //          ^
            //       +Y | 
            //          |
            //  +X <____|  

            if (xpos > midpoint + tolerance) {
                if (ypos > midpoint + tolerance) {
                    return JoyStick_Position::TopLeft;
                } else if (ypos > midpoint - tolerance) {
                    return JoyStick_Position::Left;
                } else {
                    return JoyStick_Position::BottomLeft;
                };
            } else if (xpos > midpoint - tolerance) {
                if (ypos > midpoint + tolerance) {
                    return JoyStick_Position::Top;
                } else if (ypos > midpoint - tolerance) {
                    return JoyStick_Position::Middle;
                } else {
                    return JoyStick_Position::Bottom;
                };
            } else {
                if (ypos > midpoint + tolerance) {
                    return JoyStick_Position::TopRight;
                } else if (ypos > midpoint - tolerance) {
                    return JoyStick_Position::Right;
                } else {
                    return JoyStick_Position::BottomRight;
                };
            }
        }
};

class Buzzer {
    private:
        const uint8_t buzzer_pin;
    public:
        Buzzer(const uint8_t buzzer_pin)
        : buzzer_pin(buzzer_pin) {
        }
        void init() {
            pinMode(buzzer_pin, OUTPUT);
        }
        void play(int* melody, int* durations, size_t durations_size) {
            int size = durations_size / sizeof(int);
            Serial.println(size);
            for (int note = 0; note < size; note++) {
            //to calculate the note duration, take one second divided by the note type.
            //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
            int duration = 1000 / durations[note];
            tone(buzzer_pin, melody[note], duration);

            //to distinguish the notes, set a minimum time between them.
            //the note's duration + 30% seems to work well:
            int pauseBetweenNotes = duration * 1.30;
            delay(pauseBetweenNotes);
            
            //stop the tone playing:
            noTone(buzzer_pin);
            }
        }
};

class Console {
    private: 
    public: 
        Adafruit_ST7735* display;
        SdFat* SD;
        Adafruit_ImageReader* reader;
        Joystick* joystick;
        Button* a_button;
        Button* b_button;       
        Buzzer* speaker;
        uint16_t test_background_color = ST77XX_BLACK;
        uint16_t clear_background_color = ST77XX_BLACK;
        uint16_t test_font_color = WHITE;


        //State
        JoyStick_Position joystick_position = Middle;
        bool j_button_state = false;
        bool a_button_state = false;
        bool b_button_state = false;
        bool image_state = false;
        bool music_state = false;

        Console() 
        :   display(new Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST)), 
            SD(new SdFat()),
            reader(new Adafruit_ImageReader(*SD)),
            joystick(new Joystick(JS_X, JS_Y, JS_BUTTON)), 
            a_button(new Button(A_BUTTON)),
            b_button(new Button(B_BUTTON)),
            speaker(new Buzzer(AUDIO_OUT))
        {
        }
        void init() {
            this->display->initR(INITR_BLACKTAB);
            this->display->fillScreen(this->clear_background_color);
            this->display->setRotation(3);
            this->joystick->init();
            this->speaker->init();
            this->a_button->begin();
            this->b_button->begin();
            this->joystick_position = this->joystick->current_direction();
            if(!SD->begin(CARD_CS, SD_SCK_MHZ(25))) { // ESP32 requires 25 MHz limit
                Serial.println(F("SD begin() failed"));
            for(;;); // Fatal error, do not continue
            }
        }
        void test_display() {
            testlines(this->display, GREEN);
        }
        void test_peripherals() {
            JoyStick_Position current_position = this->joystick->current_direction();
            bool j_button_pressed = this->joystick->button->read();
            bool a_button_pressed = this->a_button->read();
            bool b_button_pressed = this->b_button->read();
            bool previous_image_state = this->image_state;
            
            if (a_button_pressed && b_button_pressed && a_button_pressed != a_button_state && b_button_pressed != b_button_state) {
                image_state = !image_state;
            }

            if (image_state) {
                anyrtttl::nonblocking::play();
                if (image_state != previous_image_state) {
                    //this->music_state = true;
                    anyrtttl::nonblocking::begin(AUDIO_OUT, frosty_the_snowman);
                    this->drawImage("/frosty-the-snowman.bmp", 0, 0);
                }
            } else if (!image_state && current_position != this->joystick_position || j_button_pressed != this->j_button_state || a_button_pressed != this->a_button_state || b_button_pressed != this->b_button_state) {
                //this->music_state = false;
                this->joystick_position = current_position;
                this->j_button_state = j_button_pressed;
                this->a_button_state = a_button_pressed;
                this->b_button_state = b_button_pressed;
                char* dir_text = direction_text[current_position];
                String j_button_text = j_button_pressed ? "\nJoystick Button Pressed" : "";
                String a_button_text = a_button_pressed ? "\nA Button Pressed" : "";
                String b_button_text = b_button_pressed ? "\nB Button Pressed" : "";
                String print_text = dir_text + j_button_text + "\n" + a_button_text + b_button_text;
                testdrawtext(this->display, print_text, test_font_color, test_background_color);
            }
        }
        void clear() {
            this->display->fillScreen(clear_background_color);
        }
        void drawBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w,
                        int16_t h, uint16_t color, uint16_t bg) {
        this->display->drawBitmap(x, y, bitmap, w, h, color, bg);
        }
        void drawPixel(int16_t x, int16_t y, uint16_t color) {
            this->display->drawPixel(x, y, color);
        }
        uint8_t getInput(){
            
        }
        ImageReturnCode drawImage(const char *filename, int16_t x, int16_t y) {
            this->reader->drawBMP(filename, *(this->display), x, y);
        }
};

#endif //__CONSOLE__
