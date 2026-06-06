#pragma once
#include "../../Character System/PlayerSoldier.h"
#include "../../Collectible and Interactable System/EnemyDropSystem.h"
#include "../../Projectile System/Rocket.h"
#include "../AquaticVehicle.h"
#include "../EnemyVehicle.h"

using namespace sf;
using namespace std;

// enemySub

class EnemySub : public AquaticVehicle, public EnemyVehicle {
protected:
  IntRect moveFrames[7];
  IntRect breakFrames[6];

  bool isBreaking;
  bool isBreakFinished;
  double playerX;
  double playerY;
  Entity *pendingDrop;

public:
  EnemySub(double posX, double posY)
      : Entity(posX, posY), DamagableEntity(posX, posY, 15),
        Vehicle(posX, posY, 15, "aquatic"), AquaticVehicle(posX, posY, 15),
        EnemyVehicle(posX, posY, 15, "aquatic"), isBreaking(false),
        isBreakFinished(false), playerX(400.0), playerY(400.0),
        pendingDrop(nullptr) {
    fireCooldown = 0;
    gravity = 0; // aquatic: neutral buoyancy
    max_speed = 3;
    entityType = ENTITY_VEHICLE;

    // initialize frame arrays
    moveFrames[0] = {621, 30, 93, 46};
    moveFrames[1] = {520, 28, 93, 48};
    moveFrames[2] = {419, 27, 93, 49};
    moveFrames[3] = {319, 27, 92, 49};
    moveFrames[4] = {218, 28, 92, 48};
    moveFrames[5] = {116, 28, 93, 48};
    moveFrames[6] = {14, 27, 93, 49};

    breakFrames[0] = {530, 288, 98, 60};
    breakFrames[1] = {425, 289, 97, 59};
    breakFrames[2] = {322, 291, 95, 57};
    breakFrames[3] = {221, 293, 94, 55};
    breakFrames[4] = {119, 295, 94, 53};
    breakFrames[5] = {18, 298, 94, 50};

    loadAnimation(0);
  }

  virtual ~EnemySub() {}

  void setTarget(DamagableEntity *p) override {
    if (p) {
      playerX = p->getPosX();
      playerY = p->getPosY();
    }
  }

  void launchRocket() {
    double mx = posX + (isFacingLeft ? 0.0 : getWidth());
    double my = posY + getHeight() * 0.5;
    double dx = playerX - mx;
    double dy = playerY - my;
    double dist = sqrt(dx * dx + dy * dy);

    double travelTime = dist / 6.0;
    if (travelTime < 15.0)
      travelTime = 15.0;

    double g = 0.15; // match Rocket.h
    double velX = dx / travelTime;
    double velY = (dy / travelTime) - (0.5 * g * travelTime);

    double speed = sqrt(velX * velX + velY * velY);
    double angle = atan2(velY, velX);

    pendingShot = new Rocket(mx, my, angle, false, speed, 15, true, false);
  }

  void die() override {
    Vehicle::die();
    // drop a collectible when destroyed
    pendingDrop =
        EnemyDropSystem::roll(posX + getWidth() / 2.0, posY + getHeight());
  }

  Entity *getPendingSpawnEntity() override { return pendingDrop; }
  void clearPendingSpawnEntity() override { pendingDrop = nullptr; }

  void fire() override {
    double dx = playerX - posX;
    double dy = playerY - posY;
    double dist = sqrt(dx * dx + dy * dy);
    if (dist <= 700.0 && fireCooldown <= 0) {
      launchRocket();
      fireCooldown = 2.5f;
    }
  }

  void move() override {
    if (abs(posX - playerX) > 960)
      return;

    double dx = playerX - posX;
    double dy = playerY - posY;

    if (dx > 10.0) {
      velocityX = 1 + (rand() % 2);
      velocityY = 1 + (rand() % 2);
      isFacingLeft = false;
    } else if (dx < -10.0) {
      velocityX = -(1 + (rand() % 2));
      velocityY = (-1 - (rand() % 2));
      isFacingLeft = true;
    } else {
      velocityX = 0;
    }

    double ceiling = playerY - 64.0;
    double floor = playerY + 64.0;

    // if heightOfPlane is too high (low Y), pull it down into the visible
    // range.
    if (posY < ceiling)
      posY = ceiling;
    if (posY > floor)
      posY = floor;
  }

  void loadAnimation(int dummy) override {
    if (vehicleTexture.loadFromFile("Sprites/Enemies/Enemy Submarine.png")) {
      vehicleSprite.setTexture(vehicleTexture);
      renderScaleX = -2.5f; 
      renderScaleY = 2.5f;
    }
    animDelay = 6.0f / 60.0f;
  }

  float getWidth() const override { return 93 * 2.5f; }
  float getHeight() const override { return 49 * 2.5f; }

  void update(Level *level, float dt) override {
    if (!isAlive)
      return;

    move();
    fire();

    if (!isBreaking && hp <= (maxHp * 0.3f)) {
      isBreaking = true;
      currentFrame = 0;
      animTimer = 0;
    }

    // animation logic moved to update
    int maxFrames = 7;
    float delay = 6.0f / 60.0f;

    if (isBreaking && !isBreakFinished) {
      maxFrames = 6;
      delay = 8.0f / 60.0f;
    }

    animTimer += dt;
    if (animTimer >= delay) {
      animTimer = 0;
      currentFrame++;
      if (currentFrame >= maxFrames) {
        if (isBreaking && !isBreakFinished) {
          currentFrame = maxFrames - 1;
          isBreakFinished = true;
        } else {
          currentFrame = 0;
        }
      }
    }

    EnemyVehicle::update(level, dt);
  }

  void draw(RenderWindow &window) override {
    if (!isAlive)
      return;

    const IntRect *frames = isBreaking ? breakFrames : moveFrames;
    IntRect rect = frames[currentFrame % (isBreaking ? 6 : 7)];
    vehicleSprite.setTextureRect(rect);

    // Dynamically center origin for varying frame sizes
    vehicleSprite.setOrigin(rect.width / 2.0f, rect.height / 2.0f);
    vehicleSprite.setPosition(posX + getWidth() / 2.0f,
                              posY + getHeight() / 2.0f);

    Vehicle::draw(window);
  }
};