#pragma once
#include "../../Projectile System/FlameStream.h"
#include "FireArms.h"
#include "../../Manager/SoundManager.h"

class FlameShot : public FireArms {
private:
  int streamLength;
  float damagePerSecond;
  bool keyHeld;

public:
  FlameShot()
      : FireArms(2, 0, 12, 200, "Flame Shot"), streamLength(5),
        damagePerSecond(2), keyHeld(false) {}
  ~FlameShot() {}

  void setStreamLength(int v) { streamLength = v; }
  int getStreamLength() const { return streamLength; }
  float getDamagePerSecond() const { return damagePerSecond; }
  void setKeyHeld(bool v) { keyHeld = v; }
  bool getKeyHeld() const { return keyHeld; }

  Projectile *fire(double posX, double posY, double angle, bool isPlayerOwned) override {
    if (!canFire() || bullets <= 0) return nullptr;
    fireTimer = 1.0f / fireRate;
    bullets--;
    SoundManager::playSound("sound/flame_shot.ogg");
    return new FlameStream(posX, posY, angle, isPlayerOwned);
  }

  void streamDamage(float dt) { (void)dt; }

  void reload() override { bullets = 200; }
};