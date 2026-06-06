#pragma once
#include "EnemyBase.h"



//alien — aerial biome only, can fly
class Alien : public EnemyBase {
public:
    Alien(float x, float y) : EnemyBase(x, y, "alien") {}
    bool   canFly()  const override { return true; }
    Enemy* clone()   const override { return new Alien(*this); }
};