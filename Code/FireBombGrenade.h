#pragma once
#include "Explosives.h"
#include "../../Projectile System/FireBomb.h"



//eri's default grenade. Explodes on impact + spawns fire pool (3-block radius, 10 sec, 2 HP/sec).
class FireBombGrenade : public Explosives {
public:
    FireBombGrenade() : Explosives(0, 0, 1, 10, 3 * 64, "Fire Bomb") {}
    ~FireBombGrenade() {}

    Projectile* fire(double posX, double posY, double angle, bool isPlayerOwned) override {
        if (!canFire()) return nullptr;
        fireTimer = 1 / fireRate;
        ammo--;
        
        //throw speed 10. DPS damage handled by FireBomb projectile.
        return new FireBomb(posX, posY, angle, 10, isPlayerOwned);
    }
};