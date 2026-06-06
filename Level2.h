#pragma once
#include "LevelBase.h"

// Level2 — 200 × 70 blocks — MEDIUM difficulty
//
// Biome boundaries (LevelBase: plainsColEnd=66, aquaticColEnd=132):
//   Plains   cols [0   ..  65]   — near-flat row 38, two 1-row dips
//   Aquatic  cols [66  .. 131]   — undulating seafloor rows 56-59 (~7-9 deep)
//   Aerial   cols [132 .. 199]   — three hills, peaks rows 20/17/12
//
// waterSurfaceRow = (70*72)/100 = 50.  Seafloor 56-59 = 6-9 below surface.
// Platforms on shallowest seafloor sections (row 56) as stepping stones.
// All ground spawns placed on actual terrain collision rows.
class Level2 : public LevelBase
{
private:
    void put(int col, int row, Block* b)
    {
        if (col < 0 || col >= width || row < 0 || row >= height) { delete b; return; }
        delete grid[col][row];
        grid[col][row] = b;
    }

protected:
    int getGroundRow(int col) override
    {
        static const int GR[200] = {
            // Plains [0-65] — near-flat row 38, two subtle 1-row dips
            38, 38, 38, 38, 38, 38, 38, 38, 38, 38,
            38, 38, 38, 38, 38, 38, 38, 38, 38, 38,
            38, 38, 38, 38, 38, 38, 38, 38, 38, 38,
            38, 38, 38, 37, 37, 37, 37, 38, 38, 38,  // dip 1 cols 33-36
            38, 38, 38, 38, 38, 38, 38, 37, 37, 37,
            37, 38, 38, 38, 38, 38, 38, 38, 38, 38,  // dip 2 cols 47-50
            38, 38, 38, 38, 38, 38,

            // Aquatic [66-131] — undulating seafloor rows 56-59
            57, 57, 58, 58, 58, 57, 57, 56, 56, 57,
            57, 58, 58, 59, 59, 58, 57, 57, 56, 56,
            57, 57, 58, 58, 59, 59, 58, 57, 56, 56,
            57, 57, 58, 58, 57, 57, 56, 56, 57, 57,
            58, 58, 59, 59, 58, 57, 57, 56, 56, 57,
            57, 58, 58, 57, 57, 56, 56, 57, 57, 58,
            58, 57, 57, 57, 57, 57,

            // Aerial [132-199] — three progressively taller hills
            38, 38, 38, 38, 38, 36, 34, 32, 30, 28,
            26, 24, 22, 20, 22, 24, 26, 28, 30, 32,  // hill1 peak=20 at ~col143
            34, 36, 38, 38, 38, 36, 34, 32, 30, 28,
            25, 22, 19, 17, 19, 22, 25, 28, 30, 32,  // hill2 peak=17 at ~col161
            34, 36, 38, 38, 38, 36, 34, 31, 28, 25,
            22, 18, 15, 12, 15, 18, 22, 25, 28, 31,  // hill3 peak=12 at ~col181
            34, 36, 38, 38, 38, 38, 38, 42           // descent + wall
        };
        if (col < 0 || col >= 200) return 38;
        return GR[col];
    }

    void placeTerrain()
    {
        // ── Plains barrels ────────────────────────────────────────────────
        put(13, 36, new BarrelDestructable(plainsBiome));
        put(45, 36, new BarrelDestructable(plainsBiome));

        // ── Plains platforms ──────────────────────────────────────────────
        // Platform 1 LOW  [cols 20-22] row 36
        for (int c = 20; c <= 22; c++) {
            put(c, 35, new AirBlock(plainsBiome));
            put(c, 36, new PlatformBlock(plainsBiome));
        }
        // Platform 2 HIGH [cols 24-30] row 34
        for (int c = 24; c <= 30; c++) {
            put(c, 33, new AirBlock(plainsBiome));
            put(c, 34, new PlatformBlock(plainsBiome));
        }
        // Platform 3 MID  [cols 33-35] row 36
        for (int c = 33; c <= 35; c++) {
            put(c, 35, new AirBlock(plainsBiome));
            put(c, 36, new PlatformBlock(plainsBiome));
        }
        // Platform 4 elevated [cols 55-58] row 34 — above dip 2
        for (int c = 55; c <= 58; c++) {
            put(c, 33, new AirBlock(plainsBiome));
            put(c, 34, new PlatformBlock(plainsBiome));
        }

        // ── Aquatic U-container walls ─────────────────────────────────────
        // Left wall [col 66, rows 38-59] — top removed for jumpability
        for (int r = 39; r <= 59; r++)
            put(66, r, new IndestructibleBlock(aquaticBiome));

        // Right wall [col 131, rows 38-59]
        put(131, 38, new SolidBlock(plainsBiome));
        for (int r = 39; r <= 59; r++)
            put(131, r, new IndestructibleBlock(aquaticBiome));

        // ── Aquatic platforms — on peak clusters (row 56) ────────────────
        // platform row = 55 (one above the shallowest seafloor)
        auto aqPlat = [&](int colStart, int colEnd) {
            for (int c = colStart; c <= colEnd; c++) {
                put(c, 54, new AirBlock(aquaticBiome));
                put(c, 55, new PlatformBlock(aquaticBiome));
            }
        };
        aqPlat(73,  74);   // cluster 1
        aqPlat(84,  85);   // cluster 2
        aqPlat(94,  95);   // cluster 3
        aqPlat(102, 103);  // cluster 4
        aqPlat(113, 114);  // cluster 5
        aqPlat(121, 122);  // cluster 6

        // ── Aerial caves ──────────────────────────────────────────────────
        // Cave 1 — hill1 peak col ~143 row 20
        for (int c = 141; c <= 145; c++) put(c, 20, new AirBlock(aerialBiome));
        for (int c = 142; c <= 144; c++) put(c, 21, new AirBlock(aerialBiome));
        put(143, 22, new AirBlock(aerialBiome));

        // Cave 2 — hill2 peak col ~161 row 17
        for (int c = 158; c <= 164; c++) put(c, 17, new AirBlock(aerialBiome));
        for (int c = 159; c <= 163; c++) put(c, 18, new AirBlock(aerialBiome));
        for (int c = 160; c <= 162; c++) put(c, 19, new AirBlock(aerialBiome));

        // Cave 3 — hill3 peak col ~181 row 12: widest cave
        for (int c = 178; c <= 184; c++) put(c, 12, new AirBlock(aerialBiome));
        for (int c = 179; c <= 183; c++) put(c, 13, new AirBlock(aerialBiome));
        for (int c = 180; c <= 182; c++) put(c, 14, new AirBlock(aerialBiome));
        put(181, 15, new AirBlock(aerialBiome));
    }

