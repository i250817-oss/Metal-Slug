#pragma once
#include <cmath>
#include <cstdlib>

class PlayerSoldier;
class Enemy;

const int AI_STATE_PATROL  = 0;
const int AI_STATE_CHASE   = 1;
const int AI_STATE_ATTACK  = 2;
const int AI_STATE_RETREAT = 3;
const int AI_STATE_MELEE   = 4;
const int AI_STATE_PANIC   = 5;

class EnemyAIState {
public:
  virtual ~EnemyAIState() {}
  virtual void update(Enemy &e, float dt) = 0;
  virtual int getID() const = 0;
  virtual const char *getName() const = 0;
  virtual float getAnimCycleDuration() const { return -1.0f; } // -1 means use default animation speed
};

class PatrolState : public EnemyAIState {
private:
  float direction   = 1;
  float patrolTimer = 0;
  const float PATROL_FLIP_TIME = 2.5f;
  const float DETECT_RADIUS    = 10;
public:
  void update(Enemy &enemy, float dt) override;
  int getID() const override { return AI_STATE_PATROL; }
  const char *getName() const override { return "Patrol"; }
};

class ChaseState : public EnemyAIState {
private:
  const float KNIFE_RANGE  = 2;
  const float SHOOT_RANGE  = 8;
  const float LOSE_RADIUS  = 15;
public:
  void update(Enemy &enemy, float dt) override;
  int getID() const override { return AI_STATE_CHASE; }
  const char *getName() const override { return "Chase"; }
};

class ShootState : public EnemyAIState {
private:
  int   shotsFired     = 0;
  float betweenTimer   = 0;
  const int   BURST_SIZE    = 3;
  const float BETWEEN_SHOT  = 0.55f;
  const float SHOOT_RANGE   = 10;
  const float KNIFE_RANGE   = 2;
public:
  void update(Enemy &enemy, float dt) override;
  int getID() const override { return AI_STATE_ATTACK; }
  const char *getName() const override { return "Attack"; }
  float getAnimCycleDuration() const override { return BETWEEN_SHOT; }
};

class RetreatState : public EnemyAIState {
private:
  float retreatTimer = 0;
  const float RETREAT_DURATION = 3.5f;
  const float DETECT_RADIUS    = 10;
  const float KNIFE_RANGE      = 2;
public:
  void update(Enemy &enemy, float dt) override;
  int getID() const override { return AI_STATE_RETREAT; }
  const char *getName() const override { return "Retreat"; }
};

class MeleeState : public EnemyAIState {
private:
  const float MELEE_RANGE      = 2;
  const float DISENGAGE_RANGE  = 8;
  float swingTimer             = 0;
  const float SWING_COOLDOWN   = 0.5f;
public:
  void update(Enemy &enemy, float dt) override;
  int getID() const override { return AI_STATE_MELEE; }
  const char *getName() const override { return "Melee"; }
  float getAnimCycleDuration() const override { return SWING_COOLDOWN; }
};

class PanicState : public EnemyAIState {
private:
  float panicTimer          = 0;
  const float PANIC_DURATION = 4;
public:
  void update(Enemy &enemy, float dt) override;
  int getID() const override { return AI_STATE_PANIC; }
  const char *getName() const override { return "Panic"; }
};

class BazookaShootState : public EnemyAIState {
private:
  float windUpTimer       = 0;
  float recoveryTimer     = 0;
  bool  firedThisSequence = false;
  const float WIND_UP_TIME  = 1.2f;
  const float RECOVERY_TIME = 2.5f;
  const float ENGAGE_RANGE  = 12;
  const float PANIC_RANGE   = 2.5f;
public:
  void update(Enemy &enemy, float dt) override;
  int getID() const override { return AI_STATE_ATTACK; }
  const char *getName() const override { return "Attack"; }
  float getAnimCycleDuration() const override { return WIND_UP_TIME; }
};

class GrenadeThrowState : public EnemyAIState {
private:
  float pinTimer           = 0;
  float recoveryTimer      = 0;
  bool  thrownThisSequence = false;
  const float PIN_TIME      = 0.6f;
  const float RECOVERY_TIME = 2.2f;
  const float OPTIMAL_MIN   = 3.5f;
  const float OPTIMAL_MAX   = 7;
  const float PANIC_RANGE   = 2;
public:
  void update(Enemy &enemy, float dt) override;
  int getID() const override { return AI_STATE_ATTACK; }
  const char *getName() const override { return "Attack"; }
  float getAnimCycleDuration() const override { return PIN_TIME; }
};

class ShieldedMarchState : public EnemyAIState {
private:
  float shieldDownTimer = 0;
  float shootWindow     = 0;
  bool  shieldDown      = false;
  const float SHOOT_RANGE = 10;
  const float KNIFE_RANGE = 2;
  const float SHIELD_DOWN = 1.2f;
  const float SHIELD_UP   = 1.5f;
public:
  void update(Enemy &enemy, float dt) override;
  int getID() const override { return AI_STATE_ATTACK; }
  const char *getName() const override { return "Attack"; }
};

class ZombieHybridState : public EnemyAIState {
private:
  const float MELEE_RANGE     = 1.2f;
  const float SHOOT_RANGE     = 5;
  float pistolDelay           = 0;
  const float PISTOL_COOLDOWN = 2.5f;
public:
  void update(Enemy &enemy, float dt) override;
  int getID() const override { return AI_STATE_ATTACK; }
  const char *getName() const override { return "Attack"; }
};