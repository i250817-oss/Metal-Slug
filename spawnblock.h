#include "blocksub.h"


class SpawnBlock : public BlockSub {
public:
    SpawnBlock(Biome* biome): BlockSub(biome, false) {}   //spawn markers are never destructible
 
    //from Block  all spawn markers report true here
    bool isSpawnMarker() const override { 
        return true;
     }
 
    //Fine-grained unit-type queries 
    //defaults: false.  Each subclass flips exactly one of these.
    virtual bool isInfantrySpawn() const { 
        return false; 
    }
    virtual bool isAerialSpawn()   const { 
        return false;
     }
    virtual bool isUndeadSpawn()   const { 
        return false; 
    }
    virtual bool isAquaticSpawn()  const { 
        return false; 
    }
    virtual bool isPowSpawn()      const {
         return false; 
        }
    virtual bool isBradleySpawn()  const {
         return false;
         }
    Block* clone() const override { return new SpawnBlock(*this); }
};