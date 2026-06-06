#pragma once
#include "EnemyBase.h"



//flying Tara — can fly, spawns across the full level
class FlyingTara : public EnemyBase {
public:
    FlyingTara(float x, float y) : EnemyBase(x, y, "flying_tara") {}
    bool   canFly()  const override { return true; }
    Enemy* clone()   const override { return new FlyingTara(*this); }
};