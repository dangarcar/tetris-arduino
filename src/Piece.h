#ifndef PIECE_H
#define PIECE_H

#include <Arduino.h>
#include <MCUFRIEND_kbv.h>
#include "Block.h"

#define SMALL_SIZE 14

class Piece {
public:
    int x, y;
    const Block block;
    uint8_t schema[5][5];

    Piece() = default;
    Piece(int i, const uint8_t schema[5][5]);

    void rotate();
    void antiRotate();

    void setPos(int x, int y);
};

class PieceSpawner{
private:
    uint8_t possible;
    Piece* next;

    int x, y;
    int getRandom();
public:
    PieceSpawner(int x, int y);
    Piece* create();
    void drawNext();
};

const uint8_t PIECES_SCHEMA[7][5][5] = {
    { // I
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0,
        0, 1, 1, 1, 1, 
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0
    },
    { //J
        0, 0, 0, 0, 0,
        0, 1, 0, 0, 0,
        0, 1, 1, 1, 0, 
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0
    },
    { //L
        0, 0, 0, 0, 0,
        0, 0, 0, 1, 0,
        0, 1, 1, 1, 0, 
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0
    },
    { //O
        0, 0, 0, 0, 0,
        0, 0, 1, 1, 0,
        0, 0, 1, 1, 0, 
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0
    },
    { //S
        0, 0, 0, 0, 0,
        0, 0, 1, 1, 0,
        0, 1, 1, 0, 0, 
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0
    },
    { //Z
        0, 0, 0, 0, 0,
        0, 1, 1, 0, 0,
        0, 0, 1, 1, 0, 
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0
    },
    { //T
        0, 0, 0, 0, 0,
        0, 0, 1, 0, 0,
        0, 1, 1, 1, 0, 
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0
    }
};

const Piece pieces[] = {
    Piece(0, PIECES_SCHEMA[0]), //I
    Piece(1, PIECES_SCHEMA[1]), //J
    Piece(2, PIECES_SCHEMA[2]), //L
    Piece(3, PIECES_SCHEMA[3]), //O
    Piece(4, PIECES_SCHEMA[4]), //S
    Piece(5, PIECES_SCHEMA[5]), //Z
    Piece(6, PIECES_SCHEMA[6]), //T
};

#endif //PIECE_H