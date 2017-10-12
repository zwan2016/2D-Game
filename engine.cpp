#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>
#include <random>
#include <iomanip>
#include "enemy.h"
#include "sprite.h"
#include "gamedata.h"
#include "engine.h"
#include "frameGenerator.h"
//#include "subjectsprite.h"
#include "smartSprite.h"


Engine::~Engine() { 
  std::cout << "Terminating program" << std::endl;
    for(auto& sprite : sprites){
        delete sprite;
    }
    for(auto& sprite : sprites1){
        delete sprite;
    }
    delete strategy;
    /*
  std::vector<Drawable*>::iterator it = sprites.begin();
  while ( it != sprites.end() ) {
    delete *it;
    ++it;
  }*/
}

Engine::Engine() :
  rc( RenderContext::getInstance() ),
  io( IOmod::getInstance() ),
  clock( Clock::getInstance() ),
  hud(),
  renderer( rc->getRenderer() ),
  world("back", Gamedata::getInstance().getXmlInt("back/factor") ),
  sky("sky", Gamedata::getInstance().getXmlInt("sky/factor") ),
  viewport( Viewport::getInstance() ),
  sprites(),
  sprites1(),
  currentSprite(-1),
  makeVideo( true ),
  strategy( new PerPixelCollisionStrategy ),
  sound()
{
    constexpr float u = 1.0f; //Mean size
    constexpr float d = 0.5f; //Std deviation
    
    std::random_device rd;
    std::mt19937 mt(rd());
    std::normal_distribution<float> dist(u,d);
    

    sprites.push_back( new SubjectSprite("zombie") );
    sprites.push_back( new SmartSprite("ufo") );
    static_cast<SubjectSprite*>(sprites[0])->attach(static_cast<SmartSprite*>(sprites[1]));
    
    for ( unsigned int i = 0; i < 15; ++i ) {
        auto* s = new MultiSprite("ufo");
        s->setScale(Gamedata::getInstance().getRandFloat(0.35, 0.6));
        s->setBack();
        sprites.push_back(s);
    }
    //sprites1.push_back( new Enemy("enemy", sprites[1]->getPosition(), sprites[0]->getPosition()));
    sprites[1]->setVelocityX(0.0);
    sprites[1]->setVelocityY(0.0);
    if(MultiSprite* d = dynamic_cast<MultiSprite*>(sprites[1]))
    {
        d->setScale(1.5);
    }
    sprites[0]->setX(120.0);
    sprites[0]->setY(461.33);
    sprites[1]->setX(800.0);
    sprites[1]->setY(205.0);
    
    switchSprite();
    std::cout << "Loading complete" << std::endl;
}

void Engine::draw() {
  SDL_RenderClear(renderer);
  sky.draw();
  for(unsigned int i=2;i<17;++i){
    sprites[i]->draw();
  }
  world.draw();
  std::stringstream strm2;

    //Vector2f position = sprites[0]->getPosition();
  for(unsigned int i=0;i<2;++i){
    sprites[i]->draw();
  }
    
  for(unsigned long i = 0; i<sprites1.size(); ++i){
    sprites1[i]->draw();
  }

  viewport.draw();
  int hp=0;
  if(SmartSprite* u = dynamic_cast<SmartSprite*>(sprites[1]))
  {
    hp = u->getHP();
  }
  hud.draw(clock.getFps(), clock.getAvgFps(), clock.getSeconds(), hp, youWin, isGod);
  SDL_RenderPresent(renderer);
}

void Engine::checkForCollisions() {
    std::vector<Drawable*>::iterator it = sprites1.begin();
    Drawable* player = sprites[0];
    while ( it != sprites1.end() ) {
        //std::cout << strategy->execute(*player, **it) << std::endl;
        if ( strategy->execute(*player, **it) ) {
            std::cout << "collision: " << std::endl;
            //++collisions;
            Sprite* explode = new Sprite("zombie");
            explode->setPosition(sprites[0]->getPosition());
            explode->setFrame(sprites[0]->getFrame());
            Drawable* boom =
            new ExplodingSprite(*explode);
            delete sprites[0];
            sprites[0] = boom;
            isPlayerDead = true;
            sound[5];
            deadTime = clock.getFrames();
            Viewport::getInstance().setObjectToTrack(sprites[0]);
            break;
        }
        ++it;
    }
}

void Engine::update(Uint32 ticks) {
  for(auto* s : sprites) s->update(ticks);
  for(auto* s : sprites1) s->update(ticks);
  sky.update();
  world.update();
  viewport.update(); // always update viewport last
}

void Engine::switchSprite(){
  currentSprite = ++currentSprite % sprites.size();
  Viewport::getInstance().setObjectToTrack(sprites[currentSprite]);
}

