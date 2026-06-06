#pragma once
#include "GameMode.h"
#include "../Level and Environment System/Level1.h"
#include "../Level and Environment System/Level2.h"
#include "../Level and Environment System/Level3.h"
#include "../Level and Environment System/BossLevel.h"



//the Gauntlet: 3 levels + 1 boss level. Fixed enemy positions. Horizontal + vertical scrolling.
class SurvivalMode : public GameMode {
private:
    Level* levels[4];
    int currentLevelIndex;
    bool fusionOnCooldown;
    float fusionCooldownTimer;

public:
    SurvivalMode(PlayerSoldier* chars[], int count);
    ~SurvivalMode();

    Level* getCurrentLevel() const;
    int getCurrentLevelIndex() const;
    bool getFusionOnCooldown() const;

    void start()  override;
    void update(float dt) override;
    void end()    override;
};