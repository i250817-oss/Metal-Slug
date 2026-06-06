#pragma once
#include "Vehicle.h"



//abstract — vehicles that operate primarily on ground.
class GroundVehicle : virtual public Vehicle {
private:
  int noOfBlockClimb; //can traverse this many block height differences

public:
  GroundVehicle() : Entity(), DamagableEntity(0,0,0), Vehicle(), noOfBlockClimb(1) {}
  GroundVehicle(double posX, double posY, int maxHp)
      : Entity(posX, posY), DamagableEntity(posX, posY, maxHp), Vehicle(posX, posY, maxHp, "plains"), noOfBlockClimb(1) {
    this->posX = posX;
    this->posY = posY;
    this->hp = maxHp;
    this->maxHp = maxHp;
  }

  virtual ~GroundVehicle() {}

  void setNoOfBlockClimb(int v) { noOfBlockClimb = v; }
  int getNoOfBlockClimb() const { return noOfBlockClimb; }

  virtual void loadAnimation(int type) override = 0;

public:
  void update(Level *level, float dt) override {
    Vehicle::update(level, dt);
  }
};