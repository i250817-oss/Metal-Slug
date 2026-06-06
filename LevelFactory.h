#pragma once
#include "../Level and Environment System/Level1.h"
#include "../Level and Environment System/Level2.h"
#include "../Level and Environment System/Level3.h"
#include "../Level and Environment System/BossLevel.h"
#include "../Level and Environment System/InfinityLevel.h"
#include "../Manager/LevelManager.h"

using namespace std;

// Level construction factory
class LevelFactory {
public:
    static void createCampaign(LevelManager& lm) {
        lm.addLevel(new Level1());
        lm.addLevel(new Level2());
        lm.addLevel(new Level3());
        lm.addLevel(new BossLevel());
    }

    static void createSurvival(LevelManager& lm) {
        lm.addLevel(new Level1());
        lm.addLevel(new Level2());
        lm.addLevel(new Level3());
    }

    static void createInfinity(LevelManager& lm) {
        lm.addLevel(new InfinityLevel("Normal"));
    }
};