void Engine::play() {
  SDL_Event event;
  const Uint8* keystate;
  Uint32 ticks = clock.getElapsedTicks();
  FrameGenerator frameGen;

  while ( !done ) {
    while ( SDL_PollEvent(&event) ) {
      keystate = SDL_GetKeyboardState(NULL);
      if (event.type ==  SDL_QUIT) { done = true; break; }
          sprites[0]->setVelocityX(0.0);
          if ( keystate[SDL_SCANCODE_LEFT] ) {
              if(isPlayerDead==false){
                  static_cast<SubjectSprite*>(sprites[0])->left();
              }
          }
          if ( keystate[SDL_SCANCODE_RIGHT] ) {
              if(isPlayerDead==false){
                  static_cast<SubjectSprite*>(sprites[0])->right();
              }
          }
          if ( keystate[SDL_SCANCODE_SPACE] ) {
              if(isPlayerDead==false){
                  MultiSprite* m = (MultiSprite*)sprites[0];
                  if(m->getJump()<=2){
                      sprites[0]->setVelocityY(-Gamedata::getInstance().getXmlFloat("zombie/speedY"));
                      m->jump();
                      sound[4];
                  }
              }
          }
        if(event.type == SDL_KEYDOWN) {
        if (keystate[SDL_SCANCODE_ESCAPE] || keystate[SDL_SCANCODE_Q]) {
          done = true;
          break;
        }
        if ( keystate[SDL_SCANCODE_R] ) {
            sprites.clear();
            sprites1.clear();
            sprites.push_back( new SubjectSprite("zombie") );
            sprites.push_back( new SmartSprite("ufo") );
            static_cast<SubjectSprite*>(sprites[0])->attach(static_cast<SmartSprite*>(sprites[1]));
            
            for ( unsigned int i = 0; i < 15; ++i ) {
                auto* s = new MultiSprite("ufo");
                s->setScale(Gamedata::getInstance().getRandFloat(0.35, 0.6));
                s->setBack();
                sprites.push_back(s);
            }
            //sprites1.push_back( new Enemy("enemy", sprites[1]->getPosition(), sprites[0]->getPosition()));
            sprites[1]->setVelocityX(0.0);
            sprites[1]->setVelocityY(0.0);
            if(MultiSprite* d = dynamic_cast<MultiSprite*>(sprites[1]))
            {
                d->setScale(1.5);
            }
            sprites[0]->setX(120.0);
            sprites[0]->setY(461.33);
            sprites[1]->setX(800.0);
            sprites[1]->setY(205.0);
            isPlayerDead=false;
            youWin=false;
            
            Viewport::getInstance().setObjectToTrack(sprites[0]);
        }
        if ( keystate[SDL_SCANCODE_P] ) {
          if ( clock.isPaused() ) clock.unpause();
          else clock.pause();
        }
        if ( keystate[SDL_SCANCODE_S] ) {
          //clock.toggleSloMo();
            if(isPlayerDead==false){
                static_cast<SubjectSprite*>(sprites[0])->shoot();
                sound[3];
            }
        }
        if ( keystate[SDL_SCANCODE_G] ) {
            if(isGod==false){
                isGod=true;
            }
            else{
                isGod=false;
            }
            //switchSprite();
        }
        if ( keystate[SDL_SCANCODE_F1] ) {
            hud.hide();
          //switchSprite();
        }
        if (keystate[SDL_SCANCODE_F4] && !makeVideo) {
          std::cout << "Initiating frame capture" << std::endl;
          makeVideo = true;
        }
        else if (keystate[SDL_SCANCODE_F4] && makeVideo) {
          std::cout << "Terminating frame capture" << std::endl;
          makeVideo = false;
        }
      }
    }
    ticks = clock.getElapsedTicks();
    if ( ticks > 0 ) {
        if(clock.getFrames()%10 == 0 && isPlayerDead == false && (clock.getFrames()/100)%2 == 0 && youWin==false){
            sound[0];
            sprites1.push_back( new Enemy("enemy", sprites[1]->getPosition(), sprites[0]->getPosition()) );
        }
        if(isPlayerDead == true && clock.getFrames()-deadTime>80){
            sprites1.clear();
            sound[6];
            SubjectSprite* zombie = new SubjectSprite("zombie");
            zombie->setPosition(sprites[0]->getPosition());
            delete sprites[0];
            sprites[0] = zombie;
            isPlayerDead = false;
            static_cast<SubjectSprite*>(sprites[0])->attach(static_cast<SmartSprite*>(sprites[1]));
            Viewport::getInstance().setObjectToTrack(sprites[0]);
        }
        clock.incrFrame();
        clock.addFps(clock.getFps());
        draw();
        update(ticks);
        if(isPlayerDead==false){
          if(isGod==false){
            checkForCollisions();
          }
          if(isPlayerDead==false){
            if(static_cast<SubjectSprite*>(sprites[0])->collidedWith(sprites[1])){
                static_cast<SmartSprite*>(sprites[1])->damage();
                if(static_cast<SmartSprite*>(sprites[1])->isDead()){
                    youWin=true;
                    sound[1];
                    static_cast<SubjectSprite*>(sprites[0])->detach(static_cast<SmartSprite*>(sprites[1]));
                    Sprite* explode1 = new Sprite("ufo");
                    explode1->setPosition(sprites[1]->getPosition());
                    explode1->setFrame(sprites[1]->getFrame());
                    Drawable* boom1 =new ExplodingSprite(*explode1);
                    delete sprites[1];
                    sprites[1] = boom1;
                }
                else{
                    sound[2];
                }
            }
          }
          if(youWin==false){
              if(SmartSprite* u = dynamic_cast<SmartSprite*>(sprites[1]))
              {
                  u->setPlayerDead(false);
              }
          }
        }
        else if(youWin==false){
            if(SmartSprite* u = dynamic_cast<SmartSprite*>(sprites[1]))
            {
              u->setPlayerDead(true);
              //std::cout << "??" << std::endl;
            }
        }
        if ( makeVideo ) {
          frameGen.makeFrame();
        }
     }
   }
}
