#pragma once
#include "../../Character System/PlayerSoldier.h"
#include "../../Collectible and Interactable System/SupplyBox.h"
#include "../../Projectile System/Rocket.h"
#include "../EnemyVehicle.h"
#include "../GroundVehicle.h"

using namespace sf;
using namespace std;

// m15ABradley

class M15ABradley : public GroundVehicle, public EnemyVehicle {
protected:
  IntRect walkFrames[8];
  IntRect shootFrames[7];

  bool isShooting;
  float shootAnimTimer;
  double playerX;
  double playerY;
  Entity *pendingDrop;

  static const float SHOOT_ANIM_DURATION;

public:
  M15ABradley(double posX, double posY)
      : Entity(posX, posY), DamagableEntity(posX, posY, 15),
        Vehicle(posX, posY, 15, "plains"), GroundVehicle(posX, posY, 15),
        EnemyVehicle(posX, posY, 15, "plains"), isShooting(false),
        shootAnimTimer(0), playerX(0.0), playerY(0.0), pendingDrop(nullptr) {
    gravity = 0.5f;
    max_speed = 1.5f;
    entityType = ENTITY_VEHICLE;

    // initialize frame arrays
    walkFrames[0] = {85, 165, 80, 78};
    walkFrames[1] = {3, 165, 80, 78};
    walkFrames[2] = {413, 84, 80, 78};
    walkFrames[3] = {331, 84, 80, 78};
    walkFrames[4] = {249, 84, 80, 78};
    walkFrames[5] = {167, 84, 80, 78};
    walkFrames[6] = {85, 84, 80, 78};
    walkFrames[7] = {3, 84, 80, 78};

    shootFrames[0] = {3, 781, 81, 77};
    shootFrames[1] = {418, 702, 82, 76};
    shootFrames[2] = {333, 703, 83, 75};
    shootFrames[3] = {249, 702, 82, 76};
    shootFrames[4] = {167, 700, 80, 78};
    shootFrames[5] = {85, 700, 80, 78};
    shootFrames[6] = {3, 700, 80, 78};

    setFireMode(0);
    loadAnimation(0);
  }

  virtual ~M15ABradley() {}

  void setTarget(DamagableEntity *p) override {
    if (p) {
      playerX = p->getPosX();
      playerY = p->getPosY();
    }
  }

  void launchMissile() {
    double mx = posX + getWidth() * 0.5;
    double my = posY;
    double dx = playerX - mx;
    double dy = playerY - my;
    double dist = sqrt(dx * dx + dy * dy);
    if (dist < 1.0)
      return;

    double travelTime = dist / 8.0;
    if (travelTime < 15.0)
      travelTime = 15.0;

    double g = 0.15;
    double velX = dx / travelTime;
    double velY = (dy / travelTime) - (0.5 * g * travelTime);

    double speed = sqrt(velX * velX + velY * velY);
    double angle = atan2(velY, velX);

    
    angle += (rand() % 100 - 50) / 1000.0;

    pendingShot = new Rocket(mx, my, angle, false, speed, 70, true, false);
    isShooting = true;
    shootAnimTimer = SHOOT_ANIM_DURATION;
    currentFrame = 0;
  }

  void die() override {
    Vehicle::die();
    // 100% chance to drop a supply box when this heavy tank is destroyed
    pendingDrop = new SupplyBox(posX + getWidth() / 2.0, posY + getHeight());
  }

  Entity *getPendingSpawnEntity() override { return pendingDrop; }
  void clearPendingSpawnEntity() override { pendingDrop = nullptr; }

  void fire() override {
    double dx = playerX - posX;
    double dist = fabs(dx);
    if (dist <= 900.0 && fireCooldown <= 0) {
      launchMissile();
      fireCooldown = 3;
    }
  }

  void move() override {
    double dx = playerX - posX;
    double dist = fabs(dx);
    if (dist > 500.0) {
      velocityX = (dx > 0.0) ? 1.5f : -1.5f;
      isFacingLeft = (dx < 0.0);
    } else {
      velocityX = 0;
    }
  }

  void loadAnimation(int dummy) override {
    if (vehicleTexture.loadFromFile(
            "Sprites/Enemies/Enemy - M-15A Bradley.png")) {
      vehicleSprite.setTexture(vehicleTexture);
      renderScaleX = -2.5f;
      renderScaleY = 2.5f;
      vehicleSprite.setOrigin(40, 39);
    }
    animDelay = 6.0f / 60.0f;
  }

  float getWidth() const override { return 80 * 2.5f; }
  float getHeight() const override { return 78 * 2.5f; }

  void update(Level *level, float dt) override {
    if (!isAlive)
      return;

    if (fabs(posX - playerX) > 960.0)
      return; // optimize: only update if near

    move();
    fire();

    if (shootAnimTimer > 0) {
      shootAnimTimer -= dt;
      if (shootAnimTimer <= 0)
        isShooting = false;
    }

    // animation logic moved to update
    int maxFrames = 8;
    float delay = 6.0f / 60.0f;

    if (isShooting) {
      maxFrames = 7;
      delay = 5.0f / 60.0f;
    } else if (fabs(velocityX) < 0.01f) {
      maxFrames = 1;
    }

    animTimer += dt;
    if (animTimer >= delay) {
      animTimer = 0;
      currentFrame = (currentFrame + 1) % maxFrames;
    }

    EnemyVehicle::update(level, dt);
  }

  void draw(RenderWindow &window) override {
    if (!isAlive)
      return;

    const IntRect *frames = isShooting ? shootFrames : walkFrames;
    vehicleSprite.setTextureRect(frames[currentFrame % (isShooting ? 7 : 8)]);

    // Position adjusted for center origin (posX/posY is top-left)
    vehicleSprite.setPosition(posX + getWidth() / 2.0f,
                              posY + getHeight() / 2.0f);

    Vehicle::draw(window);
  }
};
