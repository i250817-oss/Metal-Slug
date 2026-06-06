#pragma once
#include "Vehicle.h"



//abstract — vehicles that fly.
class AerialVehicle : virtual public Vehicle {
private:
  double altitude;
  bool isAboveWater;

public:
  AerialVehicle() : Entity(), DamagableEntity(0,0,0), Vehicle(), altitude(0.0), isAboveWater(false) {
    gravity = 0;
  }

public:
  AerialVehicle(double posX, double posY, int maxHp)
      : Entity(posX, posY), DamagableEntity(posX, posY, maxHp), Vehicle(posX, posY, maxHp, "aerial"), altitude(0.0),
        isAboveWater(false) {
    this->posX = posX;
    this->posY = posY;
    this->hp = maxHp;
    this->maxHp = maxHp;
    gravity = 0;
  }

  virtual ~AerialVehicle() {}

  void setAltitude(double v) { altitude = v; }
  double getAltitude() const { return altitude; }
  void setIsAboveWater(bool v) { isAboveWater = v; }
  bool getIsAboveWater() const { return isAboveWater; }

  virtual void loadAnimation(int type) override = 0;

  void update(Level *level, float dt) override {
    Vehicle::update(level, dt);
  }
};