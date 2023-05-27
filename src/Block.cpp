#include "Block.h"
#include <MCUFRIEND_kbv.h>

extern MCUFRIEND_kbv tft;

void Block::draw(int x, int y) const {
    tft.setAddrWindow(x, y, x+TILE_SIZE-1, y+TILE_SIZE-1);
    tft.pushColors(sprites[index], TILE_SIZE*TILE_SIZE, true);
}

bool operator==(const Block& a, const Block& b){
    return (a.empty && b.empty) || (a.index == b.index);
}