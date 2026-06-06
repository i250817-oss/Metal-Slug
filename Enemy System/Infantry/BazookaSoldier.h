#pragma once
#include "../../Projectile System/Rocket.h"
#include "InfantryEnemy.h"
#include <cmath>

using namespace sf;
using namespace std;



//bazookaSoldier
//fires rockets in steep ballistic arc. 2 HP. Batches 1-2. FIX 3: shoot range
//extended to 12 blocks (was 5). FIX 4: rockets are NOT affected by tiles (pass
//through terrain).
class BazookaSoldier : public InfantryEnemy {
private:
  float fireTimer;
  static const float FIRE_COOLDOWN;

  IntRect moveFrames[6];
  IntRect jumpFrames[8];
  IntRect attackFrames[8];
  int numMoveFrames, numJumpFrames, numAttackFrames;

  // Death animation
  IntRect deathFrames[11];
  bool isDying;
  int deathFrame;
  float deathAnimTimer;

public:
  BazookaSoldier(double posX, double posY)
      : Entity(posX, posY), DamagableEntity(posX, posY, 2),
        InfantryEnemy(posX, posY, 2), fireTimer(0), numMoveFrames(6),
        numJumpFrames(8), numAttackFrames(8) {
    isDying = false;
    deathFrame = 0;
    deathAnimTimer = 0.0f;

    deathFrames[0]  = IntRect(5, 10297, 26, 37);
    deathFrames[1]  = IntRect(36, 10297, 25, 37);
    deathFrames[2]  = IntRect(66, 10298, 24, 36);
    deathFrames[3]  = IntRect(95, 10299, 24, 35);
    deathFrames[4]  = IntRect(124, 10300, 27, 34);
    deathFrames[5]  = IntRect(156, 10301, 27, 33);
    deathFrames[6]  = IntRect(188, 10303, 27, 31);
    deathFrames[7]  = IntRect(220, 10307, 27, 27);
    deathFrames[8]  = IntRect(252, 10312, 26, 22);
    deathFrames[9]  = IntRect(283, 10316, 28, 18);
    deathFrames[10] = IntRect(316, 10316, 29, 18);

    walkSpeed = 1.5f;
    runSpeed = 2;
    enemyTag = "bazooka";


    addState(AI_STATE_PATROL, new PatrolState());
    addState(AI_STATE_CHASE, new ChaseState());
    addState(AI_STATE_ATTACK, new BazookaShootState());
    addState(AI_STATE_RETREAT, new RetreatState());
    addState(AI_STATE_MELEE, new MeleeState());
    addState(AI_STATE_PANIC, new PanicState());
    changeState(AI_STATE_PATROL);

    if (enemyTexture.loadFromFile(
            "Sprites/Enemies/Rebel Soldier (Bazooka).png")) {
      enemySprite.setTexture(enemyTexture);
      enemySprite.setScale(3, 3);
    }
    animDelay = 6.0f / 60.0f;

    moveFrames[0] = IntRect(204, 3, 38, 45);
    moveFrames[1] = IntRect(163, 5, 38, 43);
    moveFrames[2] = IntRect(124, 6, 36, 42);
    moveFrames[3] = IntRect(83, 5, 38, 43);
    moveFrames[4] = IntRect(42, 3, 38, 45);
    moveFrames[5] = IntRect(3, 3, 36, 45);

    jumpFrames[0] = IntRect(310, 197, 37, 41);
    jumpFrames[1] = IntRect(269, 202, 38, 36);
    jumpFrames[2] = IntRect(226, 196, 40, 36);
    jumpFrames[3] = IntRect(181, 197, 42, 36);
    jumpFrames[4] = IntRect(135, 195, 43, 41);
    jumpFrames[5] = IntRect(89, 195, 43, 43);
    jumpFrames[6] = IntRect(43, 196, 43, 42);
    jumpFrames[7] = IntRect(3, 197, 37, 41);

    attackFrames[0] = IntRect(3, 449, 37, 41);
    attackFrames[1] = IntRect(43, 454, 38, 36);
    attackFrames[2] = IntRect(86, 457, 39, 33);
    attackFrames[3] = IntRect(128, 458, 41, 32);
    attackFrames[4] = IntRect(172, 458, 41, 32);
    attackFrames[5] = IntRect(216, 459, 41, 31);
    attackFrames[6] = IntRect(260, 458, 41, 32);
    attackFrames[7] = IntRect(304, 458, 41, 32);

    enemySprite.setTextureRect(moveFrames[0]);
  }

