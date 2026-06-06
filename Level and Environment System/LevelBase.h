#pragma once
#include "../headers/headers.h"
#include "Level.h"
#include "block/AerialBiome.h"
#include "block/BarrelDestructable.h"
#include "block/airblock.h"
#include "block/aquaticBiome.h"
#include "block/block.h"
#include "block/indistructable.h"
#include "block/mountainTop.h"
#include "block/plainsBiome.h"
#include "block/platformblock.h"
#include "block/seafloorBlock.h"
#include "block/shipHull.h"
#include "block/solidblock.h"
#include "block/waterDeepblock.h"
#include "block/waterSurfaceblock.h"
#include "camera.h"
#include "perlin.h"
#include "spawnPoint.h"
#include <iostream>

using namespace sf;
using namespace std;

// LevelBase: Core functionality for concrete levels (Grid, Spawns, Camera,
// Rendering) Tileset mapping (tileset.png): 0: brown rock, 1: grass, 2: teal
// floor, 3: water surf, 4: navy water, 5: near black, 6: light stone, 7: steel
// gray, 8: wood platform

class LevelBase : public Level {
protected:
  // instance-owned constants (no globals, no static)
  const int maxSpawns; // 512 — max baked spawn entries per level
  const int blockSize; // 32  — px per block (matches Block.h blockSize)

  // state
  int levelId;
  int width;  // blocks wide   (150–250)
  int height; // blocks tall   (20–60)
  bool isComplete;

  // dynamic grid [width][height] — stores polymorphic Block pointers─
  Block ***grid;

  // biome singletons (owned by this level)
  Biome *aerialBiome;
  Biome *plainsBiome;
  Biome *aquaticBiome;

  // baked spawn table
  SpawnPoint *spawnTable;
  int spawnCount;

  // camera (association — LevelBase does NOT delete this)
  Camera *camera;

  // SFML rendering assets
  Texture texGrass;
  Texture texStone;
  Texture texAndesite;
  Texture texDirt;
  Texture texDeepslate;
  Texture texWater;  //(Legacy, not drawn as blocks anymore)
  Texture texSand;   // used for aquatic seabed/solid
  Texture bgTexture; // current
  Texture texBgPlains;
  Texture texBgAquatic;
  Texture texBgAerial;
  Texture texBgSand;
  Texture barrelTexture; // spritesheet for barrel destruction stages
  Sprite tileSprite;
  Sprite bgSprite;
  Sprite barrelSprite;
  Texture texCloud;
  Sprite cloudSprite;

  // biome column boundaries (set once in constructor)
  // layout left → right:  Plains | Aquatic | Aerial
  int plainsColEnd;  // cols [0            .. plainsColEnd)   = PLAINS
  int aquaticColEnd; // cols [plainsColEnd .. aquaticColEnd)  = AQUATIC
                     // cols [aquaticColEnd .. width)         = AERIAL

  // water surface row — a fixed row inside the aquatic zone (≈60% height)
  int waterSurfaceRow;

  // grid memory management
  void allocateGrid() {
    grid = new Block **[width];
    for (int i = 0; i < width; i++) {
      grid[i] = new Block *[height];
      for (int j = 0; j < height; j++)
        grid[i][j] = nullptr;
    }
  }

  void freeGrid() {
    if (!grid)
      return;
    for (int i = 0; i < width; i++) {
      for (int j = 0; j < height; j++)
        delete grid[i][j];
      delete[] grid[i];
    }
    delete[] grid;
    grid = nullptr;
  }

  // biome helper — column-based (horizontal layout)
  // 0 = biomePlains   (left third)
  // 1 = biomeAquatic  (middle third)
  // 2 = biomeAerial   (right third)
  virtual int getBiomeByCol(int col) const {
    if (col < plainsColEnd)
      return 0; // plains
    if (col < aquaticColEnd)
      return 1; // aquatic
    return 2;   // aerial
  }

