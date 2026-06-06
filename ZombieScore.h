#pragma once
#include "EnemyScore.h"


class ZombieScore : public EnemyScore {
public:
    void addEnemyKillScore(const char* t, bool a) override { score = 100; }
};