#pragma once
#include "PlayerSoldier.h"

using namespace sf;



class FiolinaGermi : public PlayerSoldier {
private:
  double ammoPickupMultiplier;
  double fireRate;
  float superchargeTimer;
  double fireTime;
  bool supercharged;

public:
  FiolinaGermi(double posX, double posY) : PlayerSoldier(posX, posY) {
    ammoPickupMultiplier = 1.5;
    superchargeTimer = 0;
    supercharged = false;
    baseMaxSpeed = 5;

    if (mainSheet.loadFromFile("Sprites/Fiolina Germi.png")) {
      torsoSprite.setTexture(mainSheet);
      legSprite.setTexture(mainSheet);
    }
    torsoSprite.setScale(scaleFactor, scaleFactor);
    legSprite.setScale(scaleFactor, scaleFactor);

    frameWidth  = 27;
    frameHeight = 26;
    legOffsetY  = 26 * scaleFactor;   //torso raw height ≈ 26px  →  78px scaled
    legOffsetX  = 0;

    animDelay = 4.0f / 60.0f;

    // IDLE torso
    idleTorsoFrames[0] = IntRect(10,  500, 27, 26);
    idleTorsoFrames[1] = IntRect(42,  501, 27, 26);
    idleTorsoFrames[2] = IntRect(74,  502, 27, 26);
    idleTorsoFrames[3] = IntRect(106, 501, 27, 26);
    numIdleFrames = 4;

    // IDLE legs
    idleLegFrames[0] = IntRect(10, 534, 22, 26);
    idleLegFrames[1] = IntRect(10, 534, 22, 26);
    idleLegFrames[2] = IntRect(10, 534, 22, 26);
    idleLegFrames[3] = IntRect(10, 534, 22, 26);

    // RUN torso
    runTorsoFrames[0]  = IntRect(10,  500, 27, 26);   //sheet frame_2
    runTorsoFrames[1]  = IntRect(42,  501, 27, 26);   //frame_3
    runTorsoFrames[2]  = IntRect(74,  502, 27, 26);   //frame_4
    runTorsoFrames[3]  = IntRect(106, 501, 27, 26);   //frame_5
    runTorsoFrames[4]  = IntRect(138, 500, 27, 26);   //frame_6
    runTorsoFrames[5]  = IntRect(170, 500, 28, 26);   //frame_7
    runTorsoFrames[6]  = IntRect(203, 501, 28, 26);   //frame_8
    runTorsoFrames[7]  = IntRect(236, 502, 28, 26);   //frame_9
    runTorsoFrames[8]  = IntRect(269, 502, 29, 26);   //frame_10
    runTorsoFrames[9]  = IntRect(303, 503, 28, 26);   //frame_11
    runTorsoFrames[10] = IntRect(336, 501, 26, 27);   //frame_12
    runTorsoFrames[11] = IntRect(367, 501, 27, 27);   //frame_13
    numRunFrames = 12;

    // RUN legs
    runLegFrames[0]  = IntRect(10,  534, 22, 26);
    runLegFrames[1]  = IntRect(37,  534, 25, 25);
    runLegFrames[2]  = IntRect(67,  535, 27, 23);
    runLegFrames[3]  = IntRect(99,  534, 25, 26);
    runLegFrames[4]  = IntRect(129, 534, 17, 26);
    runLegFrames[5]  = IntRect(151, 535, 18, 25);
    runLegFrames[6]  = IntRect(174, 535, 21, 25);
    runLegFrames[7]  = IntRect(200, 535, 26, 23);
    runLegFrames[8]  = IntRect(231, 535, 30, 25);
    runLegFrames[9]  = IntRect(266, 535, 24, 25);
    runLegFrames[10] = IntRect(295, 534, 18, 26);
    runLegFrames[11] = IntRect(318, 534, 20, 26);

    // JUMP torso
    jumpTorsoFrames[0] = IntRect(10,  821, 29, 26);
    jumpTorsoFrames[1] = IntRect(44,  823, 30, 24);
    jumpTorsoFrames[2] = IntRect(79,  825, 30, 22);
    jumpTorsoFrames[3] = IntRect(114, 825, 30, 22);
    jumpTorsoFrames[4] = IntRect(149, 825, 29, 22);
    jumpTorsoFrames[5] = IntRect(183, 825, 28, 23);
    numJumpFrames = 6;

    // JUMP legs
    jumpLegFrames[0] = IntRect(10,  853, 22, 30);
    jumpLegFrames[1] = IntRect(37,  853, 22, 30);
    jumpLegFrames[2] = IntRect(64,  853, 22, 29);
    jumpLegFrames[3] = IntRect(91,  853, 23, 28);
    jumpLegFrames[4] = IntRect(119, 853, 24, 25);
    jumpLegFrames[5] = IntRect(148, 853, 24, 22);

    // SHOOT torso
    shootTorsoFrames[0] = IntRect(10,  994, 48, 19);
    shootTorsoFrames[1] = IntRect(63,  994, 43, 19);
    shootTorsoFrames[2] = IntRect(119, 993, 31, 20);
    shootTorsoFrames[3] = IntRect(175, 992, 29, 21);
    shootTorsoFrames[4] = IntRect(209, 991, 29, 22);
    shootTorsoFrames[5] = IntRect(243, 994, 33, 19);
    shootTorsoFrames[6] = IntRect(281, 994, 33, 19);
    shootTorsoFrames[7] = IntRect(319, 994, 33, 20);
    shootTorsoFrames[8] = IntRect(357, 994, 32, 23);
    shootTorsoFrames[9] = IntRect(394, 993, 30, 25);
    numShootFrames = 10;

    // SHOOT UP torso
    shootUpTorsoFrames[0]  = IntRect(179, 1683, 50, 30);
    shootUpTorsoFrames[1]  = IntRect(234, 1690, 24, 23);
    shootUpTorsoFrames[2]  = IntRect(207, 1687, 22, 26);
    shootUpTorsoFrames[3]  = IntRect(179, 1683, 23, 30);
    shootUpTorsoFrames[4]  = IntRect(154, 1682, 20, 31);
    shootUpTorsoFrames[5]  = IntRect(131, 1680, 18, 33);
    shootUpTorsoFrames[6]  = IntRect(107, 1681, 19, 32);
    shootUpTorsoFrames[7]  = IntRect(83,  1680, 19, 33);
    shootUpTorsoFrames[8]  = IntRect(59,  1679, 19, 34);
    shootUpTorsoFrames[9]  = IntRect(34,  1665, 20, 48);
    shootUpTorsoFrames[10] = IntRect(10,  1654, 19, 59);
    numShootUpFrames = 11;

    // KNIFE torso
    knifeTorsoFrames[0] = IntRect(394, 993,  30, 25);   //shared w/ shoot end
    knifeTorsoFrames[1] = IntRect(10,  1150, 22, 30);
    knifeTorsoFrames[2] = IntRect(37,  1150, 26, 34);
    knifeTorsoFrames[3] = IntRect(68,  1149, 35, 32);
    knifeTorsoFrames[4] = IntRect(116, 1147, 46, 34);
    knifeTorsoFrames[5] = IntRect(167, 1139, 43, 40);
    knifeTorsoFrames[6] = IntRect(215, 1138, 42, 41);
    numKnifeFrames = 7;

    // HMG RUNNING torso
    runTorsoHMGFrames[0]  = IntRect(10,  3232, 38, 26);
    runTorsoHMGFrames[1]  = IntRect(53,  3232, 38, 26);
    runTorsoHMGFrames[2]  = IntRect(96,  3232, 38, 26);
    runTorsoHMGFrames[3]  = IntRect(139, 3232, 38, 25);
    runTorsoHMGFrames[4]  = IntRect(10,  3232, 38, 26);   //cycle
    runTorsoHMGFrames[5]  = IntRect(53,  3232, 38, 26);
    runTorsoHMGFrames[6]  = IntRect(96,  3232, 38, 26);
    runTorsoHMGFrames[7]  = IntRect(139, 3232, 38, 25);
    runTorsoHMGFrames[8]  = IntRect(10,  3232, 38, 26);   //cycle
    runTorsoHMGFrames[9]  = IntRect(53,  3232, 38, 26);
    runTorsoHMGFrames[10] = IntRect(96,  3232, 38, 26);
    runTorsoHMGFrames[11] = IntRect(139, 3232, 38, 25);

    // HMG FIRING torso
    shootTorsoHMGFrames[0] = IntRect(10,  3609, 38, 25);
    shootTorsoHMGFrames[1] = IntRect(75,  3610, 59, 25);
    shootTorsoHMGFrames[2] = IntRect(139, 3610, 37, 24);
    shootTorsoHMGFrames[3] = IntRect(203, 3609, 59, 26);

    // HMG FIRING UP torso
    shootUpTorsoHMGFrames[0] = IntRect(10, 4396, 20, 45);
    shootUpTorsoHMGFrames[1] = IntRect(35, 4372, 20, 69);
    shootUpTorsoHMGFrames[2] = IntRect(60, 4396, 20, 45);
    shootUpTorsoHMGFrames[3] = IntRect(85, 4371, 19, 70);
    numShootHMGFrames = 4;



    torsoSprite.setTextureRect(idleTorsoFrames[0]);
    legSprite.setTextureRect(idleLegFrames[0]);

    loadTransformationAssets("Sprites/Eri Kasamoto (Mummy).png");

    // --- Fiolina Germi Custom MS6 Frames ---

    // Standing knife / melee (knifeTorsoFrames) - 6 frames
    knifeTorsoFrames[0] = IntRect(10, 7109, 24, 36);
    knifeTorsoFrames[1] = IntRect(39, 7109, 26, 36);
    knifeTorsoFrames[2] = IntRect(70, 7109, 35, 37);
    knifeTorsoFrames[3] = IntRect(118, 7107, 46, 39);
    knifeTorsoFrames[4] = IntRect(169, 7099, 43, 48);
    knifeTorsoFrames[5] = IntRect(217, 7098, 42, 49);
    numKnifeFrames = 6;

    // Crouch transition (crouchSitFrames) - 4 frames
    crouchSitFrames[0] = IntRect(10, 7633, 34, 45);
    crouchSitFrames[1] = IntRect(49, 7637, 33, 41);
    crouchSitFrames[2] = IntRect(87, 7639, 36, 39);
    crouchSitFrames[3] = IntRect(87, 7639, 36, 39);

    // Crouched Idle (crouchIdleFrames) - 7 frames
    crouchIdleFrames[0] = IntRect(10, 7719, 41, 27);
    crouchIdleFrames[1] = IntRect(56, 7719, 42, 26);
    crouchIdleFrames[2] = IntRect(103, 7719, 42, 26);
    crouchIdleFrames[3] = IntRect(150, 7719, 42, 26);
    crouchIdleFrames[4] = IntRect(10, 7719, 41, 27);
    crouchIdleFrames[5] = IntRect(56, 7719, 42, 26);
    crouchIdleFrames[6] = IntRect(103, 7719, 42, 26);

    // Crouch-walk / Crouch Slide (crouchSlideFrames) - 6 frames
    crouchSlideFrames[0] = IntRect(10, 7786, 41, 27);
    crouchSlideFrames[1] = IntRect(56, 7786, 41, 26);
    crouchSlideFrames[2] = IntRect(102, 7786, 44, 26);
    crouchSlideFrames[3] = IntRect(151, 7786, 44, 26);
    crouchSlideFrames[4] = IntRect(200, 7786, 44, 26);
    crouchSlideFrames[5] = IntRect(249, 7786, 42, 26);

    // Crouching fire / shoot (crouchFireFrames) - 10 frames
    crouchFireFrames[0] = IntRect(10, 7850, 59, 26);
    crouchFireFrames[1] = IntRect(74, 7851, 56, 25);
    crouchFireFrames[2] = IntRect(139, 7851, 59, 25);
    crouchFireFrames[3] = IntRect(203, 7851, 56, 25);
    crouchFireFrames[4] = IntRect(10, 7850, 59, 26);
    crouchFireFrames[5] = IntRect(74, 7851, 56, 25);
    crouchFireFrames[6] = IntRect(139, 7851, 59, 25);
    crouchFireFrames[7] = IntRect(203, 7851, 56, 25);
    crouchFireFrames[8] = IntRect(10, 7850, 59, 26);
    crouchFireFrames[9] = IntRect(74, 7851, 56, 25);

    // Sitting and Knife (crouchMeleeFrames) - 9 frames
    crouchMeleeFrames[0] = IntRect(10, 8100, 36, 27);
    crouchMeleeFrames[1] = IntRect(51, 8100, 36, 27);
    crouchMeleeFrames[2] = IntRect(92, 8101, 35, 26);
    crouchMeleeFrames[3] = IntRect(132, 8093, 42, 34);
    crouchMeleeFrames[4] = IntRect(185, 8093, 46, 34);
    crouchMeleeFrames[5] = IntRect(236, 8094, 45, 33);
    crouchMeleeFrames[6] = IntRect(286, 8095, 41, 32);
    crouchMeleeFrames[7] = IntRect(332, 8100, 32, 27);
    crouchMeleeFrames[8] = IntRect(369, 8100, 36, 27);

    // HMG Crouch slide / moving (hmgCrouchSlideFrames) - 5 frames
    hmgCrouchSlideFrames[0] = IntRect(10, 8274, 37, 31);
    hmgCrouchSlideFrames[1] = IntRect(52, 8274, 36, 31);
    hmgCrouchSlideFrames[2] = IntRect(93, 8275, 37, 30);
    hmgCrouchSlideFrames[3] = IntRect(135, 8278, 42, 27);
    hmgCrouchSlideFrames[4] = IntRect(182, 8280, 46, 25);

    // Pistol Sliding fire (slideFireFrames) - 5 frames
    slideFireFrames[0] = IntRect(10, 8313, 68, 26);
    slideFireFrames[1] = IntRect(83, 8312, 60, 27);
    slideFireFrames[2] = IntRect(157, 8315, 68, 24);
    slideFireFrames[3] = IntRect(230, 8314, 60, 25);
    slideFireFrames[4] = IntRect(230, 8314, 60, 25);

    // HMG Sliding fire (hmgCrouchSlideFireFrames) - 4 frames
    hmgCrouchSlideFireFrames[0] = IntRect(10, 8313, 68, 26);
    hmgCrouchSlideFireFrames[1] = IntRect(83, 8312, 60, 27);
    hmgCrouchSlideFireFrames[2] = IntRect(157, 8315, 68, 24);
    hmgCrouchSlideFireFrames[3] = IntRect(230, 8314, 60, 25);

    // Running / whistle (neatTorsoFrames) - 16 frames
    neatTorsoFrames[0] = IntRect(10, 6577, 35, 37);
    neatTorsoFrames[1] = IntRect(50, 6577, 33, 37);
    neatTorsoFrames[2] = IntRect(88, 6574, 26, 40);
    neatTorsoFrames[3] = IntRect(119, 6576, 26, 38);
    neatTorsoFrames[4] = IntRect(150, 6577, 27, 37);
    neatTorsoFrames[5] = IntRect(182, 6575, 35, 39);
    neatTorsoFrames[6] = IntRect(222, 6577, 37, 37);
    neatTorsoFrames[7] = IntRect(264, 6577, 30, 37);
    neatTorsoFrames[8] = IntRect(299, 6577, 25, 37);
    neatTorsoFrames[9] = IntRect(329, 6577, 23, 37);
    neatTorsoFrames[10] = IntRect(357, 6577, 22, 37);
    neatTorsoFrames[11] = IntRect(384, 6577, 23, 37);
    neatTorsoFrames[12] = IntRect(412, 6577, 25, 37);
    neatTorsoFrames[13] = IntRect(442, 6577, 26, 37);
    neatTorsoFrames[14] = IntRect(473, 6577, 31, 37);
    neatTorsoFrames[15] = IntRect(509, 6572, 32, 42);
    numNeatFrames = 16;

    // --- Fiolina Germi Dedicated PDF Coordinate Mappings ---

    // neatLegFrames (4 frames)
    neatLegFrames[0] = IntRect(10, 534, 22, 26);
    neatLegFrames[1] = IntRect(10, 534, 22, 26);
    neatLegFrames[2] = IntRect(10, 534, 22, 26);
    neatLegFrames[3] = IntRect(10, 534, 22, 26);

    // runJumpTorsoFrames (6 frames) at y=924
    runJumpTorsoFrames[0] = IntRect(10, 924, 24, 31);
    runJumpTorsoFrames[1] = IntRect(39, 924, 28, 32);
    runJumpTorsoFrames[2] = IntRect(72, 924, 32, 31);
    runJumpTorsoFrames[3] = IntRect(109, 924, 28, 34);
    runJumpTorsoFrames[4] = IntRect(142, 924, 23, 31);
    runJumpTorsoFrames[5] = IntRect(170, 924, 23, 28);

    // runJumpLegFrames (6 frames) at y=963
    runJumpLegFrames[0] = IntRect(10, 963, 28, 23);
    runJumpLegFrames[1] = IntRect(43, 963, 29, 21);
    runJumpLegFrames[2] = IntRect(78, 963, 33, 19);
    runJumpLegFrames[3] = IntRect(116, 963, 30, 17);
    runJumpLegFrames[4] = IntRect(151, 963, 30, 17);
    runJumpLegFrames[5] = IntRect(186, 963, 26, 19);

    // shootDownTorsoFrames (6 frames) at y=2441
    shootDownTorsoFrames[0] = IntRect(10, 2441, 18, 54);
    shootDownTorsoFrames[1] = IntRect(33, 2441, 18, 55);
    shootDownTorsoFrames[2] = IntRect(56, 2440, 18, 56);
    shootDownTorsoFrames[3] = IntRect(79, 2441, 22, 30);
    shootDownTorsoFrames[4] = IntRect(106, 2441, 18, 35);
    shootDownTorsoFrames[5] = IntRect(129, 2441, 18, 36);

    // standing attack / frontMeleeFrames (6 frames) at y=1150
    frontMeleeFrames[0] = IntRect(10, 1150, 22, 30);
    frontMeleeFrames[1] = IntRect(37, 1150, 26, 34);
    frontMeleeFrames[2] = IntRect(68, 1149, 35, 32);
    frontMeleeFrames[3] = IntRect(116, 1147, 46, 34);
    frontMeleeFrames[4] = IntRect(167, 1139, 43, 40);
    frontMeleeFrames[5] = IntRect(215, 1138, 42, 41);

    // leftMeleeFrames (6 frames)
    leftMeleeFrames[0] = IntRect(10, 1150, 22, 30);
    leftMeleeFrames[1] = IntRect(37, 1150, 26, 34);
    leftMeleeFrames[2] = IntRect(68, 1149, 35, 32);
    leftMeleeFrames[3] = IntRect(116, 1147, 46, 34);
    leftMeleeFrames[4] = IntRect(167, 1139, 43, 40);
    leftMeleeFrames[5] = IntRect(215, 1138, 42, 41);

    // jumpMeleeFrames (6 frames)
    jumpMeleeFrames[0] = IntRect(10, 1150, 22, 30);
    jumpMeleeFrames[1] = IntRect(37, 1150, 26, 34);
    jumpMeleeFrames[2] = IntRect(68, 1149, 35, 32);
    jumpMeleeFrames[3] = IntRect(116, 1147, 46, 34);
    jumpMeleeFrames[4] = IntRect(167, 1139, 43, 40);
    jumpMeleeFrames[5] = IntRect(215, 1138, 42, 41);

    // crouchKnifeAttackFrames (6 frames) at y=2095
    crouchKnifeAttackFrames[0] = IntRect(10, 2095, 36, 25);
    crouchKnifeAttackFrames[1] = IntRect(51, 2095, 35, 25);
    crouchKnifeAttackFrames[2] = IntRect(91, 2095, 34, 25);
    crouchKnifeAttackFrames[3] = IntRect(130, 2091, 35, 29);
    crouchKnifeAttackFrames[4] = IntRect(178, 2089, 46, 31);
    crouchKnifeAttackFrames[5] = IntRect(229, 2081, 43, 39);

    // crouchKnife2HitFrames (6 frames) at y=2095
    crouchKnife2HitFrames[0] = IntRect(10, 2095, 36, 25);
    crouchKnife2HitFrames[1] = IntRect(51, 2095, 35, 25);
    crouchKnife2HitFrames[2] = IntRect(91, 2095, 34, 25);
    crouchKnife2HitFrames[3] = IntRect(130, 2091, 35, 29);
    crouchKnife2HitFrames[4] = IntRect(178, 2089, 46, 31);
    crouchKnife2HitFrames[5] = IntRect(229, 2081, 43, 39);

    // pistolReloadFrames (19 frames) at y=1064
    pistolReloadFrames[0] = IntRect(10, 1064, 32, 23);
    pistolReloadFrames[1] = IntRect(47, 1059, 29, 28);
    pistolReloadFrames[2] = IntRect(81, 1057, 28, 32);
    pistolReloadFrames[3] = IntRect(114, 1063, 38, 25);
    pistolReloadFrames[4] = IntRect(157, 1063, 38, 24);
    pistolReloadFrames[5] = IntRect(200, 1063, 39, 22);
    pistolReloadFrames[6] = IntRect(244, 1063, 39, 27);
    pistolReloadFrames[7] = IntRect(288, 1063, 34, 25);
    pistolReloadFrames[8] = IntRect(327, 1063, 34, 25);
    pistolReloadFrames[9] = IntRect(366, 1063, 35, 24);
    pistolReloadFrames[10] = IntRect(406, 1063, 33, 25);
    pistolReloadFrames[11] = IntRect(444, 1063, 31, 25);
    pistolReloadFrames[12] = IntRect(480, 1063, 31, 25);
    pistolReloadFrames[13] = IntRect(516, 1063, 35, 24);
    pistolReloadFrames[14] = IntRect(556, 1063, 35, 25);
    pistolReloadFrames[15] = IntRect(596, 1063, 47, 23);
    pistolReloadFrames[16] = IntRect(648, 1056, 44, 33);
    pistolReloadFrames[17] = IntRect(704, 1055, 47, 39);
    pistolReloadFrames[18] = IntRect(756, 1063, 52, 35);



    // --- Fiolina Germi Clean Fallbacks (to avoid coordinate scrambling) ---
    for (int i = 0; i < 5; i++) {
        crouchBoxerFrames[i] = IntRect(10, 7719, 41, 27);
    }
    for (int i = 0; i < 4; i++) {
        crouchHitFrames[i] = IntRect(10, 7719, 41, 27);
        crouchKnife2LoadFrames[i] = IntRect(10, 7719, 41, 27);
        crouchKnifeLoadFrames[i] = IntRect(10, 7719, 41, 27);
        knifeLoadFrames[i] = IntRect(10, 500, 27, 26);
        knifeStandLoadFrames[i] = IntRect(10, 500, 27, 26);
    }
    for (int i = 0; i < 35; i++) {
        dieAlienFrames[i] = IntRect(87, 7639, 36, 39);
        dieNormalFrames[i] = IntRect(87, 7639, 36, 39);
    }
    for (int i = 0; i < 20; i++) {
        dieAquaticFrames[i] = IntRect(87, 7639, 36, 39);
    }
    for (int i = 0; i < 21; i++) {
        dieFireFrames[i] = IntRect(87, 7639, 36, 39);
    }
    for (int i = 0; i < 11; i++) {
        drinkWaterFrames[i] = IntRect(10, 500, 27, 26);
    }
    
    // Fast run matches Fio's normal running frames
    fastRunTorsoFrames[0]  = IntRect(10,  500, 27, 26);
    fastRunTorsoFrames[1]  = IntRect(42,  501, 27, 26);
    fastRunTorsoFrames[2]  = IntRect(74,  502, 27, 26);
    fastRunTorsoFrames[3]  = IntRect(106, 501, 27, 26);
    fastRunTorsoFrames[4]  = IntRect(138, 500, 27, 26);
    fastRunTorsoFrames[5]  = IntRect(170, 500, 28, 26);
    fastRunTorsoFrames[6]  = IntRect(203, 501, 28, 26);
    fastRunTorsoFrames[7]  = IntRect(236, 502, 28, 26);
    fastRunTorsoFrames[8]  = IntRect(269, 502, 29, 26);
    fastRunTorsoFrames[9]  = IntRect(303, 503, 28, 26);
    fastRunTorsoFrames[10] = IntRect(336, 501, 26, 27);
    fastRunTorsoFrames[11] = IntRect(367, 501, 27, 27);
    
    fastRunLegFrames[0]  = IntRect(10,  534, 22, 26);
    fastRunLegFrames[1]  = IntRect(37,  534, 25, 25);
    fastRunLegFrames[2]  = IntRect(67,  535, 27, 23);
    fastRunLegFrames[3]  = IntRect(99,  534, 25, 26);
    fastRunLegFrames[4]  = IntRect(129, 534, 17, 26);
    fastRunLegFrames[5]  = IntRect(151, 535, 18, 25);
    fastRunLegFrames[6]  = IntRect(174, 535, 21, 25);
    fastRunLegFrames[7]  = IntRect(200, 535, 26, 23);
    fastRunLegFrames[8]  = IntRect(231, 535, 30, 25);
    fastRunLegFrames[9]  = IntRect(266, 535, 24, 25);
    fastRunLegFrames[10] = IntRect(295, 534, 18, 26);
    fastRunLegFrames[11] = IntRect(318, 534, 20, 26);

    for (int i = 0; i < 8; i++) {
        standingHitFrames[i] = IntRect(10, 500, 27, 26);
    }

    // HMG crouching fallbacks (use Fio's HMG crouch slide & fire)
    for (int i = 0; i < 4; i++) {
        hmgCrouchFireFrames[i] = IntRect(10, 7850, 59, 26); // fallback
        hmgCrouchIdleFrames[i] = IntRect(10, 7719, 41, 27);
        hmgCrouchSitFrames[i] = IntRect(10, 7633, 34, 45);
        hmgDownFireFrames[i] = IntRect(10, 7850, 59, 26);
        hmgLeftFireFrames[i] = IntRect(10, 7850, 59, 26);
    }
    for (int i = 0; i < 7; i++) {
        hmgCrouchWalkFrames[i] = IntRect(10, 7786, 41, 27);
    }
    for (int i = 0; i < 8; i++) {
        hmgHitFrames[i] = IntRect(10, 500, 27, 26);
    }
    hmgNeatTorsoFrames[0] = IntRect(10, 6577, 35, 37);
    hmgPunchLoadFrames[0] = IntRect(10, 500, 27, 26);
    hmgSlowTorsoFrames[0] = IntRect(10, 500, 27, 26);
    
    // transitions
    for (int i = 0; i < 4; i++) {
        hmgUpTransFrames[i] = IntRect(10, 500, 27, 26);
        hmgFromUpTransFrames[i] = IntRect(10, 500, 27, 26);
        hmgDownTransFrames[i] = IntRect(10, 500, 27, 26);
        hmgFromDownTransFrames[i] = IntRect(10, 500, 27, 26);
        hmgLeftTransFrames[i] = IntRect(10, 500, 27, 26);
    }



    // Fat (motu) fallbacks (use Fio's standard frames)
    for (int i = 0; i < 4; i++) {
        motuCrouchIdleFrames[i] = IntRect(10, 7719, 41, 27);
        motuCrouchSitFrames[i] = IntRect(10, 7633, 34, 45);
        motuCrouchStandFrames[i] = IntRect(10, 500, 27, 26);
        motuFireCrouchFrames[i] = IntRect(10, 7850, 59, 26);
        motuFireCrouchMultiFrames[i] = IntRect(10, 7850, 59, 26);
        motuFireStraightFrames[i] = IntRect(10, 500, 27, 26);
        motuFireUp90Frames[i] = IntRect(10, 500, 27, 26);
    }
    for (int i = 0; i < 6; i++) {
        motuCrouchMoveFrames[i] = IntRect(10, 7786, 41, 27);
    }
    for (int i = 0; i < 16; i++) {
        motuCallFrames[i] = IntRect(10, 500, 27, 26);
    }
    for (int i = 0; i < 12; i++) {
        motuConvertFrames[i] = IntRect(10, 500, 27, 26);
        motuDieFrames[i] = IntRect(87, 7639, 36, 39);
        motuEatFrames[i] = IntRect(10, 500, 27, 26);
        motuFireBotMultiFrames[i] = IntRect(10, 500, 27, 26);
        motuFireDown45TransFrames[i] = IntRect(10, 500, 27, 26);
        motuFireDownFrames[i] = IntRect(10, 500, 27, 26);
        motuFireFromDown45Frames[i] = IntRect(10, 500, 27, 26);
        motuFireFromUp45Frames[i] = IntRect(10, 500, 27, 26);
        motuFireTopMultiFrames[i] = IntRect(10, 500, 27, 26);
        motuFireUp45TransFrames[i] = IntRect(10, 500, 27, 26);
        motuHitFrames[i] = IntRect(10, 500, 27, 26);
    }



  }