  // spawn placement helper  first AIR row above the ground in a column
  int getFirstSolidRow(int col) const override {
    for (int row = 0; row < height; row++) {
      if (grid[col][row] && grid[col][row]->isSolid())
        return (row > 0) ? row - 1 : 0;
    }
    return height - 2;
  }

  void generateTerrain() {
    for (int col = 0; col < width; col++) {
      int biomeIdx = getBiomeByCol(col);
      Biome *bPtr = (biomeIdx == 0)   ? plainsBiome
                    : (biomeIdx == 1) ? aquaticBiome
                                      : aerialBiome;
      int groundRow = getGroundRow(col);

      if (groundRow < 1)
        groundRow = 1;

      if (groundRow > height - 2)
        groundRow = height - 2;

      for (int row = 0; row < height; row++) {

        delete grid[col][row];

        // absolute bedrock  FINAL LAYER
        if (row == height - 1) {
          grid[col][row] = new IndestructibleBlock(bPtr);
          continue;
        }

        // PLAINS BIOME:
        // grass -> dirt(4) -> stone

        if (biomeIdx == 0) {

          if (row < groundRow) {
            grid[col][row] = new AirBlock(bPtr);
          } else if (row == groundRow) {
            grid[col][row] = new SolidBlock(bPtr); // grass layer
          } else if (row <= groundRow + 4) {
            grid[col][row] = new SolidBlock(bPtr); // dirt layer
          } else {
            grid[col][row] = new IndestructibleBlock(bPtr); // deep stone
          }
        }

        // AQUATIC BIOME:
        // air -> water surface -> deep water -> seabed

        else if (biomeIdx == 1) {

          // sky above ocean
          if (row < waterSurfaceRow) {
            grid[col][row] = new AirBlock(bPtr);
          }

          // exact water surface
          else if (row == waterSurfaceRow) {
            grid[col][row] = new WaterSurfaceBlock(bPtr);
          }

          // underwater region
          else if (row < groundRow) {
            grid[col][row] = new WaterDeepBlock(bPtr);
          }

          // seabed top
          else if (row == groundRow) {
            grid[col][row] = new SeafloorBlock(bPtr);
          }

          // dirt under seabed
          else if (row <= groundRow + 4) {
            grid[col][row] = new SolidBlock(bPtr);
          }

          // deep ocean stone
          else {
            grid[col][row] = new IndestructibleBlock(bPtr);
          }
        }

        // AERIAL BIOME
        // peak → mountain body → deep stone
        else {

          if (row < groundRow) {
            grid[col][row] = new AirBlock(bPtr);
          }

          else if (row == groundRow) {
            grid[col][row] = new MountainPeakBlock(bPtr);
          }

          else if (row <= groundRow + 4) {
            grid[col][row] = new SolidBlock(bPtr);
          }

          else {
            grid[col][row] = new IndestructibleBlock(bPtr);
          }
        }
      }
    }

    // Pre-build stairs at the wall boundary
    int startCol = aquaticColEnd - 25;
    int endCol = aquaticColEnd + 5; // clear the wall too
    for (int c = startCol; c < endCol; c++) {
      int seafloor = getGroundRow(c);
      int wallTop = 35;
      int stairHeight = seafloor - wallTop;

      int wallCol = aquaticColEnd - 1;
      int distToWall = wallCol - startCol;
      float progress = (float)(c - startCol) / (distToWall > 0 ? distToWall : 1);
      if (progress > 1.0f)
        progress = 1.0f;

      int r = seafloor - (progress * stairHeight);

      if (c < width && r >= 0 && r < height) {
        for (int fillR = r; fillR <= seafloor; fillR++) {
          if (fillR < height) {
            delete grid[c][fillR];
            grid[c][fillR] = new SolidBlock(plainsBiome);
          }
        }
      }
    }
  }

