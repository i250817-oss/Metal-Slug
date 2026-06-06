#pragma once
#include "EnemyScore.h"


class ShieldedScore : public EnemyScore {
public:
    void addEnemyKillScore(const char* t, bool a) override { score = 100; }
};