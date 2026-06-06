#pragma once
#include "EnemyBase.h"



//m-15A Bradley — ground vehicle, placed in 2 fixed locations
class M15ABradley : public EnemyBase {
public:
    M15ABradley(float x, float y) : EnemyBase(x, y, "m15a_bradley") {}
    bool   isVehicle() const override { return true;  }
    Enemy* clone()     const override { return new M15ABradley(*this); }
};