  // Batch spawn enemies in segments
  void placeEnemies(const char *type, int batches, int biomeFilter,
                    int startCol, int endCol, int batchSize = 1) {
    if (batches <= 0 || endCol <= startCol)
      return;
    if (batchSize < 1)
      batchSize = 1;

    int segSize = (endCol - startCol) / batches;
    if (segSize < 1)
      segSize = 1;

    for (int b = 0; b < batches; b++) {
      // centre column of this batch segment
      int batchCol = startCol + b * segSize + segSize / 2;
      addManualSpawn(type, batchCol, getFirstSolidRow(batchCol) - 1, batchSize);
    }
  }

  void addManualSpawn(const char *type, int col, int row, int batchSize = 1) {
    for (int i = 0; i < batchSize; i++) {
      if (spawnCount >= maxSpawns)
        return;

      int finalCol = col + (i * 3); // Separated by 3 blocks for clarity
      if (finalCol >= width)
        finalCol = width - 1;

      spawnTable[spawnCount].setEntityType(type);
      spawnTable[spawnCount].setCol(finalCol);
      spawnTable[spawnCount].setRow(row);
      spawnTable[spawnCount].setBiomeZone(getBiomeByCol(finalCol));
      spawnTable[spawnCount].setSpawned(false);
      spawnCount++;
    }
  }

  // tileset tile selection — maps (blockType, biomeZone) → IntRect in PNG
  // tileset columns (each TILE_SIZE wide):
  // 0=aerial-solid  1=plains-solid  2=seafloor  3=water-surface
  // 4=water-deep    5=indestructible 6=mountain-peak
  // 7=ship-hull     8=platform
  void drawBlocks(RenderWindow &win) {
    if (!camera)
      return;

    int startCol = (camera->getCamX() / blockSize);
    int endCol = startCol + (camera->getScreenW() / blockSize) + 2;
    int startRow = (camera->getCamY() / blockSize);
    int endRow = startRow + (camera->getScreenH() / blockSize) + 2;

    // clamp to grid bounds
    if (startCol < 0)
      startCol = 0;
    if (endCol > width)
      endCol = width;
    if (startRow < 0)
      startRow = 0;
    if (endRow > height)
      endRow = height;

    for (int col = startCol; col < endCol; col++) {
      for (int row = startRow; row < endRow; row++) {
        const Block *b = grid[col][row];
        if (!b || b->isAir())
          continue;

        // reset sprite state for each tile to prevent texture/scale leakage
        tileSprite.setTextureRect(IntRect(0, 0, blockSize, blockSize));
        tileSprite.setScale(1, 1); // map 1:1 to the block size
        tileSprite.setColor(Color::White);

        float sx = (col * blockSize);
        float sy = (row * blockSize);

        if (b->isBarrel()) {
          // barrel uses its damage stage to select a sprite frame from
          // the breakBarrel.png spritesheet (8 frames, ~50px each wide)
          static const int barrelFrameX[3] = {0, 36, 77};
          static const int barrelFrameY[3] = {2, 8, 3};
          static const int barrelFrameW[3] = {30, 36, 45};
          static const int barrelFrameH[3] = {41, 35, 40};
          int stage = b->getDamageStage();
          barrelSprite.setTexture(barrelTexture);
          barrelSprite.setTextureRect(
              IntRect(barrelFrameX[stage], barrelFrameY[stage],
                      barrelFrameW[stage], barrelFrameH[stage]));
          barrelSprite.setScale(1.0f * blockSize / barrelFrameW[stage],
                                1.0f * blockSize / barrelFrameH[stage]);
          barrelSprite.setPosition(sx, sy);
          win.draw(barrelSprite);
          continue;
        }

        bool drawSprite = true;

        if (b->isIndestructible())
          tileSprite.setTexture(texDeepslate);
        else if (b->isWater()) {
          bool isSurface = !b->isDeepWater();
          tileSprite.setTexture(texWater);
          // surface: light blue-white ripple; deep: solid ocean blue
          tileSprite.setColor(isSurface ? Color(180, 220, 255, 160)
                                        : Color(40, 100, 200, 210));
        } else if (b->isMountainPeak())
          tileSprite.setTexture(texStone);
        else if (b->isShipHull())
          tileSprite.setTexture(texStone); // placeholder
        else if (b->isPlatform())
          tileSprite.setTexture(texAndesite);
        else if (b->isSolid()) {
          if (b->getBiome() == plainsBiome) {
            bool isSurface = (row == 0 || !grid[col][row - 1] ||
                              grid[col][row - 1]->isAir());
            tileSprite.setTexture(isSurface ? texGrass : texDirt);
          } else
            tileSprite.setTexture(texDirt); // dirt sea floor
        } else if (b->isSeafloor())
          tileSprite.setTexture(texDirt); // dirt sea floor

        if (drawSprite) {
          tileSprite.setPosition(sx, sy);
          win.draw(tileSprite);
        }
      }
    }
  }

public:
  virtual int getGroundRow(int col) override = 0;
  // constructor — every member via initializer list
  LevelBase(int id, int w, int h)
      : maxSpawns(512), blockSize(64), levelId(id), width(w), height(h),
        isComplete(false), grid(nullptr), spawnTable(nullptr), spawnCount(0),
        camera(nullptr), plainsColEnd((w * 0.33f)) // first third  → Plains
        ,
        aquaticColEnd((w * 0.66f)) // middle third → Aquatic
        ,
        waterSurfaceRow((h * 72) /
                        100) // sea surface = plains ground level (~52%)
  {
    aerialBiome = new AerialBiome();
    plainsBiome = new PlainsBiome();
    aquaticBiome = new AquaticBiome();

    // set specialized biome features
    plainsBiome->setStartCol(0);
    plainsBiome->setStartX(0);

    aquaticBiome->setStartCol(plainsColEnd);
    aquaticBiome->setStartX(plainsColEnd * blockSize);

    aerialBiome->setStartCol(aquaticColEnd);
    aerialBiome->setStartX(aquaticColEnd * blockSize);

    allocateGrid();
    spawnTable = new SpawnPoint[maxSpawns];
  }

