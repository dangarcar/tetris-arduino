#include <Arduino.h>
#include <MCUFRIEND_kbv.h>
#include "Piece.h"
#include "Board.h"

MCUFRIEND_kbv tft;
extern volatile uint8_t flags;

#define TFT_WIDTH 320
#define TFT_HEIGHT 480

const uint8_t logo[5][23] PROGMEM = {
    1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,
    0,1,0,0,1,0,0,0,0,1,0,0,1,0,1,0,0,1,0,0,1,0,0,
    0,1,0,0,1,1,1,0,0,1,0,0,1,1,1,0,0,1,0,0,1,1,1,
    0,1,0,0,1,0,0,0,0,1,0,0,1,1,0,0,0,1,0,0,0,0,1,
    0,1,0,0,1,1,1,0,0,1,0,0,1,0,1,0,1,1,1,0,1,1,1,
};

void setup(){
    Serial.begin(9600);
    tft.begin(tft.readID());
    tft.setTextColor(TFT_WHITE);
    tft.setFont(&ARCADE_N6pt7b);

    pinMode(RIGHT_BUTTON, INPUT_PULLUP);
    pinMode(LEFT_BUTTON, INPUT_PULLUP);
    pinMode(ROTATE_BUTTON, INPUT_PULLUP);
    pinMode(DROP_BUTTON, INPUT_PULLUP);
    pinMode(MUSIC_PIN, OUTPUT);

    attachInterrupt(digitalPinToInterrupt(RIGHT_BUTTON), [](){  flags |= Board::RIGHT;  }, FALLING);
    attachInterrupt(digitalPinToInterrupt(LEFT_BUTTON), [](){   flags |= Board::LEFT;   }, FALLING);
    attachInterrupt(digitalPinToInterrupt(ROTATE_BUTTON), [](){ flags |= Board::ROTATE; }, FALLING);
    attachInterrupt(digitalPinToInterrupt(DROP_BUTTON), [](){   flags |= Board::DROP;   }, FALLING);
}

inline uint16_t getCenteredX(const char* str){
    int16_t x1, y1;
    uint16_t w,h;
    tft.getTextBounds(str, 0, 0, &x1, &y1, &w, &h);
    return (TFT_WIDTH-w)/2;
}

void startScreen(){
    tft.fillScreen(TFT_BLACK);

    int x = 0, y = 150;
    for(int i=0; i<5; i++){
        for(int j=0; j<23; j++){
            if(pgm_read_byte(&(logo[i][j]))){
                tft.setAddrWindow(x+j*SMALL_SIZE, y+i*SMALL_SIZE, x+j*SMALL_SIZE+SMALL_SIZE-1, y+i*SMALL_SIZE+SMALL_SIZE-1);
                tft.pushColors(sprites_small[random(7)], SMALL_SIZE*SMALL_SIZE, true);
            }
        }
    }

    tft.setTextSize(1);
    tft.setCursor(17, 300);
    tft.println(F("Press any button to play"));
}

void endScreen(const Board& b){
    tft.fillScreen(TFT_BLACK);
    tft.setTextSize(3);
    tft.setCursor(-1, 100);
    tft.println(F("GAME OVER"));
    tft.setTextSize(1);
    tft.setCursor(10, 200);
    tft.println(F("Press any button to reset"));


    char str[64];
    snprintf_P(str, 64, PSTR("SCORE:%d"), b.score());
    tft.setTextSize(2);
    tft.setCursor(getCenteredX((const char*)str), 300);
    tft.println(str);

    memset(str, 0, 64);
    snprintf_P(str, 64, PSTR("LEVEL:%d"), b.level());
    tft.setCursor(getCenteredX((const char*)str), 330);
    tft.println(str);
}

void loop(){
    flags = 0;
    startScreen();
    while(!flags) 
        delay(BUTTON_DELAY);

    flags = 0;
    tft.fillScreen(TFT_BLACK);
    digitalWrite(MUSIC_PIN, HIGH);
    unsigned long t,t1;
    Board b(3*TILE_SIZE/2, TILE_SIZE/2);

    while(1){
        if(millis()-t1 > BUTTON_DELAY){
            t1 = millis();
            if(b.handleControls())
                t = millis();
        }
        b.draw();

        if(millis()-t > b.dt()){
            t = millis();
            if(!b.update()){
                delay(500);
                digitalWrite(MUSIC_PIN, LOW);
                endScreen(b);
                flags = 0;
                delay(1000);
                while(!flags) 
                    delay(BUTTON_DELAY);;
                return;
            };
        }
    }
}