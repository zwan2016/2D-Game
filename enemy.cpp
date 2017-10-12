#include "enemy.h"
#include "gamedata.h"
#include "renderContext.h"

void Enemy::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
    currentFrame = (currentFrame+1) % numberOfFrames;
		timeSinceLastFrame = 0;
	}
}

Enemy::Enemy( const std::string& name, const Vector2f& startpos, const Vector2f& targetpos) :
  Drawable(name, 
           startpos+Vector2f(64, 64),
           Vector2f(Gamedata::getInstance().getRandFloat(-350.0, 350.0),
                    Gamedata::getInstance().getRandFloat(-350.0, 350.0))
           ),
  frames( RenderContext::getInstance()->getFrames(name) ),
  targetPosition(),
  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
  timeSinceLastFrame( 0 ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  frameWidth(frames[0]->getWidth()),
  frameHeight(frames[0]->getHeight())
{
    float ratio = fabs(targetpos[1]-startpos[1])/fabs(targetpos[0]-startpos[0]);
    float vx = sqrtf(pow(270, 2)/(pow(ratio, 2)+1));
    float vy = ratio*vx;
    if(targetpos[0]<startpos[0]){
        vx = -vx;
    }
    if(targetpos[1]<startpos[1]){
        vy = -vy;
    }
    setVelocity(Vector2f(vx, vy));
}

Enemy::Enemy(const Enemy& s) :
  Drawable(s), 
  frames(s.frames),
  targetPosition(),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight ),
  frameWidth( s.frameWidth ),
  frameHeight( s.frameHeight)
  { }

Enemy& Enemy::operator=(const Enemy& rhs){
    
    Drawable::setName(rhs.getName());
    Drawable::setPosition(rhs.getPosition());
    Drawable::setVelocity(rhs.getVelocity());
        frames = rhs.frames;
        currentFrame = rhs.currentFrame,
        numberOfFrames = rhs.numberOfFrames,
        frameInterval = rhs.frameInterval,
        timeSinceLastFrame = rhs.timeSinceLastFrame;
        worldWidth = rhs.worldWidth;
        worldHeight = rhs.worldHeight;
        frameWidth = rhs.frameWidth;
        frameHeight = rhs.frameHeight;
    
    return *this;
    
}

void Enemy::draw() const {
  frames[currentFrame]->draw(getX(), getY());
}



void Enemy::update(Uint32 ticks) {
    advanceFrame(ticks);
    
    Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
    setPosition(getPosition() + incr);
    
    if ( getY() < 0) {
        //setVelocityY( fabs( getVelocityY() ) );
    }
    if ( getY() > worldHeight-frameHeight) {
        //setVelocityY( -fabs( getVelocityY() ) );
    }
    
    if ( getX() < 0) {
        //setVelocityX( fabs( getVelocityX() ) );
    }
    if ( getX() > worldWidth-frameWidth) {
        //setVelocityX( -fabs( getVelocityX() ) );
    }
}
