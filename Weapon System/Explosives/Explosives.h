#pragma once
#include "../ProjectileWeapon.h"

// base for explosive weapons like grenades
class Explosives : public ProjectileWeapon {
protected:
  int ammo;
  float blastRadius;

public:
  Explosives(float dmg, float reloadTime, float fireRate, int ammo,
             float blastRadius, const char *name)
      : ProjectileWeapon(dmg, reloadTime, fireRate, name), ammo(ammo),
        blastRadius(blastRadius) {}
  virtual ~Explosives() {}

  virtual void reload() override { ammo = 10; }

  bool canFire() const override { return fireTimer <= 0 && ammo > 0; }
};