    void bakeSpawns()
    {
        // ── Infantry: 3 batches each across all biomes ────────────────────
        // Plains spawns (ground row 38 → spawnRow 37)
        addManualSpawn("rebel",    8,  37, 3);
        addManualSpawn("rebel",    40, 37, 3);
        addManualSpawn("shielded", 18, 37, 2);
        addManualSpawn("shielded", 55, 37, 2);
        addManualSpawn("grenade",  12, 37, 2);
        addManualSpawn("grenade",  58, 37, 2);
        addManualSpawn("bazooka",  28, 37, 2);
        addManualSpawn("bazooka",  50, 37, 2);

        // Aquatic spawns — on undulating seafloor
        addManualSpawn("rebel",    78,  55, 3);   // on platform cluster
        addManualSpawn("shielded", 93,  55, 2);   // on platform cluster
        addManualSpawn("zombie",   107, 57, 2);   // on seafloor (stairs)
        addManualSpawn("zombie",   120, 55, 2);   // on platform cluster (stairs)

        // Aerial spawns — on mountain slopes & flat sections
        addManualSpawn("rebel",    134, 37, 3);
        addManualSpawn("shielded", 155, 21, 2);   // hill2 slope ground
        addManualSpawn("grenade",  169, 37, 2);
        addManualSpawn("bazooka",  192, 37, 2);

        // ── Aerial + Alien: 2 batches each, AERIAL only ───────────────────
        addManualSpawn("paratrooper", 160, 14, 3);  // hill2 air
        addManualSpawn("paratrooper", 180, 10, 3);  // hill3 air
        addManualSpawn("alien",       170, 15, 1);
        addManualSpawn("alien",       190, 20, 1);

        // ── Undead: 3 batches each, PLAINS only ──────────────────────────
        addManualSpawn("zombie", 22, 37, 4);
        addManualSpawn("zombie", 42, 37, 4);
        addManualSpawn("zombie", 60, 37, 4);
        addManualSpawn("mummy",  15, 37, 1);
        addManualSpawn("mummy",  35, 37, 1);
        addManualSpawn("mummy",  55, 37, 1);

        // ── Zombies in AQUATIC: 3 batches ────────────────────────────────
        addManualSpawn("zombie", 75,  55, 4);   // platform cluster
        addManualSpawn("zombie", 100, 56, 4);   // seafloor
        addManualSpawn("zombie", 122, 55, 4);   // platform cluster

        // ── Flying Tara: 3 batches across level ──────────────────────────
        addManualSpawn("flying_tara",  30, 28, 2);  // plains sky
        addManualSpawn("flying_tara",  95, 40, 2);  // aquatic sky
        addManualSpawn("flying_tara", 172, 22, 2);  // aerial sky

        // ── M-15A Bradley: 2 on flat plains terrain ───────────────────────
        addManualSpawn("m15a_bradley", 25, getFirstSolidRow(25), 1);
        addManualSpawn("m15a_bradley", 52, getFirstSolidRow(52), 1);

        // ── Enemy Sub: 1 in aquatic ───────────────────────────────────────
        addManualSpawn("enemy_sub", 95, 52, 1);

        // ── Player Vehicle: Slug Mariner before aquatic entry ────────────
        addManualSpawn("slug_mariner", 64, 36, 1);

        // ── POW: 2 in plains, 1 in aquatic, 1 in aerial ──────────────────
        placeEnemies("pow", 2, 0, 0,   65,  1);
        placeEnemies("pow", 1, 1, 66,  131, 1);
        placeEnemies("pow", 1, 2, 132, 199, 1);
    }

public:
    Level2() : LevelBase(2, 200, 70) {}

    void load() override
    {
        LevelBase::load();
        placeTerrain();
        bakeSpawns();
    }
};
