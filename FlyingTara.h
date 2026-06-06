#pragma once
#include "../../Character System/PlayerSoldier.h"
#include "../../Collectible and Interactable System/EnemyDropSystem.h"
#include "../../Projectile System/Grenade.h"
#include "../../Projectile System/Rocket.h"
#include "../Vehicle.h"

#include "../AerialVehicle.h"
#include "../EnemyVehicle.h"

using namespace sf;
using namespace std;



//flyingTara

class FlyingTara : public AerialVehicle, public EnemyVehicle {
protected:
  IntRect flyFrames[6];

  double heightOfPlane;
  bool isOverPlayer;
  int spawnBatch;
  double playerX, playerY;
  Entity* pendingDrop;

public:
  FlyingTara(double posX, double posY)
      : Entity(posX, posY), DamagableEntity(posX, posY, 8),
        Vehicle(posX, posY, 8, "aerial"), AerialVehicle(posX, posY, 8), EnemyVehicle(posX, posY, 8, "aerial"), heightOfPlane(200.0),
      isOverPlayer(false), spawnBatch(2), playerX(400.0), playerY(0.0), pendingDrop(nullptr) {
    fireCooldown = 0;
    gravity = 0;
    max_speed = 2.5f;
    entityType = ENTITY_VEHICLE;

    //initialize frame arrays
    flyFrames[0] = {3, 106, 80, 37};
    flyFrames[1] = {86, 108, 80, 41};
    flyFrames[2] = {3, 155, 80, 46};
    flyFrames[3] = {86, 157, 80, 42};
    flyFrames[4] = {3, 204, 80, 43};
    flyFrames[5] = {86, 206, 80, 41};

    loadAnimation(0);
  }

  virtual ~FlyingTara() {}

  void setHeightOfPlane(double v) { heightOfPlane = v; }
  double getHeightOfPlane() const { return heightOfPlane; }
  void setIsOverPlayer(bool v) { isOverPlayer = v; }
  bool getIsOverPlayer() const { return isOverPlayer; }

  bool isAerialKill() const override { return true; }

  void setTarget(DamagableEntity *p) override {
    if (p) {
      playerX = p->getPosX();
      playerY = p->getPosY();
    }
  }

  void fly() {
    if (abs(posX - playerX) > 960)
      return;
    if (posX < playerX - 5.0) {
      velocityX = 1 + rand() % 2;
      velocityY = 1 + rand() % 2;
      isFacingLeft = false;
    } else if (posX > playerX + 5.0) {
      velocityX = -1 - rand() % 2;
      velocityY = -1 - rand() % 2;
      isFacingLeft = true;
    } else {
      velocityX = 0;
      isOverPlayer = true;
    }

    //ensure visibility: Keep between 3 and 7 blocks above player.
    
    double ceiling = playerY - 512.0;
    double floor = playerY - 192.0;

    //if heightOfPlane is too high (low Y), pull it down into the visible
    //range.
    if (posY < ceiling)
      posY = ceiling;
    if (posY > floor)
      posY = floor;

    posX += velocityX;
    posY += velocityY;
  }

  void dropGrenade() {
    if (fireCooldown > 0)
      return;
    isOverPlayer = false;
    double angle = 1.5707963267948966;
    double mx = posX + getWidth() * 0.5;
    double my = posY + getHeight();
    pendingShot = new Grenade(mx, my, angle, 5, 96, false);
    fireCooldown = 2;
  }

  void die() override {
    Vehicle::die();
    //50% chance to drop a collectible from the sky
    if (rand() % 100 < 50)
        pendingDrop = EnemyDropSystem::roll(posX + getWidth()/2.0, posY + getHeight());
  }

  Entity* getPendingSpawnEntity() override { return pendingDrop; }
  void    clearPendingSpawnEntity() override { pendingDrop = nullptr; }

  void fire() override {
    if (isOverPlayer)
      dropGrenade();
  }
  void move() override { fly(); }

  void loadAnimation(int dummy) override {
    if (vehicleTexture.loadFromFile(
            "Sprites/Enemies/Enemy - Flying Tara.png")) {
      vehicleSprite.setTexture(vehicleTexture);
      renderScaleX = 2.5f; // Natively faces right
      renderScaleY = 2.5f;
    }
    animDelay = 6.0f / 60.0f;
  }

  float getWidth() const override { return 80 * 2.5f; }
  float getHeight() const override { return 46 * 2.5f; }

  void update(Level *level, float dt) override {
    if (!isAlive) return;
    move();
    fire();

    //animation logic moved to update
    animTimer += dt;
    if (animTimer >= animDelay) {
      animTimer = 0;
      currentFrame = (currentFrame + 1) % 6;
    }

    EnemyVehicle::update(level, dt);
  }

  void draw(RenderWindow &window) override {
    if (!isAlive)
      return;

    IntRect rect = flyFrames[currentFrame % 6];
    vehicleSprite.setTextureRect(rect);
    
    // Dynamically center origin for varying frame sizes
    vehicleSprite.setOrigin(rect.width / 2.0f, rect.height / 2.0f);
    vehicleSprite.setPosition(posX + getWidth() / 2.0f, posY + getHeight() / 2.0f);

    Vehicle::draw(window);
  }
};