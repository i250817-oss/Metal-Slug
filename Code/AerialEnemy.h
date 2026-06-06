#pragma once
#include "../Enemy.h"

using namespace sf;



//abstract aerial enemy base.
class AerialEnemy : public Enemy {
private:
    float altitude;
    float descentSpeed;

public:
    AerialEnemy(double posX, double posY, int maxHp, const char* type);
    virtual ~AerialEnemy();

    void setAltitude(float v);
    float getAltitude() const;
    void setDescentSpeed(float v);
    float getDescentSpeed() const;

    virtual void descend() = 0;
    virtual void attack(float dt) override = 0;
    virtual void move(float dt) override;
    virtual void update(Level* level, float dt) override;
    virtual void draw(RenderWindow& window) override;
};