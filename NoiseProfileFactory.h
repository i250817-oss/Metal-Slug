#pragma once
#include "NoiseProfile.h"
#include "NormalProfile.h"
#include "AmplifiedProfile.h"
#include "FlatProfile.h"
#include "../headers/string.h"

using namespace std;




class NoiseProfileFactory {
public:
  
int normalType;

    static NoiseProfile* create(int normalType) {
        switch (normalType) {
            case 1: return new NormalProfile();
            case 2: return new AmplifiedProfile();
            case 3: return new FlatProfile();
            //1-> Normal, 2-> Amplified, 3-> Flat

            default: return new NormalProfile();  //default to normal
        }
    }

    static NoiseProfile* create(const char* typeName) {
        if (areEqual(typeName, "Normal")) 
        return new NormalProfile();
        if (areEqual(typeName, "Amplified")) 
        return new AmplifiedProfile();
        if (areEqual(typeName, "Flat"))
         return new FlatProfile();
        return new NormalProfile();  //default
    }
};