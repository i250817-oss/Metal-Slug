#pragma once
#include "../../Projectile System/FireBomb.h"
#include "../../Projectile System/Rocket.h"
#include "Boss.h"

using namespace sf;

//hairbuster Riberts (Aerial Boss)
//armaments:
//- Rockets            -> Rocket
//- Fire bomb grenades -> FireBomb
//30 HP. Spawns in high altitude.
class HairbusterRiberts : virtual public Boss {
private:
  float rocketCooldown;
  float bombCooldown;
  float sinTimer;

  //animations
  bool isShootingMissile;
  float missileAnimTimer;
  int missileFrame;

  static Texture texture;
  static bool textureLoaded;

  Sprite bodySprite;
  Sprite missileSprite;

  IntRect hullStages[4];
  IntRect missileFrames[34];

public:
  HairbusterRiberts(double posX, double posY)
      : Entity(posX, posY), DamagableEntity(posX, posY, 100),
        Boss(100, posX, posY, "hairbuster_riberts"), rocketCooldown(1),
        bombCooldown(2), sinTimer(0), isShootingMissile(false),
        missileAnimTimer(0), missileFrame(0) {
    if (!textureLoaded) {
      if (texture.loadFromFile("Sprites/Enemies/hairbuster.png"))
        textureLoaded = true;
    }

    float scale = 2.5f;
    bodySprite.setTexture(texture);
    bodySprite.setScale(scale, scale);
    missileSprite.setTexture(texture);
    missileSprite.setScale(scale, scale);

    //hull Stages
    hullStages[0] = {0, 16, 184, 109};
    hullStages[1] = {185, 16, 184, 109};
    hullStages[2] = {370, 16, 184, 109};
    hullStages[3] = {555, 16, 184, 109};

    //missile Shooting Frames (34 frames)
    for (int i = 0; i < 17; i++) {
      missileFrames[i] = {i * 49, 251, 48, 112};
    }
    for (int i = 0; i < 17; i++) {
      missileFrames[17 + i] = {i * 49, 364, 48, 112};
    }

    bodySprite.setTextureRect(hullStages[0]);
  }

  float getWidth() const override { return 184 * 2.5f; }
  float getHeight() const override { return 109 * 2.5f; }
  int getScoreValue() const override { return 500; }

  void launchRocket() {
    if (!target)
      return;
    isShootingMissile = true;
    missileFrame = 0;

    double mx = posX + getWidth() * 0.5;
    double my = posY + getHeight();
    double dx = target->getPosX() - mx;
    double dy = target->getPosY() - my;
    double angle = atan2(dy, dx);
    pendingShot = new Rocket(mx, my, angle, false, 7, 15, true, false);
  }

  void dropFireBomb() {
    double bx = posX + getWidth() * 0.5;
    double by = posY + getHeight();
    pendingShot = new FireBomb(bx, by, 1.57, 6, false);
  }

  void fireWeapon() override { attack(0.016f); }
  void spawnMinions() override {}
  void specialAttack() override { dropFireBomb(); }

  void retreat() override {
    isRetreating = true;
    retreatDestX = posX + 1500.0;
  }

  void attack(float dt) override {
    if (distanceToPlayer() > 20)
      return; //detection range

    if (rocketCooldown <= 0) {
      launchRocket();
      rocketCooldown = 1;
    }
    if (bombCooldown <= 0) {
      dropFireBomb();
      bombCooldown = 2;
    }
  }

  void move(float dt) override {
    if (!target || distanceToPlayer() > 20) {
      velocityX = 0;
      velocityY = 0;
      return;
    }

    //bobbing movement above the player
    sinTimer += 0.05f * dt * 60.0f;
    double targetY = (target ? target->getPosY() : 1800.0) - 350.0 + sin(sinTimer) * 50.0;

    //smoothly adjust posY towards targetY
    posY += (targetY - posY) * 0.05f * dt * 60.0f;

    double dx = target->getPosX() - (posX + getWidth() * 0.5);
    velocityX = (dx > 0) ? walkSpeed : -walkSpeed;
    isFacingLeft = (dx < 0);
    velocityY = 0; //handled by the smooth lerp above
  }

  void update(Level *level, float dt) override {
    if (!isAlive)
      return;
    if (shouldRetreat())
      retreat();

    if (isRetreating) {
      velocityX = 8;
      posX += velocityX * dt * 60.0f;
      if (posX > retreatDestX) {
        isAlive = false;
        hasRetreated = true;
      }
      return;
    }

    if (rocketCooldown > 0)
      rocketCooldown -= dt;
    if (bombCooldown > 0)
      bombCooldown -= dt;

    if (isShootingMissile) {
      missileAnimTimer += dt;
      if (missileAnimTimer >= 0.05f) {
        missileAnimTimer = 0;
        missileFrame++;
        if (missileFrame >= 34) {
          isShootingMissile = false;
          missileFrame = 0;
        }
      }
    }

    move(dt);
    attack(dt);
    posX += velocityX * dt * 60.0f;
    posY += velocityY * dt * 60.0f;
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

    bodySprite.setTextureRect(hullStages[stage]);

    //center origin for seamless flipping
    bodySprite.setOrigin(92, 54);

    float scaleX = 2.5f;
    if (!isFacingLeft) {
      bodySprite.setScale(-scaleX, scaleX);
    } else {
      bodySprite.setScale(scaleX, scaleX);
    }

    //position based on center since origin is centered
    bodySprite.setPosition(posX + getWidth() * 0.5f, posY + getHeight() * 0.5f);
    window.draw(bodySprite);

    if (isShootingMissile) {
      missileSprite.setTextureRect(missileFrames[missileFrame]);
      missileSprite.setPosition(posX + (isFacingLeft ? 80 : 260), posY + 20);
      window.draw(missileSprite);
    }
  }
};
