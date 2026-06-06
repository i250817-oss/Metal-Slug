#pragma once
#include "../../Character System/PlayerSoldier.h"
#include "../../Projectile System/Rocket.h"
#include "../PlayerVehicle.h"
#include "../AquaticVehicle.h"

using namespace sf;



//slugMariner
//vehicle. 3 missile types (3 each):
//Space          → horizontal straight missile
//Space + Up     → vertical upward missile
//Space + Down   → reverse arc missile (curves back to surface)
class SlugMariner : virtual public AquaticVehicle, virtual public PlayerVehicle {
protected:
  IntRect moveFrames[30];
  IntRect shootFrames[5];

  bool isFiring;
  float shootAnimTimer;

  int horizontalAmmo; //3
  int verticalAmmo;   //3
  int reverseAmmo;    //3

  static const float SHOOT_ANIM_DURATION;

public:
  SlugMariner(double posX, double posY)
      : Entity(posX, posY), DamagableEntity(posX, posY, 150),
        Vehicle(posX, posY, 150, "aquatic"), AquaticVehicle(posX, posY, 150), PlayerVehicle(posX, posY, 150, "aquatic"),
        isFiring(false), shootAnimTimer(0), horizontalAmmo(3),
        verticalAmmo(3), reverseAmmo(3) {
    max_speed = 6;

    //initialize frame arrays
    moveFrames[0] = {4, 4, 54, 52};
    moveFrames[1] = {61, 4, 54, 52};
    moveFrames[2] = {118, 4, 53, 52};
    moveFrames[3] = {174, 4, 53, 52};
    moveFrames[4] = {230, 4, 53, 52};
    moveFrames[5] = {286, 3, 54, 53};
    moveFrames[6] = {343, 3, 54, 53};
    moveFrames[7] = {400, 3, 54, 53};
    moveFrames[8] = {457, 3, 54, 53};
    moveFrames[9] = {514, 3, 55, 53};
    moveFrames[10] = {4, 59, 55, 53};
    moveFrames[11] = {62, 59, 55, 53};
    moveFrames[12] = {120, 59, 55, 52};
    moveFrames[13] = {178, 59, 55, 52};
    moveFrames[14] = {236, 59, 56, 52};
    moveFrames[15] = {295, 59, 56, 52};
    moveFrames[16] = {354, 58, 56, 53};
    moveFrames[17] = {413, 59, 57, 52};
    moveFrames[18] = {473, 58, 57, 52};
    moveFrames[19] = {4, 115, 57, 52};
    moveFrames[20] = {64, 115, 57, 51};
    moveFrames[21] = {124, 115, 57, 51};
    moveFrames[22] = {184, 115, 57, 51};
    moveFrames[23] = {244, 114, 57, 52};
    moveFrames[24] = {304, 114, 58, 52};
    moveFrames[25] = {365, 114, 58, 53};
    moveFrames[26] = {426, 114, 58, 53};
    moveFrames[27] = {487, 114, 59, 53};
    moveFrames[28] = {4, 170, 58, 53};
    moveFrames[29] = {65, 170, 59, 52};

    shootFrames[0] = {4, 226, 56, 51};
    shootFrames[1] = {63, 225, 56, 52};
    shootFrames[2] = {122, 228, 56, 49};
    shootFrames[3] = {181, 231, 56, 46};
    shootFrames[4] = {240, 234, 56, 43};

    loadAnimation(0);
  }

  virtual ~SlugMariner() {}

  float getWidth() const override { return 59 * 2.5f; }
  float getHeight() const override { return 53 * 2.5f; }

  int getTotalAmmo() const {
    return horizontalAmmo + verticalAmmo + reverseAmmo;
  }

  void fireHorizontal() {
    if (horizontalAmmo <= 0 || !getIsOccupied())
      return;
    double angle = isFacingLeft ? 3.14159265358979 : 0.0;
    double muzzleX = posX + (isFacingLeft ? 0.0 : getWidth());
    double muzzleY = posY + getHeight() * 0.5;
    pendingShot =
        new Rocket(muzzleX, muzzleY, angle, true, 15, 5, false, true);
    horizontalAmmo--;
    isFiring = true;
    shootAnimTimer = SHOOT_ANIM_DURATION;
    currentFrame = 0;
  }

