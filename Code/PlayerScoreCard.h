#pragma once
#include "ScoreCard.h"
#include <iostream>
#include <fstream>
#include "../headers/string.h"

//categories
#include "Enemies/EnemyScore.h"

using namespace std;




class PlayerScoreCard : public ScoreCard {
private:
    char playerName[64];
    int rebelKills;
    int zombieKills;
    int vehicleKills;
    int otherKills;

public:
    PlayerScoreCard() : ScoreCard(), rebelKills(0), zombieKills(0), vehicleKills(0), otherKills(0) {
        copyStringN(playerName, "UNKNOWN", 64);
    }

    PlayerScoreCard(const char* name) : ScoreCard(), rebelKills(0), zombieKills(0), vehicleKills(0), otherKills(0) {
        copyStringN(playerName, name, 64);
    }

    ~PlayerScoreCard() {
        ofstream outFile("score_history.txt", ios::app);
        if (outFile.is_open()) {
            outFile << "Player: " << playerName << " | Score: " << score 
                    << " | Infantry Kills: " << rebelKills 
                    << " | Undead Kills: " << zombieKills 
                    << " | Vehicle Kills: " << vehicleKills 
                    << " | Other Kills: " << otherKills << endl;
            outFile.close();
        }
        printResult();
    }

    void addEnemyKillScore(const char* enemyType, bool isAerial = false) override {
        EnemyScore* es = EnemyScore::create(enemyType);
        if (es) {
            es->addEnemyKillScore(enemyType, isAerial);
            this->score += es->getScore();
            delete es;

            if (areEqual(enemyType, "rebel") || areEqual(enemyType, "shielded") || areEqual(enemyType, "grenade") || areEqual(enemyType, "bazooka")) {
                rebelKills++;
            } else if (areEqual(enemyType, "zombie") || areEqual(enemyType, "mummy")) {
                zombieKills++;
            } else if (areEqual(enemyType, "flying_tara") || areEqual(enemyType, "enemy_sub") || areEqual(enemyType, "m15a_bradley")) {
                vehicleKills++;
            } else {
                otherKills++;
            }
        }
    }
    
    int getRebelKills() const { return rebelKills; }
    int getZombieKills() const { return zombieKills; }
    int getVehicleKills() const { return vehicleKills; }
    int getOtherKills() const { return otherKills; }

    //stubs for other categories (can be expanded later)
    void addBossKillScore(int phase) override {}
    void addFeatScore(const char* featType, int extraData = 0) override {}
    void addLevelClearScore(const char* mode, bool flawless = false) override {}

    void printResult() const override {
        cout << "\n----------------------------" << endl;
        cout << "| Player: " << playerName << endl;
        cout << "| Score: " << score << endl;
        cout << "----------------------------\n" << endl;
    }

    const char* getPlayerName() const { return playerName; }
};