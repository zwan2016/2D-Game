#include <cmath>
#include "viewport.h"
#include "smartSprite.h"


SmartSprite::SmartSprite(const std::string& name) :
 MultiSprite(name), 
 playerPos()
{ }

void SmartSprite::goLeft()  { 
  setVelocityX(-Gamedata::getInstance().getXmlFloat("ufo/speedX"));
}
void SmartSprite::goRight() {
  setVelocityX(Gamedata::getInstance().getXmlFloat("ufo/speedX"));
}

void SmartSprite::draw() const {
    MultiSprite::draw();
}

void SmartSprite::update(Uint32 ticks) {
  advanceFrame(ticks);
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);
  if(isPlayerDead==true){
    setPlayerPos(Vector2f(800, 205));
  }
  if(playerPos[0]-55<getX()){
    goLeft();
  }
  else{
    goRight();
  }
}

