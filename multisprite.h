#ifndef MULTISPRITE__H
#define MULTISPRITE__H
#include <string>
#include <vector>
#include <cmath>
#include "drawable.h"

class MultiSprite : public Drawable {
public:
  MultiSprite(const std::string&);
  MultiSprite(const MultiSprite&);

  virtual void draw() const;
  virtual void update(Uint32 ticks);
  virtual const Frame* getFrame() const { 
    return frames[currentFrame]; 
  }
  void changeFrames(const std::string&);
  void setBack(){jumpTime=4;}
  void jump(){++jumpTime;}
  void left();
  void right();
  int getJump(){return jumpTime;}
  inline float getScale()const{ return scale; }
  void  setScale(float s){ scale = s; }

protected:
  std::vector<Frame *> frames;
  bool isLeft = false;

  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int worldWidth;
  int worldHeight;
  int frameWidth;
  int frameHeight;
  int jumpTime;
  float scale;

  void advanceFrame(Uint32 ticks);
};
#endif
