#ifndef SUBJECTSPRITE__H
#define SUBJECTSPRITE__H
#include <string>
#include <iostream>
#include <vector>
#include <list>
#include "smartSprite.h"
#include "multisprite.h"
#include "bulletPool.h"

class SubjectSprite : public MultiSprite {
public:
  SubjectSprite(const std::string&);
  SubjectSprite(const SubjectSprite&);

  void update(Uint32 ticks);
  void attach( SmartSprite* o ) { observers.push_back(o); } 
  void detach( SmartSprite* o );
  virtual void draw() const;
  void shoot();
  virtual bool collidedWith(const Drawable*) const;

    
private:
  std::list<SmartSprite*> observers;
  std::string bulletName;
  BulletPool bullets;
  float minSpeed;
  SubjectSprite& operator=(const SubjectSprite&);
};

#endif
