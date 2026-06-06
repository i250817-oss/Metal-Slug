#pragma once
#include "InfantryEnemy.h"
#include "../../Weapon System/FireArms/Pistol.h"

using namespace sf;



//rebelSoldier
//most basic infantry enemy. 2 HP. Pistol. Spawns in batches of 2-4.
//sprite: "Sprites/Enemies/Rebel Soldier.png"  (all frames right-to-left)
class RebelSoldier : public InfantryEnemy {
private:
    IntRect chaseFrames[12];
    IntRect jumpFrames[8];
    IntRect attackFrames[10];
    int numChaseFrames, numJumpFrames, numAttackFrames;

    // Death animation
    IntRect deathFrames[11];
    bool isDying;
    int deathFrame;
    float deathAnimTimer;

public:
    RebelSoldier(double posX, double posY)
        : Entity(posX, posY), DamagableEntity(posX, posY, 2), InfantryEnemy(posX, posY, 2)
        , numChaseFrames(12), numJumpFrames(8), numAttackFrames(10)
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

        walkSpeed = 2.5f;   //faster than basic infantry
        runSpeed  = 5;   //aggressive advance
        enemyFireTimer = 1.2f; //shorter burst delay (veteran reflexes)
        enemyTag = "rebel";
        setWeapon(new Pistol());


        addState(AI_STATE_PATROL,  new PatrolState());
        addState(AI_STATE_CHASE,   new ChaseState());
        addState(AI_STATE_ATTACK,  new ShootState());
        addState(AI_STATE_RETREAT, new RetreatState());
        addState(AI_STATE_MELEE,   new MeleeState());
        addState(AI_STATE_PANIC,   new PanicState());
        changeState(AI_STATE_PATROL);

        if (enemyTexture.loadFromFile("Sprites/Enemies/Rebel Soldier.png")) {
            enemySprite.setTexture(enemyTexture);
            enemySprite.setScale(3, 3);
        }
        animDelay = 4.0f / 60.0f;

        //running/Chase
        chaseFrames[0]  = IntRect(450, 44, 35, 37);
        chaseFrames[1]  = IntRect(410, 45, 37, 35);
        chaseFrames[2]  = IntRect(369, 46, 38, 32);
        chaseFrames[3]  = IntRect(326, 45, 40, 30);
        chaseFrames[4]  = IntRect(285, 44, 38, 36);
        chaseFrames[5]  = IntRect(248, 44, 34, 37);
        chaseFrames[6]  = IntRect(210, 44, 35, 37);
        chaseFrames[7]  = IntRect(170, 45, 37, 35);
        chaseFrames[8]  = IntRect(125, 46, 42, 33);
        chaseFrames[9]  = IntRect( 80, 45, 42, 31);
        chaseFrames[10] = IntRect( 40, 44, 37, 37);
        chaseFrames[11] = IntRect(  3, 44, 34, 37);

        //jump
        jumpFrames[0] = IntRect(307, 136, 37, 37);
        jumpFrames[1] = IntRect(274, 128, 30, 45);
        jumpFrames[2] = IntRect(227, 126, 44, 47);
        jumpFrames[3] = IntRect(183, 125, 41, 37);
        jumpFrames[4] = IntRect(144, 126, 36, 36);
        jumpFrames[5] = IntRect( 99, 129, 42, 36);
        jumpFrames[6] = IntRect( 49, 130, 47, 43);
        jumpFrames[7] = IntRect(  3, 136, 43, 37);

        //attack/Shooting
        attackFrames[0] = IntRect(429, 298, 40, 38);
        attackFrames[1] = IntRect(380, 298, 46, 38);
        attackFrames[2] = IntRect(330, 298, 47, 38);
        attackFrames[3] = IntRect(284, 292, 43, 44);
        attackFrames[4] = IntRect(240, 289, 41, 47);
        attackFrames[5] = IntRect(196, 289, 41, 47);
        attackFrames[6] = IntRect(151, 296, 42, 40);
        attackFrames[7] = IntRect(101, 298, 47, 38);
        attackFrames[8] = IntRect( 50, 298, 48, 38);
        attackFrames[9] = IntRect(  3, 298, 44, 38);

        enemySprite.setTextureRect(chaseFrames[0]);
    }

    ~RebelSoldier() {}
    int getScoreValue() const override { return 50; }

    float getWidth()  const override { return 42 * 3; }
    float getHeight() const override { return 47 * 3; }


    bool getIsDying() const override { return isDying; }


    void takeDamage(int val) override {
        if (isDying) return;
        if (hp - val > 0) {
            InfantryEnemy::takeDamage(val);
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
            // Load and display from soldier.png
            enemySprite.setTexture(retreatTexture);
            enemySprite.setTextureRect(deathFrames[deathFrame]);
            return;
        }

        tickFiringAnim(dt);

        // retreat animation overrides everything else
        if (!applyRetreatAnim(dt)) {

        // animation: attack frames only while actively firing
        enemySprite.setTexture(enemyTexture);   // ensure we're back on our own sheet
        IntRect* frames = chaseFrames;
        int maxFrames   = numChaseFrames;
        float delay     = 4.0f / 60.0f;

        if (!isOnGround) {
            frames = jumpFrames; maxFrames = numJumpFrames;
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
        if (isSpriteOverridden) {
            enemySprite.setTextureRect(overrideRect);
        }
        InfantryEnemy::draw(window);
    }

    void fireWeapon() override {
        InfantryEnemy::fireWeapon();
        triggerFiringAnim(numAttackFrames * 6.0f / 60.0f);
    }

    void activateSpecial()     {}
    void applyCharacterBuffs() {}
};