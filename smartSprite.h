#ifndef SMARTSPRITE__H
#define SMARTSPRITE__H
#include <iostream>
#include "multisprite.h"

// SmartSprite is an observer of subject SubjectSprite, which
// in this example is a spinning star.

class SmartSprite : public MultiSprite {
public:
  SmartSprite(const std::string&);
  void draw() const;
  void update(Uint32 ticks);
  void goLeft();
  void goRight();
  bool isDead(){ if(hp==0) return true; else return false;}
  void damage(){hp--;}
  int getHP(){return hp;}

  void setPlayerPos(const Vector2f& p) { playerPos = p; }
  void setPlayerDead(bool condition) { isPlayerDead = condition; }

private:
  int hp=5;
  Vector2f playerPos;
  bool isPlayerDead = false;
  SmartSprite(const SmartSprite &);
  SmartSprite& operator=(const SmartSprite &rhs);
};

#endif
