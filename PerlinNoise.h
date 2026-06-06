#pragma once
#include "../headers/headers.h"

using namespace std;



class PerlinNoise {
public:
    class vect2d {
    private:
        float x;
        float y;

    public:
        vect2d() : x(0), y(0) {}
        vect2d(float x, float y) : x(x), y(y) {}
        vect2d(const vect2d& obj) : x(obj.x), y(obj.y) {}

        float getX() const { return x; }
        float getY() const { return y; }
    };

    float fade(float t) const {
        //direct line from the project .pdf
        // If [z,y,~] is not on the integer lattice we compute a smooth
        // interpolation of the four surrounding corners.between lattice equation
        // coefficients, applied first in x (along lattice edges), then in y
        // (within lattice z-faces), then in z. We then evaluate this
        // interpolated linear equation at [x,y,z].
        //so the cubic interpolation be
        //for smoothing here kindly click the link https://en.wikipedia.org/wiki/Smoothstep
        if (t < 0)
            return 0;
        if (t > 1)
            return 1;
        //FIX: upgraded from 3t^2 - 2t^3 (original 1985) to 6t^5 - 15t^4 + 10t^3 (Perlin's 2002 improvement)
        //the old formula had a non-zero second derivative at endpoints which caused visible grid artifacts
        //this improved version has BOTH first AND second derivative = 0 at t=0 and t=1
        //meaning transitions are smoother with zero curvature at boundaries — no grid lines visible
        //also avoided pow() since it's slow — direct multiplication is much faster
        return t * t * t * (t * (t * 6 - 15) + 10);
        //that formula is used for
        //different shading languages and is a common choice for smoothing in Perlin noise implementations.
        //it ensures that the transition between grid points is smooth and continuous, which helps to create more natural-looking noise patterns.
        //the function takes a value t between 0 and 1 and returns a smoothed value that transitions smoothly from 0 to 1 as t goes from 0 to 1.
        //this is important for creating smooth gradients in the noise pattern, which can help to reduce artifacts and create more visually appealing results.
    }

    unsigned int hashRandom(int ix, int iy) const {
        //FIX: replaced weak hash with proven constants to avoid visible pattern repetition at large coordinates
        //the old formula used pow(2,20) and pow(2,15) which are floating point operations — slow and imprecise
        //also multiplying by just 50 was too weak — nearby grid points were getting similar hash values
        //causing visible grid-like artifacts in the noise output at large coordinate values
        //new formula uses well-known hash constants (1619 and 31337) that are large primes
        //large primes ensure better distribution — two nearby inputs give very different outputs
        unsigned int hash = (ix * 1619 + iy * 31337);

        hash ^= hash >> 10;
        hash *= 3212845933;  //strong mixing constant (used in many production hash functions)
        hash ^= hash >> 15;
        //dont know what i am doing but i am making it random to minimize the hashing collision
        hash ^= hash >> 10;
        //kuch change krna ho bta dana
        return hash;
    }

    vect2d hashFunction(int ix, int iy) const {
        //for a function to be hashed it must satisfy the three properties of a hash function which are determinism, uniform distribution and efficiency
        //1) determinism means that the same input will always produce the same output. In the context of a hash function for Perlin noise, this means that the same grid coordinates (ix, iy) will always produce the same gradient vector. This is important because it ensures that the noise pattern is consistent and repeatable across different runs of the program.
        //2) fixed length of output ie the length of all output is always fixed
        //3) irreversible output (can't determine the input via reverse engineering)

        int h = hashRandom(ix, iy);
        float angle = h * 2 * 3.14159265f + 1.9392341221143214234; //forget about that i have no idea to make it random
        return vect2d(cos(angle), sin(angle)); //the hash function takes the grid coordinates (ix, iy) as input and generates a pseudo-random gradient vector for that grid point. The hash value is used to calculate an angle, which is then converted into a unit vector using cosine and sine functions. This ensures that the gradient vectors are uniformly distributed in all directions, which is important for creating natural-looking noise patterns.
    }

    float dotGridGradient(int xi, int yi, float i, float j) const {
        vect2d gradient = hashFunction(xi, yi);

        float dx = i - xi; //the formula for the vector be (x1,y1) and (x2,y2) is (x1-x2,y1-y2) so here we are treating the grid point as (xi,yi) and the input point as (i,j) so the vector from the grid point to the input point is (i-xi,j-yi)
        float dy = j - yi;

        return (dx * gradient.getX() + dy * gradient.getY()); //the dot product of the distance vector and the gradient vector gives us the contribution of that grid point to the final noise value at the input point (i,j)
    }

    float lerp(float p1, float p2, float weight) const {
        return p1 + weight * (p2 - p1); //the formula for linear interpolation is (1-weight)*p1 + weight*p2 which can be rearranged to p1 + weight*(p2-p1)
    }

    float perlin(float x, float y) const {
        int x0 = floor(x);
        int x1 = x0 + 1;
        int y0 = floor(y);
        int y1 = y0 + 1;

        float Distancex = x - x0;
        float Distancey = y - y0;

        float xalpha = fade(Distancex);
        float yalpha = fade(Distancey);

        float noise00 = dotGridGradient(x0, y0, x, y);
        float noise10 = dotGridGradient(x1, y0, x, y);
        float noise01 = dotGridGradient(x0, y1, x, y);
        float noise11 = dotGridGradient(x1, y1, x, y);

        float nx0 = lerp(noise00, noise10, xalpha);  //lerping through the bottom two corners of the x axis
        float nx1 = lerp(noise01, noise11, xalpha);  //lerping through the top two corners of the x axis

        return lerp(nx0, nx1, yalpha); //finally lerping through the y axis to get the final noise value at the input point (x,y)
    }

    double generateHeight(double x, double y, double frequency, double amplitude, double translation) const {
        return perlin(static_cast<float>(x * frequency), static_cast<float>(y * frequency)) * amplitude + translation;
    }

   
    double generateHeightOctaves(double x, double y, double frequency, double amplitude, double translation, double pixelSize = 0.01) const {
        double total = 0;
        double currentFreq = frequency;
        double currentAmp = amplitude;

        //AS PER PAPER: "a 1/f signal over space can be simulated by looping over octaves"
        //noise(point * 2^i) / 2^i
        //frequency doubles each octave (finer detail)
        //amplitude halves each octave (1/f relationship)
        //stops when feature size drops below pixel level
        while ((1.0 / currentFreq) > pixelSize) {
            total += perlin( static_cast<float>(x * currentFreq), static_cast<float>(y * currentFreq) ) * currentAmp;
            currentFreq *= 2.0;
            currentAmp *= 0.5;   //halve amplitude: 1/2^i (NOT amplitude/freq which was 12.5× wrong!)
        }

        return total + translation;
    }

    double generateTurbulence(double x, double y, double frequency, double amplitude, double translation, double pixelSize = 0.01) const {
        double total = 0;
        double scale = 1.0;

        //EXACTLY matching paper:
        //function turbulence(p)
        //scale = 1
        //while (scale > pixelsize)
        //t += abs(Noise(p / scale) * scale)
        //scale /= 2
        while (scale > pixelSize) {
            total += abs(perlin(
                        static_cast<float>(x * frequency / scale),
                        static_cast<float>(y * frequency / scale)
                    )) * (amplitude * scale);
          
            scale /= 2.0;
        }

        return total + translation;
    }
};