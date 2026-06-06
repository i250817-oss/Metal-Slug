#pragma once
#include "EnemyScore.h"


class MartianScore : public EnemyScore {
public:
    void addEnemyKillScore(const char* t, bool a) override { score = 100; }
};