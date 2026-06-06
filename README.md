# 🎮 Metal Slug — C++ & SFML

> A fully playable Metal Slug inspired 2D run-and-gun shooter built from scratch in C++ and SFML with zero game engine, for CS-1004 Object Oriented Programming under instructor **Shehreyar Rashid**.

---

## 👥 Team

-> **Muhammad Osaid Zahid** ( Group Leader)

-> **Ahmad Ali Shah** (Developer)

---

## 🕹️ What Is This

A Metal Slug inspired 2D action shooter where the **Peregrine Falcon Strike Force** fights through procedurally generated and handcrafted levels filled with rebel soldiers, zombies, aliens, and war machines. Every system, terrain, AI, physics, rendering, voice recognition, was built from scratch without any engine or STL containers.

---

## 🧑‍🤝‍🧑 Playable Characters

-> **Marco Rossi** , balanced stats, reliable heavy machine gun

-> **Tarma Roving** , vehicle specialist, faster slug boarding

-> **Eri Kasamoto** ,grenade expert, high explosive damage

-> **Fio Germi** , ammo specialist, extended weapon duration

-> Each character has unique buffs, weaknesses, and a separate independent inventory

-> Switch between characters mid-game with full item state preservation

---

## ⚔️ Enemy Roster

-> Rebel Soldiers, Shielded Soldiers, Grenade Soldiers, Bazooka Soldiers

-> Paratroopers, Zombies, Mummies, Martian Aliens

-> Flying Tara, Enemy Submarines, M-15A Bradley Tanks

---

## 🚗 Vehicles

-> **Metal Slug Tank** , ground assault, heavy cannon

-> **Slug Flyer** , aerial combat, air to ground missiles

-> **Slug Mariner** , underwater combat, torpedo system

-> **Amphibious Slug** , auto-transforms based on current biome, ground, water, air

---

## 💣 Weapon System

-> Heavy Machine Gun (HMG)

-> Rocket Launcher

-> Flame Shot

-> Laser Gun

-> Hand Grenades

-> Fire Bomb Grenades

---

## 🗺️ Level Architecture

```
Level (abstract)
    └── LevelBase
            ├── Level1        (200 x 50 blocks — Easy)
            ├── Level2        (200 x 70 blocks — Medium)
            ├── Level3        (250 x 60 blocks — Hard)
            ├── BossLevel     (300 x 70 blocks — 5 phases)
            └── InfinityLevel (10000 x 70 blocks — Campaign / Infinite World)
```

-> Every level has three biome zones, **Plains**, **Aquatic**, **Aerial**

-> Each biome has its own block types, spawn pools, and terrain profile

-> Camera follows the player with lerp smoothing and clamp-bounded scroll

-> Blocks are individually typed, Dirt, Stone, Water, Air, Indestructible, Platform, Barrel

-> Destructible blocks leave craters that fill with water over time when it rains

---

## 👾 Boss Level — 5 Phase Design

-> **Phase 1 (Plains)** ,Iron Nokana tank boss

-> **Phase 2 (Aerial)** , Hairbuster Riberts airborne boss

-> **Phase 3 (Aquatic)** , Sea Satan deep water boss

-> **Phase 4 (Fusion Prep)** , 3-second dramatic countdown at col 280

-> **Phase 5 (Fusion Fight)** ,Fusion Boss spawns dynamically, inherits from all three boss classes and dispatches attacks, sprites, and health polymorphically based on combat mode

-> World pixel width is locked per phase, player cannot advance until the active boss is killed

---

## 🌍 Infinite World — Perlin Noise Terrain Generation

-> **InfinityLevel** extends 10000 blocks in each direction, roughly 1 million block range

-> Terrain generated using **Fractal Brownian Motion** following Ken Perlin's 1985 and 2002 papers

-> **Two uncorrelated noise channels** with widely spaced Y offsets to prevent correlation

-> Channel 1 (y = 55.7), low frequency biome trend, selects Plains, Aerial, Aquatic

-> Channel 2 (y = 7.3), high frequency local detail for grass bumps and small hills

-> **Domain Warping** , Perlin noise added to sine wave phase, stretches and squashes biome widths randomly, giving each mountain and ocean basin a unique width while remaining perfectly smooth

-> **Sine cubing (x³ shaping)** , flattens the sine wave near zero to produce wide beautiful plains while keeping mountain peaks and ocean depths sharp

