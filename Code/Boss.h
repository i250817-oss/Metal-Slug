#pragma once
#include "../Enemy.h"

using namespace sf;




class Boss : virtual public Enemy {
protected:
    bool hasRetreated;
    double retreatHpThreshold;
    int miniBatchMin;
    int miniBatchMax;
    
    bool isRetreating;
    double retreatDestX;

public:
    Boss() : Entity(), DamagableEntity(0,0,0), Enemy(0,0,0), hasRetreated(false), retreatHpThreshold(0.5), miniBatchMin(2), miniBatchMax(4), isRetreating(false), retreatDestX(0) {}
    Boss(int maxHp, double posX, double posY, const char* type)
        : Entity(posX, posY), DamagableEntity(posX, posY, maxHp), Enemy(posX, posY, maxHp)
        , hasRetreated(false)
        , retreatHpThreshold(0.5)
        , miniBatchMin(2)
        , miniBatchMax(4)
        , isRetreating(false)
        , retreatDestX(0)
    {
        this->posX = posX;
        this->posY = posY;
        this->hp = maxHp;
        this->maxHp = maxHp;
        enemyTag = type;
    }
    virtual ~Boss() {}
    
    bool isEnemy() const override { return true; }

    bool getIsRetreating() const { return isRetreating; }
    void setRetreatDest(double x) { isRetreating = true; retreatDestX = x; }

    bool shouldRetreat() const {
        return !hasRetreated && !isRetreating && ((double)hp / maxHp <= retreatHpThreshold);
    }

    virtual void retreat() = 0;
    virtual void spawnMinions() = 0;
    virtual void specialAttack() = 0;

    virtual void attack(float dt) override = 0;
    virtual void move(float dt) override = 0;
    void update(Level* level, float dt) override = 0;
    virtual void draw(RenderWindow& window) override = 0;
};