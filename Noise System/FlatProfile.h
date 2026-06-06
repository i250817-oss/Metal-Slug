#pragma once
#include "NoiseProfile.h"



//flat: no dramatic peaks or deep oceans. Opposite of Amplified.
class FlatProfile : public NoiseProfile {
private:
    double amplitude;
    double frequency;
    double translation;
    int octaves;
    double persistence;

public:
    FlatProfile() : amplitude(0.4), frequency(0.12), translation(0.0), octaves(2), persistence(0.3) {}

    virtual double getAmplitude() const { return amplitude; }
    virtual double getFrequency() const { return frequency; }
    virtual double getTranslation() const { return translation; }
    virtual int    getOctaves() const { return octaves; }
    virtual double getPersistence() const { return persistence; }
    virtual const char* getProfileName() const { return "Flat"; }

    void setAmplitude(double value) { amplitude = value; }
    void setFrequency(double value) { frequency = value; }
    void setTranslation(double value) { translation = value; }
};