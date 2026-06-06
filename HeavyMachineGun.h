#pragma once
#include "FireArms.h"
#include "../../Projectile System/Bullet.h"
#include "../../Manager/SoundManager.h"

class HeavyMachineGun : public FireArms {
public:
    HeavyMachineGun() : FireArms(3, 0, 8, 200, "Heavy Machine Gun") {}
    ~HeavyMachineGun() {}

    Projectile* fire(double posX, double posY, double angle, bool isPlayerOwned) override {
        if (!canFire()) return nullptr;
        fireTimer = 1.0f / fireRate;
        bullets--;
        SoundManager::playSound("sound/heavy_machine_gun.ogg");
        return new Bullet(posX, posY, angle, 14, 3, isPlayerOwned);
    }

    void reload() override { bullets = 200; }
};