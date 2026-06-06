#include "EnemyAIState.h"
#include "Enemy.h"

// Default idle state: walk back and forth.
void PatrolState::update(Enemy &enemy, float dt) {
  patrolTimer += dt;
  if (patrolTimer >= PATROL_FLIP_TIME || enemy.isAgainstWall()) {
    direction *= -1;
    patrolTimer = 0;
  }
  enemy.moveX(direction * enemy.getWalkSpeed());
  if (enemy.distanceToPlayer() <= DETECT_RADIUS)
    enemy.changeState(AI_STATE_CHASE);
}

// Pursuit state: run directly towards the player.
void ChaseState::update(Enemy &enemy, float dt) {
  float dist = enemy.distanceToPlayer();

  if (dist > LOSE_RADIUS) {
    enemy.changeState(AI_STATE_PATROL);
    return;
  }
  if (dist <= KNIFE_RANGE) {
    enemy.changeState(AI_STATE_MELEE);
    return;
  }

  if (dist <= SHOOT_RANGE) {
    enemy.changeState(AI_STATE_ATTACK);
    return;
  }

  float dir = (enemy.getPlayerX() > enemy.getPosX()) ? 1 : -1;
  enemy.moveX(dir * enemy.getRunSpeed());
  if (enemy.isAgainstWall() && enemy.isGrounded())
    enemy.jump();
}

// Firing state: crouch and shoot a 3round burst.
void ShootState::update(Enemy &enemy, float dt) {
  float dist = enemy.distanceToPlayer();

  if (dist <= KNIFE_RANGE) {
    shotsFired = 0;
    betweenTimer = 0;
    enemy.setCrouching(false);
    enemy.changeState(AI_STATE_MELEE);
    return;
  }

  if (shotsFired >= BURST_SIZE) {
    shotsFired = 0;
    betweenTimer = 0;
    enemy.setCrouching(false);
    if (enemy.hasState(AI_STATE_PANIC) && (rand() % 10 == 0)) {
      enemy.changeState(AI_STATE_PANIC);
    } else if (enemy.hasState(AI_STATE_RETREAT)) {
      enemy.changeState(AI_STATE_RETREAT);
    } else {
      enemy.changeState(AI_STATE_PATROL);
    }
    return;
  }

  enemy.setCrouching(true);
  enemy.moveX(0);

  betweenTimer += dt;
  if (betweenTimer >= BETWEEN_SHOT) {
    betweenTimer = 0;
    enemy.fireWeapon();
    shotsFired++;
  }
}

// withdrawal: run away for 3.5 seconds.
void RetreatState::update(Enemy &enemy, float dt) {
  float dist = enemy.distanceToPlayer();

  if (dist <= KNIFE_RANGE) {
    retreatTimer = RETREAT_DURATION;
    if (enemy.hasState(AI_STATE_PANIC))
      enemy.changeState(AI_STATE_PANIC);
    else
      enemy.changeState(AI_STATE_MELEE);
    return;
  }

  if (dist > DETECT_RADIUS) {
    retreatTimer = RETREAT_DURATION;
    enemy.changeState(AI_STATE_PATROL);
    return;
  }

  float dir = (enemy.getPlayerX() > enemy.getPosX()) ? -1 : 1;
  enemy.moveX(dir * enemy.getRunSpeed());

  retreatTimer -= dt;
  if (retreatTimer <= 0) {
    retreatTimer = RETREAT_DURATION;
    enemy.changeState(dist <= DETECT_RADIUS ? AI_STATE_CHASE : AI_STATE_PATROL);
  }
}

// Close combat: stand still and deal melee damage.
void MeleeState::update(Enemy &enemy, float dt) {
  float dist = enemy.distanceToPlayer();
  if (dist > DISENGAGE_RANGE) {
    swingTimer = 0;
    enemy.changeState(AI_STATE_CHASE);
    return;
  }
  if (dist > MELEE_RANGE) {
    if (enemy.hasState(AI_STATE_RETREAT))
      enemy.changeState(AI_STATE_RETREAT);
    else
      enemy.changeState(AI_STATE_CHASE);
    return;
  }
  enemy.moveX(0);
  swingTimer += dt;
  if (swingTimer >= SWING_COOLDOWN) {
    swingTimer = 0;
    enemy.meleeDamagePlayer();
  }
}

// Survival state: run away very fast for 4 seconds.
void PanicState::update(Enemy &enemy, float dt) {
  panicTimer += dt;
  float dir = (enemy.getPlayerX() > enemy.getPosX()) ? -1 : 1;
  enemy.moveX(dir * enemy.getRunSpeed() * 1.3f);
  if (panicTimer >= PANIC_DURATION) {
    panicTimer = 0;
    enemy.changeState(AI_STATE_PATROL);
  }
}

