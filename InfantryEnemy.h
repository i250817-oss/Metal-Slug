#pragma once
#include "../../Projectile System/Bullet.h"
#include "../../Weapon System/Weapon.h"
#include "../Enemy.h"

using namespace sf;
using namespace std;

// infantryEnemy
// abstract base for all ground infantry enemies.
class InfantryEnemy : virtual public Enemy {
protected:
  Weapon *weapon;

  Texture enemyTexture;
  Sprite enemySprite;

  // fire rate limiter
  float enemyFireTimer;
  static const float ENEMY_FIRE_COOLDOWN;

  // melee knife damage
  float knifeTimer;
  static const float KNIFE_COOLDOWN;
  static const int KNIFE_DAMAGE;

  // sprite override for special states (e.g. paratrooper descent)
  bool isSpriteOverridden;
  IntRect overrideRect;
  float renderScaleY;

  // shared retreat animation (soldier.png, 12 frames, right-to-left)
  Texture retreatTexture;
  bool retreatTextureLoaded;
  IntRect retreatFrames[12];
  static const int NUM_RETREAT_FRAMES = 12;

public:
  InfantryEnemy() : Entity(), DamagableEntity(0,0,0), Enemy(0,0,0), weapon(nullptr), enemyFireTimer(0), knifeTimer(0), isSpriteOverridden(false), renderScaleY(3.0f), retreatTextureLoaded(false) {
    loadRetreatFrames();
  }
  InfantryEnemy(double posX, double posY, int maxHp)
      : Entity(posX, posY), DamagableEntity(posX, posY, maxHp), Enemy(posX, posY, maxHp), weapon(nullptr), enemyFireTimer(0),
        knifeTimer(0), isSpriteOverridden(false), renderScaleY(3.0f), retreatTextureLoaded(false) {
    this->posX = posX;
    this->posY = posY;
    this->hp = maxHp;
    this->maxHp = maxHp;
    loadRetreatFrames();
  }



  void loadRetreatFrames() {
    retreatTextureLoaded = retreatTexture.loadFromFile("Sprites/Enemies/soldier.png");
    retreatFrames[0]  = IntRect(396, 536, 30, 37);
    retreatFrames[1]  = IntRect(360, 536, 31, 37);
    retreatFrames[2]  = IntRect(322, 535, 33, 38);
    retreatFrames[3]  = IntRect(284, 534, 33, 39);
    retreatFrames[4]  = IntRect(249, 534, 30, 39);
    retreatFrames[5]  = IntRect(217, 535, 27, 38);
    retreatFrames[6]  = IntRect(182, 536, 30, 37);
    retreatFrames[7]  = IntRect(148, 536, 29, 37);
    retreatFrames[8]  = IntRect(111, 535, 32, 38);
    retreatFrames[9]  = IntRect( 74, 534, 32, 39);
    retreatFrames[10] = IntRect( 39, 534, 30, 39);
    retreatFrames[11] = IntRect(  5, 535, 29, 38);
  }


  void setOverrideRect(const IntRect &rect) {
    isSpriteOverridden = true;
    overrideRect = rect;
    enemySprite.setTextureRect(rect);
  }
  void clearOverrideRect() { isSpriteOverridden = false; }

  virtual ~InfantryEnemy() { delete weapon; }

  void setWeapon(Weapon *w) {
    delete weapon;
    weapon = w;
  }
  Weapon *getWeapon() const { return weapon; }

  void fireWeapon() override {
    if (!weapon || enemyFireTimer > 0)
      return;

    double muzzleX = isFacingLeft ? posX : (posX + getWidth());
    double muzzleY = posY + (crouching ? getHeight() * 0.5 : getHeight() * 0.3);
    double angle = isFacingLeft ? M_PI : 0.0;

    pendingShot = weapon->fire(muzzleX, muzzleY, angle, false);
    if (pendingShot)
      enemyFireTimer = ENEMY_FIRE_COOLDOWN;
  }

  void meleeDamagePlayer() override {
    if (!target)
      return;
    target->takeDamage(KNIFE_DAMAGE);
  }

  virtual float getWidth() const override = 0;
  virtual float getHeight() const override = 0;

  void update(Level *level, float dt) override {
    Enemy::update(level, dt);

    if (enemyFireTimer > 0)
      enemyFireTimer -= dt;
    if (weapon)
      weapon->tickCooldown(dt);
  }



  // Returns true if the retreat animation should override the subclass animation.
  // Subclasses call this FIRST in their update() tothe short-circuit their own anim logic.
  bool applyRetreatAnim(float dt) {
    if (!currentAIState || currentAIState->getID() != AI_STATE_RETREAT)
      return false;
    if (isFiringAnim)        // still showing fire anim; let it finish first
      return false;
    if (!retreatTextureLoaded)
      return false;

    float delay = 5.0f / 60.0f;
    animTimer += dt;
    if (animTimer >= delay) {
      animTimer = 0;
      currentFrame = (currentFrame + 1) % NUM_RETREAT_FRAMES;
    }
    enemySprite.setTexture(retreatTexture);
    enemySprite.setTextureRect(retreatFrames[currentFrame % NUM_RETREAT_FRAMES]);
    return true;
  }

  void draw(RenderWindow &window) override {
    if (enemySprite.getTexture()) {
      IntRect rect = enemySprite.getTextureRect();

      float w = abs(rect.width);
      float h = abs(rect.height);

      enemySprite.setOrigin(w / 2.0f, h);
      float sx = posX + getWidth() / 2.0f;
      float sy = posY + getHeight();
      enemySprite.setPosition(sx, sy);

      enemySprite.setScale(isFacingLeft ? 3.f : -3.f, renderScaleY);

      window.draw(enemySprite);
    }
  }
};
