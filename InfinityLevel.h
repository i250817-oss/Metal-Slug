#pragma once
#include "../Noise System/NoiseProfileFactory.h"
#include "../Noise System/factrialNoise.h"
#include "LevelBase.h"

using namespace std;


//infinityLevel
//WORLD a.k.a Campaign Mode.
//
//ken Perlin Paper Principles Applied:
//1. TWO uncorrelated noise channels (widely spaced Y offsets = decorrelated)
//- CHANNEL 1: Biome trend  (low freq, y=55.7)  → selects
//Aerial/Plains/Aquatic
//- CHANNEL 2: Local detail (high freq, y=7.3)   → hills, valleys, depth
//2. sin() composition with noise → smooth, climbable mountain profiles
//"Through functional composition we may do many different things with
//the value returned by the Noise() function" — Ken Perlin 1985
//3. Profile-driven: Normal/Amplified/Flat polymorphically set noise inputs
//4. FractalNoise delegates to PerlinNoise::generateHeightOctaves() which
//implements the paper's 1/f octave loop: Noise(point*2^i)/2^i
//
//note: Enemy Spawning will be done dynamically at runtime (No fixed
//positions). To complete the single Campaign Level, you must kill 5 of each of
//the enemies. The player must also destroy 3 of each enemy vehicle.
class InfinityLevel : public LevelBase {
private:
  NoiseProfile *noiseProfile;
  FractalNoise fNoise;

  //completion Tracking
  int kills_rebel, kills_shielded, kills_grenade, kills_bazooka;
  int kills_zombie, kills_mummy, kills_alien;
  int kills_tara, kills_sub, kills_bradley;

  float spawnTimer;
  float timeAccumulator;

  //... (getBiomeIdxAtHeight and calculateGroundRow omitted for brevity in instruction, keeping them)

  //biome Mapping based on Height
  int getBiomeIdxAtHeight(int groundRow) const {
    if (groundRow > 52)
      return 1; //AQUATIC (seafloor below waterSurfaceRow=50)
    if (groundRow < 28)
      return 2; //AERIAL  (mountain peaks)
    return 0;   //PLAINS
  }

protected:
  //fractal Height Generation with sin() composition
  //
  //fractalNoise::getNoise() now correctly delegates to PerlinNoise's
  //generateHeightOctaves() which applies frequency ONCE internally.
  //so we pass RAW world coordinates — no pre-scaling needed.
  //
  //two uncorrelated channels (widely spaced Y offsets per paper principle):
  //Channel 1 (biome):  low effective freq → slow transitions
  //Channel 2 (detail): profile freq → terrain bumps
  //
  //sin() composition creates smooth, climbable mountain profiles.
  //adjacent columns differ by < 1 row → NO vertical cliffs.
  //pure Math Helper for Terrain Height (const)
  int calculateGroundRow(int col) const {
    //variable Width Biomes (Domain Warping)
    //as requested: we add Perlin noise ("k * perlin") to the phase of the sine
    //wave. This randomly stretches and squashes the sine wave, giving each
    //hill, plain, and aquatic basin a unique, random width while keeping the
    //curve perfectly smooth!

    //1. Get a low-frequency perlin noise for the warp (ranges ~[-0.5, 0.5])
    double warpNoise = fNoise.getNoise(col * 0.008, 88.3, noiseProfile);

    //2. Add k * perlin to the base phase.
    //base freq 0.045 + warp creates biome widths varying from ~70 to ~250
    //blocks.
    double phase = col * 0.045 + warpNoise * 3.5;

    //3. Generate the smooth mountain-to-water cycle
    double waveShape = sin(phase);

    //4. SHAPING FUNCTION: We cube the sine wave (waveShape^3).
    //this keeps the peaks (+1.0) and deep water (-1.0) exactly the same,
    //but it drastically flattens the values near 0 to create wide, beautiful
    //"plains".
    double flatPlainsShape = waveShape * waveShape * waveShape;

    //map the shaped sine wave [-1.0, 1.0] to world rows.
    //we use a separate multiplier for mountains vs water so we can
    //keep the water deep but lower the mountain peaks as requested.
    //because flatPlainsShape is x^3, its slope is 0 at the center, meaning
    //this change in amplitude is perfectly smooth with no sharp kinks!
    double amplitude = (flatPlainsShape > 0) ? 14.0 : 22.0;

    //center at row 37 (plains).
    //+1.0 -> row 37 - 14 = 23 (lower, more reasonable mountain peak)
    //-1.0 -> row 37 + 22 = 59 (still huge deep water basin)
    double baseRow = 37.0 - flatPlainsShape * amplitude;

    //CHANNEL 2 — Local Detail (higher frequency)
    //add just a tiny bit of fractal noise for grass/dirt bumps (max ±2 blocks)
    double detailSignal = fNoise.getNoise(col, 7.3, noiseProfile);

    int row = (baseRow + detailSignal * 2.0);

    //safe world bounds
    if (row < 5)
      row = 5;
    if (row > height - 7)
      row = height - 7;

    return row;
  }

  int getGroundRow(int col) override { return calculateGroundRow(col); }

