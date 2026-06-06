#pragma once
#include "Boss.h"
#include "../../Manager/EntityManager.h"
#include "IronNokana.h"
#include "HairbusterRiberts.h"
#include "SeaSatan.h"
#include "../../Collectible and Interactable System/SupplyBox.h"
#include "../Infantry/RebelSoldier.h"
#include "../../Vehicle System/Enemy Vehicles/FlyingTara.h"
#include "../../Vehicle System/Enemy Vehicles/EnemySub.h"

// FusionBoss: A polymorphic hybrid of the three main bosses.
// Requirement: Cycle states every 10-15s, separate healths, spawn mode-specific minions in batches of 2-4.
class FusionBoss : public IronNokana, public HairbusterRiberts, public SeaSatan {
public:
    static const int MODE_GROUND = 0;
    static const int MODE_AIR    = 1;
    static const int MODE_WATER  = 2;

private:
    int currentMode;
    float modeTimer;
    float switchCooldown;

    int groundHp;
    int airHp;
    int waterHp;

    // Minion Batch System
    Entity* currentBatch[4];
    int     currentBatchSize;
    bool    isBatchActive;
    float   minionSpawnTimer;

public:
    FusionBoss(double posX, double posY)
        : DamagableEntity(posX, posY, 300)
        , Enemy(posX, posY, 300)
        , Boss(300, posX, posY, "fusion_boss")
        , IronNokana(posX, posY)
        , HairbusterRiberts(posX, posY)
        , SeaSatan(posX, posY)
        , currentMode(MODE_GROUND)
        , modeTimer(0)
        , switchCooldown(12.0f) // 10-15s range
        , groundHp(100), airHp(100), waterHp(100)
        , currentBatchSize(0), isBatchActive(false), minionSpawnTimer(5.0f)
    {
        enemyTag = "fusion_boss";
        for(int i=0; i<4; i++) currentBatch[i] = nullptr;
    }

    void takeDamage(int val) override {
        if (currentMode == MODE_GROUND) groundHp -= val;
        else if (currentMode == MODE_AIR) airHp -= val;
        else if (currentMode == MODE_WATER) waterHp -= val;

        if (groundHp <= 0 && currentMode == MODE_GROUND) { groundHp = 0; cycleMode(); }
        if (airHp <= 0 && currentMode == MODE_AIR) { airHp = 0; cycleMode(); }
        if (waterHp <= 0 && currentMode == MODE_WATER) { waterHp = 0; cycleMode(); }

        hp = groundHp + airHp + waterHp;
        if (hp <= 0) die();
    }

    void cycleMode() {
        if (hp <= 0) return;
        int startMode = currentMode;
        do {
            currentMode = (currentMode + 1) % 3;
        } while (isModeDead(currentMode) && currentMode != startMode);
        modeTimer = 0;
    }

    bool isModeDead(int mode) const {
        if (mode == MODE_GROUND) return groundHp <= 0;
        if (mode == MODE_AIR) return airHp <= 0;
        if (mode == MODE_WATER) return waterHp <= 0;
        return true;
    }

    float getWidth() const override {
        if (currentMode == MODE_GROUND) return IronNokana::getWidth();
        if (currentMode == MODE_AIR) return HairbusterRiberts::getWidth();
        return SeaSatan::getWidth();
    }

    float getHeight() const override {
        if (currentMode == MODE_GROUND) return IronNokana::getHeight();
        if (currentMode == MODE_AIR) return HairbusterRiberts::getHeight();
        return SeaSatan::getHeight();
    }

    void update(Level* level, float dt) override {
        if (!isAlive) return;
        modeTimer += dt;
        if (modeTimer >= switchCooldown) cycleMode();

        // Delegate update polymorphically
        if (currentMode == MODE_GROUND) IronNokana::update(level, dt);
        else if (currentMode == MODE_AIR) HairbusterRiberts::update(level, dt);
        else SeaSatan::update(level, dt);

        // Minion Batch Logic
        if (isBatchActive) {
            bool batchDead = true;
            for (int i = 0; i < currentBatchSize; i++) {
                if (currentBatch[i] && currentBatch[i]->getIsAlive()) {
                    batchDead = false;
                    break;
                }
            }
            if (batchDead) {
                // Batch Killed! Spawn Crate and reset
                pendingShot = new SupplyBox(posX + getWidth()*0.5, posY);
                isBatchActive = false;
                minionSpawnTimer = 8.0f; // Wait before next batch
            }
        } else {
            minionSpawnTimer -= dt;
            if (minionSpawnTimer <= 0) {
                spawnMinions();
            }
        }
    }

    void draw(RenderWindow& window) override {
        if (!isAlive) return;
        if (currentMode == MODE_GROUND) IronNokana::draw(window);
        else if (currentMode == MODE_AIR) HairbusterRiberts::draw(window);
        else SeaSatan::draw(window);
    }

    void attack(float dt) override {
        if (currentMode == MODE_GROUND) IronNokana::attack(dt);
        else if (currentMode == MODE_AIR) HairbusterRiberts::attack(dt);
        else SeaSatan::attack(dt);
    }

    void move(float dt) override {
        if (currentMode == MODE_GROUND) IronNokana::move(dt);
        else if (currentMode == MODE_AIR) HairbusterRiberts::move(dt);
        else SeaSatan::move(dt);
    }

    void spawnMinions() override {
        currentBatchSize = 2 + (rand() % 3); // 2-4 minions
        for (int i = 0; i < currentBatchSize; i++) {
            double offsetX = (i - currentBatchSize / 2.0) * 100.0;
            Entity* m = nullptr;
            if (currentMode == MODE_GROUND) m = new RebelSoldier(posX + offsetX, posY);
            else if (currentMode == MODE_AIR) m = new FlyingTara(posX + offsetX, posY - 200);
            else m = new EnemySub(posX + offsetX, posY + 200);
            
            currentBatch[i] = m;
            // Use pendingSpawn system to add to EntityManager
            // FusionBoss inherits Boss which has no pendingSpawn, but DamagableEntity has pendingShot
            // We'll use the EntityManager directly via instance for spawning if possible, 
            // or use a queue.
            EntityManager::getInstance()->addEntity(m);
        }
        isBatchActive = true;
    }

    void specialAttack() override {
        if (currentMode == MODE_GROUND) IronNokana::specialAttack();
        else if (currentMode == MODE_AIR) HairbusterRiberts::specialAttack();
        else SeaSatan::specialAttack();
    }

    void retreat() override {}

    void fireWeapon() override { attack(0.016f); }
    int getScoreValue() const override { return 1500; }
};