-> **NoiseProfile polymorphism** ,InfinityLevel holds a `NoiseProfile*` pointer and never knows the concrete type

-> `NormalProfile` , amplitude 1.0, frequency 0.08, 4 octaves, persistence 0.5

-> `AmplifiedProfile` , amplitude 2.5, frequency 0.06, 6 octaves, extreme mountains

-> `FlatProfile` ,amplitude 0.4, frequency 0.12, 2 octaves, near-flat terrain

-> **NoiseProfileFactory::create()** returns the correct subtype at runtime, adding a new terrain profile means adding one class and touching nothing else

-> **FractalNoise** wraps **PerlinNoise** and delegates to `generateHeightOctaves()` which implements the paper's 1/f octave loop: `Noise(point * 2^i) / 2^i`

-> **Improved fade function** , upgraded from Perlin's 1985 cubic `3t² - 2t³` to his 2002 quintic `6t⁵ - 15t⁴ + 10t³`, zero second derivative at endpoints, eliminating visible grid artifacts

-> **Dynamic enemy spawning** , every 5-8 seconds a batch of enemies spawns 900 pixels ahead of the player, selected from biome-appropriate pools

-> **Completion condition** , kill 5 of each enemy type and 3 of each enemy vehicle

---

## 🤖 Self-Playing AI aka NEAT

**NEAT (NeuroEvolution of Augmenting Topologies)** proposed by Kenneth Stanley and Risto Miikkulainen (2002).

-> Every agent has a **NeatGenome**, a genome of node genes and connection genes

-> Each **ConnectionGene** stores inNode, outNode, weight, isEnabled, innovationNumber

-> Network starts with **zero hidden neurons**, earns every neuron through evolution

-> Agent sees game state, position, enemy distance, velocity, and maps four network outputs directly to jump, shoot, move left, move right

-> Fitness scored as `timeAlive + (kills × 10)`

**Crossover**

-> Matching genes inherited randomly from either parent

-> Excess and disjoint genes always inherited from the fitter parent

-> 75% chance a gene stays disabled if either parent had it disabled

**Three Mutation Types**

-> Weight mutation, nudges or completely replaces a connection weight

-> Add connection, creates a new synapse between two existing nodes

-> Add node, splits an existing connection, places a hidden node in the middle, disables the old connection

**Speciation**

-> Compatible genomes grouped into species using compatibility distance: `(C1 × excess / N) + (C2 × disjoint / N) + (C3 × avgWeightDiff)`

-> New mutations compete only within their species so innovations survive long enough to prove themselves

-> Champion of each species survives every generation via elitism

-> Global best brain saved every generation, replayable live with keyboard key **B**

-> No STL containers, everything runs on manually managed dynamic arrays

-> Activation function: `φ(x) = 1 / (1 + e^(-4.9x))` evaluated over 5 forward passes

---

## 🎙️ Voice Activated Launch — FFT

-> Game startup gate requires speaking **"Valar Morghulis"** into your microphone

-> Custom **Whisper, Spectrum, Complex** classes perform FFT from scratch

-> Extracts magnitude spectrum template from `valar_morghulis.wav`

-> Records microphone input in real time, performs same FFT decomposition

-> Applies **cosine similarity** over speech frequency band 150–3500 Hz to handle environmental noise

-> Match above **40%** triggers game launch

-> Same FFT infrastructure used in **Echo Chambers**, runtime low-pass filtering simulates muffled underwater acoustics

---

## 🏗️ OOP Architecture

**Entity Hierarchy**

```
Entity (abstract)
    └── DamagableEntity  (physics, health, collision dispatch)
            └── Soldier  (transformation states)
                    └── PlayerSoldier  (input, combat, animation)
                            ├── MarcoRossi
                            ├── TarmaRoving
                            ├── EriKasamoto
                            └── FioGermi
```

-> `EntityManager` holds a single `Entity**` array and calls `update()` and `draw()` on every object, players, enemies, vehicles, projectiles, collectibles, without knowing their concrete types

**Multiple Inheritance**

-> `AmphibiousSlug` inherits from `MetalSlug`, `SlugFlyer`, and `SlugMariner` using virtual inheritance

-> `switchVehicle()` delegates `handleDriverInput()`, `update()`, and `draw()` to whichever parent class matches the current biome

-> `FusionBoss` inherits from all three boss classes and dispatches attacks polymorphically based on combat mode