// Heavy weapon state: position carefully, wind up, and fire.
void BazookaShootState::update(Enemy &enemy, float dt) {
  float dist = enemy.distanceToPlayer();

  if (dist <= PANIC_RANGE) {
    windUpTimer = recoveryTimer = 0;
    firedThisSequence = false;
    if (enemy.hasState(AI_STATE_PANIC))
      enemy.changeState(AI_STATE_PANIC);
    else
      enemy.changeState(AI_STATE_RETREAT);
    return;
  }
  if (dist > ENGAGE_RANGE) {
    windUpTimer = recoveryTimer = 0;
    firedThisSequence = false;
    enemy.changeState(dist > 15 ? AI_STATE_PATROL : AI_STATE_CHASE);
    return;
  }
  if (recoveryTimer > 0) {
    recoveryTimer -= dt;
    enemy.moveX(0);
    return;
  }

  if (!firedThisSequence) {
    float dir = (enemy.getPlayerX() > enemy.getPosX()) ? 1 : -1;
    if (abs(dist - 7) > 1.5f)
      enemy.moveX(dir * enemy.getWalkSpeed() * 0.5f);
    else
      enemy.moveX(0);
  }

  windUpTimer += dt;

  // Show attack anim as soon as winding up (safe to call every frame)
  enemy.startFiringAnimIfNotActive(WIND_UP_TIME + 8 * 8.0f / 60.0f);

  if (windUpTimer < WIND_UP_TIME)
    return;

  if (!firedThisSequence) {
    firedThisSequence = true;
    enemy.fireWeapon();
    recoveryTimer = RECOVERY_TIME;
    windUpTimer = 0;
  } else {
    firedThisSequence = false;
    windUpTimer = 0;
  }
}

// Grenadier state: hold optimal distance, pull pin, and throw.
void GrenadeThrowState::update(Enemy &enemy, float dt) {
  float dist = enemy.distanceToPlayer();

  if (dist <= PANIC_RANGE) {
    pinTimer = recoveryTimer = 0;
    thrownThisSequence = false;
    if (enemy.hasState(AI_STATE_PANIC))
      enemy.changeState(AI_STATE_PANIC);
    else
      enemy.changeState(AI_STATE_RETREAT);
    return;
  }

  if (dist < OPTIMAL_MIN) {
    float dir = (enemy.getPlayerX() > enemy.getPosX()) ? -1 : 1;
    enemy.moveX(dir * enemy.getWalkSpeed());
    return;
  }
  if (dist > OPTIMAL_MAX) {
    float dir = (enemy.getPlayerX() > enemy.getPosX()) ? 1 : -1;
    enemy.moveX(dir * enemy.getWalkSpeed());
    return;
  }

  enemy.moveX(0);
  if (recoveryTimer > 0) {
    recoveryTimer -= dt;
    return;
  }

  pinTimer += dt;

  // Show throw anim as soon as pin-pulling begins (safe to call every frame)
  enemy.startFiringAnimIfNotActive(PIN_TIME + 16 * 8.0f / 60.0f);

  if (pinTimer < PIN_TIME)
    return;

  if (!thrownThisSequence) {
    thrownThisSequence = true;
    float variation = 0.9f + (rand() % 20) / 100.0f;
    enemy.fireWeaponWithVariation(variation);
    recoveryTimer = RECOVERY_TIME;
    pinTimer = 0;
  } else {
    thrownThisSequence = false;
    pinTimer = 0;
  }
}

// Shield state: advance slowly, periodically drop shield to shoot.
void ShieldedMarchState::update(Enemy &enemy, float dt) {
  float dist = enemy.distanceToPlayer();

  if (dist <= KNIFE_RANGE) {
    shieldDown = false;
    shieldDownTimer = 0;
    enemy.setShieldActive(true);
    enemy.setCrouching(false);
    enemy.changeState(AI_STATE_MELEE);
    return;
  }
  if (dist > SHOOT_RANGE + 3) {
    shieldDown = false;
    shieldDownTimer = 0;
    enemy.setShieldActive(true);
    enemy.setCrouching(false);
    enemy.changeState(AI_STATE_CHASE);
    return;
  }

  float dir = (enemy.getPlayerX() > enemy.getPosX()) ? 1 : -1;
  enemy.moveX(dir * enemy.getWalkSpeed() * 0.6f);

  if (!shieldDown) {
    shieldDownTimer += dt;
    if (shieldDownTimer >= SHIELD_UP && dist <= SHOOT_RANGE) {
      shieldDown = true;
      shootWindow = SHIELD_DOWN;
      shieldDownTimer = 0;
      enemy.setShieldActive(false);
    }
  } else {
    shootWindow -= dt;
    enemy.fireWeapon();
    if (shootWindow <= 0) {
      shieldDown = false;
      shieldDownTimer = 0;
      enemy.setShieldActive(true);
    }
  }
}

// Zombie state: relentless advance, bites close, shoots from afar.
void ZombieHybridState::update(Enemy &enemy, float dt) {
  float dist = enemy.distanceToPlayer();

  if (dist <= MELEE_RANGE) {
    enemy.moveX(0);
    enemy.meleeDamagePlayer();
    return;
  }

  float dir = (enemy.getPlayerX() > enemy.getPosX()) ? 1 : -1;
  enemy.moveX(dir * enemy.getWalkSpeed());

  if (dist > MELEE_RANGE * 2 && dist <= SHOOT_RANGE) {
    pistolDelay += dt;
    if (pistolDelay >= PISTOL_COOLDOWN) {
      pistolDelay = 0;
      enemy.fireWeapon();
    }
  }

  if (dist > SHOOT_RANGE + 2)
    enemy.changeState(AI_STATE_CHASE);
}
