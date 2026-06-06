#pragma once
#include "EnemyBase.h"



//aerial enemy — aerial biome only, can fly
class AerialEnemy : public EnemyBase {
public:
    AerialEnemy(float x, float y) : EnemyBase(x, y, "aerial_enemy") {}
    bool   canFly()       const override { return true; }
    Enemy* clone()        const override { return new AerialEnemy(*this); }
};