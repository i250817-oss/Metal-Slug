#pragma once
#include "LevelBase.h"


class Level1 : public LevelBase {
private:
  void put(int col, int row, Block *b) {
    if (col < 0 || col >= width || row < 0 || row >= height) {
      delete b;
      return;
    }
    delete grid[col][row];
    grid[col][row] = b;
  }

protected:
  int getGroundRow(int col) override {
    static const int GR[200] = {
        
        35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35,
        35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 34, 34,
        34, 34, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35,
        35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35,

        
        43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43,
        43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43,
        43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43,
        43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43,

        
        35, 35, 35, 35, 35, 34, 33, 31, 29, 27, 25, 23, 25, 27, 29, 31, 33, 34,
        35, 35, 
        35, 35, 34, 33, 31, 29, 27, 25, 22, 20, 22, 25, 27, 29, 31, 33, 34, 35,
        35, 35, 
        35, 34, 33, 31, 29, 26, 23, 20, 17, 16, 17, 20, 23, 26, 29, 31, 33, 34,
        35, 35,                        
        35, 35, 35, 35, 35, 35, 35, 42 
    };
    if (col < 0 || col >= 200)
      return 35;
    return GR[col];
  }

  void placeTerrain() {
    
    for (int c = 20; c <= 22; c++) {
      put(c, 32, new AirBlock(plainsBiome));
      put(c, 33, new PlatformBlock(plainsBiome));
    }
    
    for (int c = 24; c <= 30; c++) {
      put(c, 30, new AirBlock(plainsBiome));
      put(c, 31, new PlatformBlock(plainsBiome));
    }
    
    for (int c = 33; c <= 35; c++) {
      put(c, 32, new AirBlock(plainsBiome));
      put(c, 33, new PlatformBlock(plainsBiome));
    }

    
    put(66, 35, new SolidBlock(aerialBiome));
    for (int r = 36; r <= 43; r++)
      put(66, r, new IndestructibleBlock(aquaticBiome));
    put(66, 44, new SeafloorBlock(aquaticBiome));

   
    put(131, 35, new SolidBlock(plainsBiome));
    for (int r = 36; r <= 43; r++)
      put(131, r, new IndestructibleBlock(aquaticBiome));
    put(131, 44, new SeafloorBlock(aquaticBiome));

   
    for (int c = 67; c <= 130; c++) {
      int gr = getGroundRow(c);
      if (gr == 44)
        put(c, 44, new SeafloorBlock(aquaticBiome));
    }

   
    auto aqPlat = [&](int colStart, int colEnd) {
      for (int c = colStart; c <= colEnd; c++) {
        put(c, 40, new AirBlock(aquaticBiome));
        put(c, 41, new PlatformBlock(aquaticBiome));
      }
    };
    aqPlat(74, 75);   
    aqPlat(83, 85);   
    aqPlat(93, 94);   
    aqPlat(101, 102); 
    aqPlat(109, 110); 
    aqPlat(117, 118); 

    
    for (int c = 141; c <= 145; c++)
      put(c, 23, new AirBlock(aerialBiome));
    for (int c = 142; c <= 144; c++)
      put(c, 24, new AirBlock(aerialBiome));
    put(143, 25, new AirBlock(aerialBiome));

    
    for (int c = 158; c <= 163; c++)
      put(c, 20, new AirBlock(aerialBiome));
    for (int c = 159; c <= 162; c++)
      put(c, 21, new AirBlock(aerialBiome));
    for (int c = 160; c <= 161; c++)
      put(c, 22, new AirBlock(aerialBiome));

    
    for (int c = 178; c <= 184; c++)
      put(c, 16, new AirBlock(aerialBiome));
    for (int c = 179; c <= 183; c++)
      put(c, 17, new AirBlock(aerialBiome));
    for (int c = 180; c <= 182; c++)
      put(c, 18, new AirBlock(aerialBiome));
    put(181, 19, new AirBlock(aerialBiome));
  }

  void bakeSpawns() {
    
    
    addManualSpawn("rebel", 9, 34, 3);     
    addManualSpawn("rebel", 55, 34, 3);    
    addManualSpawn("shielded", 20, 34, 2); 
    addManualSpawn("shielded", 50, 34, 2); 
    addManualSpawn("grenade", 15, 34, 2);  
    addManualSpawn("grenade", 60, 34, 2);  
    addManualSpawn("bazooka", 30, 34, 2);  
    addManualSpawn("bazooka", 45, 34, 2);  

    
    addManualSpawn("rebel", 80, 42, 3);     
    addManualSpawn("shielded", 100, 41, 2); 
    addManualSpawn("zombie", 115, 60, 2);   
    addManualSpawn("zombie", 125, 60, 2);   

    
    addManualSpawn("rebel", 135, 39, 3);    
    addManualSpawn("shielded", 155, 21, 2); 
    addManualSpawn("grenade", 170, 34, 2);  
    addManualSpawn("bazooka", 190, 34, 2);  

    
    
    addManualSpawn("paratrooper", 168, 18, 3); 
    addManualSpawn("alien", 188, 18, 1);       

    
    addManualSpawn("zombie", 40, 34, 4); 
    addManualSpawn("zombie", 56, 34, 4); 
    addManualSpawn("mummy", 25, 34, 1);  
    addManualSpawn("mummy", 48, 34, 1);  

    
    addManualSpawn("zombie", 78, 42, 4);  
    addManualSpawn("zombie", 112, 55, 4); 

    
    addManualSpawn("flying_tara", 95, 30, 2);  
    addManualSpawn("flying_tara", 168, 25, 2); 

    
    addManualSpawn("enemy_sub", 90, 40, 1); 

    
    addManualSpawn("metal_slug", 3, getFirstSolidRow(3), 1);

    
    placeEnemies("pow", 1, 0, 0, 65, 1);
    placeEnemies("pow", 1, 1, 66, 131, 1);
    placeEnemies("pow", 1, 2, 132, 199, 1);
  }

public:
  Level1() : LevelBase(1, 200, 50) {}

  void load() override {
    LevelBase::load();
    placeTerrain();
    bakeSpawns();
  }
};
