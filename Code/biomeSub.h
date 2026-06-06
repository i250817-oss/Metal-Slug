#pragma once
#include "biome.h"



class BiomeSub : public Biome {
protected:
    int startCol;   //first column index of this biome zone in the level
    int startX;     //first pixel X coordinate (startCol * blockSize)

    BiomeSub(int id) : Biome(id), startCol(0), startX(0) {}

public:
    virtual ~BiomeSub() {}

    //Define EVERY pure virtual declared in Biome 
    //size getters read the protected members inherited from Biome
    int  getBlockSize()      const override { 
        return blockSize;     
     }
    int  getLevelMinWidth()  const override { 
        return levelMinWidth;
      }
    int  getLevelMaxWidth()  const override { 
        return levelMaxWidth; 
     }
    int  getLevelMinHeight() const override { 
        return levelMinHeight; 
    }
    int  getLevelMaxHeight() const override { 
        return levelMaxHeight;
     }

 
    bool isAerial()  const override { return false; }
    bool isPlains()  const override { return false; }
    bool isAquatic() const override { return false; }

    //specialized feature implementation
    int  getStartCol() const override { return startCol; }
    int  getStartX()   const override { return startX;   }
    void setStartCol(int c) override  { startCol = c;    }
    void setStartX(int x)   override  { startX   = x;    }
};