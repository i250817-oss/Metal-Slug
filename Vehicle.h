#pragma once
#include "../Entity Root/DamagableEntity.h"
#include "../headers/string.h"

using namespace sf;
using namespace std;

class PlayerSoldier;
class Projectile;
class Enemy;
class Level;

// abstract base for all vehicles.
class Vehicle : virtual public DamagableEntity {
private:
  char biomeName[16];
  double speedX;
  double speedY;
  float vulnerability;
  float speedFactor;

protected:
  float fireCooldown;
  float renderScaleX;
  float renderScaleY;
  Texture vehicleTexture;
  Sprite vehicleSprite;

public:
  Vehicle() : Entity(), DamagableEntity(0,0,0), speedX(0.0), speedY(0.0), vulnerability(1), speedFactor(1), fireCooldown(0), renderScaleX(1.0f), renderScaleY(1.0f) {
      biomeName[0] = '\0';
  }
  // specialized behavior hooks
  virtual bool getIsOccupied() const { return false; }
  virtual PlayerSoldier *getDriver() const { return nullptr; }
  virtual void setDriver(PlayerSoldier *p) { (void)p; }

  Vehicle(double posX, double posY, int maxHp, const char *biome)
      : Entity(posX, posY), DamagableEntity(posX, posY, maxHp), speedX(0.0), speedY(0.0),
        vulnerability(1), speedFactor(1), fireCooldown(0), renderScaleX(1.0f), renderScaleY(1.0f) {
    this->posX = posX;
    this->posY = posY;
    this->hp = maxHp;
    this->maxHp = maxHp;
    copyStringN(biomeName, biome, 16);
  }

  void resolveCollision(PlayerSoldier *p, float dt) override;

  void resolveCollision(Projectile *p, float dt) override;

  virtual ~Vehicle() {}

  void setBiomeName(const char *v) { copyStringN(biomeName, v, 16); }
  const char *getBiomeName() const { return biomeName; }
  virtual bool isPlayer() const override = 0;
  virtual bool isEnemy() const override = 0;
  Vehicle *getAsVehicle() override { return this; }
  int getFaction() const override {
    return isPlayer() ? FACTION_PLAYER : FACTION_ENEMY;
  }

  const char *getEnemyTag() const override {
    return isEnemy() ? "vehicle" : "generic";
  }

  void setSpeedX(double v) { speedX = v; }
  double getSpeedX() const { return speedX; }
  void setSpeedY(double v) { speedY = v; }
  double getSpeedY() const { return speedY; }

  float getVulnerability() const { return vulnerability; }

  float getEffectiveFireRateMultiplier() const;

  virtual void fire() {}
  virtual void move() {}

  void takeDamage(int val) override;
  void die() override;

  virtual void loadAnimation(int type) = 0;

  void update(Level *level, float dt) override;
  void draw(RenderWindow &window) override;
  void onCollideWith(Entity *other, float dt) override;
};
