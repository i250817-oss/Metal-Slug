#pragma once



//levelProfile (Strategy pattern)
//controls fractal Perlin noise parameters for InfinityLevel terrain.
//concrete profiles: NormalProfile, AmplifiedProfile, FlatProfile.
class LevelProfile {
public:
    virtual float getScale()               const = 0;
    virtual int   getOctaves()             const = 0;
    virtual float getPersistence()         const = 0;
    virtual float getAmplitudeMultiplier() const = 0;
    virtual ~LevelProfile() {}
};

class NormalProfile : public LevelProfile {
public:
    float getScale()               const override { return 0.08f; }
    int   getOctaves()             const override { return 4;     }
    float getPersistence()         const override { return 0.52f; }
    float getAmplitudeMultiplier() const override { return 1.2f;  }
};

class AmplifiedProfile : public LevelProfile {
public:
    float getScale()               const override { return 0.06f; }
    int   getOctaves()             const override { return 6;     }
    float getPersistence()         const override { return 0.65f; }
    float getAmplitudeMultiplier() const override { return 2;  } //extreme mountains
};

class FlatProfile : public LevelProfile {
public:
    float getScale()               const override { return 0.15f; }
    int   getOctaves()             const override { return 2;     }
    float getPersistence()         const override { return 0.3f;  }
    float getAmplitudeMultiplier() const override { return 0.4f;  } //near-flat terrain
};

//factory
class LevelProfileFactory {
public:
    //type: "Normal", "Amplified", "Flat" (case-insensitive first char)
    static LevelProfile* create(const char* type) {
        if (!type) return new NormalProfile();
        if (type[0] == 'A' || type[0] == 'a') return new AmplifiedProfile();
        if (type[0] == 'F' || type[0] == 'f') return new FlatProfile();
        return new NormalProfile();
    }
};