  //dynamic Biome Override
  int getBiomeByCol(int col) const override {
    //we now call the const helper directly, eliminating the const_cast
    //completely!
    int gr = calculateGroundRow(col);
    return getBiomeIdxAtHeight(gr);
  }

public:
  InfinityLevel(const char *profileType = "Normal")
      : LevelBase(99, 10000, 70), kills_rebel(0), kills_shielded(0),
        kills_grenade(0), kills_bazooka(0), kills_zombie(0), kills_mummy(0),
        kills_alien(0), kills_tara(0), kills_sub(0), kills_bradley(0),
        spawnTimer(0), timeAccumulator(0) {
    noiseProfile = NoiseProfileFactory::create(profileType);
  }

  virtual ~InfinityLevel() { delete noiseProfile; }

  void load() override {
    LevelBase::load();

    //debug: print biome map for verification
    cout << "[InfinityLevel] Biome map (first 600 blocks):" << endl;
    for (int c = 0; c < 600; c += 25) {
      int gr = getGroundRow(c);
      int bi = getBiomeIdxAtHeight(gr);
      const char *name = (bi == 0)   ? "PLAINS "
                         : (bi == 1) ? "AQUATIC"
                                     : "AERIAL ";
      cout << "  col " << c << " row " << gr << " " << name << endl;
    }
  }

  void update(float dt) override {
    LevelBase::update(dt);
    timeAccumulator += dt;
    spawnTimer += dt;

    //dynamic Spawning Logic
    //spawns a batch of enemies ahead of the player every 5-8 seconds.
    if (camera && spawnTimer > (5 + (rand() % 4))) {
      float playerX = camera->getCamX() + camera->getScreenW() / 2;
      int currentBiome = getBiomeAt(playerX);

      //define pools based on biome
      const char *pool[10];
      int poolSize = 0;

      if (currentBiome == 0) { //PLAINS
        const char *plainsPool[] = {"rebel", "shielded", "grenade", "bazooka", "mummy", "zombie", "m15a_bradley"};
        poolSize = 7;
        for(int i=0; i<poolSize; i++) pool[i] = plainsPool[i];
      } else if (currentBiome == 1) { //AQUATIC
        const char *waterPool[] = {"zombie", "enemy_sub"};
        poolSize = 2;
        for(int i=0; i<poolSize; i++) pool[i] = waterPool[i];
      } else { //AERIAL
        const char *airPool[] = {"alien", "flying_tara"};
        poolSize = 2;
        for(int i=0; i<poolSize; i++) pool[i] = airPool[i];
      }

      if (poolSize > 0) {
        const char *selected = pool[rand() % poolSize];
        
        //batch Size: Infantry 2-4, Vehicles 1, Aliens/Undead 2-3
        int batchSize = 1;
        if (areEqual(selected, "rebel") || areEqual(selected, "shielded") || 
            areEqual(selected, "grenade") || areEqual(selected, "bazooka")) {
            batchSize = 2 + (rand() % 3);
        } else if (areEqual(selected, "zombie") || areEqual(selected, "mummy") || areEqual(selected, "alien")) {
            batchSize = 2 + (rand() % 2);
        }

        float spawnX = playerX + 900;
        int startCol = (spawnX / blockSize);

        static int runningSpawnCount = 0;
        for (int i = 0; i < batchSize; i++) {
          int col = startCol + (i * 2); //space them out
          if (col < width - 10) {
            //wrap to reuse the fixed-size table in an infinite world
            int idx = runningSpawnCount % maxSpawns;
            
            int row = getFirstSolidRow(col);
            if (areEqual(selected, "flying_tara") || areEqual(selected, "alien")) row = 5 + (rand() % 10);
            if (areEqual(selected, "enemy_sub")) row = 55 + (rand() % 5);

            spawnTable[idx].setEntityType(selected);
            spawnTable[idx].setCol(col);
            spawnTable[idx].setRow(row);
            spawnTable[idx].setBiomeZone(getBiomeByCol(col));
            spawnTable[idx].setSpawned(false);
            
            runningSpawnCount++;
            if (spawnCount < maxSpawns) {
                spawnCount++;
            }
          }
        }
      }
      spawnTimer = 0;
    }

    //completion Condition
    //"kill 5 of each type of enemies. 3 of each enemy vehicles must also be
    //destroyed."
    bool killsMet =
        (kills_rebel >= 5 && kills_shielded >= 5 && kills_grenade >= 5 &&
         kills_bazooka >= 5 && kills_zombie >= 5 && kills_mummy >= 5 &&
         kills_alien >= 5 && kills_tara >= 3 && kills_sub >= 3 && kills_bradley >= 3);

    if (killsMet)
      isComplete = true;

    if (camera) {
      float playerX = camera->getCamX() + camera->getScreenW() / 2;
      if (playerX > width * blockSize * 0.98f)
        isComplete = true;
    }
  }

  //external Kill Registry
  void reportKill(const char *type) override {
    if (!type) return;
    if (areEqual(type, "rebel"))          kills_rebel++;
    else if (areEqual(type, "shielded"))  kills_shielded++;
    else if (areEqual(type, "grenade"))   kills_grenade++;
    else if (areEqual(type, "bazooka"))   kills_bazooka++;
    else if (areEqual(type, "zombie"))    kills_zombie++;
    else if (areEqual(type, "mummy"))     kills_mummy++;
    else if (areEqual(type, "alien"))     kills_alien++;
    else if (areEqual(type, "flying_tara")) kills_tara++;
    else if (areEqual(type, "enemy_sub"))   kills_sub++;
    else if (areEqual(type, "m15a_bradley")) kills_bradley++;
  }
};