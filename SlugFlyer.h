#pragma once
#include "../../Character System/PlayerSoldier.h"
#include "../../Projectile System/Bullet.h"
#include "../../Projectile System/Rocket.h"
#include "../AerialVehicle.h"
#include "../PlayerVehicle.h"

using namespace sf;

// slugFlyer

class SlugFlyer : virtual public AerialVehicle, virtual public PlayerVehicle {
protected:
  IntRect flyFrames[19];
  int missileNum;
  bool isFiring;

public:
  SlugFlyer(double posX, double posY)
      : Entity(posX, posY), DamagableEntity(posX, posY, 120),
        Vehicle(posX, posY, 120, "aerial"), AerialVehicle(posX, posY, 120),
        PlayerVehicle(posX, posY, 120, "aerial"), missileNum(4),
        isFiring(false) {
    max_speed = 3;
    renderScaleX = 2.5f;
    renderScaleY = 2.5f;

    // initialize frame arrays
    flyFrames[0] = {4, 116, 80, 46};
    flyFrames[1] = {87, 115, 80, 48};
    flyFrames[2] = {170, 114, 80, 50};
    flyFrames[3] = {253, 113, 80, 52};
    flyFrames[4] = {336, 113, 80, 52};
    flyFrames[5] = {4, 170, 79, 48};
    flyFrames[6] = {86, 169, 79, 50};
    flyFrames[7] = {168, 168, 79, 52};
    flyFrames[8] = {250, 168, 79, 53};
    flyFrames[9] = {332, 168, 79, 53};
    flyFrames[10] = {4, 227, 79, 50};
    flyFrames[11] = {86, 225, 79, 53};
    flyFrames[12] = {250, 224, 79, 54};
    flyFrames[13] = {332, 224, 79, 54};
    flyFrames[14] = {4, 285, 77, 53};
    flyFrames[15] = {84, 283, 77, 56};
    flyFrames[16] = {164, 282, 77, 58};
    flyFrames[17] = {244, 283, 77, 56};
    flyFrames[18] = {324, 283, 77, 55};

    loadAnimation(0);
  }

  virtual ~SlugFlyer() {}

  float getWidth() const override { return 80 * 2.5f; }
  float getHeight() const override { return 58 * 2.5f; }

  int getMissileNum() const { return missileNum; }
  bool getIsFiring() const { return isFiring; }

  void launchMissile() {
    if (missileNum <= 0 || !getIsOccupied())
      return;
    double angle = isFacingLeft ? 3.14159265358979 : 0.0;
    double muzzleX = posX + (isFacingLeft ? 0.0 : getWidth());
    double muzzleY = posY + getHeight() * 0.5;
    pendingShot = new Rocket(muzzleX, muzzleY, angle, true, 15, 5, 9, true);
    missileNum--;
  }

  void loadAnimation(int a) override {
    if (vehicleTexture.loadFromFile("Sprites/Slug Flyer.png")) {
      vehicleSprite.setTexture(vehicleTexture);
    }
    animDelay = 4.0f / 60.0f;
  }

  void handleDriverInput() override {
    if (Keyboard::isKeyPressed(Keyboard::Right)) {
      velocityX = max_speed;
      isFacingLeft = false;
    } else if (Keyboard::isKeyPressed(Keyboard::Left)) {
      velocityX = -max_speed;
      isFacingLeft = true;
    } else {
      velocityX = 0;
    }

    if (Keyboard::isKeyPressed(Keyboard::Up))
      velocityY = -max_speed;
    else if (Keyboard::isKeyPressed(Keyboard::Down))
      velocityY = max_speed;
    else
      velocityY = 0;

    if (Keyboard::isKeyPressed(Keyboard::Space) && fireCooldown <= 0) {
      double angle = isFacingLeft ? 3.14159265358979 : 0.0;
      double muzzleX = posX + (isFacingLeft ? 0.0 : getWidth());
      double muzzleY = posY + getHeight() * 0.3;
      pendingShot = new Bullet(muzzleX, muzzleY, angle, true);
      fireCooldown = 0.2f;
      isFiring = true;
    }

    if (Keyboard::isKeyPressed(Keyboard::LShift))
      launchMissile();
  }

  void update(Level *level, float dt) override {
    isFiring = false;
    PlayerVehicle::update(level, dt);
    if (posX < 0.0)
      posX = 0.0;
    if (posY < 0.0)
      posY = 0.0;

    // animation logic moved to update
    animTimer += dt;
    if (animTimer >= animDelay) {
      animTimer = 0;
      currentFrame = (currentFrame + 1) % 19;
    }
  }

  void draw(RenderWindow &window) override {
    if (!isAlive)
      return;

    IntRect frame = flyFrames[currentFrame % 19];
    vehicleSprite.setTextureRect(frame);

    // Apply scale and mirror based on facing direction
    float scaleX = renderScaleX;
    if (isFacingLeft) {
      scaleX = -renderScaleX;
    }
    vehicleSprite.setScale(scaleX, renderScaleY);

    // Center origin
    vehicleSprite.setOrigin(frame.width / 2.0f, frame.height / 2.0f);
    vehicleSprite.setPosition(posX + getWidth() / 2.0f,
                              posY + getHeight() / 2.0f);

    window.draw(vehicleSprite);
  }
};