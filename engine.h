#include <vector>
#include <SDL.h>
//#include "ioMod.h"
#include "renderContext.h"
#include "clock.h"
#include "world.h"
#include "viewport.h"
#include "hud.h"
#include "explodingSprite.h"
//#include "collisionStrategy.h"
#include "subjectsprite.h"
#include "sound.h"


class Engine {
public:
  Engine ();
  ~Engine ();
  void play();
  void switchSprite();

private:
  const RenderContext* rc;
  const IOmod& io;
  Clock& clock;
  Hud hud;

  SDL_Renderer * const renderer;
  World world;
  World sky;
  Viewport& viewport;

  std::vector<Drawable*> sprites;
  std::vector<Drawable*> sprites1;

  int currentSprite;
  bool makeVideo;
  bool done = false;
  bool isPlayerDead = false;
  bool youWin = false;
  bool isGod = false;
  int deadTime=0;
    
  CollisionStrategy* strategy;
  SDLSound sound;

  void draw();
  void update(Uint32);

  Engine(const Engine&) = delete;
  Engine& operator=(const Engine&) = delete;
  void checkForCollisions();
};
