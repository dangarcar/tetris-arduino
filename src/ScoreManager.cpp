#include "ScoreManager.h"
#include "Board.h"

extern MCUFRIEND_kbv tft;

const int LINES_TO_NEXT[] = {
    10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 100, 100, 100, 100, 100, 100, 110, 120, 130, 140, 150, 160, 170, 180, 190
};

const int LEVEL_DT[] = {
    800, 716, 633, 550, 466, 383, 300, 216, 133, 83, 83, 83, 83, 66, 66, 66, 66, 66, 50, 50, 50, 50, 16, 16, 16
};

ScoreManager::ScoreManager(int x, int y): x{x}, y{y} {
    dt = 1000;
    drawScorePanel();
}

void ScoreManager::incrementLine(int i){
    lines += i;
    linesToNext -= i;
    if(linesToNext <= 0){
        dt = LEVEL_DT[level];
        linesToNext = LINES_TO_NEXT[level];
        level++;
    }

    switch (i){
    case 2:
        score += 100 * (level + 1);
        break;
    case 3:
        score += 300 * (level + 1);
        break;
    case 4:
        score += 1200 * (level + 1);
        break;
    
    default:
        score += 40 * (level + 1);
        break;
    }

    drawScorePanel();
}

void ScoreManager::drawScorePanel() const {
    Serial.println(x);
    Serial.println(y);

    tft.fillRect(x, y, 80, 120, TFT_BLACK);

    tft.setTextSize(1);

    tft.setCursor(x, y);
    tft.println(F("SCORE"));
    tft.setCursor(x, y+15);
    tft.println(score);

    tft.setCursor(x, y+15+30);
    tft.println(F("LEVEL"));
    tft.setCursor(x, y+15+30+15);
    tft.println(level);

    tft.setCursor(x, y+15+30+15+30);
    tft.println(F("LINES"));
    tft.setCursor(x, y+15+30+15+30+15);
    tft.println(lines);
}