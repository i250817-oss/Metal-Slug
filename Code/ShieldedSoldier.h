#pragma once
#include "InfantryEnemy.h"
#include "../../Weapon System/FireArms/Pistol.h"

using namespace sf;



//shieldedSoldier
//5 HP. Riot shield blocks frontal bullets. Must hit from above/behind/explosive.
//sprite: "Sprites/Enemies/Rebel Soldier (Shield).png" (right-to-left)
class ShieldedSoldier : public InfantryEnemy {
private:
    bool hasShield;
    bool shieldIsActive;  //toggled by ShieldedMarchState to lower/raise shield

    IntRect moveFrames[11];
    IntRect jumpFrames[10];
    IntRect attackFrames[10];
    int numMoveFrames, numJumpFrames, numAttackFrames;

    // Death animation
    IntRect deathFrames[11];
    bool isDying;
    int deathFrame;
    float deathAnimTimer;

public:
    ShieldedSoldier(double posX, double posY)
        : Entity(posX, posY), DamagableEntity(posX, posY, 5), InfantryEnemy(posX, posY, 5)
        , hasShield(true)
        , shieldIsActive(true)
        , numMoveFrames(11), numJumpFrames(10), numAttackFrames(10)
    {
        isDying = false;
        deathFrame = 0;
        deathAnimTimer = 0.0f;

        deathFrames[0]  = IntRect(5, 10297, 26, 37);
        deathFrames[1]  = IntRect(36, 10297, 25, 37);
        deathFrames[2]  = IntRect(66, 10298, 24, 36);
        deathFrames[3]  = IntRect(95, 10299, 24, 35);
        deathFrames[4]  = IntRect(124, 10300, 27, 34);
        deathFrames[5]  = IntRect(156, 10301, 27, 33);
        deathFrames[6]  = IntRect(188, 10303, 27, 31);
        deathFrames[7]  = IntRect(220, 10307, 27, 27);
        deathFrames[8]  = IntRect(252, 10312, 26, 22);
        deathFrames[9]  = IntRect(283, 10316, 28, 18);
        deathFrames[10] = IntRect(316, 10316, 29, 18);

        walkSpeed = 2;
        runSpeed  = 3.5f;
        enemyTag  = "shielded";
        setWeapon(new Pistol());

        addState(AI_STATE_PATROL,  new PatrolState());
        addState(AI_STATE_CHASE,   new ChaseState());
        addState(AI_STATE_ATTACK,  new ShieldedMarchState());
        addState(AI_STATE_RETREAT, new RetreatState());
        addState(AI_STATE_MELEE,   new MeleeState());
        changeState(AI_STATE_PATROL);

        if (enemyTexture.loadFromFile("Sprites/Enemies/Rebel Soldier (Shield).png")) {
            enemySprite.setTexture(enemyTexture);
            enemySprite.setScale(3, 3);
        }
        animDelay = 5.0f / 60.0f;

        //moving (11 frames, right-to-left)
        moveFrames[0]  = IntRect(359, 48, 27, 39);
        moveFrames[1]  = IntRect(329, 49, 27, 38);
        moveFrames[2]  = IntRect(267, 46, 28, 40);
        moveFrames[3]  = IntRect(235, 46, 29, 41);
        moveFrames[4]  = IntRect(202, 47, 30, 40);
        moveFrames[5]  = IntRect(167, 48, 32, 39);
        moveFrames[6]  = IntRect(133, 49, 31, 38);
        moveFrames[7]  = IntRect( 99, 47, 31, 39);
        moveFrames[8]  = IntRect( 66, 46, 30, 40);
        moveFrames[9]  = IntRect( 35, 46, 28, 41);
        moveFrames[10] = IntRect(  3, 47, 29, 40);

        //jump (10 frames)
        jumpFrames[0] = IntRect(338, 180, 30, 38);
        jumpFrames[1] = IntRect(303, 180, 32, 38);
        jumpFrames[2] = IntRect(266, 179, 34, 39);
        jumpFrames[3] = IntRect(227, 177, 36, 38);
        jumpFrames[4] = IntRect(188, 175, 36, 38);
        jumpFrames[5] = IntRect(148, 173, 37, 42);
        jumpFrames[6] = IntRect(109, 172, 36, 46);
        jumpFrames[7] = IntRect( 71, 175, 35, 43);
        jumpFrames[8] = IntRect( 36, 180, 32, 38);
        jumpFrames[9] = IntRect(  3, 180, 30, 38);

        //attack/Shooting (10 frames)
        attackFrames[0] = IntRect(  3, 305, 30, 38);
        attackFrames[1] = IntRect( 36, 305, 28, 38);
        attackFrames[2] = IntRect( 67, 305, 31, 38);
        attackFrames[3] = IntRect(101, 305, 31, 38);
        attackFrames[4] = IntRect(135, 305, 38, 38);
        attackFrames[5] = IntRect(176, 305, 37, 38);
        attackFrames[6] = IntRect(216, 301, 35, 42);
        attackFrames[7] = IntRect(254, 305, 34, 38);
        attackFrames[8] = IntRect(291, 305, 35, 38);
        attackFrames[9] = IntRect(329, 302, 37, 41);

        enemySprite.setTextureRect(moveFrames[0]);
    }

    ~ShieldedSoldier() {}
    int getScoreValue() const override { return 75; }

    float getWidth()  const override { return 38 * 3; }
    float getHeight() const override { return 46 * 3; }

    bool getHasShield()   const { return hasShield; }
    bool getShieldActive() const { return hasShield && shieldIsActive; }

    //called by ShieldedMarchState to lower/raise the shield each firing cycle
    void setShieldActive(bool v) override { shieldIsActive = v; }

    bool getIsDying() const override { return isDying; }

    void takeDamage(int val) override {
        if (isDying) return;
        if (lastDamageSourceX >= 0 && hasShield && shieldIsActive
            && !(currentAIState && currentAIState->getID() == AI_STATE_ATTACK)) {
            float centre = posX + getWidth() / 2;
            bool attackFromRight = (lastDamageSourceX > centre);
            bool shieldFacesRight = !isFacingLeft;
            if (attackFromRight == shieldFacesRight) return;
        }
        if (hp - val > 0) {
            DamagableEntity::takeDamage(val);
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
            if (deathFrame >= 11) {
                isDying = false;
                InfantryEnemy::takeDamage(9999);
                return;
            }
            applyPhysics();
            tileCollisionImpl(level);
            enemySprite.setTexture(retreatTexture);
            enemySprite.setTextureRect(deathFrames[deathFrame]);
            return;
        }

        tickFiringAnim(dt);


        // retreat animation overrides everything else
        if (!applyRetreatAnim(dt)) {

        // attack frames only while actively firing, not during cooldown
        enemySprite.setTexture(enemyTexture);   // ensure we're back on our own sheet
        IntRect* frames = moveFrames;
        int maxFrames   = numMoveFrames;
        float delay     = 5.0f / 60.0f;

        if (!isOnGround) {
            frames = jumpFrames; maxFrames = numJumpFrames; delay = 4.0f / 60.0f;
        } else if (isFiringAnim) {
            frames = attackFrames; maxFrames = numAttackFrames; delay = 6.0f / 60.0f;
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

        } // end !applyRetreatAnim

        InfantryEnemy::update(level, dt);
    }

    void draw(RenderWindow& window) override {
        InfantryEnemy::draw(window);
    }

    void fireWeapon() override {
        InfantryEnemy::fireWeapon();
        triggerFiringAnim(numAttackFrames * 6.0f / 60.0f);
    }

    void activateSpecial()     {}
    void applyCharacterBuffs() {}
};