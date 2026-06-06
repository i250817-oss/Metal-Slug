#pragma once
#include "../../headers/string.h"
#include "../ScoreCard.h"

class EnemyScore : public ScoreCard {
public:
    virtual void addEnemyKillScore(const char* enemyType, bool isAerial = false) override = 0;
    void addBossKillScore(int phase) override {}
    void addFeatScore(const char* featType, int extraData = 0) override {}
    void addLevelClearScore(const char* mode, bool flawless = false) override {}
    void printResult() const override {}

    static EnemyScore* create(const char* tag);
};

#include "RebelScore.h"
#include "ShieldedScore.h"
#include "BazookaScore.h"
#include "GrenadeScore.h"
#include "MummyScore.h"
#include "ZombieScore.h"
#include "MartianScore.h"

using namespace std;



inline EnemyScore* EnemyScore::create(const char* tag) {
    if (areEqual(tag, "rebel"))   return new RebelScore();
    if (areEqual(tag, "shielded")) return new ShieldedScore();
    if (areEqual(tag, "bazooka"))  return new BazookaScore();
    if (areEqual(tag, "grenade"))  return new GrenadeScore();
    if (areEqual(tag, "mummy"))    return new MummyScore();
    if (areEqual(tag, "zombie"))   return new ZombieScore();
    if (areEqual(tag, "alien"))    return new MartianScore();
    return nullptr;
}