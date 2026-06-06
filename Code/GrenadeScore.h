#pragma once
#include "EnemyScore.h"


class GrenadeScore : public EnemyScore {
public:
    void addEnemyKillScore(const char* t, bool a) override { score = 100; }
};