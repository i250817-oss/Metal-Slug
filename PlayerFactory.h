#pragma once
#include "../Character System/EriKasamoto.h"
#include "../Character System/FiolinaGermi.h"
#include "../Character System/MarcoRossi.h"
#include "../Character System/Tarma.h"
#include "../Manager/EntityManager.h"

// Factory to create all playable characters
class PlayerFactory {
public:
  static void createAll(EntityManager &em, float spawnX, float spawnY) {
    MarcoRossi *marco = new MarcoRossi(spawnX, spawnY);
    marco->applyCharacterBuffs();
    em.addEntity(marco);

    Tarma *tarma = new Tarma(spawnX, spawnY);
    tarma->applyCharacterBuffs();
    em.addEntity(tarma);

    EriKasamoto *eri = new EriKasamoto(spawnX, spawnY);
    eri->applyCharacterBuffs();
    em.addEntity(eri);

    FiolinaGermi *fio = new FiolinaGermi(spawnX, spawnY);
    fio->applyCharacterBuffs();
    em.addEntity(fio);
  }
};