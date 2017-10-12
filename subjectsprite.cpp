#include <iostream>
#include <cmath>
#include "subjectsprite.h"
#include "gamedata.h"

SubjectSprite::SubjectSprite( const std::string& name) :
  MultiSprite(name),
  observers(),
  bulletName( Gamedata::getInstance().getXmlStr(name+"/bullet") ),
  bullets( bulletName ),
  minSpeed( Gamedata::getInstance().getXmlInt(bulletName+"/speedX") )
{ }

SubjectSprite::SubjectSprite(const SubjectSprite& s) :
  MultiSprite(s),
  observers(s.observers),
  bulletName(s.bulletName),
  bullets(s.bullets),
  minSpeed(s.minSpeed)
  { }

void SubjectSprite::shoot() {
    float x = getX()+getFrame()->getWidth()/2;
    float y = getY()+getFrame()->getHeight()/3;
    // I'm not adding minSpeed to y velocity:
    if(!isLeft){
        bullets.shoot( Vector2f(x, y),
                  Vector2f(minSpeed+getVelocityX(), 0));
    }
    else{
        bullets.shoot( Vector2f(x, y),
                      Vector2f(-minSpeed+getVelocityX(), 0));
    }
}

bool SubjectSprite::collidedWith(const Drawable* obj) const {
    return bullets.collidedWith( obj );
}

void SubjectSprite::draw() const {
    MultiSprite::draw();
    bullets.draw();
}

void SubjectSprite::detach( SmartSprite* o ) {
  std::list<SmartSprite*>::iterator ptr = observers.begin();
  while ( ptr != observers.end() ) {
    if ( *ptr == o ) {
      ptr = observers.erase(ptr);
      return;
    }
    ++ptr;
  }
}

void SubjectSprite::update(Uint32 ticks) { 
  MultiSprite::update(ticks);
  bullets.update(ticks);
  std::list<SmartSprite*>::iterator ptr = observers.begin();
  while ( ptr != observers.end() ) {
    (*ptr)->setPlayerPos( getPosition() );
    ++ptr;
  }
}
