#pragma once
#include "../ProjectileWeapon.h"

class FireArms : public ProjectileWeapon {
protected:
  int bullets;
  bool isAvailableFlag;

public:
  FireArms(float dmg, float reloadTime, float fireRate, int bullets, const char *name)
      : ProjectileWeapon(dmg, reloadTime, fireRate, name), bullets(bullets),
        isAvailableFlag(true) {}
  virtual ~FireArms() {}

  int getBullets() const { return bullets; }
  void setBullets(int v) { bullets = v; }

  virtual Projectile *fire(double posX, double posY, double angle, bool isPlayerOwned) override = 0;
  virtual void reload() override = 0;

  bool canFire() const override {
    return isAvailableFlag && fireTimer <= 0 && bullets > 0;
  }

  void applyAmmoMultiplier(float multiplier) override {
    bullets = (int)(bullets * multiplier);
  }

  void replenishAmmo(int quantity) override { bullets += quantity; }
};