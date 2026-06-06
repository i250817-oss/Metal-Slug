#pragma once
#include "Level.h"



//forward declaration
class NoiseProfile;

//infinite procedurally generated level. Uses Fractal/Perlin noise.
//extends left and right up to ~1 million blocks. Enemies spawn dynamically.
class CampaignLevel : public Level {
private:
    double seed;
    int generatedChunks;
    static const int CHUNK_WIDTH = 50;
    NoiseProfile* noiseProfile;

public:
    CampaignLevel(NoiseProfile* profile, double seed);
    ~CampaignLevel();

    double getSeed() const;
    int getGeneratedChunks() const;
    NoiseProfile* getNoiseProfile() const;

    void generateChunk(int chunkIndex);   //generate next chunk on demand
    void generate() override;
    void spawnEnemies() override;
    void spawnVehicles() override;
};