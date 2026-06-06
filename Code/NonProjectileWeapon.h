#pragma once
#include "Weapon.h"
#include "../Manager/SoundManager.h"

class NonProjectileWeapon : public Weapon {
protected:
  float meleeRange; 
  float meleeCooldown;
  float cooldownTimer;
  bool onCooldown;
  bool isAttacking;
  float attackTimer;

public:
  NonProjectileWeapon(float dmg, float range, float cooldown, const char *name)
      : Weapon(dmg, 0, name), meleeRange(range), meleeCooldown(cooldown),
        cooldownTimer(0), onCooldown(false), isAttacking(false),
        attackTimer(0) {}

  virtual ~NonProjectileWeapon() {}

  void setMeleeRange(float v) { meleeRange = v; }
  float getMeleeRange() const override { return meleeRange; }
  void setMeleeCooldown(float v) { meleeCooldown = v; }
  float getMeleeCooldown() const { return meleeCooldown; }
  bool getIsOnCooldown() const { return onCooldown; }
  bool getIsAttacking() const override { return isAttacking; }
  bool isMelee() const override { return true; }

  void tickCooldown(float dt) override {
    if (cooldownTimer > 0) {
      cooldownTimer -= dt;
      if (cooldownTimer <= 0) {
        cooldownTimer = 0;
        onCooldown = false;
      }
    }
    if (isAttacking) {
      attackTimer -= dt;
      if (attackTimer <= 0)
        isAttacking = false;
    }
  }

  virtual void attackMelee(double posX, double posY, bool facingRight) override {
    if (onCooldown) return;
    isAttacking = true;
    attackTimer = 0.1f;
    onCooldown = true;
    cooldownTimer = meleeCooldown;
    SoundManager::playSound("sound/knife_melee.ogg");
  }

  virtual void attack(double posX, double posY, bool facingRight) {
    attackMelee(posX, posY, facingRight);
  }

  Projectile *fire(double posX, double posY, double angle, bool isPlayerOwned) override {
    return nullptr; 
  }

  void reload() override {}
  bool canFire() const override { return !onCooldown; }
};