#pragma once
#include "Explosives.h"
#include "../../Projectile System/Grenade.h"



//default grenade. Ballistic arc throw. 5 HP in blast radius of 3 blocks.
class HandGrenade : public Explosives {
public:
    HandGrenade() : Explosives(5, 0, 1, 10, 3 * 64, "Hand Grenade") {}
    ~HandGrenade() {}

    Projectile* fire(double posX, double posY, double angle, bool isPlayerOwned) override {
        if (!canFire()) return nullptr;
        fireTimer = 1 / fireRate;
        ammo--;
        
        //throw speed 10, Damage 5.
        return new Grenade(posX, posY, angle, 10, 5, isPlayerOwned);
    }
};