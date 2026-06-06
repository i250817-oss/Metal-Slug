#pragma once
#include "../Entity Root/DamagableEntity.h"
#include "EnemyAIState.h"
#include <iostream>

class Collectible;

class Enemy : virtual public DamagableEntity {
protected:
  Collectible *pendingDrop;
  const char *enemyTag;

  static const int MAX_STATES = 8;
  EnemyAIState *states[MAX_STATES];
  EnemyAIState *currentAIState;

  float walkSpeed;
  float runSpeed;
  bool crouching;
  bool shieldActive;
  bool wallAhead;

  // firing animation gate: true only for the duration of the shot anim
  bool  isFiringAnim;
  float firingAnimTimer;
  bool deathSoundPlayed;

public:
  void playDeathSound();
  Enemy() : Entity(), DamagableEntity(0,0,0), pendingDrop(nullptr), enemyTag("generic"), currentAIState(nullptr), walkSpeed(2.0f), runSpeed(4.0f), crouching(false), shieldActive(false), wallAhead(false), isFiringAnim(false), firingAnimTimer(0), deathSoundPlayed(false) {
    for (int i = 0; i < MAX_STATES; i++) states[i] = nullptr;
  }
  Enemy(double posX, double posY, int maxHp)
      : Entity(posX, posY), DamagableEntity(posX, posY, maxHp), pendingDrop(nullptr),
        enemyTag("generic"), currentAIState(nullptr), walkSpeed(2.0f),
        runSpeed(4.0f), crouching(false), shieldActive(false),
        wallAhead(false), isFiringAnim(false), firingAnimTimer(0), deathSoundPlayed(false) {
    this->posX = posX;
    this->posY = posY;
    this->hp = maxHp;
    this->maxHp = maxHp;
    for (int i = 0; i < MAX_STATES; i++)
      states[i] = nullptr;
  }

  virtual ~Enemy();

  Entity *getPendingSpawnEntity() override { return (Entity *)pendingDrop; }
  void clearPendingSpawnEntity() override { pendingDrop = nullptr; }

  virtual bool isEnemy() const override { return true; }
  virtual bool isPlayer() const override { return false; }
  int getFaction() const override { return FACTION_ENEMY; }
  const char *getEnemyTag() const override { return enemyTag; }

  virtual void fireWeapon() {}
  virtual void fireWeaponWithVariation(float v) {
    (void)v;
    fireWeapon();
  }
  virtual void move(float dt) { (void)dt; }
  virtual void attack(float dt) { (void)dt; }

  void die() override;

  // AI Helpers
  void addState(int id, EnemyAIState *state) {
    if (id >= 0 && id < MAX_STATES)
      states[id] = state;
  }
  void changeState(int id) {
    if (id >= 0 && id < MAX_STATES && states[id])
      currentAIState = states[id];
  }
  bool hasState(int id) const {
    return (id >= 0 && id < MAX_STATES && states[id] != nullptr);
  }

  float distanceToPlayer();
  double getPlayerX();
  double getPlayerY();

  // Call this from fireWeapon() to show attack animation for 'dur' seconds
  void triggerFiringAnim(float dur) {
    isFiringAnim    = true;
    firingAnimTimer = dur;
  }
  // Safe to call every frame during wind-up — only starts once
  void startFiringAnimIfNotActive(float dur) {
    if (!isFiringAnim) {
      isFiringAnim    = true;
      firingAnimTimer = dur;
    }
  }
  // Tick firing anim timer — call once per update before animation selection
  void tickFiringAnim(float dt) {
    if (isFiringAnim) {
      firingAnimTimer -= dt;
      if (firingAnimTimer <= 0) isFiringAnim = false;
    }
  }

  void moveX(float amount) {
    velocityX = amount;
    if (amount != 0)
      isFacingLeft = (amount < 0);
  }
  bool isAgainstWall() const { return wallAhead; }
  bool isGrounded() { return isOnGround; }

  void setCrouching(bool v) { crouching = v; }
  bool isCrouching() const { return crouching; }
  virtual void setShieldActive(bool v) { shieldActive = v; }
  bool isShieldActive() const { return shieldActive; }

  float getWalkSpeed() const { return walkSpeed; }
  float getRunSpeed() const { return runSpeed; }

  virtual void meleeDamagePlayer() {}
  virtual void transformPlayer() {}

  virtual int getScoreValue() const { return 0; }
  bool killedByMelee = false;

  void onCollideWith(Entity *other, float dt) override {
    other->resolveCollision(this, dt);
  }

  void update(Level *level, float dt) override;
};
