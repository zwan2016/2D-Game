#include "multisprite.h"
#include "gamedata.h"
#include "renderContext.h"

void MultiSprite::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
        currentFrame = (currentFrame+1) % numberOfFrames;
		timeSinceLastFrame = 0;
	}
}

MultiSprite::MultiSprite( const std::string& name) :
  Drawable(name, 
           Vector2f(Gamedata::getInstance().getRandFloat(0.0, 2000.0),
                    Gamedata::getInstance().getRandFloat(0.0, 570.0)),
           Vector2f(Gamedata::getInstance().getRandFloat(-20.0, 20.0),
                    Gamedata::getInstance().getRandFloat(-20.0, 20.0))
           ),
  frames( RenderContext::getInstance()->getFrames(name) ),

  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
  timeSinceLastFrame( 0 ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  frameWidth(frames[0]->getWidth()),
  frameHeight(frames[0]->getHeight()),
  jumpTime(0),
  scale(1)
{ }

MultiSprite::MultiSprite(const MultiSprite& s) :
  Drawable(s), 
  frames(s.frames),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight ),
  frameWidth( s.frameWidth ),
  frameHeight( s.frameHeight),
  jumpTime(0),
  scale(1)
  { }

void MultiSprite::draw() const { 
  frames[currentFrame]->draw(getX(), getY(), scale);
}

void MultiSprite::changeFrames(const std::string& name){
    frames.clear();
    frames = RenderContext::getInstance()->getFrames(name);
}

void MultiSprite::left(){
    isLeft = true;
    changeFrames("zombieLeft");
    setVelocityX(-Gamedata::getInstance().getXmlFloat("zombie/speedX"));
}

void MultiSprite::right(){
    isLeft = false;
    changeFrames("zombie");
    setVelocityX(Gamedata::getInstance().getXmlFloat("zombie/speedX"));
}

void MultiSprite::update(Uint32 ticks) { 
  advanceFrame(ticks);
    if(jumpTime==4){
        Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
        setPosition(getPosition() + incr);
        
        if ( getY() < 0) {
            setVelocityY( fabs( getVelocityY() ) );
        }
        if ( getY() > worldHeight-frameHeight) {
            setVelocityY( -fabs( getVelocityY() ) );
        }
        
        if ( getX() < 0) {
            setVelocityX( fabs( getVelocityX() ) );
        }
        if ( getX() > worldWidth-frameWidth) {
            setVelocityX( -fabs( getVelocityX() ) );
        }  

    }
    else{
        float g = Gamedata::getInstance().getXmlFloat("zombie/gravity");
        float incrY = g * ticks * 0.001;
        if(getVelocityY() !=0){
            setVelocityY(getVelocityY()+incrY);
        }

        Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
        setPosition(getPosition() + incr);

        if ( getY() < 0) {
            setVelocityY( fabs( getVelocityY() ) );
        }
        if ( getY() > worldHeight-frameHeight) {
            jumpTime=0;
            setVelocityY( 0.0 );
            setY(worldHeight-frameHeight-20.0);
        }

        if ( getX() < 0) {
            setVelocityX( 0.0 );
            setX(0.0);
        }
        if ( getX() > worldWidth-frameWidth) {
            setVelocityX( 0.0 );
            setX(worldWidth-frameWidth);
        }
    }

}
