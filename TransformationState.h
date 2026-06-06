#pragma once
#include "../headers/headers.h"



//forward declaration
class Soldier;

const int TRANS_STATE_NORMAL = 0;
const int TRANS_STATE_UNDEAD = 1;
const int TRANS_STATE_MUMMY  = 2;
const int TRANS_STATE_FAT    = 3;

//abstract State — handles NORMAL / UNDEAD / MUMMY transformations
class TransformationState {
protected:
  float duration; //how long this state lasts (seconds)
  float elapsedTime;

public:
  TransformationState(float duration) : duration(duration), elapsedTime(0) {}
  virtual ~TransformationState() {}

  virtual void enter(Soldier *soldier) = 0;
  virtual void update(Soldier *soldier, float dt) = 0;
  virtual void exit(Soldier *soldier) = 0;
  virtual int getID() const = 0;
  virtual const char *getStateName() const = 0;

  bool isExpired() const {
    return (duration > 0) && (elapsedTime >= duration);
  }
};

//concrete States
class NormalState : public TransformationState {
public:
  NormalState();
  void enter(Soldier *soldier) override;
  void update(Soldier *soldier, float dt) override;
  void exit(Soldier *soldier) override;
  const char *getStateName() const override;
  int getID() const override;
};

class UndeadState : public TransformationState {
private:
  float speedPenalty; //50% speed reduction
public:
  UndeadState();
  void enter(Soldier *soldier) override;
  void update(Soldier *soldier, float dt) override;
  void exit(Soldier *soldier) override;
  const char *getStateName() const override;
  int getID() const override;
};

class MummyState : public TransformationState {
public:
  MummyState();
  void enter(Soldier *soldier) override;
  void update(Soldier *soldier, float dt) override;
  void exit(Soldier *soldier) override;
  const char *getStateName() const override;
  int getID() const override;
};

class FatState : public TransformationState {
public:
  FatState();
  void enter(Soldier *soldier) override;
  void update(Soldier *soldier, float dt) override;
  void exit(Soldier *soldier) override;
  const char *getStateName() const override;
  int getID() const override;
};

// ============================================================================
// Inline Implementations
// ============================================================================
#include "Soldier.h"

inline NormalState::NormalState() : TransformationState(0) {}
inline void NormalState::enter(Soldier *soldier) { soldier->setMaxSpeed(5); }
inline void NormalState::update(Soldier *soldier, float dt) {}
inline void NormalState::exit(Soldier *soldier) {}
inline const char *NormalState::getStateName() const { return "Normal"; }
inline int NormalState::getID() const { return TRANS_STATE_NORMAL; }

inline UndeadState::UndeadState() : TransformationState(10), speedPenalty(0.5f) {}
inline void UndeadState::enter(Soldier *soldier) { soldier->setMaxSpeed(soldier->getMaxSpeed() * speedPenalty); }
inline void UndeadState::update(Soldier *soldier, float dt) { elapsedTime += dt; }
inline void UndeadState::exit(Soldier *soldier) { soldier->restoreBaseMaxSpeed(); }
inline const char *UndeadState::getStateName() const { return "Undead"; }
inline int UndeadState::getID() const { return TRANS_STATE_UNDEAD; }

inline MummyState::MummyState() : TransformationState(10) {}
inline void MummyState::enter(Soldier *soldier) { soldier->setMaxSpeed(soldier->getMaxSpeed() * 0.3f); }
inline void MummyState::update(Soldier *soldier, float dt) { elapsedTime += dt; }
inline void MummyState::exit(Soldier *soldier) { soldier->restoreBaseMaxSpeed(); }
inline const char *MummyState::getStateName() const { return "Mummy"; }
inline int MummyState::getID() const { return TRANS_STATE_MUMMY; }

inline FatState::FatState() : TransformationState(15) {}
inline void FatState::enter(Soldier *soldier) { soldier->setMaxSpeed(soldier->getMaxSpeed() * 0.4f); }
inline void FatState::update(Soldier *soldier, float dt) { elapsedTime += dt; }
inline void FatState::exit(Soldier *soldier) { soldier->restoreBaseMaxSpeed(); }
inline const char *FatState::getStateName() const { return "Fat"; }
inline int FatState::getID() const { return TRANS_STATE_FAT; }
