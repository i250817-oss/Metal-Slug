#pragma once

using namespace std;



//enemy (abstract base)
//mirrors the Block hierarchy: Enemy → EnemyBase → ConcreteEnemy
//each enemy has a fixed pixel position (enemyX, enemyY) set at level load.
//aggregation: Level owns enemies — when Level dies, enemies die.
class Enemy {
protected:
    float enemyX;       //pixel X position (fixed, set per level)
    float enemyY;       //pixel Y position (fixed, set per level)
    bool  isAlive;
    char  enemyName[32];

    //only EnemyBase calls this
    Enemy(float x, float y, const char* name)
        : enemyX(x), enemyY(y), isAlive(true)
    {
        int i = 0;
        while (name[i] != '\0' && i < 31) { enemyName[i] = name[i]; i++; }
        enemyName[i] = '\0';
    }

public:
    virtual ~Enemy() {}

    //pure virtuals — every concrete enemy implements these
    virtual bool canFly()        const = 0;  //aerial movement
    virtual bool isUnderwater()  const = 0;  //aquatic movement
    virtual bool isVehicle()     const = 0;  //tank / sub / bradley
    virtual Enemy* clone()       const = 0;

    //getters
    float       getEnemyX()    const { return enemyX;    }
    float       getEnemyY()    const { return enemyY;    }
    bool        getIsAlive()   const { return isAlive;   }
    const char* getName()      const { return enemyName; }

    //setters
    void setEnemyX(float x)   { enemyX  = x; }
    void setEnemyY(float y)   { enemyY  = y; }
    void setIsAlive(bool a)   { isAlive = a; }
};