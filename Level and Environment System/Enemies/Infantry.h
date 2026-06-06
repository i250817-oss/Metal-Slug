#pragma once
#include "EnemyBase.h"



//standard ground infantry — spawns in all 3 biomes
class Infantry : public EnemyBase {
public:
    Infantry(float x, float y) : EnemyBase(x, y, "infantry") {}
    Enemy* clone() const override { return new Infantry(*this); }
};