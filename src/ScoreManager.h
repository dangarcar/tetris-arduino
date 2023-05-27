#ifndef SCORE_MANAGER_H
#define SCORE_MANAGER_H

class ScoreManager{
private:
    int x, y;

    int score {0};
    int lines {0};
    int level {0};
    int linesToNext {0};
public:
    unsigned long dt;

    ScoreManager(int x, int y);

    void drawScorePanel() const;

    void incrementLine(int i);

    int getScore() const { return score; }
    int getLevel() const { return level; }
};

#endif //SCORE_MANAGER_H