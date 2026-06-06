#pragma once
#include "EnemyScore.h"



class RebelScore : public EnemyScore {
public:
    void addEnemyKillScore(const char* tag, bool isAerial) override {
        score = isAerial ? 150 : 100;
    }
};