  // destructor
  virtual ~LevelBase() {
    freeGrid();
    delete[] spawnTable;
    spawnTable = nullptr;
    delete aerialBiome;
    delete plainsBiome;
    delete aquaticBiome;
  }

  // rule of Three — copy-ctor disabled (LevelBase owns dynamic memory)─
  // levels are never copied — they are always heap-allocated and owned by
  // levelManager via Level* pointers. Accidental copy would double-delete the
  // grid.
  LevelBase(const LevelBase &) = delete;

  // deep-copy assignment (used by LevelManager when resizing its array)
  // const members (maxSpawns, blockSize) are always the same value so they
  // need not be re-assigned. width/height must match for a valid copy.
  // after assignment, call load() to reload SFML textures on the copy.
  LevelBase &operator=(const LevelBase &other) {
    if (this == &other)
      return *this;

    // width / height must match — otherwise the grid sizes differ
    if (width != other.width || height != other.height)
      return *this;

    freeGrid();
    allocateGrid();

    levelId = other.levelId;
    isComplete = other.isComplete;
    spawnCount = other.spawnCount;
    camera = other.camera;
    plainsColEnd = other.plainsColEnd;
    aquaticColEnd = other.aquaticColEnd;
    waterSurfaceRow = other.waterSurfaceRow;

    // deep-copy grid
    for (int col = 0; col < width; col++) {
      for (int row = 0; row < height; row++) {
        if (other.grid[col][row])
          grid[col][row] = other.grid[col][row]->clone();
      }
    }

    // deep-copy spawn table
    delete[] spawnTable;
    spawnTable = new SpawnPoint[maxSpawns];
    for (int i = 0; i < spawnCount; i++)
      spawnTable[i] = other.spawnTable[i];

    // note: SFML Texture/Sprite NOT copied — call load() on new object
    return *this;
  }

