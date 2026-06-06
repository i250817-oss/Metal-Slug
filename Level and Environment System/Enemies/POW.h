#pragma once
#include "EnemyBase.h"



//POW prisoner — one per biome, must be rescued
class POW : public EnemyBase {
public:
    POW(float x, float y) : EnemyBase(x, y, "pow") {}
    Enemy* clone() const override { return new POW(*this); }
};