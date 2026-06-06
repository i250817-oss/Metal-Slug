#pragma once
#include "../headers/string.h"
#include <iostream>

using namespace std;




class ScoreCard {
protected:
    int score;
public:
    static ScoreCard* activeInstance;

    ScoreCard() : score(0) {}
    virtual ~ScoreCard() {}

    virtual void addEnemyKillScore(const char* enemyType, bool isAerial = false) = 0;
    virtual void addBossKillScore(int phase) = 0;
    virtual void addFeatScore(const char* featType, int extraData = 0) = 0;
    virtual void addLevelClearScore(const char* mode, bool flawless = false) = 0;

    virtual int getScore() const { return score; }
    virtual void setScore(int s) { score = s; }

    virtual ScoreCard& operator+=(int val) {
        score += val;
        return *this;
    }

    virtual void printResult() const = 0;

    static void reportEnemyKill(const char* tag, bool isAerial = false) {
        if (activeInstance) activeInstance->addEnemyKillScore(tag, isAerial);
    }
};