  // level interface implementations
  void setCamera(Camera *cam) override { camera = cam; }
  Camera *getCamera() const override { return camera; }
  int getLevelId() const override { return levelId; }
  int getWidth() const override { return width; }
  int getHeight() const override { return height; }
  int getWorldPixelW() const override { return width * blockSize; }
  int getWorldPixelH() const override { return height * blockSize; }
  bool getIsComplete() const override { return isComplete; }
  void setIsComplete(bool val) override { isComplete = val; }

  // load — textures + terrain (concrete subclass calls this first)─────
  void load() override {
    texGrass.loadFromFile("Sprites/blocks/grass_block_side.png");
    texGrass.setRepeated(true);
    texStone.loadFromFile("Sprites/blocks/stone.png");
    texStone.setRepeated(true);
    texAndesite.loadFromFile("Sprites/blocks/andesite.png");
    texAndesite.setRepeated(true);
    texDirt.loadFromFile("Sprites/blocks/dirt.png");
    texDirt.setRepeated(true);
    texDeepslate.loadFromFile("Sprites/blocks/deepslate_top.png");
    texDeepslate.setRepeated(true);
    texWater.loadFromFile("Sprites/blocks/water.png");
    texWater.setRepeated(true);
    texSand.loadFromFile("Sprites/sand.png");
    texSand.setRepeated(true);
    barrelTexture.loadFromFile("Sprites/breakBarrel.png");

    // load backgrounds dynamically using custom string library
    char plainsBg[128], aquaticBg[128], aerialBg[128];
    if (levelId == 4) {
      copyStringN(plainsBg, "Sprites/bgs/plainsboss.png", 128);
      copyStringN(aquaticBg, "Sprites/bgs/aquaticboss.png", 128);
      copyStringN(aerialBg, "Sprites/bgs/aerialboss.png", 128);
    } else {
      int id = (levelId == 99) ? 1 : levelId;
      char num[16];
      intToString(id, num);

      char *p1 = concatenate("Sprites/bgs/plains", num);
      char *p2 = concatenate(p1, ".png");
      copyStringN(plainsBg, p2, 128);
      delete[] p1;
      delete[] p2;

      char *aq1 = concatenate("Sprites/bgs/aquatic", num);
      char *aq2 = concatenate(aq1, ".png");
      copyStringN(aquaticBg, aq2, 128);
      delete[] aq1;
      delete[] aq2;

      char *ae1 = concatenate("Sprites/bgs/aerial", num);
      char *ae2 = concatenate(ae1, ".png");
      copyStringN(aerialBg, ae2, 128);
      delete[] ae1;
      delete[] ae2;
    }

    if (texBgPlains.loadFromFile(plainsBg)) {
      texBgPlains.setRepeated(true);
      bgTexture = texBgPlains;
      bgSprite.setTexture(bgTexture);
      bgSprite.setTextureRect(
          IntRect(0, 0, width * blockSize, texBgPlains.getSize().y));
      bgSprite.setScale(1.0f, 900.0f / texBgPlains.getSize().y);
    }
    if (texBgAquatic.loadFromFile(aquaticBg)) {
      texBgAquatic.setRepeated(true);
    }
    if (texBgSand.loadFromFile("Sprites/sand.png")) {
      texBgSand.setRepeated(true);
    }
    if (texBgAerial.loadFromFile(aerialBg)) {
      texBgAerial.setRepeated(true);
    }

    if (texCloud.loadFromFile("Sprites/cloud.png")) {
      texCloud.setRepeated(true);
      cloudSprite.setTexture(texCloud);
      // user requested Frame_1: x=0, y=6, w=494, h=531
      cloudSprite.setTextureRect(IntRect(0, 6, 494, 531));
    }

    tileSprite.setTexture(texGrass);
    barrelSprite.setTexture(barrelTexture);
    generateTerrain();
  }

  // per-frame update: Rain System
  // AS PER PROJECT REQUIREMENT: "Rain is an environment feature which can
  // happen at a random time for a random interval. If craters are created
  //<= 3 blocks wide and >= 1 block deep, then this crater must fill up with
  // water."
  bool isRaining = false;
  float rainTimer = 0;
  float nextToggle = 10;
  float totalTime = 0;
  int lastBiomeZone = -1;

