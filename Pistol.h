#pragma once
#include "../ProjectileWeapon.h"
#include "../../Projectile System/Bullet.h"
#include "../../Manager/SoundManager.h"
#include <cmath>

class Pistol : public ProjectileWeapon {
public:
    Pistol() : ProjectileWeapon(3, 0, 4, "Pistol") {}
    ~Pistol() {}

    Projectile* fire(double posX, double posY, double angle, bool isPlayerOwned) override {
        if (!canFire()) return nullptr;
        fireTimer = 1.0f / fireRate;
        SoundManager::playSound("sound/pistol.ogg");
        return new Bullet(posX, posY, angle, 12, 3, isPlayerOwned);
    }

    void reload() override {}
};