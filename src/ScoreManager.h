#ifndef SCORE_MANAGER_H
#define SCORE_MANAGER_H

#include <Arduino.h>

class ScoreManager{
private:
    int x, y;

    uint32_t score {0};
    uint32_t lines {0};
    uint32_t level{0};
    int linesToNext{0};
public:
    unsigned long dt;

    ScoreManager(int x, int y);

    void drawScorePanel() const;

    void incrementLine(int i);

    uint32_t getScore() const { return score; }
    uint32_t getLevel() const { return level; }
};

#endif //SCORE_MANAGER_H