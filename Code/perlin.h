#pragma once
#include "../headers/headers.h"
#include <cstdlib>

using namespace std;



//perlin Noise
//ported from Ver1/perlin.H
//used by Level1/Level2/Level3 to drive procedural ground-row generation.

class vect2d {
private:
    float x;
    float y;
public:
    vect2d() {
        this->x = 0;
        this->y = 0;
    }
    vect2d(float x, float y) {
        this->x = x;
        this->y = y;
    }
    vect2d(const vect2d& obj) {
        this->x = obj.x;
        this->y = obj.y;
    }
    float getX() const { return x; }
    float getY() const { return y; }
};

//smoothstep fade — cubic interpolation for smooth Perlin transitions
inline float perlinFade(float t) {
    if (t < 0) return 0;
    if (t > 1) return 1;
    return (3 * pow(t, 2) - 2 * pow(t, 3));
}

//hash function — combines (ix, iy) into a pseudo-random unsigned int
inline unsigned int hashRandom(int ix, int iy) {
    unsigned int hash = ix * (static_cast<unsigned int>(pow(2, 20)) - 1)
                      + iy * (static_cast<unsigned int>(pow(2, 15)) - 1);
    hash ^= hash >> 10;
    hash *= 50;
    hash ^= hash >> 15;
    hash ^= hash >> 10;
    return hash;
}

//maps grid point (ix, iy) → a unit-length gradient vector
inline vect2d hashFunction(int ix, int iy) {
    int h = hashRandom(ix, iy);
    float angle = h * 2 * 3.14159265f + 1.9392341221143214234f;
    return vect2d(cos(angle), sin(angle));
}

//dot product of the gradient at grid point (xi,yi) with the offset to (i,j)
inline float dotGridGradient(int xi, int yi, float i, float j) {
    vect2d gradient = hashFunction(xi, yi);
    float dx = i - xi;
    float dy = j - yi;
    return (dx * gradient.getX() + dy * gradient.getY());
}

//linear interpolation helper
inline float perlinLerp(float p1, float p2, float weight) {
    return p1 + weight * (p2 - p1);
}

//main Perlin noise function — returns a value in roughly [-1, +1]
inline float perlin(float x, float y) {
    int x0 = floor(x);
    int x1 = x0 + 1;
    int y0 = floor(y);
    int y1 = y0 + 1;

    float Distancex = x - x0;
    float Distancey = y - y0;

    float xalpha = perlinFade(Distancex);
    float yalpha = perlinFade(Distancey);

    float noise00 = dotGridGradient(x0, y0, x, y);
    float noise10 = dotGridGradient(x1, y0, x, y);
    float noise01 = dotGridGradient(x0, y1, x, y);
    float noise11 = dotGridGradient(x1, y1, x, y);

    float nx0 = perlinLerp(noise00, noise10, xalpha);
    float nx1 = perlinLerp(noise01, noise11, xalpha);

    return perlinLerp(nx0, nx1, yalpha);
}