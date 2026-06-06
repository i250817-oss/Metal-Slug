#pragma once
#include "LevelBase.h"
//-----------------------------------------------------------------
// BossLevel that is 300 × 70 blocks 

// Phase state :
//   PLAINS       player starts here, Iron Nokana active
//   AERIAL       player crosses col 100, Hairbuster active
//   AQUATIC      player crosses col 200, Sea Satan active
//   FUSION_PREP  player crosses col 280, 3-second countdown
//   FUSION_FIGHT fusion boss spawned at col 290
//   FINISHED     fusion_boss killed -> canLevelEnd() = true


//------------------------------------------------------------
class BossLevel : public LevelBase
{
public:
    struct Phase {
        static const int PLAINS       = 0;
        static const int AERIAL       = 1;
        static const int AQUATIC      = 2;
        static const int FUSION_PREP  = 3;
        static const int FUSION_FIGHT = 4;
        static const int FINISHED     = 5;
    };

private:
    int   currentPhase;
    bool  fusionBossSpawned;
    float fusionTimer;

    void put(int col, int row, Block* b)
    {
        if (col < 0 || col >= width || row < 0 || row >= height) { delete b; return; }
        delete grid[col][row];
        grid[col][row] = b;
    }

    int getBiomeByCol(int col) const override
    {
        if (col < 100) return 0;  // plains
        if (col < 200) return 2;  // aerial
        return 1;                 // aquatic
    }

protected:
    int getGroundRow(int col) override
    {
        // Aquatic deep zone
        if (col >= 200 && col <= 299) return 55;

        // Guaranteed-flat boss arenas
        if (col >= 75  && col <= 90)  return 38;  // Iron Nokana
        if (col >= 135 && col <= 150) return 38;  // Hairbuster
        if (col >= 195 && col <= 210) return 38;  // coast / Slug Mariner
        if (col >= 285 && col <= 299) return 38;  // Fusion Boss

        // Subtle 1-row micro-variation elsewhere (non-boss zones only)
        if (col < 100) return 38 + (col % 22 == 0 ? 1 : 0);
        if (col < 200) return 38 - (col % 26 == 0 ? 1 : 0);
        return 38;
    }

    void placeTerrain()
    {
        //plain
        for (int r = 34; r <= 38; r++)
            put(45, r, new IndestructibleBlock(plainsBiome));

       
        for (int c = 20; c <= 30; c++) put(c, 35, new PlatformBlock(plainsBiome));  // tier 1
        for (int c = 32; c <= 42; c++) put(c, 32, new PlatformBlock(plainsBiome));  // tier 2

        // Aerial 
        for (int c = 110; c <= 120; c++) put(c, 35, new PlatformBlock(aerialBiome));
        for (int c = 122; c <= 132; c++) put(c, 32, new PlatformBlock(aerialBiome));
        for (int c = 152; c <= 162; c++) put(c, 35, new PlatformBlock(aerialBiome));

        // Coastal transition steps 
        for (int c = 195; c <= 199; c++) put(c, 36, new PlatformBlock(plainsBiome));

        //  Aquatic wall marking deep-water entry
        for (int r = 38; r <= 55; r++)
            put(200, r, new IndestructibleBlock(aquaticBiome));

        //  Aquatic platforms 
     
        for (int c = 215; c <= 230; c++) put(c, 52, new PlatformBlock(aquaticBiome));
        for (int c = 245; c <= 260; c++) put(c, 49, new PlatformBlock(aquaticBiome));
        for (int c = 265; c <= 280; c++) put(c, 52, new PlatformBlock(aquaticBiome));
    }

