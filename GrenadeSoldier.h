#pragma once
#include "../../Projectile System/Grenade.h"
#include "InfantryEnemy.h"
#include <cmath>

using namespace sf;
using namespace std;

// grenadeSoldier
// lobs grenades toward the player in a moderate ballistic arc.
// distinguished from BazookaSoldier by a shallower launch angle and faster
// horizontal component — the grenade travels further and lower.
// 2 HP. Spawns in batches of 1-2.
class GrenadeSoldier : public InfantryEnemy {
private:
  float fireTimer;
  static const float FIRE_COOLDOWN; // seconds between lobs

  IntRect walkFrames[7];
  IntRect chaseFrames[11];
  IntRect jumpFrames[10];
  IntRect attackFrames[16];
  int numWalkFrames, numChaseFrames, numJumpFrames, numAttackFrames;

  // Death animation
  IntRect deathFrames[11];
  bool isDying;
  int deathFrame;
  float deathAnimTimer;

public:
  GrenadeSoldier(double posX, double posY)
      : Entity(posX, posY), DamagableEntity(posX, posY, 2),
        InfantryEnemy(posX, posY, 2), fireTimer(0), numWalkFrames(7),
        numChaseFrames(11), numJumpFrames(10), numAttackFrames(16) {
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

    walkSpeed = 3;
    runSpeed = 5;
    enemyTag = "grenade";


    addState(AI_STATE_PATROL, new PatrolState());
    addState(AI_STATE_CHASE, new ChaseState());
    addState(AI_STATE_ATTACK, new GrenadeThrowState());
    addState(AI_STATE_RETREAT, new RetreatState());
    addState(AI_STATE_PANIC, new PanicState());
    changeState(AI_STATE_PATROL);

    if (enemyTexture.loadFromFile("Sprites/Enemies/soldier.png")) {
      enemySprite.setTexture(enemyTexture);
      enemySprite.setScale(3, 3);
    }

    animDelay = 5.0f / 60.0f;

    // walking (7 frames, right-to-left, soldier.png)
    walkFrames[0] = IntRect(201, 423, 25, 37);
    walkFrames[1] = IntRect(169, 423, 26, 37);
    walkFrames[2] = IntRect(136, 423, 28, 37);
    walkFrames[3] = IntRect(102, 423, 29, 37);
    walkFrames[4] = IntRect( 68, 422, 29, 38);
    walkFrames[5] = IntRect( 36, 422, 27, 38);
    walkFrames[6] = IntRect(  5, 422, 26, 38);

    // chasing player (11 frames)
    chaseFrames[0]  = IntRect(429, 53, 37, 41);
    chaseFrames[1]  = IntRect(390, 51, 36, 42);
    chaseFrames[2]  = IntRect(350, 52, 37, 41);
    chaseFrames[3]  = IntRect(309, 53, 38, 41);
    chaseFrames[4]  = IntRect(265, 54, 41, 40);
    chaseFrames[5]  = IntRect(221, 55, 41, 39);
    chaseFrames[6]  = IntRect(178, 56, 40, 38);
    chaseFrames[7]  = IntRect(134, 54, 41, 39);
    chaseFrames[8]  = IntRect( 91, 53, 40, 40);
    chaseFrames[9]  = IntRect( 47, 54, 41, 40);
    chaseFrames[10] = IntRect(  3, 55, 41, 39);

    // jumping (10 frames, right-to-left, soldier.png)
    jumpFrames[0] = IntRect(282, 857, 27, 35);
    jumpFrames[1] = IntRect(247, 857, 30, 34);
    jumpFrames[2] = IntRect(212, 857, 30, 33);
    jumpFrames[3] = IntRect(175, 857, 32, 35);
    jumpFrames[4] = IntRect(140, 857, 30, 37);
    jumpFrames[5] = IntRect(106, 857, 29, 39);
    jumpFrames[6] = IntRect( 78, 857, 23, 40);
    jumpFrames[7] = IntRect( 54, 862, 19, 35);
    jumpFrames[8] = IntRect( 29, 861, 20, 36);
    jumpFrames[9] = IntRect(  5, 860, 19, 37);

    // grenade-throw (16 frames, right-to-left, soldier.png)
    attackFrames[0]  = IntRect(1549, 834, 27, 37);
    attackFrames[1]  = IntRect(1515, 835, 29, 36);
    attackFrames[2]  = IntRect(1482, 835, 28, 36);
    attackFrames[3]  = IntRect(1449, 838, 28, 33);
    attackFrames[4]  = IntRect(1415, 837, 29, 34);
    attackFrames[5]  = IntRect(1375, 836, 35, 35);
    attackFrames[6]  = IntRect(1330, 823, 40, 48);
    attackFrames[7]  = IntRect(1287, 823, 38, 48);
    attackFrames[8]  = IntRect(1242, 830, 40, 41);
    attackFrames[9]  = IntRect(1197, 829, 40, 42);
    attackFrames[10] = IntRect(1153, 833, 39, 38);
    attackFrames[11] = IntRect(1113, 835, 35, 36);
    attackFrames[12] = IntRect(1077, 835, 31, 36);
    attackFrames[13] = IntRect(1044, 835, 28, 36);
    attackFrames[14] = IntRect(1012, 835, 27, 36);
    attackFrames[15] = IntRect( 981, 834, 26, 37);

    enemySprite.setTextureRect(walkFrames[0]);
  }

