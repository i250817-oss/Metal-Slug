#pragma once
#include "../Projectile System/Projectile.h"
#include "../headers/headers.h"
#include <iostream>

class Weapon {
protected:
  float damage;
  float reloadTime;
  bool isPickedUp;
  char weaponName[32];

public:
  Weapon(float d, float r, const char *name)
      : damage(d), reloadTime(r), isPickedUp(false) {
    int i = 0;
    while (name[i] != '\0' && i < 31) {
      weaponName[i] = name[i];
      i++;
    }
    weaponName[i] = '\0';
  }

  virtual ~Weapon() {}

  float getDamage() const { return damage; }
  void setDamage(float v) {
    if (v >= 0)
      damage = v;
  }
  float getReloadTime() const { return reloadTime; }
  bool getIsPickedUp() const { return isPickedUp; }
  void setIsPickedUp(bool v) { isPickedUp = v; }
  const char *getWeaponName() const { return weaponName; }

  virtual void tickCooldown(float dt) {}
  virtual void resetFireTimer() {}
  virtual bool canFire() const { return true; }
  virtual void reload() {}
  virtual void applyAmmoMultiplier(float multiplier) { (void)multiplier; }
  virtual void replenishAmmo(int quantity) { (void)quantity; }

  virtual Projectile *fire(double posX, double posY, double angle,
                           bool isPlayerOwned) = 0;

  virtual bool isMelee() const { return false; }
  virtual bool getIsAttacking() const { return false; }
  virtual float getMeleeRange() const { return 0; }

  virtual void attackMelee(double posX, double posY, bool facingRight) {}
};