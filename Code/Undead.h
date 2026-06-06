#pragma once
#include "EnemyBase.h"



//undead — plains biome only
class Undead : public EnemyBase {
public:
    Undead(float x, float y) : EnemyBase(x, y, "undead") {}
    Enemy* clone() const override { return new Undead(*this); }
};