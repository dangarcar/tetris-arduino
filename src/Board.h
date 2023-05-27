#ifndef BOARD_H
#define BOARD_H

#include <Arduino.h>
#include "Piece.h"
#include "Block.h"
#include "ScoreManager.h"
#include "ARCADE_N6pt7b.h"

#define RIGHT_BUTTON 18
#define LEFT_BUTTON 21
#define ROTATE_BUTTON 19
#define DROP_BUTTON 20
#define MUSIC_PIN 39

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20

#define BUTTON_DELAY 100

class Board {
private:
    PieceSpawner pieceSpawner {13*TILE_SIZE-TILE_SIZE/2, TILE_SIZE*2-10};
    Block board[BOARD_HEIGHT][BOARD_WIDTH];
    ScoreManager scoreManager {13*TILE_SIZE-2, TILE_SIZE*6-10};

    int x, y;

    bool isValidMovement();
    bool isValidLateralMovement();
    void eraseCurrentPiece();
public:
    Piece* currentPiece;
    static const uint8_t LEFT = 1 << 1, RIGHT = 1 << 2, ROTATE = 1 << 3, DROP = 1 << 4;

    Board(int x, int y);

    void draw();
    void drawBorder();
    bool update();
    bool handleControls();

    void rotate(); 
    void drop();

    unsigned long dt() const { return scoreManager.dt; }
    int score() const { return scoreManager.getScore(); }
    int level() const { return scoreManager.getLevel(); }
};

#endif //BOARD_H