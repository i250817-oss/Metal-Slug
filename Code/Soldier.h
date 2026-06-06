#pragma once
#include "../Entity Root/DamagableEntity.h"
#include "../Level and Environment System/Level.h"

// forward declarations
class Level;
class TransformationState;

class Soldier : public DamagableEntity {
protected:
  bool isCrouching;
  TransformationState *currentTransState;

  bool upWasPressed;
  float baseMaxSpeed;

public:
  Soldier(double posX, double posY, int maxHp)
      : DamagableEntity(posX, posY, maxHp), isCrouching(false),
        currentTransState(nullptr), upWasPressed(false), baseMaxSpeed(5) {}

  void onCollideWith(Entity *other, float dt) override {
    other->resolveCollision(this, dt);
  }

  virtual ~Soldier();

  
  bool getIsCrouching() const { return isCrouching; }
  void setIsOnGround(bool v) { isOnGround = v; }
  void setMaxSpeed(float v) { max_speed = v; }
  float getMaxSpeed() const { return max_speed; }
  virtual void restoreBaseMaxSpeed() { max_speed = baseMaxSpeed; }

  
  void changeTransState(TransformationState *newState);
  TransformationState *getTransState() const { return currentTransState; }
  const char *getTransStateName() const;

  
  virtual void handleInput() {
    
    bool wantCrouch = Keyboard::isKeyPressed(Keyboard::Down);
    if (wantCrouch != isCrouching) {
      float oldH = getHeight();
      isCrouching = wantCrouch;
      float newH = getHeight();
      posY += (oldH - newH); 
    }


    if (Keyboard::isKeyPressed(Keyboard::Right)) {
      moveRight();
      if (isCrouching && velocityX > max_speed * 0.5f) {
        velocityX = max_speed * 0.5f;
      }
    } else if (Keyboard::isKeyPressed(Keyboard::Left)) {
      moveLeft();
      if (isCrouching && velocityX < -max_speed * 0.5f) {
        velocityX = -max_speed * 0.5f;
      }
    } else {
      stopX();
    }

    
    bool jumpNow = Keyboard::isKeyPressed(Keyboard::X);
    if (jumpNow) {
      if (isOnGround || !upWasPressed)
        jump();
    }
    upWasPressed = jumpNow;
  }

  virtual void crouch() { isCrouching = true; }

  // lifecycle
  virtual void die() override = 0;
  virtual void update(Level *level, float dt) override = 0;
  virtual void draw(RenderWindow &window) override {}
};

#include "TransformationState.h"

inline Soldier::~Soldier() {
  delete currentTransState;
  currentTransState = nullptr;
}

inline void Soldier::changeTransState(TransformationState *newState) {
  if (currentTransState) {
    currentTransState->exit(this);
    delete currentTransState;
  }
  currentTransState = newState;
  if (currentTransState)
    currentTransState->enter(this);
}

inline const char *Soldier::getTransStateName() const {
  return currentTransState ? currentTransState->getStateName() : "Normal";
}