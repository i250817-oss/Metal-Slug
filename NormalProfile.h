#pragma once
#include "NoiseProfile.h"



class NormalProfile : public NoiseProfile {
private:
    double amplitude;
    double frequency;
    double translation;
    int octaves;
    double persistence;

public:
    NormalProfile() : amplitude(1.0), frequency(0.08), translation(0.0), octaves(4), persistence(0.5) {}

    virtual double getAmplitude() const { return amplitude; }
    virtual double getFrequency() const { return frequency; }
    virtual double getTranslation() const { return translation; }
    virtual int    getOctaves() const { return octaves; }
    virtual double getPersistence() const { return persistence; }
    virtual const char* getProfileName() const { return "Normal"; }

    void setAmplitude(double value) { amplitude = value; }
    void setFrequency(double value) { frequency = value; }
    void setTranslation(double value) { translation = value; }
};