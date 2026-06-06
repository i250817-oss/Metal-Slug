#pragma once




class Biome {
protected:
   
    const int zoneId;
    const int blockSize;        //32x32 px voxel size, drives all rendering
    const int levelMinWidth;    //spec: 150–250 blocks wide
    const int levelMaxWidth;
    const int levelMinHeight;   //spec: 20–60 blocks tall
    const int levelMaxHeight;

    //only BiomeSub calls this — passes id up the chain
    Biome(int id): zoneId(id),blockSize(64), levelMinWidth(150), levelMaxWidth(250), levelMinHeight(20), levelMaxHeight(60) {}

public:
    virtual ~Biome() {}

    virtual int  getBlockSize()      const = 0;
    virtual int  getLevelMinWidth()  const = 0;
    virtual int  getLevelMaxWidth()  const = 0;
    virtual int  getLevelMinHeight() const = 0;
    virtual int  getLevelMaxHeight() const = 0;
    virtual bool isAerial()          const = 0;
    virtual bool isPlains()          const = 0;
    virtual bool isAquatic()         const = 0;

    //specialized feature: where this biome zone starts in the level
    virtual int  getStartCol() const = 0;   //first column index
    virtual int  getStartX()   const = 0;   //first pixel X coordinate
    virtual void setStartCol(int c) = 0;
    virtual void setStartX(int x)   = 0;
};


