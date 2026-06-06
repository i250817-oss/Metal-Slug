#pragma once
#include "../Level and Environment System/CampaignLevel.h"
#include "../Noise System/NoiseProfile.h"
#include "GameMode.h"

class CampaignMode : public GameMode {
private:
  CampaignLevel *level;
  NoiseProfile *activeProfile;

  static const double FUSION_COOLDOWN;
  double fusionCooldownTimer;

  int enemyKillCounts[10];
  int vehicleKillCounts[3];

public:
  CampaignMode(PlayerSoldier *chars[], int count, NoiseProfile *profile);
  ~CampaignMode();

  CampaignLevel *getLevel() const;
  bool isQuotaMet() const;
  void recordKill(int typeId);
  void recordVehicleKill(int typeId);

  void start() override;
  void update(float dt) override;
  void end() override;
};