  ~FiolinaGermi() {}

  void setAmmoPickupMultiplier(double v) { ammoPickupMultiplier = v; }
  double getAmmoPickupMultiplier() const { return ammoPickupMultiplier; }
  void setFireRate(double v) { fireRate = v; }
  double getFireRate() const { return fireRate; }
  void setSuperchargeTimer(float v) { superchargeTimer = v; }
  float getSuperchargeTimer() const { return superchargeTimer; }
  bool isSupercharged() const { return supercharged; }

  void pickupWeapon(Weapon *w) override {
    PlayerSoldier::pickupWeapon(w);
  }

  void activateSupercharge() {
    supercharged = true;
    superchargeTimer = 10.0f;
  }

  void activateSpecial() override {
    specialActive = true;
    specialDuration = 10.0f;
    activateSupercharge();
  }

  bool isFio() const override { return true; }

  const char *getCharacterName() const override { return "Fio"; }
  float getSpecialFireRateMultiplier() const override {
    return (specialActive) ? 2.0f : 1.0f;
  }

  void applyCharacterBuffs() override {
    pistolFireRate *= 1.1;
    knifeDamage *= 0.75f;
    grenadeMax = 8;
    grenadeCount = 8;
    ammoPickupMultiplier = 1.5;
  }

  double getEffectiveFireRate() const {
    return supercharged ? pistolFireRate * 2.0 : pistolFireRate;
  }

  void update(Level *level, float dt) override {
    PlayerSoldier::update(level, dt);
    if (supercharged) {
      superchargeTimer -= dt;
      if (superchargeTimer <= 0)
        supercharged = false;
    }
  }

  void draw(RenderWindow &window) override { PlayerSoldier::draw(window); }
};