  void fireVertical() {
    if (verticalAmmo <= 0 || !getIsOccupied())
      return;
    double angle = -1.5707963267948966; //straight up
    double muzzleX = posX + getWidth() * 0.5;
    double muzzleY = posY;
    pendingShot =
        new Rocket(muzzleX, muzzleY, angle, true, 15, 5, false, true);
    verticalAmmo--;
    isFiring = true;
    shootAnimTimer = SHOOT_ANIM_DURATION;
    currentFrame = 0;
  }

  void fireReverse() {
    if (reverseAmmo <= 0 || !getIsOccupied())
      return;
    double angle = isFacingLeft ? (3.14159265358979 + 0.7854) : (-0.7854);
    double muzzleX = posX + getWidth() * 0.5;
    double muzzleY = posY;
    pendingShot =
        new Rocket(muzzleX, muzzleY, angle, true, 15, 5, true, true);
    reverseAmmo--;
    isFiring = true;
    shootAnimTimer = SHOOT_ANIM_DURATION;
    currentFrame = 0;
  }

  void loadAnimation(int dummy) override {
    if (vehicleTexture.loadFromFile("Sprites/Slug Mariner.png")) {
      vehicleSprite.setTexture(vehicleTexture);
      renderScaleX = 2.5f;
      renderScaleY = 2.5f;
      vehicleSprite.setOrigin(27, 26); // Center of 54x52 frame
    }
    animDelay = 4.0f / 60.0f;
  }

  void handleDriverInput() override {
    bool upHeld = Keyboard::isKeyPressed(Keyboard::Up);
    bool downHeld = Keyboard::isKeyPressed(Keyboard::Down);

    if (Keyboard::isKeyPressed(Keyboard::Right)) {
      velocityX = max_speed;
      isFacingLeft = false;
    } else if (Keyboard::isKeyPressed(Keyboard::Left)) {
      velocityX = -max_speed;
      isFacingLeft = true;
    } else {
      velocityX = 0;
    }

    if (upHeld)
      velocityY = -max_speed;
    else if (downHeld)
      velocityY = max_speed;
    else
      velocityY = 0;

    if (Keyboard::isKeyPressed(Keyboard::Space) &&
        fireCooldown <= 0) {
      if (upHeld)
        fireVertical();
      else if (downHeld)
        fireReverse();
      else
        fireHorizontal();
      fireCooldown = 0.5f;
    }
  }

  void update(Level *level, float dt) override {
    isFiring = false;
    PlayerVehicle::update(level, dt);

    //animation logic moved to update
    int maxFrames = 30;
    float delay = 4.0f / 60.0f;

    if (shootAnimTimer > 0) {
      shootAnimTimer -= dt;
      maxFrames = 5;
      delay = 5.0f / 60.0f;
    }

    animTimer += dt;
    if (animTimer >= delay) {
      animTimer = 0;
      currentFrame = (currentFrame + 1) % maxFrames;
    }
  }

  void draw(RenderWindow& window) override {
    if (!isAlive)
      return;

    const IntRect *frames = (shootAnimTimer > 0) ? shootFrames : moveFrames;
    IntRect frame = frames[currentFrame % (shootAnimTimer > 0 ? 5 : 30)];
    vehicleSprite.setTextureRect(frame);

    // Apply scale and mirror based on facing direction
    float scaleX = renderScaleX;
    if (isFacingLeft) {
      scaleX = -renderScaleX;
    }
    vehicleSprite.setScale(scaleX, renderScaleY);

    // Center origin
    vehicleSprite.setOrigin(frame.width / 2.0f, frame.height / 2.0f);
    vehicleSprite.setPosition(posX + getWidth() / 2.0f, posY + getHeight() / 2.0f);

    window.draw(vehicleSprite);
  }
};