  ~BazookaSoldier() {}
  int getScoreValue() const override { return 100; }

  float getWidth() const override { return 43 * 3; }
  float getHeight() const override { return 45 * 3; }


  bool getIsDying() const override { return isDying; }


  void takeDamage(int val) override {
    if (isDying) return;
    if (hp - val > 0) {
      InfantryEnemy::takeDamage(val);
    } else {
      hp = 0;
      isDying = true;
      deathFrame = 0;
      deathAnimTimer = 0.0f;
      velocityX = 0;
      velocityY = 0;
      playDeathSound();
    }
  }

  void update(Level *level, float dt) override {
    if (!isAlive) return;

    if (isDying) {
      deathAnimTimer += dt;
      float frameDelay = 6.0f / 60.0f;
      if (deathAnimTimer >= frameDelay) {
        deathAnimTimer = 0;
        deathFrame++;
      }
      if (deathFrame >= 11) {
        isDying = false;
        InfantryEnemy::takeDamage(9999);
        return;
      }
      applyPhysics();
      tileCollisionImpl(level);
      enemySprite.setTexture(retreatTexture);
      enemySprite.setTextureRect(deathFrames[deathFrame]);
      return;
    }

    InfantryEnemy::update(level, dt);

    if (fireTimer > 0)
      fireTimer -= dt;
    if (enemyFireTimer > 0)
      enemyFireTimer -= dt;

    tickFiringAnim(dt);

    // retreat animation overrides everything else
    if (!applyRetreatAnim(dt)) {

    // attack frames only while actively firing, not during cooldown
    enemySprite.setTexture(enemyTexture);   // ensure we're back on our own sheet
    IntRect *frames = moveFrames;
    int maxFrames = numMoveFrames;
    float delay = 6.0f / 60.0f;

    if (!isOnGround) {
      frames = jumpFrames;
      maxFrames = numJumpFrames;
      delay = 4.0f / 60.0f;
    } else if (isFiringAnim) {
      frames = attackFrames;
      maxFrames = numAttackFrames;
      delay = 8.0f / 60.0f;
    }

    if (currentAIState && currentAIState->getAnimCycleDuration() > 0) {
        delay = currentAIState->getAnimCycleDuration() / maxFrames;
    }

    animTimer += dt;
    if (animTimer >= delay) {
        animTimer = 0; 
        currentFrame = (currentFrame + 1) % maxFrames; 
    }
    enemySprite.setTextureRect(frames[currentFrame % maxFrames]);

    } // end !applyRetreatAnim
  }

  
  void fireWeapon() override {
    if (!target || fireTimer > 0)
      return;

    float dx = (target->getPosX() - posX);
    float hDir = (dx > 0) ? 1 : -1;

    if (abs(dx) > 8 * 64)
      return;

    double velX = hDir * 2.5;
    double velY = -8.0;
    double muzzleX = posX + (isFacingLeft ? 0.0 : getWidth());
    double muzzleY = posY + getHeight() * 0.2;
    double angle = atan2(velY, velX);
    double speed = sqrt(velX * velX + velY * velY);

    Rocket *r = new Rocket(muzzleX, muzzleY, angle, false, speed, 10,
                           true, false);
    pendingShot = r;
    fireTimer = FIRE_COOLDOWN;

    // show attack anim and retreat after firing
    triggerFiringAnim(numAttackFrames * 8.0f / 60.0f);
    if (hasState(AI_STATE_RETREAT))
      changeState(AI_STATE_RETREAT);
  }

  void draw(RenderWindow &window) override {
    if (isSpriteOverridden) {
        enemySprite.setTextureRect(overrideRect);
    }
    InfantryEnemy::draw(window);
  }

};