  ~GrenadeSoldier() {}
  int getScoreValue() const override { return 100; }

  float getWidth() const override { return 36 * 3; }
  float getHeight() const override { return 40 * 3; }

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
    }
  }

  // update
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

    tickFiringAnim(dt);

    // retreat animation overrides everything else
    if (!applyRetreatAnim(dt)) {

    // all frames are on soldier.png (enemyTexture)
    IntRect *frames = walkFrames;
    int maxFrames = numWalkFrames;

    if (!isOnGround) {
      frames    = jumpFrames;
      maxFrames = numJumpFrames;
    } else if (isFiringAnim) {
      frames    = attackFrames;
      maxFrames = numAttackFrames;
    } else if (currentAIState && currentAIState->getID() == AI_STATE_CHASE) {
      frames    = chaseFrames;
      maxFrames = numChaseFrames;
    }

    if (currentAIState && currentAIState->getAnimCycleDuration() > 0) {
      animDelay = currentAIState->getAnimCycleDuration() / maxFrames;
    }

    animTimer += dt;
    if (animTimer >= animDelay) {
      animTimer = 0;
      currentFrame = (currentFrame + 1) % maxFrames;
    }

    enemySprite.setTexture(enemyTexture);
    enemySprite.setTextureRect(frames[currentFrame % maxFrames]);

    } // end !applyRetreatAnim
  }

  void fireWeapon() override {
    if (!target || fireTimer > 0)
      return;
    lobGrenade(1);
  }
  void fireWeaponWithVariation(float variation) override {
    if (!target || fireTimer > 0)
      return;
    lobGrenade(variation);
  }

  void lobGrenade(float arcVariation) {
    float dx = (target->getPosX() - posX);
    float hDir = (dx > 0) ? 1 : -1;

    double velX = hDir * 8.0;
    double velY = -10.0 * arcVariation;

    double muzzleX = posX + (isFacingLeft ? 0.0 : getWidth());
    double muzzleY = posY + getHeight() * 0.3;

    double angle = atan2(velY, velX);
    double speed = sqrt(velX * velX + velY * velY);

    Grenade *g = new Grenade(muzzleX, muzzleY, angle, speed, 5, false);
    g->setAffectedByTiles(false);
    pendingShot = g;
    fireTimer   = FIRE_COOLDOWN;

    // show throw anim and retreat after tossing
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
