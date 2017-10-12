#ifndef HUD__H
#define HUD__H

#include <iostream>
#include <string>
#include <SDL.h>
#include "ioMod.h"

class Hud {
public:
  Hud();
    Hud(const Hud&);

  void draw(int fps, int avg, int time, int hp, bool isOver, bool isGod) const;
    void hide(){if(on==true) on=false;
        else if(on==false) on=true;}

private:
  SDL_Renderer * renderer;
       Hud& operator=(const Hud&)=delete;
    const IOmod& io;
    bool on;
};

#endif
