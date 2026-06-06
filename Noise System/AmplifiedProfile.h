#pragma once
#include "NoiseProfile.h"



class AmplifiedProfile : public NoiseProfile {
private:
    double amplitude;
    double frequency;
    double translation;
    int octaves;
    double persistence;

public:
    AmplifiedProfile() : amplitude(2.5), frequency(0.06), translation(0.0), octaves(6), persistence(0.6) {}

    virtual double getAmplitude() const { return amplitude; }
    virtual double getFrequency() const { return frequency; }
    virtual double getTranslation() const { return translation; }
    virtual int    getOctaves() const { return octaves; }
    virtual double getPersistence() const { return persistence; }
    virtual const char* getProfileName() const { return "Amplified"; }

    void setAmplitude(double value) { amplitude = value; }
    void setFrequency(double value) { frequency = value; }
    void setTranslation(double value) { translation = value; }
};