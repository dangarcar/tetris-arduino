#include "Piece.h"
#include "Board.h"

Piece::Piece(int spriteIndex, const uint8_t schema[5][5]): block{spriteIndex, true, false} {
    for(int i=0; i<5; ++i){
        for(int j=0; j<5; ++j){
            this->schema[i][j] = schema[i][j];
        }
    }
}

void Piece::setPos(int x, int y){
    this->x = x;
    this->y = y;
}

template<typename T>
void swap(T& a, T& b){
    T c = a;
    a = b;
    b = c;
}

void Piece::rotate(){
    //Rotate around the main diagonal
    for(int i=0; i<5; ++i){
        for(int j=0; j<i; ++j){
            swap(schema[i][j], schema[j][i]);
        }
    }

    //Rotate both sides
    for(int i=0; i<5; ++i){
        for(int j=0; j*2<5; ++j){
            swap(schema[i][j], schema[i][5-j-1]);
        }
    }
}

void Piece::antiRotate(){
    //Rotate both sides
    for(int i=0; i<5; ++i){
        for(int j=0; j*2<5; ++j){
            swap(schema[i][j], schema[i][5-j-1]);
        }
    }

    //Rotate around the main diagonal
    for(int i=0; i<5; ++i){
        for(int j=0; j<i; ++j){
            swap(schema[i][j], schema[j][i]);
        }
    }
}

PieceSpawner::PieceSpawner(int x, int y): x{x}, y{y} {
    possible = 0;
    randomSeed((micros()+analogRead(A15))*micros());
    int i = getRandom();
    next = new Piece(i, PIECES_SCHEMA[i]);
    drawNext();
}

int PieceSpawner::getRandom() {
    if(possible == 0b01111111)
        possible = 0;

    int i =  -1;
    do {
        i = random(7);
    } while (possible & 1<<i);
    possible |= 1<<i;

    return i;
}

Piece* PieceSpawner::create(){
    int i = getRandom();    

    Piece* old = next;
    next = new Piece(i, PIECES_SCHEMA[i]);

    drawNext();

    return old;
}

extern MCUFRIEND_kbv tft;
void PieceSpawner::drawNext(){
    tft.fillRect(x, y, SMALL_SIZE*5, SMALL_SIZE*5, TFT_BLACK);

    tft.setTextSize(1);
    tft.setFont(&ARCADE_N6pt7b);
    tft.setCursor(x+12, y+8);
    tft.println(F("NEXT"));

    int x1 = INT16_MAX, x2 = INT16_MIN;
    int y1 = INT16_MAX, y2 = INT16_MIN;
    for(int i=0; i<5; i++){
        for(int j=0; j<5; j++){
            if(next->schema[i][j]){
                x1 = min(x1, j);
                x2 = max(x2, j);
                y1 = min(y1, i);
                y2 = max(y2, i);
            }
        }
    }
    int x0 = x + (5-x2+x1-1)*SMALL_SIZE/2;
    int y0 = y + (5-y2+y1-1)*SMALL_SIZE/2;

    for(int i=y1; i<5; i++){
        for(int j=x1; j<5; j++){
            if(next->schema[i][j]){
                tft.setAddrWindow(x0+(j-x1)*SMALL_SIZE, y0+(i-y1)*SMALL_SIZE, x0+(j-x1)*SMALL_SIZE+SMALL_SIZE-1, y0+(i-y1)*SMALL_SIZE+SMALL_SIZE-1);
                tft.pushColors(sprites_small[next->block.index], SMALL_SIZE*SMALL_SIZE, true);
            }
        }
    }
}