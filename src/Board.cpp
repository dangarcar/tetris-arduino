#include "Board.h"
#include "Piece.h"

volatile uint8_t flags = 0;

Board::Board(int x, int y): x{x}, y{y} {
    currentPiece = pieceSpawner.create();
    currentPiece->setPos((BOARD_WIDTH-5)/2, -2);
    drawBorder();
}

void Board::draw() {
    for(int i=0; i<BOARD_HEIGHT; ++i){
        for(int j=0; j<BOARD_WIDTH; ++j){
            if(board[i][j].falling){
                board[i][j].draw(x + j*TILE_SIZE, y + i*TILE_SIZE);
                board[i][j].falling = false;
            }
        }
    }
}

void Board::drawBorder() {
    Block greyBlock {8};
    for(int i=0; i<BOARD_HEIGHT; i++){
        greyBlock.draw(x-TILE_SIZE, y+i*TILE_SIZE);
    }
    for(int i=0; i<BOARD_HEIGHT; i++){
        greyBlock.draw(x+BOARD_WIDTH*TILE_SIZE, y+i*TILE_SIZE);
    }
    for(int i=0; i<BOARD_WIDTH+2; i++){
        greyBlock.draw(x-TILE_SIZE+i*TILE_SIZE, y+BOARD_HEIGHT*TILE_SIZE);
    }

    for(int i=0; i<BOARD_HEIGHT; ++i){
        for(int j=0; j<BOARD_WIDTH; ++j){
            board[i][j].draw(x + j*TILE_SIZE, y + i*TILE_SIZE);
        }
    }
}

bool Board::update(){
    eraseCurrentPiece();

    //Move down
    currentPiece->setPos(currentPiece->x, currentPiece->y + 1);

    bool valid = isValidMovement();
    if(!valid){
        currentPiece->setPos(currentPiece->x, currentPiece->y - 1);
    }

    for(int i=0; i<5; i++){
        if(currentPiece->y+i < 0) 
            continue;
        for(int j=0; j<5; j++){
            if(currentPiece->schema[i][j]){
                board[currentPiece->y+i][currentPiece->x+j] = currentPiece->block;
                board[currentPiece->y+i][currentPiece->x+j].falling = valid;
            }
        }
    }

    //Check for lines to remove
    int lines = 0;
    for(int y=0; y<BOARD_HEIGHT; y++){
        bool lineCompleted = true;
        for(int x=0; x<BOARD_WIDTH; x++){
            lineCompleted &= board[y][x].isBlock();
        }
        if(lineCompleted){
            lines++;
            for(int i=y; i>0; i--){
                for(int j=0; j<BOARD_WIDTH; j++){
                    if(board[i][j] == board[i-1][j])
                        continue;
                    board[i][j] = board[i-1][j];
                    board[i][j].falling = true;
                }
            }
            for(int j=0; j<BOARD_WIDTH; j++){
                board[0][j] = FALLING_EMPTY;
            }
        }
    }
    if(lines) 
        scoreManager.incrementLine(lines);

    if(!valid) {
        delete currentPiece;
        currentPiece = pieceSpawner.create();
        currentPiece->setPos((BOARD_WIDTH-5)/2, -2);
        return isValidMovement();
    }

    return true;
}

bool Board::handleControls(){
    eraseCurrentPiece();
    
    bool done = flags;

    if(flags & Board::RIGHT){
        currentPiece->setPos(currentPiece->x + 1, currentPiece->y);
        if(!isValidLateralMovement())
            currentPiece->setPos(currentPiece->x - 1, currentPiece->y);
    }
    else if(flags & Board::LEFT){
        currentPiece->setPos(currentPiece->x - 1, currentPiece->y);
        if(!isValidLateralMovement())
            currentPiece->setPos(currentPiece->x + 1, currentPiece->y);
    }
    else if(flags & Board::ROTATE){
        currentPiece->rotate();
        if(!isValidMovement() || !isValidLateralMovement())
            currentPiece->antiRotate();
    }
    else if(digitalRead(DROP_BUTTON) == LOW){
        currentPiece->setPos(currentPiece->x, currentPiece->y+1);
        if(!isValidMovement())
            currentPiece->setPos(currentPiece->x, currentPiece->y-1);
    }

    for(int i=0; i<5; i++){
        if(currentPiece->y+i < 0) 
            continue;
        for(int j=0; j<5; j++){
            if(currentPiece->schema[i][j]){
                board[currentPiece->y+i][currentPiece->x+j] = currentPiece->block;
            }
        }
    }

    flags = 0;
    return done;
}

void Board::eraseCurrentPiece() {
    //Set current block to empty
    for(int i=0; i<5; i++){
        if(currentPiece->y+i < 0) 
            continue;
        for(int j=0; j<5; j++){
            if(currentPiece->schema[i][j]){
                board[currentPiece->y+i][currentPiece->x+j] = FALLING_EMPTY;
            }
        }
    }
}

bool Board::isValidMovement(){
    for(int i=0; i<5; i++){
        if(currentPiece->y+i < 0) 
            continue;
        for(int j=0; j<5; j++){
            if(currentPiece->schema[i][j]){
                if(currentPiece->y+i >= BOARD_HEIGHT){
                    return false;
                }
                else if (board[currentPiece->y+i][currentPiece->x+j].isBlock()){
                    return false;
                }
            }
        }
    }

    return true;
}

bool Board::isValidLateralMovement(){
    for(int i=0; i<5; i++){
        if(currentPiece->y+i < 0)
            continue;
        for(int j=0; j<5; j++){
            if(currentPiece->schema[i][j]){
                if(currentPiece->x+j < 0){
                    return false;
                }
                if(currentPiece->x+j >= BOARD_WIDTH){
                    return false;
                }
                else if (board[currentPiece->y+i][currentPiece->x+j].isBlock()){
                    return false;
                }
            }
        }
    }

    return true;
}