#pragma once



class NoiseProfile {
public:
    virtual ~NoiseProfile() = default;
    virtual double getAmplitude() const = 0;
    virtual double getFrequency() const = 0;
    virtual double getTranslation() const = 0;
    virtual int    getOctaves() const = 0;
    virtual double getPersistence() const = 0;
    virtual const char* getProfileName() const = 0;
};