  // stair Animation State
  bool animatingStairs = false;
  float stairAnimTimer = 0;
  int stairColCounter = 0;

  void startStairAnimation() override {
    // Disabled: stairs are now pre-spawned/pre-built
  }

  void update(float dt) override {
    totalTime += dt;
    rainTimer += dt;

    // biome Check — "Listen whenever the biome changes"
    if (camera) {
      float centerX = camera->getCamX() + camera->getScreenW() / 2.0;
      int currentCol = (centerX / blockSize);
      int currentZone = getBiomeByCol(currentCol);

      // completion check: If player reaches the end of the level (width)
      if (currentCol >= width - 5 && canLevelEnd()) {
        isComplete = true;
      }

      if (currentZone != lastBiomeZone) {
        // update Background based on biome
        if (currentZone == 0) {
          bgTexture = texBgPlains;
          bgSprite.setTexture(bgTexture);
          bgSprite.setTextureRect(
              IntRect(0, 0, width * blockSize, bgTexture.getSize().y));
          bgSprite.setScale(
              1.0f,
              900.0f / bgTexture.getSize().y); // Scale to screen height (900px)
        } else if (currentZone == 1) {
          bgTexture = texBgSand;
          bgSprite.setTexture(bgTexture);
          bgSprite.setTextureRect(
              IntRect(0, 0, width * blockSize, height * blockSize));
          bgSprite.setScale(1.0f, 1.0f);
        } else if (currentZone == 2) {
          bgTexture = texBgAerial;
          bgSprite.setTexture(bgTexture);
          bgSprite.setTextureRect(
              IntRect(0, 0, width * blockSize, bgTexture.getSize().y));
          bgSprite.setScale(
              1.0f,
              900.0f / bgTexture.getSize().y); // Scale to screen height (900px)
        }

        lastBiomeZone = currentZone;
      }
    }

    // PROJECT REQUIREMENT: Kill All Enemies
    //"RIGHT NOW IF PLAYER REACHES AT THE END OF LEVEL OR AT THE END OF BIOME
    // BOTH CHANGES BUT IN ACTUAL IT NEED TO KILL ALL"

    if (rainTimer > nextToggle) {
      isRaining = !isRaining;
      rainTimer = 0;
      float noise = perlin(totalTime * 0.1f, 55.5f);
      nextToggle = 15 + (noise * 5);
    }
    if (isRaining) {
      // Throttle: only run the fill check once per second (not every frame)
      static float rainFillTimer = 0;
      rainFillTimer += dt;
      if (rainFillTimer >= 1.0f) {
        rainFillTimer = 0;
        // Only iterate columns visible to the camera (+/- 5 cols margin)
        int cStart = 1, cEnd = width - 1;
        if (camera) {
          cStart = (int)(camera->getCamX() / blockSize) - 5;
          cEnd   = cStart + (int)(camera->getScreenW() / blockSize) + 10;
          if (cStart < 1) cStart = 1;
          if (cEnd > width - 1) cEnd = width - 1;
        }
        for (int c = cStart; c < cEnd; c++) {
          for (int r = 1; r < height - 1; r++) {
            if (grid[c][r] && grid[c][r]->isAir() && grid[c][r + 1] &&
                grid[c][r + 1]->isSolid() && grid[c - 1][r] &&
                grid[c - 1][r]->isSolid() && grid[c + 1][r] &&
                grid[c + 1][r]->isSolid()) {
              if (rand() % 5 == 0) {  // 20% chance per eligible cell per second
                delete grid[c][r];
                Biome *b = (getBiomeByCol(c) == 1) ? aquaticBiome : plainsBiome;
                grid[c][r] = new WaterSurfaceBlock(b);
              }
            }
          }
        }
      }
    }
  }

