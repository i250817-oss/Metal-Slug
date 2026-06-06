#pragma once
#include "LevelBase.h"

// Level3 — 250 × 60 blocks — HARDEST regular level
//
// Biome boundaries (LevelBase: plainsColEnd=82, aquaticColEnd=165):
//   Plains   cols [0   ..  81]   — near-flat row 32, one 1-row dip
//   Aquatic  cols [82  .. 164]   — undulating seafloor rows 49-53 (~7-10 deep)
//   Aerial   cols [165 .. 249]   — three hills, peaks rows 20/17/12
//
// waterSurfaceRow = (60*72)/100 = 43.  Seafloor 49-53 = 6-10 below surface.
// Platforms on shallowest seafloor sections (row 49) as stepping stones.
// All ground spawns placed on actual terrain collision rows.
class Level3 : public LevelBase
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
        static const int GR[250] = {
            // Plains [0-81] — near-flat row 32, single 1-row dip at cols 62-65
            32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
            32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
            32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
            32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
            32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
            32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
            32, 32, 31, 31, 31, 31, 32, 32, 32, 32,
            32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
            32, 32,

            // Aquatic [82-164] — rhythmic undulation, seafloor rows 49-53
            50, 50, 51, 51, 51, 50, 50, 49, 49, 50,
            50, 51, 51, 52, 52, 51, 50, 49, 49, 50,
            50, 51, 51, 52, 53, 52, 51, 49, 49, 50,
            50, 51, 51, 50, 50, 49, 49, 50, 50, 51,
            51, 52, 52, 51, 50, 49, 49, 50, 50, 51,
            51, 52, 52, 51, 50, 49, 49, 50, 51, 51,
            50, 50, 49, 49, 50, 50, 51, 51, 50, 50,
            49, 49, 50, 50, 51, 51, 50, 50, 49, 49,
            50, 50, 50,

            // Aerial [165-249] — three normalized hills, peaks rows 20/17/12
            32, 32, 32, 32, 32, 32, 32, 30, 28, 26,
            24, 22, 20, 22, 24, 26, 28, 30, 32, 32,  // hill1 peak=20 at ~col177
            32, 32, 32, 30, 28, 25, 23, 21, 19, 17,
            19, 21, 23, 25, 28, 30, 32, 32, 32, 32,  // hill2 peak=17 at ~col195
            32, 30, 27, 24, 21, 18, 16, 14, 12, 14,
            16, 18, 21, 24, 27, 30, 32, 32, 32, 32,  // hill3 peak=12 at ~col213
            32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
            32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
            32, 32, 32, 32, 50                        // descent + final wall
        };
        if (col < 0 || col >= 250) return 32;
        return GR[col];
    }

    void placeTerrain()
    {
        // ── Plains barrels ────────────────────────────────────────────────
        put(20, 30, new BarrelDestructable(plainsBiome));
        put(50, 30, new BarrelDestructable(plainsBiome));
        put(72, 30, new BarrelDestructable(plainsBiome));

        // ── Plains platforms (harder, more gaps than Level 2) ─────────────
        for (int c = 10; c <= 13; c++) {
            put(c, 29, new AirBlock(plainsBiome));
            put(c, 30, new PlatformBlock(plainsBiome));
        }
        for (int c = 17; c <= 21; c++) {
            put(c, 27, new AirBlock(plainsBiome));
            put(c, 28, new PlatformBlock(plainsBiome));
        }
        for (int c = 40; c <= 45; c++) {
            put(c, 29, new AirBlock(plainsBiome));
            put(c, 30, new PlatformBlock(plainsBiome));
        }
        for (int c = 55; c <= 60; c++) {
            put(c, 27, new AirBlock(plainsBiome));
            put(c, 28, new PlatformBlock(plainsBiome));
        }

        // ── Aquatic U-container walls ─────────────────────────────────────
        // Left wall [col 82, rows 32-53]
        put(82, 32, new SolidBlock(plainsBiome));
        for (int r = 33; r <= 53; r++)
            put(82, r, new IndestructibleBlock(aquaticBiome));

        // Right wall [col 165, rows 32-53]
        put(165, 32, new SolidBlock(aerialBiome));
        for (int r = 33; r <= 53; r++)
            put(165, r, new IndestructibleBlock(aquaticBiome));

        // ── Aquatic platforms — on peak clusters (row 49) ────────────────
        // platform row = 48 (one above shallowest seafloor)
        auto aqPlat = [&](int colStart, int colEnd) {
            for (int c = colStart; c <= colEnd; c++) {
                put(c, 47, new AirBlock(aquaticBiome));
                put(c, 48, new PlatformBlock(aquaticBiome));
            }
        };
        aqPlat(89,  90);   // cluster 1
        aqPlat(99,  100);  // cluster 2
        aqPlat(109, 110);  // cluster 3
        aqPlat(117, 118);  // cluster 4
        aqPlat(127, 128);  // cluster 5
        aqPlat(137, 138);  // cluster 6
        aqPlat(144, 145);  // cluster 7
        aqPlat(152, 153);  // cluster 8
        aqPlat(160, 161);  // cluster 9

        // ── Aerial caves ──────────────────────────────────────────────────
        // Cave 1 — hill1 peak col ~177 row 20
        for (int c = 175; c <= 179; c++) put(c, 20, new AirBlock(aerialBiome));
        for (int c = 176; c <= 178; c++) put(c, 21, new AirBlock(aerialBiome));
        put(177, 22, new AirBlock(aerialBiome));

        // Cave 2 — hill2 peak col ~195 row 17
        for (int c = 192; c <= 198; c++) put(c, 17, new AirBlock(aerialBiome));
        for (int c = 193; c <= 197; c++) put(c, 18, new AirBlock(aerialBiome));
        for (int c = 194; c <= 196; c++) put(c, 19, new AirBlock(aerialBiome));

        // Cave 3 — hill3 peak col ~213 row 12: largest cave in level
        for (int c = 209; c <= 217; c++) put(c, 12, new AirBlock(aerialBiome));
        for (int c = 210; c <= 216; c++) put(c, 13, new AirBlock(aerialBiome));
        for (int c = 211; c <= 215; c++) put(c, 14, new AirBlock(aerialBiome));
        for (int c = 212; c <= 214; c++) put(c, 15, new AirBlock(aerialBiome));
        put(213, 16, new AirBlock(aerialBiome));
    }

    void bakeSpawns()
    {
        // ── Infantry: 3 batches each across all biomes ────────────────────
        // Plains (ground row 32 → spawnRow 31)
        addManualSpawn("rebel",    8,  31, 3);
        addManualSpawn("rebel",    42, 31, 3);
        addManualSpawn("shielded", 18, 31, 2);
        addManualSpawn("shielded", 60, 31, 2);
        addManualSpawn("grenade",  12, 31, 2);
        addManualSpawn("grenade",  70, 31, 2);
        addManualSpawn("bazooka",  30, 31, 2);
        addManualSpawn("bazooka",  55, 31, 2);

        // Aquatic (on seafloor or platforms)
        addManualSpawn("rebel",    92,  48, 3);   // platform cluster
        addManualSpawn("zombie",   111, 48, 2);   // platform cluster (stairs)
        addManualSpawn("zombie",   130, 49, 2);   // seafloor peak (stairs)
        addManualSpawn("bazooka",  155, 49, 2);   // seafloor peak

        // Aerial (on mountain slopes or flat sections)
        addManualSpawn("rebel",    167, 31, 3);
        addManualSpawn("shielded", 186, 22, 2);   // hill2 slope
        addManualSpawn("grenade",  205, 31, 2);
        addManualSpawn("bazooka",  230, 31, 2);

        // ── Aerial + Alien: 3 batches each, AERIAL only ───────────────────
        addManualSpawn("paratrooper", 175, 16, 3);  // hill1 air
        addManualSpawn("paratrooper", 196, 13, 3);  // hill2 air
        addManualSpawn("paratrooper", 215, 9,  3);  // hill3 air
        addManualSpawn("alien",       180, 16, 1);
        addManualSpawn("alien",       200, 13, 1);
        addManualSpawn("alien",       220, 9,  1);

        // ── Undead: 4 batches each, PLAINS only ──────────────────────────
        addManualSpawn("zombie", 15, 31, 4);
        addManualSpawn("zombie", 32, 31, 4);
        addManualSpawn("zombie", 50, 31, 4);
        addManualSpawn("zombie", 68, 31, 4);
        addManualSpawn("mummy",  10, 31, 1);
        addManualSpawn("mummy",  25, 31, 1);
        addManualSpawn("mummy",  45, 31, 1);
        addManualSpawn("mummy",  65, 31, 1);

        // ── Zombies in AQUATIC: 4 batches ────────────────────────────────
        addManualSpawn("zombie", 90,  48, 4);   // platform
        addManualSpawn("zombie", 110, 48, 4);   // platform
        addManualSpawn("zombie", 135, 49, 4);   // seafloor
        addManualSpawn("zombie", 158, 49, 4);   // seafloor

        // ── Flying Tara: 3 batches across level ──────────────────────────
        addManualSpawn("flying_tara",  40, 22, 2);  // plains sky
        addManualSpawn("flying_tara", 115, 35, 2);  // aquatic sky
        addManualSpawn("flying_tara", 200, 22, 2);  // aerial sky

        // ── M-15A Bradley: 2 on flat plains terrain ───────────────────────
        addManualSpawn("m15a_bradley", 25, getFirstSolidRow(25), 1);
        addManualSpawn("m15a_bradley", 60, getFirstSolidRow(60), 1);

        // ── Enemy Sub: 2 in aquatic ───────────────────────────────────────
        addManualSpawn("enemy_sub", 100, 46, 1);
        addManualSpawn("enemy_sub", 145, 46, 1);

        // ── Player Vehicles ───────────────────────────────────────────────
        addManualSpawn("amphibious_slug", 35, getFirstSolidRow(35), 1);  // plains ground
        addManualSpawn("slug_flyer", 163, 20, 1);                        // sky before aerial

        // ── POW: 2 per biome (6 total) ────────────────────────────────────
        placeEnemies("pow", 2, 0, 0,   81,  1);
        placeEnemies("pow", 2, 1, 82,  164, 1);
        placeEnemies("pow", 2, 2, 165, 249, 1);
    }

public:
    Level3() : LevelBase(3, 250, 60) {}

    void load() override
    {
        LevelBase::load();
        placeTerrain();
        bakeSpawns();
    }
};
