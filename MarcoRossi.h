#pragma once
#include "PlayerSoldier.h"

using namespace sf;

class MarcoRossi : public PlayerSoldier {
public:
  MarcoRossi(double posX, double posY) : PlayerSoldier(posX, posY) {
    grenadeMax = 10;
    grenadeCount = 10;

    if (mainSheet.loadFromFile("Sprites/Marco Rossi 1.png")) {
      torsoSprite.setTexture(mainSheet);
      legSprite.setTexture(mainSheet);
    }

    torsoSprite.setScale(scaleFactor, scaleFactor);
    legSprite.setScale(scaleFactor, scaleFactor);

    baseMaxSpeed = 5;
    animDelay = 4.0f / 60.0f;

    // iDLE torso (4 frames, height = 30px raw)
    idleTorsoFrames[0] = IntRect(10, 259, 29, 30);
    idleTorsoFrames[1] = IntRect(44, 259, 29, 30);
    idleTorsoFrames[2] = IntRect(78, 259, 30, 29);
    idleTorsoFrames[3] = IntRect(113, 259, 31, 28);
    numIdleFrames = 4;

    // iDLE legs (1 frame, height = 17px raw)
    idleLegFrames[0] = IntRect(149, 272, 21, 17);
    idleLegFrames[1] = IntRect(149, 272, 21, 17);
    idleLegFrames[2] = IntRect(149, 272, 21, 17);
    idleLegFrames[3] = IntRect(149, 272, 21, 17);

    // legOffsetY = idle torso height * scaleFactor
    legOffsetY = 28 * scaleFactor;
    legOffsetX = 0;

    // rUNNING torso (12 frames)
    runTorsoFrames[0] = IntRect(10, 473, 32, 29);
    runTorsoFrames[1] = IntRect(47, 473, 30, 30);
    runTorsoFrames[2] = IntRect(82, 473, 28, 29);
    runTorsoFrames[3] = IntRect(115, 473, 27, 28);
    runTorsoFrames[4] = IntRect(147, 473, 29, 29);
    runTorsoFrames[5] = IntRect(181, 473, 30, 29);
    runTorsoFrames[6] = IntRect(216, 473, 32, 30);
    runTorsoFrames[7] = IntRect(254, 473, 32, 30);
    runTorsoFrames[8] = IntRect(291, 473, 32, 29);
    runTorsoFrames[9] = IntRect(328, 473, 31, 28);
    runTorsoFrames[10] = IntRect(364, 473, 31, 29);
    runTorsoFrames[11] = IntRect(400, 473, 31, 29);
    numRunFrames = 12;

    // rUNNING legs (12 frames)
    runLegFrames[0] = IntRect(10, 507, 21, 21);
    runLegFrames[1] = IntRect(36, 507, 28, 21);
    runLegFrames[2] = IntRect(69, 507, 31, 17);
    runLegFrames[3] = IntRect(105, 507, 19, 21);
    runLegFrames[4] = IntRect(129, 507, 15, 21);
    runLegFrames[5] = IntRect(149, 507, 16, 21);
    runLegFrames[6] = IntRect(170, 507, 21, 21);
    runLegFrames[7] = IntRect(196, 507, 26, 21);
    runLegFrames[8] = IntRect(227, 507, 31, 17);
    runLegFrames[9] = IntRect(263, 507, 20, 21);
    runLegFrames[10] = IntRect(288, 507, 15, 21);
    runLegFrames[11] = IntRect(308, 507, 16, 21);

    // fIRING torso (10 frames)

    shootTorsoFrames[0] = IntRect(423, 1043, 32, 28);
    shootTorsoFrames[1] = IntRect(385, 1044, 33, 28);
    shootTorsoFrames[2] = IntRect(346, 1044, 34, 27);
    shootTorsoFrames[3] = IntRect(304, 1044, 37, 24);
    shootTorsoFrames[4] = IntRect(262, 1042, 37, 26);
    shootTorsoFrames[5] = IntRect(220, 1042, 37, 26);
    shootTorsoFrames[6] = IntRect(178, 1044, 37, 24);
    shootTorsoFrames[7] = IntRect(121, 1044, 37, 24);
    shootTorsoFrames[8] = IntRect(65, 1044, 51, 24);
    shootTorsoFrames[9] = IntRect(10, 1044, 50, 24);
    numShootFrames = 10;

    // fIRING UP torso (10 frames)

    shootUpTorsoFrames[0] = IntRect(269, 1585, 29, 28);
    shootUpTorsoFrames[1] = IntRect(238, 1575, 26, 38);
    shootUpTorsoFrames[2] = IntRect(207, 1572, 26, 41);
    shootUpTorsoFrames[3] = IntRect(178, 1571, 24, 42);
    shootUpTorsoFrames[4] = IntRect(148, 1572, 25, 41);
    shootUpTorsoFrames[5] = IntRect(118, 1574, 25, 39);
    shootUpTorsoFrames[6] = IntRect(91, 1573, 22, 40);
    shootUpTorsoFrames[7] = IntRect(64, 1572, 22, 41);
    shootUpTorsoFrames[8] = IntRect(37, 1548, 22, 65);
    shootUpTorsoFrames[9] = IntRect(10, 1548, 22, 65);
    numShootUpFrames = 10;

    // kNIFE torso (6 frames)
    knifeTorsoFrames[0] = IntRect(10, 1245, 24, 30);
    knifeTorsoFrames[1] = IntRect(39, 1245, 23, 27);
    knifeTorsoFrames[2] = IntRect(67, 1245, 26, 28);
    knifeTorsoFrames[3] = IntRect(98, 1246, 41, 24);
    knifeTorsoFrames[4] = IntRect(144, 1241, 48, 29);
    knifeTorsoFrames[5] = IntRect(197, 1241, 48, 30);
    numKnifeFrames = 6;

    // hMG RUNNING torso (12 frames)
    runTorsoHMGFrames[0] = IntRect(10, 3385, 42, 27);
    runTorsoHMGFrames[1] = IntRect(57, 3384, 39, 28);
    runTorsoHMGFrames[2] = IntRect(101, 3384, 38, 29);
    runTorsoHMGFrames[3] = IntRect(144, 3385, 39, 28);
    runTorsoHMGFrames[4] = IntRect(188, 3385, 41, 29);
    runTorsoHMGFrames[5] = IntRect(234, 3385, 42, 29);
    runTorsoHMGFrames[6] = IntRect(281, 3385, 42, 30);
    runTorsoHMGFrames[7] = IntRect(328, 3385, 43, 30);
    runTorsoHMGFrames[8] = IntRect(376, 3385, 44, 30);
    runTorsoHMGFrames[9] = IntRect(425, 3385, 42, 30);
    runTorsoHMGFrames[10] = IntRect(472, 3385, 43, 29);
    runTorsoHMGFrames[11] = IntRect(520, 3385, 43, 29);

    // hMG FIRING torso (4 frames)i
    shootTorsoHMGFrames[0] = IntRect(205, 3656, 36, 27);
    shootTorsoHMGFrames[1] = IntRect(140, 3657, 60, 27);
    shootTorsoHMGFrames[2] = IntRect(75, 3656, 36, 28);
    shootTorsoHMGFrames[3] = IntRect(10, 3655, 60, 28);

    // hMG FIRING UP torso (4 frames)
    shootUpTorsoHMGFrames[0] = IntRect(91, 4138, 24, 73);
    shootUpTorsoHMGFrames[1] = IntRect(64, 4140, 22, 71);
    shootUpTorsoHMGFrames[2] = IntRect(37, 4139, 22, 72);
    shootUpTorsoHMGFrames[3] = IntRect(10, 4141, 22, 70);
    numShootHMGFrames = 4;

    // jUMP (Extended to 12 frames by repeating last 3)
    jumpLegFrames[0] = IntRect(132, 890, 21, 19);
    jumpLegFrames[1] = IntRect(106, 890, 21, 20);
    jumpLegFrames[2] = IntRect(81, 890, 20, 23);
    jumpLegFrames[3] = IntRect(57, 890, 20, 25);
    jumpLegFrames[4] = IntRect(33, 890, 19, 25);
    jumpLegFrames[5] = IntRect(10, 890, 18, 25);
    // Repeating last 3 frames to extend airtime visual
    jumpLegFrames[6] = IntRect(57, 890, 20, 25);
    jumpLegFrames[7] = IntRect(33, 890, 19, 25);
    jumpLegFrames[8] = IntRect(10, 890, 18, 25);
    jumpLegFrames[9] = IntRect(57, 890, 20, 25);
    jumpLegFrames[10] = IntRect(33, 890, 19, 25);
    jumpLegFrames[11] = IntRect(10, 890, 18, 25);

    jumpTorsoFrames[0] = IntRect(181, 861, 29, 24);
    jumpTorsoFrames[1] = IntRect(147, 861, 29, 23);
    jumpTorsoFrames[2] = IntRect(113, 861, 29, 24);
    jumpTorsoFrames[3] = IntRect(79, 861, 29, 25);
    jumpTorsoFrames[4] = IntRect(44, 860, 29, 26);
    jumpTorsoFrames[5] = IntRect(10, 859, 29, 27);
    // Repeating last 3 frames
    jumpTorsoFrames[6] = IntRect(79, 861, 29, 25);
    jumpTorsoFrames[7] = IntRect(44, 860, 29, 26);
    jumpTorsoFrames[8] = IntRect(10, 859, 29, 27);
    jumpTorsoFrames[9] = IntRect(79, 861, 29, 25);
    jumpTorsoFrames[10] = IntRect(44, 860, 29, 26);
    jumpTorsoFrames[11] = IntRect(10, 859, 29, 27);
    numJumpIntroFrames = 6;
    numJumpFrames = 12;

    // Crouch animations
    crouchSitFrames[0] = IntRect(8, 848, 29, 39);
    crouchSitFrames[1] = IntRect(45, 846, 29, 41);
    crouchSitFrames[2] = IntRect(81, 849, 29, 38);
    crouchSitFrames[3] = IntRect(125, 860, 31, 27);

    crouchIdleFrames[0] = IntRect(6, 898, 34, 24);
    crouchIdleFrames[1] = IntRect(47, 898, 34, 24);
    crouchIdleFrames[2] = IntRect(87, 898, 34, 24);
    crouchIdleFrames[3] = IntRect(126, 898, 34, 24);
    crouchIdleFrames[4] = IntRect(167, 898, 35, 24);
    crouchIdleFrames[5] = IntRect(206, 898, 35, 24);
    crouchIdleFrames[6] = IntRect(247, 898, 35, 24);

    crouchSlideFrames[0] = IntRect(297, 898, 33, 24);
    crouchSlideFrames[1] = IntRect(337, 895, 32, 27);
    crouchSlideFrames[2] = IntRect(377, 896, 31, 26);
    crouchSlideFrames[3] = IntRect(417, 898, 35, 24);
    crouchSlideFrames[4] = IntRect(458, 900, 42, 22);

    crouchFireFrames[0] = IntRect(10, 1815, 50, 28);
    crouchFireFrames[1] = IntRect(65, 1815, 51, 28);
    crouchFireFrames[2] = IntRect(121, 1815, 52, 28);
    crouchFireFrames[3] = IntRect(178, 1815, 37, 28);
    crouchFireFrames[4] = IntRect(221, 1813, 37, 30);
    crouchFireFrames[5] = IntRect(263, 1813, 37, 30);
    crouchFireFrames[6] = IntRect(305, 1815, 37, 28);
    crouchFireFrames[7] = IntRect(347, 1815, 34, 28);
    crouchFireFrames[8] = IntRect(386, 1815, 33, 28);
    crouchFireFrames[9] = IntRect(424, 1814, 32, 29);

    slideFireFrames[0] = IntRect(10, 2121, 33, 25);
    slideFireFrames[1] = IntRect(48, 2118, 32, 28);
    slideFireFrames[2] = IntRect(85, 2119, 31, 27);
    slideFireFrames[3] = IntRect(121, 2121, 35, 25);
    slideFireFrames[4] = IntRect(161, 2123, 42, 23);
    slideFireFrames[5] = IntRect(10, 2152, 47, 23);
    slideFireFrames[6] = IntRect(62, 2152, 49, 23);
    slideFireFrames[7] = IntRect(116, 2152, 49, 23);
    slideFireFrames[8] = IntRect(170, 2151, 42, 24);
    slideFireFrames[9] = IntRect(217, 2150, 42, 25);

    crouchHitFrames[0] = IntRect(455, 860, 32, 26);
    crouchHitFrames[1] = IntRect(494, 860, 32, 26);
    crouchHitFrames[2] = IntRect(535, 860, 31, 26);
    crouchHitFrames[3] = IntRect(572, 860, 32, 26);

    // Standing hit reaction (torso flinch frames)
    standingHitFrames[0] = IntRect(7, 385, 29, 30);
    standingHitFrames[1] = IntRect(42, 386, 28, 30);
    standingHitFrames[2] = IntRect(78, 387, 26, 30);
    standingHitFrames[3] = IntRect(112, 387, 25, 34);
    standingHitFrames[4] = IntRect(146, 387, 25, 34);
    standingHitFrames[5] = IntRect(179, 387, 25, 34);
    standingHitFrames[6] = IntRect(214, 387, 25, 34);
    standingHitFrames[7] = IntRect(247, 387, 25, 34);

    hmgCrouchSitFrames[0] = IntRect(8, 2502, 39, 39);
    hmgCrouchSitFrames[1] = IntRect(56, 2499, 39, 42);
    hmgCrouchSitFrames[2] = IntRect(105, 2503, 40, 38);

    hmgCrouchIdleFrames[0] = IntRect(160, 2517, 41, 24);
    hmgCrouchIdleFrames[1] = IntRect(209, 2517, 41, 24);
    hmgCrouchIdleFrames[2] = IntRect(257, 2517, 41, 24);
    hmgCrouchIdleFrames[3] = IntRect(305, 2517, 41, 24);

    hmgCrouchWalkFrames[0] = IntRect(7, 2556, 41, 24);
    hmgCrouchWalkFrames[1] = IntRect(56, 2556, 41, 24);
    hmgCrouchWalkFrames[2] = IntRect(107, 2556, 41, 24);
    hmgCrouchWalkFrames[3] = IntRect(157, 2556, 41, 24);
    hmgCrouchWalkFrames[4] = IntRect(206, 2556, 41, 24);
    hmgCrouchWalkFrames[5] = IntRect(255, 2556, 41, 24);
    hmgCrouchWalkFrames[6] = IntRect(303, 2556, 41, 24);

    hmgCrouchSlideFrames[0] = IntRect(359, 2556, 40, 24);
    hmgCrouchSlideFrames[1] = IntRect(404, 2553, 39, 27);
    hmgCrouchSlideFrames[2] = IntRect(449, 2554, 37, 26);
    hmgCrouchSlideFrames[3] = IntRect(493, 2556, 39, 24);
    hmgCrouchSlideFrames[4] = IntRect(539, 2558, 42, 22);

    hmgCrouchFireFrames[0] = IntRect(6, 2855, 67, 28);
    hmgCrouchFireFrames[1] = IntRect(82, 2857, 67, 26);
    hmgCrouchFireFrames[2] = IntRect(159, 2857, 67, 26);
    hmgCrouchFireFrames[3] = IntRect(235, 2857, 67, 26);

    hmgCrouchSlideFireFrames[0] = IntRect(322, 2859, 63, 24);
    hmgCrouchSlideFireFrames[1] = IntRect(393, 2862, 65, 21);
    hmgCrouchSlideFireFrames[2] = IntRect(465, 2860, 61, 23);
    hmgCrouchSlideFireFrames[3] = IntRect(535, 2861, 63, 22);

    crouchMeleeFrames[0] = IntRect(6, 1285, 33, 26);
    crouchMeleeFrames[1] = IntRect(46, 1285, 28, 26);
    crouchMeleeFrames[2] = IntRect(81, 1285, 31, 26);
    crouchMeleeFrames[3] = IntRect(119, 1283, 41, 28);
    crouchMeleeFrames[4] = IntRect(167, 1285, 38, 26);
    crouchMeleeFrames[5] = IntRect(212, 1285, 36, 26);

    crouchKnifeLoadFrames[0] = IntRect(6, 1383, 37, 26);
    crouchKnifeLoadFrames[1] = IntRect(56, 1379, 34, 30);
    crouchKnifeLoadFrames[2] = IntRect(105, 1375, 35, 34);
    crouchKnifeLoadFrames[3] = IntRect(148, 1375, 34, 34);

    crouchKnifeAttackFrames[0] = IntRect(203, 1383, 30, 26);
    crouchKnifeAttackFrames[1] = IntRect(242, 1383, 26, 26);
    crouchKnifeAttackFrames[2] = IntRect(278, 1382, 26, 27);
    crouchKnifeAttackFrames[3] = IntRect(315, 1381, 29, 28);
    crouchKnifeAttackFrames[4] = IntRect(365, 1376, 49, 33);
    crouchKnifeAttackFrames[5] = IntRect(421, 1376, 49, 33);

    crouchBoxerFrames[0] = IntRect(6, 1467, 31, 27);
    crouchBoxerFrames[1] = IntRect(44, 1469, 28, 25);
    crouchBoxerFrames[2] = IntRect(80, 1468, 48, 26);
    crouchBoxerFrames[3] = IntRect(135, 1469, 54, 25);
    crouchBoxerFrames[4] = IntRect(196, 1469, 55, 25);

    crouchKnife2LoadFrames[0] = IntRect(9, 3597, 39, 36);
    crouchKnife2LoadFrames[1] = IntRect(61, 3597, 36, 36);
    crouchKnife2LoadFrames[2] = IntRect(112, 3597, 37, 36);
    crouchKnife2LoadFrames[3] = IntRect(157, 3597, 36, 36);

    crouchKnife2HitFrames[0] = IntRect(219, 3604, 43, 29);
    crouchKnife2HitFrames[1] = IntRect(272, 3603, 44, 30);
    crouchKnife2HitFrames[2] = IntRect(324, 3600, 38, 33);
    crouchKnife2HitFrames[3] = IntRect(370, 3600, 43, 33);
    crouchKnife2HitFrames[4] = IntRect(430, 3600, 59, 33);
    crouchKnife2HitFrames[5] = IntRect(495, 3600, 60, 33);

    torsoSprite.setTextureRect(idleTorsoFrames[0]);
    legSprite.setTextureRect(idleLegFrames[0]);

    loadTransformationAssets("Sprites/Marco Rossi (Mummy).png");

    // fAT Torso (4 frames)
    fatTorsoIdleFrames[0] = IntRect(919, 7234, 42, 28);
    fatTorsoIdleFrames[1] = IntRect(966, 7234, 43, 29);
    fatTorsoIdleFrames[2] = IntRect(1014, 7234, 44, 30);
    fatTorsoIdleFrames[3] = IntRect(1063, 7234, 45, 30);

    // fAT Legs (1 frame)
    fatLegIdleFrames[0] = IntRect(1113, 7248, 24, 16);

    // fAT Walking Torso (13 frames)
    fatTorsoWalkFrames[0] = IntRect(1320, 7415, 40, 28);
    fatTorsoWalkFrames[1] = IntRect(919, 7414, 37, 27);
    fatTorsoWalkFrames[2] = IntRect(961, 7414, 36, 27);
    fatTorsoWalkFrames[3] = IntRect(1002, 7415, 37, 27);
    fatTorsoWalkFrames[4] = IntRect(1044, 7414, 38, 30);
    fatTorsoWalkFrames[5] = IntRect(1087, 7414, 41, 30);
    fatTorsoWalkFrames[6] = IntRect(1133, 7414, 44, 30);
    fatTorsoWalkFrames[7] = IntRect(1182, 7414, 42, 29);
    fatTorsoWalkFrames[8] = IntRect(1229, 7415, 40, 29);
    fatTorsoWalkFrames[9] = IntRect(1274, 7415, 41, 28);
    fatTorsoWalkFrames[10] = IntRect(1320, 7415, 40, 28);
    fatTorsoWalkFrames[11] = IntRect(1365, 7414, 40, 29);
    fatTorsoWalkFrames[12] = IntRect(1410, 7414, 39, 28);

    // fAT Walking Legs (12 frames)
    fatLegWalkFrames[0] = IntRect(919, 7449, 22, 18);
    fatLegWalkFrames[1] = IntRect(946, 7449, 20, 18);
    fatLegWalkFrames[2] = IntRect(971, 7449, 19, 18);
    fatLegWalkFrames[3] = IntRect(995, 7449, 20, 18);
    fatLegWalkFrames[4] = IntRect(1020, 7449, 25, 18);
    fatLegWalkFrames[5] = IntRect(1050, 7449, 25, 18);
    fatLegWalkFrames[6] = IntRect(1080, 7449, 22, 18);
    fatLegWalkFrames[7] = IntRect(1107, 7449, 20, 18);
    fatLegWalkFrames[8] = IntRect(1132, 7449, 21, 18);
    fatLegWalkFrames[9] = IntRect(1158, 7449, 21, 18);
    fatLegWalkFrames[10] = IntRect(1184, 7449, 26, 18);
    fatLegWalkFrames[11] = IntRect(1215, 7449, 26, 18);

    // fAT Firing Torso (8 frames)
    fatTorsoShootFrames[0] = IntRect(1311, 7620, 44, 29);
    fatTorsoShootFrames[1] = IntRect(1262, 7620, 44, 29);
    fatTorsoShootFrames[2] = IntRect(1216, 7619, 41, 30);
    fatTorsoShootFrames[3] = IntRect(1170, 7619, 41, 30);
    fatTorsoShootFrames[4] = IntRect(1124, 7619, 41, 30);
    fatTorsoShootFrames[5] = IntRect(1052, 7620, 67, 29);
    fatTorsoShootFrames[6] = IntRect(983, 7620, 64, 29);
    fatTorsoShootFrames[7] = IntRect(919, 7620, 59, 29);

    // fAT Firing Up Torso (7 frames)
    fatTorsoShootUpFrames[0] = IntRect(1168, 8022, 31, 41);
    fatTorsoShootUpFrames[1] = IntRect(1131, 8024, 32, 39);
    fatTorsoShootUpFrames[2] = IntRect(1061, 8025, 30, 38);
    fatTorsoShootUpFrames[3] = IntRect(1026, 8025, 30, 38);
    fatTorsoShootUpFrames[4] = IntRect(991, 7999, 30, 64);
    fatTorsoShootUpFrames[5] = IntRect(955, 8002, 31, 61);
    fatTorsoShootUpFrames[6] = IntRect(919, 8007, 31, 56);

    // Transformation and Swimming Frames copied from Tarma
    // Zombie conversion frames (from zombieSheet, Y=186-205)
    zombieConvertFrames[0] = IntRect(10, 186, 27, 40);
    zombieConvertFrames[1] = IntRect(41, 186, 31, 40);
    zombieConvertFrames[2] = IntRect(75, 186, 31, 40);
    zombieConvertFrames[3] = IntRect(112, 186, 30, 40);
    zombieConvertFrames[4] = IntRect(147, 186, 31, 40);
    zombieConvertFrames[5] = IntRect(183, 187, 32, 39);
    zombieConvertFrames[6] = IntRect(220, 189, 32, 37);
    zombieConvertFrames[7] = IntRect(255, 191, 29, 35);
    zombieConvertFrames[8] = IntRect(290, 193, 28, 33);
    zombieConvertFrames[9] = IntRect(323, 192, 27, 34);
    zombieConvertFrames[10] = IntRect(355, 194, 25, 32);
    zombieConvertFrames[11] = IntRect(386, 195, 24, 31);
    zombieConvertFrames[12] = IntRect(417, 197, 21, 29);
    zombieConvertFrames[13] = IntRect(447, 199, 22, 27);
    zombieConvertFrames[14] = IntRect(475, 201, 23, 25);
    zombieConvertFrames[15] = IntRect(503, 205, 27, 21);

    // 10.3 Zombie Hand Straight
    zombieHandStraightFrames[0] = IntRect(8, 6649, 35, 33);
    zombieHandStraightFrames[1] = IntRect(50, 6648, 36, 34);
    zombieHandStraightFrames[2] = IntRect(93, 6650, 35, 32);
    zombieHandStraightFrames[3] = IntRect(135, 6649, 37, 33);
    zombieHandStraightFrames[4] = IntRect(179, 6649, 35, 33);
    zombieHandStraightFrames[5] = IntRect(221, 6650, 35, 32);
    zombieHandStraightFrames[6] = IntRect(263, 6650, 35, 32);
    zombieHandStraightFrames[7] = IntRect(305, 6650, 35, 32);
    zombieHandStraightFrames[8] = IntRect(347, 6649, 35, 33);
    zombieHandStraightFrames[9] = IntRect(389, 6650, 36, 32);
    zombieHandStraightFrames[10] = IntRect(432, 6650, 36, 32);
    zombieHandStraightFrames[11] = IntRect(475, 6649, 35, 33);

    // 10.4 Zombie Hand Fire
    zombieHandFireFrames[0] = IntRect(8, 6689, 37, 33);
    zombieHandFireFrames[1] = IntRect(52, 6688, 38, 34);
    zombieHandFireFrames[2] = IntRect(97, 6690, 37, 32);
    zombieHandFireFrames[3] = IntRect(141, 6689, 38, 33);
    zombieHandFireFrames[4] = IntRect(186, 6689, 36, 33);
    zombieHandFireFrames[5] = IntRect(229, 6690, 36, 32);
    zombieHandFireFrames[6] = IntRect(272, 6690, 36, 32);
    zombieHandFireFrames[7] = IntRect(315, 6690, 36, 32);
    zombieHandFireFrames[8] = IntRect(358, 6689, 37, 33);
    zombieHandFireFrames[9] = IntRect(402, 6690, 38, 32);
    zombieHandFireFrames[10] = IntRect(447, 6690, 37, 32);
    zombieHandFireFrames[11] = IntRect(491, 6689, 36, 33);

    // 10.5 Zombie Hand Up
    zombieHandUpFrames[0] = IntRect(9, 6729, 30, 48);
    zombieHandUpFrames[1] = IntRect(46, 6730, 27, 47);
    zombieHandUpFrames[2] = IntRect(80, 6731, 28, 46);
    zombieHandUpFrames[3] = IntRect(115, 6732, 29, 45);
    zombieHandUpFrames[4] = IntRect(151, 6731, 29, 46);
    zombieHandUpFrames[5] = IntRect(187, 6730, 31, 47);
    zombieHandUpFrames[6] = IntRect(225, 6730, 32, 47);
    zombieHandUpFrames[7] = IntRect(264, 6730, 32, 47);
    zombieHandUpFrames[8] = IntRect(303, 6728, 31, 49);
    zombieHandUpFrames[9] = IntRect(341, 6727, 30, 50);
    zombieHandUpFrames[10] = IntRect(378, 6727, 31, 50);
    zombieHandUpFrames[11] = IntRect(416, 6728, 30, 49);

    // 10.6 Zombie Hand Up Fire
    zombieHandUpFireFrames[0] = IntRect(8, 6784, 30, 48);
    zombieHandUpFireFrames[1] = IntRect(45, 6784, 28, 48);
    zombieHandUpFireFrames[2] = IntRect(80, 6785, 29, 47);
    zombieHandUpFireFrames[3] = IntRect(116, 6786, 30, 46);
    zombieHandUpFireFrames[4] = IntRect(153, 6785, 30, 47);
    zombieHandUpFireFrames[5] = IntRect(190, 6785, 32, 47);
    zombieHandUpFireFrames[6] = IntRect(229, 6784, 32, 48);
    zombieHandUpFireFrames[7] = IntRect(268, 6784, 32, 48);
    zombieHandUpFireFrames[8] = IntRect(307, 6783, 32, 49);
    zombieHandUpFireFrames[9] = IntRect(346, 6782, 31, 50);

    // 10.7 Zombie Fast Body
    zombieFastBodyFrames[0] = IntRect(5, 6842, 36, 29);
    zombieFastBodyFrames[1] = IntRect(48, 6843, 35, 28);
    zombieFastBodyFrames[2] = IntRect(90, 6843, 34, 28);
    zombieFastBodyFrames[3] = IntRect(131, 6842, 34, 29);
    zombieFastBodyFrames[4] = IntRect(172, 6842, 35, 29);
    zombieFastBodyFrames[5] = IntRect(214, 6843, 34, 28);
    zombieFastBodyFrames[6] = IntRect(255, 6843, 34, 28);
    zombieFastBodyFrames[7] = IntRect(296, 6842, 34, 29);
    zombieFastBodyFrames[8] = IntRect(337, 6842, 36, 29);
    zombieFastBodyFrames[9] = IntRect(380, 6843, 35, 28);
    zombieFastBodyFrames[10] = IntRect(422, 6843, 34, 28);
    zombieFastBodyFrames[11] = IntRect(463, 6842, 34, 29);
    zombieFastBodyFrames[12] = IntRect(504, 6842, 35, 29);
    zombieFastBodyFrames[13] = IntRect(546, 6843, 34, 28);
    zombieFastBodyFrames[14] = IntRect(587, 6843, 34, 28);
    zombieFastBodyFrames[15] = IntRect(628, 6842, 34, 29);
    zombieFastBodyFrames[16] = IntRect(669, 6842, 36, 29);
    zombieFastBodyFrames[17] = IntRect(712, 6843, 35, 28);
    zombieFastBodyFrames[18] = IntRect(754, 6843, 34, 28);
    zombieFastBodyFrames[19] = IntRect(795, 6842, 34, 29);
    zombieFastBodyFrames[20] = IntRect(836, 6842, 35, 29);
    zombieFastBodyFrames[21] = IntRect(878, 6843, 34, 28);
    zombieFastBodyFrames[22] = IntRect(919, 6843, 34, 28);
    zombieFastBodyFrames[23] = IntRect(960, 6842, 34, 29);

    // 10.8 Zombie Fast Hand Straight
    zombieFastHandStraightFrames[0] = IntRect(8, 6881, 35, 33);
    zombieFastHandStraightFrames[1] = IntRect(50, 6882, 35, 32);
    zombieFastHandStraightFrames[2] = IntRect(92, 6883, 35, 31);
    zombieFastHandStraightFrames[3] = IntRect(134, 6882, 35, 32);
    zombieFastHandStraightFrames[4] = IntRect(176, 6881, 35, 33);
    zombieFastHandStraightFrames[5] = IntRect(218, 6882, 35, 32);
    zombieFastHandStraightFrames[6] = IntRect(260, 6883, 35, 31);
    zombieFastHandStraightFrames[7] = IntRect(302, 6882, 35, 32);
    zombieFastHandStraightFrames[8] = IntRect(344, 6881, 35, 33);
    zombieFastHandStraightFrames[9] = IntRect(386, 6882, 35, 32);
    zombieFastHandStraightFrames[10] = IntRect(428, 6883, 35, 31);
    zombieFastHandStraightFrames[11] = IntRect(470, 6882, 35, 32);
    zombieFastHandStraightFrames[12] = IntRect(512, 6881, 35, 33);
    zombieFastHandStraightFrames[13] = IntRect(554, 6882, 35, 32);
    zombieFastHandStraightFrames[14] = IntRect(596, 6883, 35, 31);
    zombieFastHandStraightFrames[15] = IntRect(638, 6882, 35, 32);
    zombieFastHandStraightFrames[16] = IntRect(680, 6881, 35, 33);
    zombieFastHandStraightFrames[17] = IntRect(722, 6882, 35, 32);
    zombieFastHandStraightFrames[18] = IntRect(764, 6883, 35, 31);
    zombieFastHandStraightFrames[19] = IntRect(806, 6882, 35, 32);
    zombieFastHandStraightFrames[20] = IntRect(848, 6881, 35, 33);
    zombieFastHandStraightFrames[21] = IntRect(890, 6882, 35, 32);
    zombieFastHandStraightFrames[22] = IntRect(932, 6883, 35, 31);
    zombieFastHandStraightFrames[23] = IntRect(974, 6882, 35, 32);

    // 10.9 Zombie Fast Hand Up
    zombieFastHandUpFrames[0] = IntRect(9, 6922, 31, 46);
    zombieFastHandUpFrames[1] = IntRect(47, 6923, 32, 45);
    zombieFastHandUpFrames[2] = IntRect(86, 6923, 31, 45);
    zombieFastHandUpFrames[3] = IntRect(124, 6923, 30, 45);
    zombieFastHandUpFrames[4] = IntRect(161, 6922, 31, 46);
    zombieFastHandUpFrames[5] = IntRect(199, 6923, 32, 45);
    zombieFastHandUpFrames[6] = IntRect(238, 6923, 31, 45);
    zombieFastHandUpFrames[7] = IntRect(276, 6923, 30, 45);
    zombieFastHandUpFrames[8] = IntRect(313, 6922, 31, 46);
    zombieFastHandUpFrames[9] = IntRect(351, 6923, 32, 45);
    zombieFastHandUpFrames[10] = IntRect(390, 6923, 31, 45);
    zombieFastHandUpFrames[11] = IntRect(428, 6923, 30, 45);
    zombieFastHandUpFrames[12] = IntRect(465, 6922, 31, 46);
    zombieFastHandUpFrames[13] = IntRect(503, 6923, 32, 45);
    zombieFastHandUpFrames[14] = IntRect(542, 6923, 31, 45);
    zombieFastHandUpFrames[15] = IntRect(580, 6923, 30, 45);
    zombieFastHandUpFrames[16] = IntRect(617, 6922, 31, 46);
    zombieFastHandUpFrames[17] = IntRect(655, 6923, 32, 45);
    zombieFastHandUpFrames[18] = IntRect(694, 6923, 31, 45);
    zombieFastHandUpFrames[19] = IntRect(732, 6923, 30, 45);
    zombieFastHandUpFrames[20] = IntRect(769, 6922, 31, 46);
    zombieFastHandUpFrames[21] = IntRect(807, 6923, 32, 45);
    zombieFastHandUpFrames[22] = IntRect(846, 6923, 31, 45);
    zombieFastHandUpFrames[23] = IntRect(884, 6923, 30, 45);

    // 10.10 Zombie Jump Body
    zombieJumpBodyFrames[0] = IntRect(8, 6982, 35, 29);
    zombieJumpBodyFrames[1] = IntRect(50, 6980, 36, 31);
    zombieJumpBodyFrames[2] = IntRect(93, 6979, 36, 32);
    zombieJumpBodyFrames[3] = IntRect(136, 6979, 37, 32);
    zombieJumpBodyFrames[4] = IntRect(180, 6978, 37, 33);
    zombieJumpBodyFrames[5] = IntRect(224, 6980, 36, 31);
    zombieJumpBodyFrames[6] = IntRect(267, 6980, 36, 31);
    zombieJumpBodyFrames[7] = IntRect(310, 6980, 36, 31);
    zombieJumpBodyFrames[8] = IntRect(353, 6980, 37, 31);
    zombieJumpBodyFrames[9] = IntRect(397, 6981, 35, 30);
    zombieJumpBodyFrames[10] = IntRect(439, 6982, 36, 29);
    zombieJumpBodyFrames[11] = IntRect(482, 6982, 35, 29);
    zombieJumpBodyFrames[12] = IntRect(524, 6984, 34, 27);
    zombieJumpBodyFrames[13] = IntRect(565, 6984, 34, 27);
    zombieJumpBodyFrames[14] = IntRect(606, 6984, 34, 27);
    zombieJumpBodyFrames[15] = IntRect(647, 6984, 34, 27);

    // 10.11 Zombie Jump Hand Straight
    zombieJumpHandStraightFrames[0] = IntRect(8, 7018, 35, 33);
    zombieJumpHandStraightFrames[1] = IntRect(50, 7016, 36, 35);
    zombieJumpHandStraightFrames[2] = IntRect(93, 7016, 36, 35);
    zombieJumpHandStraightFrames[3] = IntRect(136, 7016, 37, 35);
    zombieJumpHandStraightFrames[4] = IntRect(180, 7015, 37, 36);
    zombieJumpHandStraightFrames[5] = IntRect(224, 7016, 36, 35);
    zombieJumpHandStraightFrames[6] = IntRect(267, 7016, 36, 35);
    zombieJumpHandStraightFrames[7] = IntRect(310, 7016, 36, 35);
    zombieJumpHandStraightFrames[8] = IntRect(353, 7016, 37, 35);
    zombieJumpHandStraightFrames[9] = IntRect(397, 7017, 35, 34);
    zombieHandStraightFrames[10] = IntRect(439, 7018, 36, 33);
    zombieHandStraightFrames[11] = IntRect(482, 7018, 35, 33);
    zombieHandStraightFrames[12] = IntRect(524, 7019, 34, 32);
    zombieHandStraightFrames[13] = IntRect(565, 7019, 34, 32);
    zombieHandStraightFrames[14] = IntRect(606, 7019, 34, 32);
    zombieHandStraightFrames[15] = IntRect(647, 7019, 34, 32);

    // === AUTO INJECTED ===

    // 10.7 ZOMBIE FAST MOVEMENT — BODY ONLY
    zombieFastBodyFrames[0] = IntRect(928, 5722, 25, 39);
    zombieFastBodyFrames[1] = IntRect(961, 5722, 24, 39);
    zombieFastBodyFrames[2] = IntRect(993, 5723, 26, 38);
    zombieFastBodyFrames[3] = IntRect(1026, 5725, 27, 36);
    zombieFastBodyFrames[4] = IntRect(1061, 5725, 28, 36);
    zombieFastBodyFrames[5] = IntRect(1098, 5725, 29, 36);
    zombieFastBodyFrames[6] = IntRect(1136, 5723, 30, 38);
    zombieFastBodyFrames[7] = IntRect(1174, 5722, 32, 39);
    zombieFastBodyFrames[8] = IntRect(1215, 5721, 34, 40);
    zombieFastBodyFrames[9] = IntRect(1257, 5721, 30, 40);
    zombieFastBodyFrames[10] = IntRect(1295, 5721, 28, 40);
    zombieFastBodyFrames[11] = IntRect(1332, 5722, 28, 39);
    zombieFastBodyFrames[12] = IntRect(1368, 5722, 28, 39);
    zombieFastBodyFrames[13] = IntRect(1403, 5722, 30, 39);
    zombieFastBodyFrames[14] = IntRect(1440, 5722, 29, 39);
    zombieFastBodyFrames[15] = IntRect(1477, 5721, 29, 40);
    zombieFastBodyFrames[16] = IntRect(1514, 5720, 29, 41);
    zombieFastBodyFrames[17] = IntRect(1550, 5720, 27, 41);
    zombieFastBodyFrames[18] = IntRect(1585, 5721, 27, 40);
    zombieFastBodyFrames[19] = IntRect(1618, 5724, 30, 37);
    zombieFastBodyFrames[20] = IntRect(1656, 5725, 34, 36);
    zombieFastBodyFrames[21] = IntRect(1696, 5724, 29, 37);
    zombieFastBodyFrames[22] = IntRect(1732, 5724, 23, 37);
    zombieFastBodyFrames[23] = IntRect(1765, 5723, 22, 38);

    // 10.8 ZOMBIE FAST — STRAIGHT HAND
    zombieFastHandStraightFrames[0] = IntRect(929, 5769, 25, 11);
    zombieFastHandStraightFrames[1] = IntRect(964, 5769, 24, 12);
    zombieFastHandStraightFrames[2] = IntRect(997, 5770, 24, 12);
    zombieFastHandStraightFrames[3] = IntRect(1029, 5771, 23, 13);
    zombieFastHandStraightFrames[4] = IntRect(1066, 5772, 21, 15);
    zombieFastHandStraightFrames[5] = IntRect(1104, 5773, 22, 14);
    zombieFastHandStraightFrames[6] = IntRect(1142, 5774, 23, 13);
    zombieFastHandStraightFrames[7] = IntRect(1182, 5777, 24, 10);
    zombieFastHandStraightFrames[8] = IntRect(1220, 5776, 24, 13);
    zombieFastHandStraightFrames[9] = IntRect(1262, 5776, 22, 14);
    zombieFastHandStraightFrames[10] = IntRect(1303, 5775, 21, 15);
    zombieFastHandStraightFrames[11] = IntRect(1339, 5774, 22, 13);
    zombieFastHandStraightFrames[12] = IntRect(1372, 5773, 24, 12);
    zombieFastHandStraightFrames[13] = IntRect(1408, 5772, 24, 12);
    zombieFastHandStraightFrames[14] = IntRect(1443, 5772, 25, 11);
    zombieFastHandStraightFrames[15] = IntRect(1478, 5772, 24, 12);
    zombieFastHandStraightFrames[16] = IntRect(1515, 5773, 24, 12);
    zombieFastHandStraightFrames[17] = IntRect(1552, 5774, 22, 13);
    zombieFastHandStraightFrames[18] = IntRect(1590, 5775, 21, 15);
    zombieFastHandStraightFrames[19] = IntRect(1625, 5776, 22, 14);
    zombieFastHandStraightFrames[20] = IntRect(1664, 5777, 19, 12);
    zombieFastHandStraightFrames[21] = IntRect(1701, 5774, 22, 13);
    zombieFastHandStraightFrames[22] = IntRect(1738, 5773, 17, 12);
    zombieFastHandStraightFrames[23] = IntRect(1766, 5772, 21, 12);

    // 10.9 ZOMBIE FAST — UP HAND
    zombieFastHandUpFrames[0] = IntRect(935, 5797, 11, 25);
    zombieFastHandUpFrames[1] = IntRect(969, 5798, 12, 24);
    zombieFastHandUpFrames[2] = IntRect(1003, 5798, 12, 24);
    zombieFastHandUpFrames[3] = IntRect(1033, 5799, 13, 23);
    zombieFastHandUpFrames[4] = IntRect(1068, 5800, 15, 21);
    zombieFastHandUpFrames[5] = IntRect(1106, 5799, 14, 22);
    zombieFastHandUpFrames[6] = IntRect(1146, 5798, 13, 23);
    zombieFastHandUpFrames[7] = IntRect(1186, 5797, 10, 24);
    zombieFastHandUpFrames[8] = IntRect(1223, 5797, 13, 24);
    zombieFastHandUpFrames[9] = IntRect(1264, 5799, 14, 22);
    zombieFastHandUpFrames[10] = IntRect(1304, 5800, 15, 21);
    zombieFastHandUpFrames[11] = IntRect(1344, 5799, 12, 24);
    zombieFastHandUpFrames[12] = IntRect(1376, 5797, 12, 25);
    zombieFastHandUpFrames[13] = IntRect(1411, 5796, 12, 28);
    zombieFastHandUpFrames[14] = IntRect(1447, 5797, 13, 26);
    zombieFastHandUpFrames[15] = IntRect(1482, 5798, 12, 22);
    zombieFastHandUpFrames[16] = IntRect(1519, 5798, 12, 22);
    zombieFastHandUpFrames[17] = IntRect(1555, 5798, 13, 22);
    zombieFastHandUpFrames[18] = IntRect(1591, 5799, 15, 21);
    zombieFastHandUpFrames[19] = IntRect(1626, 5798, 14, 22);
    zombieFastHandUpFrames[20] = IntRect(1664, 5797, 14, 26);
    zombieFastHandUpFrames[21] = IntRect(1705, 5796, 12, 22);
    zombieFastHandUpFrames[22] = IntRect(1739, 5795, 12, 19);
    zombieFastHandUpFrames[23] = IntRect(1768, 5794, 12, 23);

    // 10.10 ZOMBIE JUMP (from zombieSheet, Y=635)
    zombieJumpBodyFrames[0] = IntRect(10, 635, 40, 41);
    zombieJumpBodyFrames[1] = IntRect(55, 635, 41, 42);
    zombieJumpBodyFrames[2] = IntRect(101, 635, 40, 43);
    zombieJumpBodyFrames[3] = IntRect(146, 635, 41, 44);
    zombieJumpBodyFrames[4] = IntRect(192, 635, 41, 45);
    zombieJumpBodyFrames[5] = IntRect(238, 635, 41, 45);
    zombieJumpBodyFrames[6] = IntRect(284, 635, 45, 42);

    // 10.11 ZOMBIE JUMP — STRAIGHT HAND
    zombieJumpHandStraightFrames[0] = IntRect(930, 5885, 24, 11);
    zombieJumpHandStraightFrames[1] = IntRect(964, 5885, 23, 11);
    zombieJumpHandStraightFrames[2] = IntRect(998, 5885, 24, 11);
    zombieJumpHandStraightFrames[3] = IntRect(1031, 5885, 24, 11);
    zombieJumpHandStraightFrames[4] = IntRect(1063, 5885, 24, 10);
    zombieJumpHandStraightFrames[5] = IntRect(1094, 5885, 24, 10);
    zombieJumpHandStraightFrames[6] = IntRect(1126, 5885, 25, 10);
    zombieJumpHandStraightFrames[7] = IntRect(1161, 5885, 25, 10);
    zombieJumpHandStraightFrames[8] = IntRect(1193, 5884, 25, 10);
    zombieJumpHandStraightFrames[9] = IntRect(1225, 5885, 25, 9);
    zombieJumpHandStraightFrames[10] = IntRect(1255, 5883, 25, 9);
    zombieJumpHandStraightFrames[11] = IntRect(1285, 5883, 25, 9);
    zombieJumpHandStraightFrames[12] = IntRect(1316, 5883, 25, 9);
    zombieJumpHandStraightFrames[13] = IntRect(1346, 5883, 25, 9);
    zombieJumpHandStraightFrames[14] = IntRect(1378, 5883, 23, 9);
    zombieJumpHandStraightFrames[15] = IntRect(1409, 5883, 25, 9);

    // 10.12 ZOMBIE JUMP — UP HAND
    zombieJumpHandUpFrames[0] = IntRect(938, 5908, 11, 25);
    zombieJumpHandUpFrames[1] = IntRect(969, 5907, 11, 26);
    zombieJumpHandUpFrames[2] = IntRect(1003, 5906, 10, 27);
    zombieJumpHandUpFrames[3] = IntRect(1036, 5906, 10, 27);
    zombieJumpHandUpFrames[4] = IntRect(1069, 5906, 10, 27);
    zombieJumpHandUpFrames[5] = IntRect(1101, 5906, 10, 27);
    zombieJumpHandUpFrames[6] = IntRect(1134, 5906, 10, 27);
    zombieJumpHandUpFrames[7] = IntRect(1167, 5906, 10, 27);
    zombieJumpHandUpFrames[8] = IntRect(1201, 5906, 10, 27);
    zombieJumpHandUpFrames[9] = IntRect(1232, 5906, 10, 27);
    zombieJumpHandUpFrames[10] = IntRect(1262, 5906, 10, 27);
    zombieJumpHandUpFrames[11] = IntRect(1290, 5906, 10, 27);
    zombieJumpHandUpFrames[12] = IntRect(1319, 5906, 10, 21);
    zombieJumpHandUpFrames[13] = IntRect(1351, 5907, 11, 20);
    zombieJumpHandUpFrames[14] = IntRect(1383, 5907, 11, 20);
    zombieJumpHandUpFrames[15] = IntRect(1414, 5907, 11, 26);

    // 11.10 MUMMY CROUCHING — SITTING DOWN
    mummyCrouchSitFrames[0] = IntRect(928, 4600, 40, 40);
    mummyCrouchSitFrames[1] = IntRect(977, 4600, 38, 40);
    mummyCrouchSitFrames[2] = IntRect(1024, 4604, 36, 36);
    mummyCrouchSitFrames[3] = IntRect(1068, 4607, 39, 33);
    mummyCrouchSitFrames[4] = IntRect(1110, 4616, 46, 24);
    mummyCrouchSitFrames[5] = IntRect(1163, 4620, 48, 20);

    // 11.11 MUMMY CROUCH — IDLE
    mummyCrouchIdleFrames[0] = IntRect(1230, 4616, 46, 24);
    mummyCrouchIdleFrames[1] = IntRect(1284, 4616, 45, 24);
    mummyCrouchIdleFrames[2] = IntRect(1334, 4616, 45, 24);

    // 11.12 MUMMY CROUCH — MOVING
    mummyCrouchMoveFrames[0] = IntRect(927, 4656, 46, 24);
    mummyCrouchMoveFrames[1] = IntRect(980, 4656, 47, 24);
    mummyCrouchMoveFrames[2] = IntRect(1035, 4656, 47, 24);
    mummyCrouchMoveFrames[3] = IntRect(1090, 4657, 47, 23);
    mummyCrouchMoveFrames[4] = IntRect(1143, 4656, 47, 24);
    mummyCrouchMoveFrames[5] = IntRect(1197, 4656, 46, 24);
    mummyCrouchMoveFrames[6] = IntRect(1250, 4656, 45, 24);
    mummyCrouchMoveFrames[7] = IntRect(1303, 4656, 45, 24);

    // 11.13 MUMMY JUMP — BOTTOM BODY
    mummyJumpLegFrames[0] = IntRect(1124, 4382, 15, 24);
    mummyJumpLegFrames[1] = IntRect(1149, 4383, 15, 23);
    mummyJumpLegFrames[2] = IntRect(1173, 4382, 15, 24);
    mummyJumpLegFrames[3] = IntRect(1196, 4377, 13, 29);
    mummyJumpLegFrames[4] = IntRect(1219, 4377, 11, 29);
    mummyJumpLegFrames[5] = IntRect(1240, 4377, 11, 29);

    // 11.14 MUMMY DIE
    mummyDieFrames[0] = IntRect(930, 4997, 43, 47);
    mummyDieFrames[1] = IntRect(991, 4998, 37, 47);
    mummyDieFrames[2] = IntRect(1046, 4991, 45, 62);
    mummyDieFrames[3] = IntRect(1100, 4997, 54, 58);
    mummyDieFrames[4] = IntRect(1163, 5001, 50, 51);
    mummyDieFrames[5] = IntRect(1225, 4999, 53, 51);
    mummyDieFrames[6] = IntRect(1291, 4991, 57, 60);
    mummyDieFrames[7] = IntRect(1355, 4993, 58, 58);
    mummyDieFrames[8] = IntRect(1421, 4996, 57, 54);
    mummyDieFrames[9] = IntRect(1488, 4997, 54, 51);
    mummyDieFrames[10] = IntRect(1551, 4997, 55, 55);
    mummyDieFrames[11] = IntRect(1615, 4993, 57, 59);
    mummyDieFrames[12] = IntRect(1680, 4996, 54, 57);
    mummyDieFrames[13] = IntRect(1748, 4996, 58, 57);
    mummyDieFrames[14] = IntRect(946, 5073, 56, 53);
    mummyDieFrames[15] = IntRect(1014, 5073, 60, 53);
    mummyDieFrames[16] = IntRect(1086, 5076, 60, 50);
    mummyDieFrames[17] = IntRect(1157, 5077, 57, 49);
    mummyDieFrames[18] = IntRect(1225, 5074, 50, 53);
    mummyDieFrames[19] = IntRect(1295, 5075, 53, 52);
    mummyDieFrames[20] = IntRect(1368, 5073, 54, 54);
    mummyDieFrames[21] = IntRect(1439, 5075, 55, 52);
    mummyDieFrames[22] = IntRect(1504, 5103, 48, 24);
    mummyDieFrames[23] = IntRect(1571, 5097, 47, 30);
    mummyDieFrames[24] = IntRect(1640, 5107, 47, 20);

    // 12 AQUATIC SWIMMING FRAMES (ALL)
    swimLegStillStraightFrames[0] = IntRect(409, 10191, 26, 12);
    swimLegStillStraightFrames[1] = IntRect(443, 10191, 26, 12);
    swimLegStillStraightFrames[2] = IntRect(476, 10191, 26, 12);
    swimLegStillStraightFrames[3] = IntRect(511, 10191, 26, 12);

    swimLegSwimStraightFrames[0] = IntRect(10, 3341, 26, 16);
    swimLegSwimStraightFrames[1] = IntRect(41, 3340, 25, 17);
    swimLegSwimStraightFrames[2] = IntRect(71, 3338, 24, 22);
    swimLegSwimStraightFrames[3] = IntRect(100, 3337, 24, 26);
    swimLegSwimStraightFrames[4] = IntRect(129, 3336, 26, 28);
    swimLegSwimStraightFrames[5] = IntRect(160, 3337, 24, 26);
    swimLegSwimStraightFrames[6] = IntRect(189, 3338, 24, 22);
    swimLegSwimStraightFrames[7] = IntRect(218, 3340, 25, 17);
    swimLegSwimStraightFrames[8] = IntRect(248, 3341, 26, 16);
    swimLegSwimStraightFrames[9] = IntRect(279, 3341, 23, 18);
    swimLegSwimStraightFrames[10] = IntRect(307, 3340, 25, 19);
    swimLegSwimStraightFrames[11] = IntRect(337, 3339, 24, 23);
    swimLegSwimStraightFrames[12] = IntRect(366, 3336, 23, 26);
    swimLegSwimStraightFrames[13] = IntRect(394, 3336, 23, 30);
    swimLegSwimStraightFrames[14] = IntRect(422, 3336, 23, 26);
    swimLegSwimStraightFrames[15] = IntRect(450, 3339, 24, 23);
    swimLegSwimStraightFrames[16] = IntRect(479, 3340, 25, 19);
    swimLegSwimStraightFrames[17] = IntRect(509, 3341, 23, 18);

    swimLegUpStraightFrames[0] = IntRect(678, 10191, 17, 18);
    swimLegUpStraightFrames[1] = IntRect(705, 10191, 17, 18);
    swimLegUpStraightFrames[2] = IntRect(730, 10191, 17, 18);
    swimLegUpStraightFrames[3] = IntRect(757, 10191, 17, 18);

    swimLegStillSlightFrames[0] = IntRect(409, 10223, 26, 13);
    swimLegStillSlightFrames[1] = IntRect(442, 10223, 26, 13);
    swimLegStillSlightFrames[2] = IntRect(475, 10223, 26, 13);
    swimLegStillSlightFrames[3] = IntRect(509, 10223, 27, 13);

    swimLegSwimSlightFrames[0] = IntRect(552, 10223, 24, 15);
    swimLegSwimSlightFrames[1] = IntRect(584, 10223, 22, 16);
    swimLegSwimSlightFrames[2] = IntRect(615, 10223, 20, 18);
    swimLegSwimSlightFrames[3] = IntRect(644, 10223, 19, 20);

    swimLegUpSlightFrames[0] = IntRect(678, 10223, 19, 20);
    swimLegUpSlightFrames[1] = IntRect(704, 10223, 19, 20);
    swimLegUpSlightFrames[2] = IntRect(730, 10223, 19, 20);
    swimLegUpSlightFrames[3] = IntRect(756, 10223, 19, 20);

    swimLegStillMoreFrames[0] = IntRect(407, 10257, 26, 15);
    swimLegStillMoreFrames[1] = IntRect(441, 10257, 26, 15);
    swimLegStillMoreFrames[2] = IntRect(474, 10257, 26, 15);
    swimLegStillMoreFrames[3] = IntRect(508, 10257, 26, 15);

    swimLegSwimMoreFrames[0] = IntRect(551, 10257, 25, 15);
    swimLegSwimMoreFrames[1] = IntRect(583, 10257, 23, 16);
    swimLegSwimMoreFrames[2] = IntRect(613, 10256, 23, 20);
    swimLegSwimMoreFrames[3] = IntRect(642, 10256, 20, 20);

    swimLegUpMoreFrames[0] = IntRect(674, 10256, 20, 20);
    swimLegUpMoreFrames[1] = IntRect(700, 10256, 20, 20);
    swimLegUpMoreFrames[2] = IntRect(727, 10256, 20, 20);
    swimLegUpMoreFrames[3] = IntRect(754, 10256, 20, 20);

    swimLegFastFrames[0] = IntRect(408, 10293, 17, 22);
    swimLegFastFrames[1] = IntRect(437, 10293, 22, 19);
    swimLegFastFrames[2] = IntRect(468, 10293, 26, 21);
    swimLegFastFrames[3] = IntRect(504, 10290, 33, 25);
    swimLegFastFrames[4] = IntRect(545, 10287, 26, 28);
    swimLegFastFrames[5] = IntRect(580, 10293, 23, 22);
    swimLegFastFrames[6] = IntRect(614, 10293, 17, 22);
    swimLegFastFrames[7] = IntRect(640, 10293, 25, 19);
    swimLegFastFrames[8] = IntRect(674, 10293, 28, 21);
    swimLegFastFrames[9] = IntRect(713, 10289, 35, 25);
    swimLegFastFrames[10] = IntRect(756, 10286, 28, 28);
    swimLegFastFrames[11] = IntRect(793, 10294, 23, 22);

    swimUpperPistolStraightFrames[0] = IntRect(410, 191, 36, 24);
    swimUpperPistolStraightFrames[1] = IntRect(491, 191, 36, 24);
    swimUpperPistolStraightFrames[2] = IntRect(911, 192, 36, 23);
    swimUpperPistolStraightFrames[3] = IntRect(134, 10192, 36, 23);

    swimUpperPistolSlightFrames[0] = IntRect(510, 223, 34, 26);
    swimUpperPistolSlightFrames[1] = IntRect(481, 223, 34, 26);
    swimUpperPistolSlightFrames[2] = IntRect(891, 224, 34, 25);
    swimUpperPistolSlightFrames[3] = IntRect(133, 10223, 34, 26);

    swimUpperPistolMoreFrames[0] = IntRect(610, 254, 34, 23);
    swimUpperPistolMoreFrames[1] = IntRect(501, 254, 34, 23);
    swimUpperPistolMoreFrames[2] = IntRect(931, 254, 34, 23);
    swimUpperPistolMoreFrames[3] = IntRect(137, 10254, 34, 23);

    swimUpperHMGStraightFrames[0] = IntRect(410, 647, 39, 29);
    swimUpperHMGStraightFrames[1] = IntRect(511, 647, 39, 29);
    swimUpperHMGStraightFrames[2] = IntRect(981, 648, 39, 28);
    swimUpperHMGStraightFrames[3] = IntRect(144, 10648, 39, 28);

    swimUpperHMGSlightFrames[0] = IntRect(610, 684, 37, 28);
    swimUpperHMGSlightFrames[1] = IntRect(531, 684, 37, 28);
    swimUpperHMGSlightFrames[2] = IntRect(1001, 685, 37, 27);
    swimUpperHMGSlightFrames[3] = IntRect(144, 10684, 37, 28);

    swimUpperHMGMoreFrames[0] = IntRect(510, 719, 38, 23);
    swimUpperHMGMoreFrames[1] = IntRect(531, 719, 38, 23);
    swimUpperHMGMoreFrames[2] = IntRect(991, 719, 38, 23);
    swimUpperHMGMoreFrames[3] = IntRect(146, 10719, 38, 23);

    swimFastUpperPistolStraightFrames[0] = IntRect(10, 3308, 37, 23);
    swimFastUpperPistolStraightFrames[1] = IntRect(52, 3308, 36, 24);
    swimFastUpperPistolStraightFrames[2] = IntRect(93, 3308, 36, 24);
    swimFastUpperPistolStraightFrames[3] = IntRect(134, 3308, 35, 24);
    swimFastUpperPistolStraightFrames[4] = IntRect(174, 3308, 36, 24);
    swimFastUpperPistolStraightFrames[5] = IntRect(215, 3308, 35, 24);
    swimFastUpperPistolStraightFrames[6] = IntRect(255, 3308, 36, 24);
    swimFastUpperPistolStraightFrames[7] = IntRect(296, 3308, 36, 24);
    swimFastUpperPistolStraightFrames[8] = IntRect(337, 3308, 37, 23);
    swimFastUpperPistolStraightFrames[9] = IntRect(379, 3308, 37, 23);
    swimFastUpperPistolStraightFrames[10] = IntRect(421, 3308, 36, 23);
    swimFastUpperPistolStraightFrames[11] = IntRect(462, 3308, 36, 23);
    swimFastUpperPistolStraightFrames[12] = IntRect(503, 3308, 35, 24);
    swimFastUpperPistolStraightFrames[13] = IntRect(543, 3308, 35, 24);
    swimFastUpperPistolStraightFrames[14] = IntRect(583, 3308, 35, 24);
    swimFastUpperPistolStraightFrames[15] = IntRect(623, 3308, 36, 23);
    swimFastUpperPistolStraightFrames[16] = IntRect(664, 3308, 36, 23);
    swimFastUpperPistolStraightFrames[17] = IntRect(705, 3308, 37, 23);

    swimFastUpperPistolSlightFrames[0] = IntRect(710, 315, 33, 26);
    swimFastUpperPistolSlightFrames[1] = IntRect(501, 316, 33, 26);
    swimFastUpperPistolSlightFrames[2] = IntRect(911, 318, 32, 25);
    swimFastUpperPistolSlightFrames[3] = IntRect(132, 10318, 32, 26);
    swimFastUpperPistolSlightFrames[4] = IntRect(172, 10316, 32, 28);
    swimFastUpperPistolSlightFrames[5] = IntRect(213, 10317, 32, 27);
    swimFastUpperPistolSlightFrames[6] = IntRect(254, 10318, 32, 26);
    swimFastUpperPistolSlightFrames[7] = IntRect(294, 10318, 32, 26);

    swimFastUpperPistolMoreFrames[0] = IntRect(7, 10349, 34, 23);
    swimFastUpperPistolMoreFrames[1] = IntRect(49, 10349, 33, 24);
    swimFastUpperPistolMoreFrames[2] = IntRect(90, 10349, 32, 24);
    swimFastUpperPistolMoreFrames[3] = IntRect(131, 10349, 31, 24);
    swimFastUpperPistolMoreFrames[4] = IntRect(173, 10349, 31, 24);
    swimFastUpperPistolMoreFrames[5] = IntRect(214, 10349, 31, 24);
    swimFastUpperPistolMoreFrames[6] = IntRect(255, 10349, 31, 24);
    swimFastUpperPistolMoreFrames[7] = IntRect(296, 10349, 31, 24);

    swimFastUpperHMGStraightFrames[0] = IntRect(510, 749, 38, 28);
    swimFastUpperHMGStraightFrames[1] = IntRect(501, 749, 38, 29);
    swimFastUpperHMGStraightFrames[2] = IntRect(951, 750, 37, 28);
    swimFastUpperHMGStraightFrames[3] = IntRect(139, 10750, 37, 29);
    swimFastUpperHMGStraightFrames[4] = IntRect(184, 10749, 37, 30);
    swimFastUpperHMGStraightFrames[5] = IntRect(228, 10749, 37, 30);
    swimFastUpperHMGStraightFrames[6] = IntRect(275, 10750, 37, 29);
    swimFastUpperHMGStraightFrames[7] = IntRect(320, 10750, 37, 29);

    swimFastUpperHMGSlightFrames[0] = IntRect(710, 784, 36, 28);
    swimFastUpperHMGSlightFrames[1] = IntRect(511, 785, 36, 28);
    swimFastUpperHMGSlightFrames[2] = IntRect(941, 786, 36, 27);
    swimFastUpperHMGSlightFrames[3] = IntRect(139, 10786, 36, 28);
    swimFastUpperHMGSlightFrames[4] = IntRect(183, 10784, 36, 30);
    swimFastUpperHMGSlightFrames[5] = IntRect(228, 10785, 36, 29);
    swimFastUpperHMGSlightFrames[6] = IntRect(273, 10786, 36, 28);
    swimFastUpperHMGSlightFrames[7] = IntRect(322, 10786, 36, 28);

    swimFastUpperHMGMoreFrames[0] = IntRect(710, 821, 37, 23);
    swimFastUpperHMGMoreFrames[1] = IntRect(511, 821, 37, 25);
    swimFastUpperHMGMoreFrames[2] = IntRect(961, 821, 36, 25);
    swimFastUpperHMGMoreFrames[3] = IntRect(140, 10821, 36, 25);
    swimFastUpperHMGMoreFrames[4] = IntRect(184, 10821, 36, 25);
    swimFastUpperHMGMoreFrames[5] = IntRect(229, 10821, 36, 25);
    swimFastUpperHMGMoreFrames[6] = IntRect(275, 10821, 36, 25);
    swimFastUpperHMGMoreFrames[7] = IntRect(322, 10821, 36, 25);

    swimFirePistolStraightFrames[0] = IntRect(10, 3435, 41, 23);
    swimFirePistolStraightFrames[1] = IntRect(56, 3435, 53, 23);
    swimFirePistolStraightFrames[2] = IntRect(114, 3435, 54, 23);
    swimFirePistolStraightFrames[3] = IntRect(173, 3435, 55, 23);
    swimFirePistolStraightFrames[4] = IntRect(233, 3435, 39, 23);
    swimFirePistolStraightFrames[5] = IntRect(277, 3435, 39, 23);
    swimFirePistolStraightFrames[6] = IntRect(321, 3435, 40, 24);
    swimFirePistolStraightFrames[7] = IntRect(366, 3435, 40, 24);
    swimFirePistolStraightFrames[8] = IntRect(411, 3435, 37, 26);
    swimFirePistolStraightFrames[9] = IntRect(453, 3435, 37, 26);

    swimFirePistolSlightFrames[0] = IntRect(6, 10413, 40, 22);
    swimFirePistolSlightFrames[1] = IntRect(54, 10414, 52, 20);
    swimFirePistolSlightFrames[2] = IntRect(114, 10413, 53, 21);
    swimFirePistolSlightFrames[3] = IntRect(176, 10413, 38, 21);
    swimFirePistolSlightFrames[4] = IntRect(239, 10413, 38, 21);
    swimFirePistolSlightFrames[5] = IntRect(286, 10413, 38, 22);
    swimFirePistolSlightFrames[6] = IntRect(332, 10413, 39, 22);
    swimFirePistolSlightFrames[7] = IntRect(379, 10414, 39, 22);
    swimFirePistolSlightFrames[8] = IntRect(426, 10413, 36, 26);
    swimFirePistolSlightFrames[9] = IntRect(470, 10414, 36, 25);

    swimFirePistolMoreFrames[0] = IntRect(610, 449, 40, 22);
    swimFirePistolMoreFrames[1] = IntRect(531, 449, 52, 22);
    swimFirePistolMoreFrames[2] = IntRect(113, 10448, 53, 23);
    swimFirePistolMoreFrames[3] = IntRect(174, 10449, 53, 22);
    swimFirePistolMoreFrames[4] = IntRect(237, 10448, 38, 23);
    swimFirePistolMoreFrames[5] = IntRect(285, 10447, 38, 24);
    swimFirePistolMoreFrames[6] = IntRect(332, 10448, 39, 22);
    swimFirePistolMoreFrames[7] = IntRect(378, 10448, 36, 22);
    swimFirePistolMoreFrames[8] = IntRect(426, 10447, 35, 23);
    swimFirePistolMoreFrames[9] = IntRect(470, 10447, 34, 23);

    swimFireHMGStraightFrames[0] = IntRect(510, 853, 59, 29);
    swimFireHMGStraightFrames[1] = IntRect(731, 854, 60, 29);
    swimFireHMGStraightFrames[2] = IntRect(140, 10853, 58, 29);
    swimFireHMGStraightFrames[3] = IntRect(208, 10853, 60, 29);

    swimFireHMGSlightFrames[0] = IntRect(710, 890, 58, 27);
    swimFireHMGSlightFrames[1] = IntRect(731, 891, 60, 27);
    swimFireHMGSlightFrames[2] = IntRect(143, 10890, 57, 27);
    swimFireHMGSlightFrames[3] = IntRect(208, 10890, 60, 28);

    swimFireHMGMoreFrames[0] = IntRect(710, 924, 58, 22);
    swimFireHMGMoreFrames[1] = IntRect(731, 924, 60, 23);
    swimFireHMGMoreFrames[2] = IntRect(142, 10924, 57, 24);
    swimFireHMGMoreFrames[3] = IntRect(208, 10924, 60, 23);

    // === END INJECT ===

  }

protected:
  bool fireForward = true;

public:
  ~MarcoRossi() {}

