#pragma once
#include "../../Projectile System/FireBomb.h"
#include "../../Projectile System/FlameStream.h"
#include "../../Projectile System/Rocket.h"
#include "Boss.h"
#include "../../Entity Root/Entity.h"
#include "../../Entity Root/DamagableEntity.h"

using namespace sf;
using namespace std;

class IronNokana : virtual public Boss {
private:
  float missileCooldown;
  float flameCooldown;
  float grenadeCooldown;
  bool isFlameActive;
  float flameTimer;

  static Texture texture;
  static bool textureLoaded;
  Sprite sprite;
  IntRect hullStages[4];

public:
  IronNokana(double posX, double posY)
      : Entity(posX, posY), DamagableEntity(posX, posY, 100),
        Boss(100, posX, posY, "iron_nokana"), missileCooldown(0.8f),
        flameCooldown(3), grenadeCooldown(1.5f), isFlameActive(false),
        flameTimer(0) {
    if (!textureLoaded) {
      if (texture.loadFromFile("Sprites/Enemies/iron nokana.png"))
        textureLoaded = true;
    }
    sprite.setTexture(texture);
    sprite.setScale(2.5f, 2.5f);

    hullStages[0] = {10, 3466, 175, 120};
    hullStages[1] = {190, 3468, 177, 122};
    hullStages[2] = {372, 3434, 176, 156};
    hullStages[3] = {553, 3429, 177, 161};

    sprite.setTextureRect(hullStages[0]);
  }

  float getWidth() const override { return 175 * 2.5f; }
  float getHeight() const override { return 120 * 2.5f; }
  int getScoreValue() const override { return 500; }

  void launchMissile() {
    if (!target)
      return;
    double mx = posX + getWidth() * 0.5;
    double my = posY - 40.0; // spawn ABOVE the turret
    double dx = target->getPosX() - mx;
    double dy = target->getPosY() - my;
    double angle = atan2(dy, dx);
    pendingShot = new Rocket(mx, my, angle, false, 15, 10, true, false);
  }

  void activateFlamethrower() {
    double fx =
        posX +
        (isFacingLeft ? -50.0 : getWidth() + 50.0); // spawn AHEAD of hull
    double fy = posY + getHeight() * 0.8;
    double angle = isFacingLeft ? 3.14159 : 0.0;
    pendingShot = new FlameStream(fx, fy, angle, false, false);
    isFlameActive = true;
    flameTimer = 2;
  }

  void throwFireBombGrenade() {
    double gx = posX + getWidth() * 0.5;
    double gy = posY + getHeight() * 0.2;
    double angle = isFacingLeft ? 3.14159 - 0.78 : -0.78; // 45 deg up
    pendingShot = new FireBomb(gx, gy, angle, 10, false);
  }

  void fireWeapon() override { attack(0.016f); }

  void spawnMinions() override {
    // bossLevel or EntityManager handles this usually, but we can spawn
    // directly
  }

  void specialAttack() override {
    // combined attack
    launchMissile();
    activateFlamethrower();
  }

  void retreat() override {
    isRetreating = true;
    retreatDestX = posX - 1500.0;
  }

  void attack(float dt) override {
    if (missileCooldown <= 0) {
      launchMissile();
      missileCooldown = 0.8f;
    }
    if (flameCooldown <= 0) {
      activateFlamethrower();
      flameCooldown = 4;
    }
    if (grenadeCooldown <= 0) {
      throwFireBombGrenade();
      grenadeCooldown = 1.5f;
    }
  }

  void move(float dt) override {
    if (!target || distanceToPlayer() > 20) {
      velocityX = 0;
      return;
    }

    double dx = target->getPosX() - posX;
    isFacingLeft = (dx < 0);

    float dist = fabs(dx);
    if (dist > 400) {
      velocityX = (dx > 0) ? walkSpeed : -walkSpeed;
    } else if (dist < 200) {
      velocityX = (dx > 0) ? -walkSpeed : walkSpeed; // keep distance
    } else {
      velocityX = 0;
    }
  }

  void update(Level *level, float dt) override {
    if (!isAlive)
      return;

    if (shouldRetreat())
      retreat();

    if (isRetreating) {
      velocityX = -5;
      posX += velocityX;
      if (posX < retreatDestX) {
        isAlive = false;
        hasRetreated = true;
      }
      return;
    }

    if (missileCooldown > 0)
      missileCooldown -= dt;
    if (flameCooldown > 0)
      flameCooldown -= dt;
    if (grenadeCooldown > 0)
      grenadeCooldown -= dt;

    if (isFlameActive) {
      flameTimer -= dt;
      if (flameTimer <= 0) {
        isFlameActive = false;
      } else {

        static float fTimer = 0;
        fTimer += dt;
        if (fTimer >= 0.08f) {
          fTimer = 0;
          double fx = posX + (isFacingLeft ? -50.0 : getWidth() + 50.0);
          double fy = posY + getHeight() * 0.8;
          double angle = isFacingLeft ? 3.14159 : 0.0;
          pendingShot = new FlameStream(fx, fy, angle, false, false);
        }
      }
    }

    move(dt);
    attack(dt);
    applyPhysics();
    tileCollisionImpl(level);
  }

  void draw(RenderWindow &window) override {
    if (!isAlive)
      return;

    int stage = 0;
    float hpPercent = (float)hp / maxHp;
    if (hpPercent < 0.25f)
      stage = 3;
    else if (hpPercent < 0.50f)
      stage = 2;
    else if (hpPercent < 0.75f)
      stage = 1;

    sprite.setTextureRect(hullStages[stage]);
    sprite.setPosition(posX, posY);

    float scaleX = 2.5f;
    if (!isFacingLeft) {
      sprite.setScale(-scaleX, scaleX);
      sprite.setOrigin(hullStages[stage].width, 0);
    } else {
      sprite.setScale(scaleX, scaleX);
      sprite.setOrigin(0, 0);
    }
    window.draw(sprite);
  }
};
