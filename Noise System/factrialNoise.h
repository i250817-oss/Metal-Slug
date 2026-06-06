#pragma once
#include "PerlinNoise.h"
#include "NoiseProfile.h"

using namespace std;



//fractalNoise
//AS PER PROJECT REQUIREMENT: "Fractal noise is n number of Perlin Noise layers 
//stacked on top of each other for more diversity and detail."
//
//this class wraps PerlinNoise and delegates to its paper-correct methods:
//- getNoise()      → uses PerlinNoise::generateHeightOctaves() (1/f octave loop)
//- getTurbulence() → uses PerlinNoise::generateTurbulence()    (abs() per paper appendix)
//
//the NoiseProfile provides frequency, amplitude, translation which are passed
//directly to the PerlinNoise methods — NO double-multiplication of frequency.
class FractalNoise {
private:
    PerlinNoise perlin;

public:
    //getNoise — fractal Brownian motion (1/f octave stacking)
    //AS PER PAPER: sum of Noise(point * 2^i) / 2^i
    //x, y are RAW world coordinates — the profile's frequency is applied ONCE inside
    double getNoise(double x, double y, const NoiseProfile* profile) const {
        return perlin.generateHeightOctaves(
            x, y,
            profile->getFrequency(),
            profile->getAmplitude(),
            profile->getTranslation()
        );
    }

    //getTurbulence — same as getNoise but with abs() for sharp creases
    //AS PER PAPER APPENDIX: t += abs(Noise(p / scale) * scale)
    double getTurbulence(double x, double y, const NoiseProfile* profile) const {
        return perlin.generateTurbulence(
            x, y,
            profile->getFrequency(),
            profile->getAmplitude(),
            profile->getTranslation()
        );
    }

    //direct access to raw single-sample perlin for quick lookups
    double getRawPerlin(double x, double y) const {
        return perlin.perlin(static_cast<float>(x), static_cast<float>(y));
    }
};