#pragma once
#include "../../Projectile System/EnergyBeam.h"
#include "FireArms.h"
#include "../../Manager/SoundManager.h"

// spawns a beam to end of screen. Instantly kills on impact.
class LaserGun : public FireArms {
private:
  int beamLength; // extends to screen edge

public:
  LaserGun() : FireArms(999, 0, 2, 30, "Laser Gun"), beamLength(1600) {}
  ~LaserGun() {}

  void setBeamLength(int v);
  int getBeamLength() const;

  Projectile *fire(double posX, double posY, double angle,
                   bool isPlayerOwned) override;
  void instantKill(double posX, double posY, double angle);
  void reload() override;
};

inline void LaserGun::setBeamLength(int v) { beamLength = v; }
inline int LaserGun::getBeamLength() const { return beamLength; }

inline Projectile *LaserGun::fire(double posX, double posY, double angle,
                                  bool isPlayerOwned) {
  if (!canFire() || bullets <= 0)
    return nullptr;
  fireTimer = 1 / fireRate;
  bullets--;
  SoundManager::playSound("sound/laser.ogg");
  return new EnergyBeam(posX, posY, angle, isPlayerOwned);
}

inline void LaserGun::instantKill(double posX, double posY, double angle) {}

inline void LaserGun::reload() { bullets = 30; }