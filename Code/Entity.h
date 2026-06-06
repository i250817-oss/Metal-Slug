#pragma once
#include "../Sprite System/MainSprite.h"
#include "../headers/headers.h"

using namespace sf;

const int ENTITY_GENERIC = 0;
const int ENTITY_PLAYER = 1;
const int ENTITY_ENEMY = 2;
const int ENTITY_VEHICLE = 3;
const int ENTITY_PROJECTILE = 4;
const int ENTITY_COLLECTIBLE = 5;
const int ENTITY_BOSS = 6;

const int FACTION_PLAYER = 0;
const int FACTION_ENEMY = 1;
const int FACTION_NEUTRAL = 2;

class Level;
class Camera;
class PlayerSoldier;
class DamagableEntity;
class Projectile;
class Enemy;
class Vehicle;
class Collectible;
class Soldier;

class Entity {
protected:
  double posX;
  double posY;
  MainSprite *sprite;
  Entity *pendingShot;
  DamagableEntity *target;
  int entityType;

public:
  Entity() : posX(0), posY(0), sprite(nullptr), pendingShot(nullptr), target(nullptr), entityType(ENTITY_GENERIC) {}
  Entity(double posX, double posY)
      : posX(posX), posY(posY), sprite(nullptr), pendingShot(nullptr),
        target(nullptr), entityType(ENTITY_GENERIC) {}

  virtual ~Entity() {
    delete sprite;
    sprite = nullptr;
  }

  int getEntityType() const { return entityType; }

  virtual PlayerSoldier *getAsPlayerSoldier() { return nullptr; }
  virtual Vehicle *getAsVehicle() { return nullptr; }

  void setPosX(double v) { posX = v; }
  double getPosX() const { return posX; }
  void setPosY(double v) { posY = v; }
  double getPosY() const { return posY; }

  MainSprite *getSprite() const { return sprite; }
  void setSprite(MainSprite *s) { sprite = s; }

  virtual float getWidth() const { return 0; }
  virtual float getHeight() const { return 0; }

  virtual bool getIsAlive() const { return true; }
  virtual bool getIsDying() const { return false; }

  virtual void onCollideWith(Entity *other, float dt) = 0;

  virtual void resolveCollision(Projectile *p, float dt) {}
  virtual void resolveCollision(PlayerSoldier *p, float dt) {}
  virtual void resolveCollision(Enemy *e, float dt) {}
  virtual void resolveCollision(Vehicle *v, float dt) {}
  virtual void resolveCollision(Collectible *c, float dt) {}
  virtual void resolveCollision(Soldier *s, float dt) {}
  virtual void resolveCollision(DamagableEntity *d, float dt) {}
  virtual void resolveCollision(class POWPrisoner *pow, float dt) {}

  virtual void resolveMeleeAttack(Entity *attacker, float damage) {}

  virtual bool isEnemy() const { return false; }
  virtual bool isPlayer() const { return false; }
  virtual DamagableEntity *asTarget() { return nullptr; }
  virtual bool isAerialKill() const { return false; }
  virtual bool isHostileTarget() const { return true; }
  virtual const char *getEnemyTag() const { return "generic"; }

  virtual int getFaction() const { return FACTION_NEUTRAL; }

  virtual float getDamageValue() const { return 0; }
  virtual float getDamagePerSecond() const { return 0; }
  virtual bool getIsPlayerOwned() const { return false; }
  virtual bool isExplosiveProjectile() const { return false; }

  virtual Entity *getPendingShot() { return pendingShot; }
  virtual void clearPendingShot() { pendingShot = nullptr; }

  virtual Entity *getPendingSpawnEntity() { return nullptr; }
  virtual void clearPendingSpawnEntity() {}

  virtual void handleDeath(Level *level) { (void)level; }

  virtual void update(Level *level, float dt) = 0;
  virtual void draw(RenderWindow &window) = 0;

  virtual void setTarget(DamagableEntity *t) { target = t; }
  DamagableEntity *getTarget() const { return target; }
};