    void bakeSpawns()
    {
        //  Heavy trash mob gauntlet
        placeEnemies("rebel",    5, -1, 0, 299, 4);
        placeEnemies("shielded", 4, -1, 0, 299, 2);
        placeEnemies("grenade",  4, -1, 0, 299, 2);
        placeEnemies("bazooka",  4, -1, 0, 299, 2);
        placeEnemies("alien",    5,  2, 100, 199, 2);
        placeEnemies("zombie",   5,  1, 200, 299, 4);
        placeEnemies("mummy",   10,  0,   0,  99, 1);

        //  Support vehicles 
        addManualSpawn("m15a_bradley", 25, getFirstSolidRow(25), 1);
        addManualSpawn("m15a_bradley", 55, getFirstSolidRow(55), 1);
        addManualSpawn("m15a_bradley", 80, getFirstSolidRow(80), 1);
        placeEnemies("enemy_sub",    4, 1, 210, 280, 1);
        placeEnemies("flying_tara",  5, -1,  0, 299, 2);
        placeEnemies("pow",          3, -1,  0, 299, 1);

        // Iron Nokana
        spawnTable[spawnCount].setEntityType("iron_nokana");
        spawnTable[spawnCount].setCol(82);
        spawnTable[spawnCount].setRow(getFirstSolidRow(82) - 1);
        spawnTable[spawnCount].setBiomeZone(0);
        spawnTable[spawnCount].setSpawned(false);
        spawnCount++;

        //  Hairbuster
        spawnTable[spawnCount].setEntityType("hairbuster");
        spawnTable[spawnCount].setCol(142);
        spawnTable[spawnCount].setRow(30);   // airborne
        spawnTable[spawnCount].setBiomeZone(2);
        spawnTable[spawnCount].setSpawned(false);
        spawnCount++;

        //  Sea Satan 
        spawnTable[spawnCount].setEntityType("sea_satan");
        spawnTable[spawnCount].setCol(248);
        spawnTable[spawnCount].setRow(51);   // deep water
        spawnTable[spawnCount].setBiomeZone(1);
        spawnTable[spawnCount].setSpawned(false);
        spawnCount++;

        // Player Vehicle
        spawnTable[spawnCount].setEntityType("slug_mariner");
        spawnTable[spawnCount].setCol(205);
        spawnTable[spawnCount].setRow(50);
        spawnTable[spawnCount].setBiomeZone(1);
        spawnTable[spawnCount].setSpawned(false);
        spawnCount++;

        // NOTE: Fusion Boss is injected dynamically in update() after the
  
    }

    bool isBossAlive(const char* tag) const
    {
        EntityManager* em = EntityManager::getInstance();
        if (!em) return false;
        Entity** arr = em->getEntityArray();
        int count = em->getEntityCount();
        for (int i = 0; i < count; i++) {
            if (arr[i] && arr[i]->getIsAlive() && arr[i]->isEnemy()) {
                const char* eTag = arr[i]->getEnemyTag();
                if (eTag && areEqual(eTag, tag)) {
                    return true;
                }
            }
        }
        return false;
    }

    int getWorldPixelW() const override
    {
        if (currentPhase == Phase::PLAINS) {
            if (isBossAlive("iron_nokana")) {
                return 95 * blockSize;
            }
        }
        else if (currentPhase == Phase::AERIAL) {
            if (isBossAlive("hairbuster_riberts")) {
                return 160 * blockSize;
            }
        }
        else if (currentPhase == Phase::AQUATIC) {
            if (isBossAlive("sea_satan")) {
                return 260 * blockSize;
            }
        }
        else if (currentPhase == Phase::FUSION_PREP || currentPhase == Phase::FUSION_FIGHT) {
            if (isBossAlive("fusion_boss")) {
                return 295 * blockSize;
            }
        }
        return width * blockSize;
    }

public:
    BossLevel()
        : LevelBase(4, 300, 70),
          currentPhase(Phase::PLAINS),
          fusionBossSpawned(false),
          fusionTimer(0.0f)
    {}

    int getCurrentPhase() const { return currentPhase; }

    void update(float dt) override
    {
        LevelBase::update(dt);
        if (!camera) return;

        float playerX   = camera->getCamX() + camera->getScreenW() * 0.5f;
        int   playerCol = static_cast<int>(playerX / blockSize);

        // One-way phase transitions driven by player position
        if      (currentPhase == Phase::PLAINS  && playerCol > 100) currentPhase = Phase::AERIAL;
        else if (currentPhase == Phase::AERIAL  && playerCol > 200) currentPhase = Phase::AQUATIC;
        else if (currentPhase == Phase::AQUATIC && playerCol > 280) currentPhase = Phase::FUSION_PREP;

        // Fusion Boss injection after 3-second dramatic pause
        if (currentPhase == Phase::FUSION_PREP && !fusionBossSpawned)
        {
            fusionTimer += dt;
            if (fusionTimer >= 3.0f && spawnCount < maxSpawns)
            {
                spawnTable[spawnCount].setEntityType("fusion_boss");
                spawnTable[spawnCount].setCol(290);
                spawnTable[spawnCount].setRow(getFirstSolidRow(290));
                spawnTable[spawnCount].setBiomeZone(1);
                spawnTable[spawnCount].setSpawned(false);
                spawnCount++;

                EntityManager::getInstance()->resetAllSpawns();

                fusionBossSpawned = true;
                currentPhase = Phase::FUSION_FIGHT;
            }
        }
    }

    // Blocks level completion until Fusion Boss is actually killed
    bool canLevelEnd() const override
    {
        return (currentPhase != Phase::FUSION_PREP &&
                currentPhase != Phase::FUSION_FIGHT);
    }

    void reportKill(const char* type) override
    {
        if (type && areEqual(type, "fusion_boss"))
            currentPhase = Phase::FINISHED;
    }

    void load() override
    {
        LevelBase::load();
        placeTerrain();
        bakeSpawns();
    }
};
