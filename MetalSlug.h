#pragma once
#include "../../Character System/PlayerSoldier.h"
#include "../../Projectile System/Bullet.h"
#include "../GroundVehicle.h"

#include "../PlayerVehicle.h"

using namespace sf;
using namespace std;

// metalSlug

class MetalSlug : virtual public GroundVehicle, virtual public PlayerVehicle {
protected:
  IntRect moveFrames[8];
  IntRect shootFrames[10];
  IntRect jumpFrames[4];

  bool isFiring;
  bool isJumping;

public:
  MetalSlug(double posX, double posY)
      : Entity(posX, posY), DamagableEntity(posX, posY, 150),
        Vehicle(posX, posY, 150, "plains"), GroundVehicle(posX, posY, 150),
        PlayerVehicle(posX, posY, 150, "plains"), isFiring(false),
        isJumping(false) {
    setNoOfBlockClimb(1);
    gravity = 0.5f;
    jump_strength = 14;
    max_speed = 4.5f;

    // initialize frame arrays
    moveFrames[0] = {10, 305, 60, 56};
    moveFrames[1] = {75, 305, 60, 56};
    moveFrames[2] = {140, 305, 60, 56};
    moveFrames[3] = {205, 305, 60, 56};
    moveFrames[4] = {10, 366, 61, 56};
    moveFrames[5] = {76, 366, 61, 56};
    moveFrames[6] = {142, 366, 61, 56};
    moveFrames[7] = {208, 366, 61, 56};

    shootFrames[0] = {10, 440, 60, 56};
    shootFrames[1] = {75, 440, 60, 56};
    shootFrames[2] = {140, 440, 59, 56};
    shootFrames[3] = {204, 440, 59, 56};
    shootFrames[4] = {268, 441, 58, 55};
    shootFrames[5] = {331, 442, 58, 54};
    shootFrames[6] = {394, 443, 58, 53};
    shootFrames[7] = {457, 444, 59, 52};
    shootFrames[8] = {521, 443, 58, 53};
    shootFrames[9] = {584, 441, 58, 55};

    jumpFrames[0] = {10, 580, 55, 61};
    jumpFrames[1] = {70, 578, 55, 64};
    jumpFrames[2] = {130, 576, 53, 70};
    jumpFrames[3] = {188, 575, 53, 71};

    loadAnimation(0);
  }

  virtual ~MetalSlug() {}

  float getWidth() const override { return 62 * 2.5f; }
  float getHeight() const override { return 56 * 2.5f; }

  void loadAnimation(int dummy) override {
    if (vehicleTexture.loadFromFile("Sprites/Metal Slug 1.png")) {
      vehicleSprite.setTexture(vehicleTexture);
      renderScaleX = 2.5f;
      renderScaleY = 2.5f;
      vehicleSprite.setTextureRect(moveFrames[0]);
      vehicleSprite.setOrigin(30, 28); // Center of 60x56 frame
    }
    animDelay = 6.0f / 60.0f;
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

    bool jumpPressed = Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::X);
    if (jumpPressed && isOnGround) {
      velocityY = -jump_strength;
      isOnGround = false;
      isJumping = true;
    }

    if (Keyboard::isKeyPressed(Keyboard::Space) && fireCooldown <= 0) {
      double angle = isFacingLeft ? 3.14159265358979 : 0.0;
      double muzzleX = posX + (isFacingLeft ? 0.0 : getWidth());
      double muzzleY = posY + getHeight() * 0.4;
      pendingShot = new Bullet(muzzleX, muzzleY, angle, true);
      fireCooldown = 0.1f;
      isFiring = true;
    }
  }

  void update(Level *level, float dt) override {
    isFiring = false;
    isJumping = !isOnGround;
    PlayerVehicle::update(level, dt);

    // animation logic moved to update
    int maxFrames = 8;
    float delay = 6.0f / 60.0f;

    if (isJumping) {
      maxFrames = 4;
      delay = 8.0f / 60.0f;
    } else if (isFiring) {
      maxFrames = 10;
      delay = 4.0f / 60.0f;
    } else if (fabs(velocityX) < 0.01f) {
      maxFrames = 1; // idle
    }

    animTimer += dt;
    if (animTimer >= delay) {
      animTimer = 0;
      currentFrame = (currentFrame + 1) % maxFrames;
    }
  }

  void draw(RenderWindow &window) override {
    if (!isAlive)
      return;

    const IntRect *frames = moveFrames;
    int maxFrames = 8;
    if (isJumping) {
      frames = jumpFrames;
      maxFrames = 4;
    } else if (isFiring) {
      frames = shootFrames;
      maxFrames = 10;
    }

    IntRect frame = frames[currentFrame % maxFrames];
    vehicleSprite.setTextureRect(frame);

    // Apply scale and mirror based on facing direction
    float scaleX = renderScaleX;
    if (isFacingLeft) {
      scaleX = -renderScaleX;
    }
    vehicleSprite.setScale(scaleX, renderScaleY);

    // Center origin
    vehicleSprite.setOrigin(frame.width / 2.0f, frame.height / 2.0f);

    // Align the bottom of the sprite exactly flush with the bottom of the collision box
    float drawX = posX + getWidth() / 2.0f;
    float drawY = posY + getHeight() - (frame.height * renderScaleY) / 2.0f;
    vehicleSprite.setPosition(drawX, drawY);

    window.draw(vehicleSprite);
  }
};