  void activateSpecial() override {
    if (!currentWeapon || currentWeapon->isMelee())
      return;
    specialActive = true;
    specialDuration = 10;
    fireForward = false; // reset to forward when starting
  }

  float getSpecialFireRateMultiplier() const override {
    return (specialActive) ? 2 : 1;
  }

  void shoot() override {
    if (!specialActive || !currentWeapon || currentWeapon->isMelee()) {
      PlayerSoldier::shoot();
      return;
    }

    double angle;
    if (isLookingUp) {
      angle = isFacingLeft ? (3.14159265 + aimAngle) : -aimAngle;
    } else {
      angle = isFacingLeft ? 3.14159 : 0.0;
    }

    double muzzleX;
    if (angle == 0.0) {
      muzzleX = posX + getWidth() + 2;
    } else if (angle == 3.14159) {
      muzzleX = posX - 2;
    } else {
      muzzleX = posX + (isFacingLeft ? 0.0 : getWidth()); // upward etc
    }

    double muzzleY = posY + getHeight() * 0.3 - aimAngle * getHeight() * 0.2;

    pendingShot = currentWeapon->fire(muzzleX, muzzleY, angle, true);

    if (pendingShot) {
      if (fireAnimTimer <= 0) {
        currentFrame = 0;
        animTimer = 0;
      }
      fireAnimTimer = numShootFrames * (animDelay * 0.5f / 1.5f);

      // If horizontal, fire in the opposite direction too!
      if (!isLookingUp) {
        currentWeapon->resetFireTimer();
        double oppositeAngle = isFacingLeft ? 0.0 : 3.14159;
        double oppositeMuzzleX = (oppositeAngle == 0.0) ? (posX + getWidth() + 2) : (posX - 2);
        
        Projectile* backwardShot = currentWeapon->fire(oppositeMuzzleX, muzzleY, oppositeAngle, true);
        if (backwardShot) {
          EntityManager::getInstance()->addEntity(backwardShot);
        }
      }
    }
  }

  bool isMarco() const override { return true; }

  const char *getCharacterName() const override { return "Marco"; }

  void applyCharacterBuffs() override {
    pistolFireRate = 5.0;
  }
};