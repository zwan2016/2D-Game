#ifndef ENEMY__H
#define ENEMY__H
#include <string>
#include <vector>
#include <cmath>
#include "drawable.h"

class Enemy : public Drawable {
public:
  Enemy(const std::string&, const Vector2f& startpos, const Vector2f& targetpos);
  Enemy(const Enemy&);
  Enemy& operator=(const Enemy&);

  virtual void draw() const;
  virtual void update(Uint32 ticks);
  virtual const Frame* getFrame() const { 
    return frames[currentFrame]; 
  }
  inline float getScale()const{ return scale; }

protected:
  std::vector<Frame *> frames;
  Vector2f targetPosition;

  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int worldWidth;
  int worldHeight;
  int frameWidth;
  int frameHeight;
  float scale = 1;

  void advanceFrame(Uint32 ticks);
};
#endif