**State Pattern**

-> `EnemyAIState` — abstract base with six concrete states, Patrol, Chase, Shoot, Retreat, Melee, Panic

-> Each enemy holds a state pointer and calls `state->update(enemy, dt)`, zero conditional logic inside the enemy itself

-> `TransformationState` — Undead, Mummy, Fat, Normal each implement `enter()`, `update()`, `exit()` to apply speed penalties, weapon restrictions, and visuals without any switch-case

**Camera**

-> Lerp-smoothed follow with `weight = 0.198`

-> Clamp algorithm: `max(minVal, min(limit, maxVal))` bounds scroll to world edges

**GameStateManager**

-> Stack-based state machine, adding a new screen is one subclass and one `changeState()` call

---

## 🛠️ Custom Urdu Assembly REPL

-> Developer mode runtime REPL that accepts **Urdu Assembly language** commands

-> Allows live injection of game variables without recompiling

---

## 📋 Constraints Met

-> No STL containers anywhere, all data structures are manually managed dynamic arrays

-> Strict polymorphism throughout, every system operates through abstract base pointers

-> No game engine, pure C++ and SFML only

---

## 🎬 Demo (2nd link)

-> [GitHub Repository Link]

-> [Gameplay Video Link]

---
## 📁 Project Directory
 
Metal-Slug/
├── main.cpp
├── player_save.txt
├── score_history.txt
│
├── headers/
│   ├── headers.h
│   └── string.h
│
├── assembly/
│   ├── assembly.cpp
│   └── assembly.h
│
├── Entity Root/
│   ├── Entity.h
│   └── DamagableEntity.h
│
├── Character System/
│   ├── Soldier.h
│   ├── PlayerSoldier.h
│   ├── MarcoRossi.h
│   ├── Tarma.h
│   ├── EriKasamoto.h
│   ├── FiolinaGermi.h
│   ├── TransformationState.h
│   ├── FusionCompanion.h
│   └── FusionCompanion.cpp
│
├── Enemy System/
│   ├── Enemy.h / Enemy.cpp
│   ├── EnemyAIState.h / EnemyAIState.cpp
│   ├── Aerial/
│   │   ├── AerialEnemy.h
│   │   └── Paratrooper.h
│   ├── Alien/
│   │   └── AlienEnemy.h
│   ├── Infantry/
│   │   ├── InfantryEnemy.h / InfantryEnemy.cpp
│   │   ├── RebelSoldier.h
│   │   ├── ShieldedSoldier.h
│   │   ├── GrenadeSoldier.h
│   │   └── BazookaSoldier.h
│   ├── Undead/
│   │   ├── UndeadEnemy.h
│   │   ├── Zombie.h
│   │   └── MummyWarrior.h
│   └── Boss/
│       ├── Boss.h
│       ├── IronNokana.h
│       ├── HairbusterRiberts.h
│       ├── SeaSatan.h
│       └── FusionBoss.h
│
├── Vehicle System/
│   ├── Vehicle.h / Vehicle.cpp
│   ├── GroundVehicle.h
│   ├── AerialVehicle.h
│   ├── AquaticVehicle.h
│   ├── EnemyVehicle.h
│   ├── PlayerVehicle.h
│   ├── Player Vehicles/
│   │   ├── MetalSlug.h
│   │   ├── SlugFlyer.h
│   │   ├── SlugMariner.h
│   │   └── AmphibiousSlug.h
│   └── Enemy Vehicles/
│       ├── FlyingTara.h
│       ├── EnemySub.h
│       └── M15ABradley.h
│
├── Weapon System/
│   ├── Weapon.h
│   ├── Ammo.h
│   ├── NonProjectileWeapon.h
│   ├── ProjectileWeapon.h
│   ├── FireArms/
│   │   ├── FireArms.h
│   │   ├── HeavyMachineGun.h
│   │   ├── RocketLauncher.h
│   │   ├── FlameShot.h
│   │   ├── LaserGun.h
│   │   └── Pistol.h
│   ├── Explosives/
│   │   ├── Explosives.h
│   │   ├── HandGrenade.h
│   │   └── FireBombGrenade.h
│   └── Melee/
│       └── Knife.h
│
├── Projectile System/
│   ├── Projectile.h
│   ├── StraightProjectile.h
│   ├── BallisticProjectile.h
│   ├── Bullet.h / Bullet.cpp
│   ├── Rocket.h / Rocket.cpp
│   ├── Grenade.h / Grenade.cpp
│   ├── EnergyBeam.h / EnergyBeam.cpp
│   ├── FireBomb.h / FireBomb.cpp
│   ├── FlameStream.h / FlameStream.cpp
│   └── StraightProjectile.h
│
├── Noise System/
│   ├── NoiseProfile.h
│   ├── NoiseProfileFactory.h
│   ├── NormalProfile.h
│   ├── AmplifiedProfile.h
│   ├── FlatProfile.h
│   ├── PerlinNoise.h
│   └── factrialNoise.h
│
├── NEAT/
│   ├── NodeGene.h
│   ├── ConnectionGene.h
│   ├── NeatGenome.h
│   ├── NeatAgent.h
│   └── NeatPopulation.h
│
├── Level and Environment System/
│   ├── Level.h
│   ├── LevelBase.h
│   ├── LevelProfile.h
│   ├── Level1.h
│   ├── Level2.h
│   ├── Level3.h
│   ├── BossLevel.h
│   ├── InfinityLevel.h
│   ├── CampaignLevel.h
│   ├── camera.h
│   ├── spawnPoint.h
│   ├── perlin.h
│   ├── Biome.h / AerialBiome.h / AquaticBiome.h / PlainsBiome.h
│   ├── Block.h
│   ├── block/
│   │   ├── airblock.h, solidblock.h, platformblock.h
│   │   ├── indistructable.h, seafloorBlock.h, shipHull.h
│   │   ├── waterSurfaceblock.h, waterDeepblock.h
│   │   ├── BarrelDestructable.h, mountainTop.h
│   │   └── (spawn blocks, biome subs)
│   └── Enemies/
│       └── (enemy header references)
│
├── Manager/
│   ├── EntityManager.h / EntityManager.cpp
│   ├── LevelManager.h
│   └── SoundManager.h
│
├── Core Engine And States/
│   ├── Game.h
│   ├── GameState.h
│   ├── GameStateManager.h
│   ├── PlayState.h
│   ├── GameOverState.h
│   ├── LoadingState.h
│   ├── ResultsState.h
│   ├── mainmenu.h
│   ├── PlayerFactory.h
│   ├── LevelFactory.h
│   ├── DamageOverlay.h / DamageOverlay.cpp
│   ├── VoiceRecognition.h
│   └── UrduDeveloperTerminal.h
│
├── Game Mode System/
│   ├── GameMode.h
│   ├── CampaignMode.h
│   └── SurvivalMode.h
│
├── Collectible and Interactable System/
│   ├── Collectible.h / Collectible.cpp
│   ├── InteractableObject.h
│   ├── EnemyDropSystem.h
│   ├── Food.h
│   ├── Fruit.h / Fruit.cpp
│   ├── Turkey.h / Turkey.cpp
│   ├── SupplyBox.h / SupplyBox.cpp
│   └── POWPrisoner.h / POWPrisoner.cpp
│
├── ScoreCard/
│   ├── ScoreCard.h
│   ├── PlayerScoreCard.h
│   └── Enemies/
│       ├── EnemyScore.h, RebelScore.h, ShieldedScore.h
│       ├── GrenadeScore.h, BazookaScore.h, MummyScore.h
│       ├── ZombieScore.h, MartianScore.h
│
├── Sprite System/
│   └── MainSprite.h
│
├── Sprites/
│   ├── (character sprites, enemy sprites, block textures)
│   ├── bgs/        (background images per biome)
│   ├── blocks/     (block textures)
│   ├── Enemies/    (enemy sprite sheets)
│   └── Character/  (companion sprites)
│
├── sound/
│   ├── (stage music .mp3)
│   ├── (character voice .ogg)
│   ├── (weapon and effect sounds .ogg)
│   └── valar_morghulis.wav
│
├── Founts/
│   └── Metal-Slug-Latino-Regular.ttf
│
├── main menu/
│   └── (menu background images)
│
└── Update/
    ├── fio_coords.txt
    └── FiolitaGermi_Sprite_Doc.pdf
```
 
---
## 🙏 Special Thanks

Special thanks to our **goated instructor Shehreyar Rashid** for assigning this project and pushing us far beyond typical coursework.

Special salute to group leader **Muhammad Osaid Zahid** for giving 100 percent every single time.

---

*Built with C++ | SFML | Pure OOP | No Engine | No STL*
