#include "FusionCompanion.h"
#include "../Manager/EntityManager.h"
#include "../Projectile System/Bullet.h"

void FusionCompanion::attackNearestEnemy() {
  if (!active || !hasTarget || fireTimer > 0)
    return;

  double dx = targetEnemyX - posX;
  double dy = targetEnemyY - posY;
  double angle = atan2(dy, dx);

  pendingShot = new Bullet(posX, posY, angle, 15.0f, 1, true);
  fireTimer = FUSION_FIRE_COOLDOWN_BASE;
}
