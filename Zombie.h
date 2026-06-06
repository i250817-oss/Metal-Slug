#pragma once
#include "UndeadEnemy.h"
#include "../../Weapon System/FireArms/Pistol.h"

using namespace sf;



//zombie
//5 HP. Slow. Transforms player on touch. Has a pistol. Batches 3-5.
//sprite: "Sprites/Enemies/Zombies.png"  (left-to-right frames)
class Zombie : public UndeadEnemy {
private:
    Weapon *weapon;

    IntRect idleFrames[8];    //basic moving/idle (left-to-right)
    IntRect chaseFrames[11];  //chasing player (left-to-right)
    int numIdleFrames, numChaseFrames;

    // Death animation
    IntRect deathFrames[14];
    bool isDying;
    int deathFrame;
    float deathAnimTimer;

public:
    Zombie(double posX, double posY)
        : Entity(posX, posY), DamagableEntity(posX, posY, 5), UndeadEnemy(posX, posY, 5)
        , weapon(nullptr), numIdleFrames(8), numChaseFrames(11)
    {
        isDying = false;
        deathFrame = 0;
        deathAnimTimer = 0.0f;

        deathFrames[0] = IntRect(5, 270, 30, 41);
        deathFrames[1] = IntRect(44, 272, 37, 39);
        deathFrames[2] = IntRect(93, 274, 37, 37);
        deathFrames[3] = IntRect(143, 276, 36, 35);
        deathFrames[4] = IntRect(187, 277, 31, 34);
        deathFrames[5] = IntRect(225, 277, 28, 34);
        deathFrames[6] = IntRect(255, 278, 27, 33);
        deathFrames[7] = IntRect(289, 281, 28, 30);
        deathFrames[8] = IntRect(323, 284, 30, 27);
        deathFrames[9] = IntRect(357, 288, 29, 23);
        deathFrames[10] = IntRect(394, 293, 33, 18);
        deathFrames[11] = IntRect(437, 294, 34, 17);
        deathFrames[12] = IntRect(6, 320, 36, 17);
        deathFrames[13] = IntRect(48, 320, 34, 17);

        walkSpeed = 1;
        runSpeed  = 1.5f;

        enemyTag  = "zombie";
        weapon    = new Pistol();

        addState(AI_STATE_PATROL, new PatrolState());
        addState(AI_STATE_CHASE,  new ChaseState());
        addState(AI_STATE_ATTACK, new ZombieHybridState());
        changeState(AI_STATE_PATROL);

        if (enemyTexture.loadFromFile("Sprites/Enemies/Zombies.png")) {
            enemySprite.setTexture(enemyTexture);
            enemySprite.setScale(3, 3);
        }
        animDelay = 8.0f / 60.0f;

        //idle / Moving (left-to-right — sprite faces right by default)
        idleFrames[0] = IntRect(105, 45, 32, 40);
        idleFrames[1] = IntRect(215,  3, 40, 40);
        idleFrames[2] = IntRect(173,  3, 40, 40);
        idleFrames[3] = IntRect(139,  3, 32, 40);
        idleFrames[4] = IntRect(105,  3, 32, 40);
        idleFrames[5] = IntRect( 71,  3, 32, 40);
        idleFrames[6] = IntRect( 37,  3, 32, 40);
        idleFrames[7] = IntRect(  3,  3, 32, 40);

        //chase / running toward player (left-to-right)
        chaseFrames[0]  = IntRect(439, 121, 32, 48);
        chaseFrames[1]  = IntRect(405, 121, 32, 48);
        chaseFrames[2]  = IntRect(363, 129, 40, 40);
        chaseFrames[3]  = IntRect(321, 129, 40, 40);
        chaseFrames[4]  = IntRect(271, 129, 48, 40);
        chaseFrames[5]  = IntRect(221, 129, 48, 40);
        chaseFrames[6]  = IntRect(171, 129, 48, 40);
        chaseFrames[7]  = IntRect(121, 129, 48, 40);
        chaseFrames[8]  = IntRect( 71, 129, 48, 40);
        chaseFrames[9]  = IntRect( 37, 129, 32, 40);
        chaseFrames[10] = IntRect(  3, 129, 32, 40);

        enemySprite.setTextureRect(idleFrames[0]);
    }

    ~Zombie() { delete weapon; }
    int getScoreValue() const override { return 100; }

    float getWidth()  const override { return 48 * 3; }
    float getHeight() const override { return 48 * 3; }

    void meleeDamagePlayer() override { transformPlayer(); }

    void fireWeapon() override {
        if (!weapon || !target) return;
        double muzzleX = isFacingLeft ? posX : (posX + getWidth());
        double muzzleY = posY + getHeight() * 0.3;
        double angle = isFacingLeft ? M_PI : 0.0;
        pendingShot = weapon->fire(muzzleX, muzzleY, angle, false);
    }

    bool getIsDying() const override { return isDying; }

    void takeDamage(int val) override {
        if (isDying) return;
        if (hp - val > 0) {
            UndeadEnemy::takeDamage(val);
        } else {
            hp = 0;
            isDying = true;
            deathFrame = 0;
            deathAnimTimer = 0.0f;
            velocityX = 0;
            velocityY = 0;
            playDeathSound();
        }
    }

    void update(Level* level, float dt) override {
        if (!isAlive) return;

        if (isDying) {
            deathAnimTimer += dt;
            float frameDelay = 6.0f / 60.0f;
            if (deathAnimTimer >= frameDelay) {
                deathAnimTimer = 0;
                deathFrame++;
            }
            if (deathFrame >= 14) {
                isDying = false;
                UndeadEnemy::takeDamage(9999);
                return;
            }
            applyPhysics();
            tileCollisionImpl(level);
            enemySprite.setTexture(enemyTexture);
            enemySprite.setTextureRect(deathFrames[deathFrame]);
            return;
        }

        //animation logic moved to update
        IntRect* frames = idleFrames;
        int maxFrames   = numIdleFrames;
        float delay     = 8.0f / 60.0f;

        if (currentAIState && (currentAIState->getID() == AI_STATE_CHASE ||
                               currentAIState->getID() == AI_STATE_ATTACK)) {
            frames = chaseFrames; maxFrames = numChaseFrames; delay = 6.0f / 60.0f;
        }

        if (currentAIState && currentAIState->getAnimCycleDuration() > 0) {
            delay = currentAIState->getAnimCycleDuration() / maxFrames;
        }

        animTimer += dt;
        if (animTimer >= delay) {
            animTimer = 0; 
            currentFrame = (currentFrame + 1) % maxFrames; 
        }
        enemySprite.setTextureRect(frames[currentFrame % maxFrames]);

        if (weapon) weapon->tickCooldown(dt);

        UndeadEnemy::update(level, dt);
    }

    void draw(RenderWindow& window) override {
        UndeadEnemy::draw(window);
    }
};