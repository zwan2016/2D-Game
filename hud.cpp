#include "hud.h"
#include "renderContext.h"
#include "gamedata.h"
#include <sstream>

Hud::Hud() :
  renderer(RenderContext::getInstance()->getRenderer()),
  io(IOmod::getInstance()),
  on(true)
{ }

Hud::Hud( const Hud& hud ) :
renderer(hud.renderer),
io(hud.io),
on(true)
{ }

void Hud::draw(int fps, int avg, int time, int hp, bool isOver, bool isGod) const {
    if(on==false) return;
    SDL_Rect r;
    SDL_Rect r1;
    r.x = Gamedata::getInstance().getXmlInt("hud/startLoc/x");
    r.y = Gamedata::getInstance().getXmlInt("hud/startLoc/y");
    r.w = Gamedata::getInstance().getXmlInt("hud/width");
    r.h = Gamedata::getInstance().getXmlInt("hud/height");
    r1.x = Gamedata::getInstance().getXmlInt("hud1/startLoc/x");
    r1.y = Gamedata::getInstance().getXmlInt("hud1/startLoc/y");
    r1.w = Gamedata::getInstance().getXmlInt("hud1/width");
    r1.h = Gamedata::getInstance().getXmlInt("hud1/height");
    
    // First set the blend mode so that alpha blending will work;
    // the default blend mode is SDL_BLENDMODE_NONE!
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255/2 );
    
    // Render rect
    SDL_RenderFillRect( renderer, &r );
    SDL_RenderFillRect( renderer, &r1 );
    
    SDL_SetRenderDrawColor( renderer, 255, 0, 0, 255 );
    SDL_RenderDrawRect( renderer, &r );
    SDL_RenderDrawRect( renderer, &r1 );
    std::stringstream strm, strm1,strm2,strm3;
    strm << "fps: " <<fps;
    io.writeText(strm.str(), 30, 30, 255, 0, 255, 0);
    strm1 << "avg fps: "<<avg;
    io.writeText(strm1.str(), 30, 60, 255, 0, 255, 0);
    strm2 << "time: "<<time<<"s";
    io.writeText(strm2.str(), 30, 90, 255, 0, 255, 0);
    strm3 << "Boss HP: ";
    for(int i=0; i<hp; i++){
        strm3 <<"||";
    }
    if(!isOver){
        io.writeText(strm3.str(), 610, 90, 255, 0, 255, 0);
    }
    io.writeText("move left : <--", 30, 120, 255, 0, 255, 0);
    io.writeText("move right : -->", 30, 150, 255, 0, 255, 0);
    io.writeText("double jump : [space]", 30, 180, 255, 0, 255, 0);
    io.writeText("shoot : [s]", 30, 210, 255, 0, 255, 0);
    if(isGod){
        io.writeText("turn off godMode : [g]", 30, 240, 255, 0, 255, 0);
    }
    else{
        io.writeText("turn on godMode : [g]", 30, 240, 255, 0, 255, 0);
    }
    io.writeText("Created by Zicheng Wan", 320, 30, 255, 0, 255, 0);
    if(isOver){
        io.writeText("You Win! Press [r] to restart.", 285, 190, 255, 0, 255, 0);
    }


}