  // blast System
  // AS PER PROJECT REQUIREMENT: "Blocks / Voxels in both modes must be
  // destructible via blasts if they fall into the blast radius."
  // circular inequality: dx^2 + dy^2 <= R^2
  void applyBlast(float worldX, float worldY, float radiusBlocks) {
    int centerC = (worldX / blockSize);
    int centerR = (worldY / blockSize);
    int rad = radiusBlocks;
    for (int c = centerC - rad; c <= centerC + rad; c++) {
      for (int r = centerR - rad; r <= centerR + rad; r++) {
        if (c < 0 || c >= width || r < 0 || r >= height)
          continue;
        if (!grid[c][r] || grid[c][r]->isIndestructible() ||
            !grid[c][r]->isSolid())
          continue;
        float dc = (c - centerC);
        float dr = (r - centerR);
        if (dc * dc + dr * dr <= radiusBlocks * radiusBlocks) {
          Biome *b = grid[c][r]->getBiome();
          delete grid[c][r];
          grid[c][r] = new AirBlock(b);
        }
      }
    }
  }

  void draw(RenderWindow &win) override {
    if (!camera)
      return;

    if (lastBiomeZone == 1) {
      // AQUATIC SINGLE LAYER PARALLAX (Level 1)
      // Background moves at 50% speed for maximum depth
      float bgWorldX = camera->getCamX() * 0.5f;
      float bgWorldY = camera->getCamY(); // Locked to camera vertical

      bgSprite.setTexture(texBgAquatic);
      bgSprite.setTextureRect(
          IntRect(0, 0, width * blockSize, texBgAquatic.getSize().y));
      bgSprite.setScale(
          1.0f, 900.0f / texBgAquatic.getSize().y); // Scale to screen height
      bgSprite.setPosition(bgWorldX, bgWorldY);
      win.draw(bgSprite);
    } else {
      // standard background logic (Plains/Aerial) with PARALLAX
      // Background moves at 60% speed relative to camera for depth
      float bgWorldX = camera->getCamX() * 0.6f;
      float bgWorldY = camera->getCamY();

      bgSprite.setPosition(bgWorldX, bgWorldY);
      win.draw(bgSprite);

      // PARALLAX CLOUDS (Ahmed Ali's Request)
      // only show clouds in Plains (0) and Aerial (2) biomes
      if (lastBiomeZone == 0 || lastBiomeZone == 2) {
        // clouds move even slower for more depth (0.15 parallax)
        float cloudWorldX = camera->getCamX() * 0.85f;
        float cloudWorldY = -100; // high in the sky
        float camLeft  = camera->getCamX();
        float camRight = camLeft + camera->getScreenW();

        // draw multiple clouds across the level width — cull off-screen ones
        for (int i = 0; i < width / 10; i++) {
          float cx = cloudWorldX + (i * 800);
          if (cx + 800 < camLeft || cx > camRight) continue;
          cloudSprite.setPosition(cx, cloudWorldY + (i % 2 * 50));
          win.draw(cloudSprite);
        }
      }
    }

    // 2. Foreground Block Layer
    drawBlocks(win);
  }

  // spawn table accessors (used by EntityManager)
  SpawnPoint *getSpawnTable() override { return spawnTable; }
  int getSpawnCount() const override { return spawnCount; }

  // collision query — called by PlayState every frame
  // returns true when the block at grid (col, row) is solid.
  // aquatic-biome water blocks and air blocks are NOT solid for the player.
  bool isSolidAt(int col, int row) const override {
    if (col < 0 || col >= width || row < 0 || row >= height)
      return false;
    if (!grid[col][row])
      return false;
    return grid[col][row]->isSolid();
  }

  bool isPlatformAt(int col, int row) const override {
    if (col < 0 || col >= width || row < 0 || row >= height)
      return false;
    if (!grid[col][row])
      return false;
    return grid[col][row]->isPlatform();
  }

  // biome query helper for entities
  int getBiomeAt(double worldX) const override {
    int col = (worldX / blockSize);
    return getBiomeByCol(col);
  }

  // block pixel size (32 px — matches Block.h blockSize constant)
  int getBlockSize() const override { return blockSize; }
};