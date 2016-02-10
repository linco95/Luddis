#include "Obstacle.h"
#include "ResourceManager.h"

const Entity::RenderLayer LAYER = Entity::RenderLayer::OBSTACLES;
const int DAMAGE = 0;
static const sf::CircleShape HITBOX_SHAPE = sf::CircleShape(15, 8);
static const float IDLE_TIME = 4;
static const float ACTIVE_TIME = 2;

Obstacle::Obstacle(std::string textureFilename, sf::RenderWindow* window, ObstacleType type, sf::Vector2f direction, const sf::Vector2f& position) :
mIsAlive(true),
mIsActive(true),
mWindow(window),
mSprite(ResourceManager::getInstance().getTexture(textureFilename)),
mType(type),
mHitbox(new sf::CircleShape(HITBOX_SHAPE)),
// The following three are only used by changing objects
mActive(true),
mActiveTime(ACTIVE_TIME),
mIdleTime(IDLE_TIME)
{
	mSprite.setOrigin((float)mSprite.getTextureRect().width / 2, (float)mSprite.getTextureRect().height / 2);
	setPosition(position);
	
	mHitbox->setOrigin(mHitbox->getLocalBounds().width / 2, mHitbox->getLocalBounds().height / 2);
	mHitbox->setPosition(getPosition());
	mHitbox->setScale(getScale());
	mHitbox->setRotation(getRotation());
}

Obstacle::~Obstacle(){
	delete mHitbox;
}

void Obstacle::tick(const sf::Time& deltaTime){
	if (mType == BG_DAMAGE){
		if (mActive == true){
			mActiveTime -= float(deltaTime.asSeconds());
			if (mActiveTime <= 0){
				mActive = false;
				mActiveTime = ACTIVE_TIME;
			}
		}
		else if (mActive == false){
			mIdleTime -= float(deltaTime.asSeconds());
			if (mIdleTime <= 0){
				mActive = false;
				mIdleTime = IDLE_TIME;
			}
		}
	}
	else {
		return;
	}
}

void Obstacle::draw(sf::RenderTarget& target, sf::RenderStates states)const{
	states.transform *= getTransform();
	target.draw(mSprite, states);
}

bool Obstacle::isAlive() const{
	return mIsAlive;
}

bool Obstacle::isActive() const{
	return mIsActive;
}

void Obstacle::setActive(const bool& active){
	mIsActive = active;
}

Entity::RenderLayer Obstacle::getRenderLayer() const{
	return LAYER;
}

sf::FloatRect Obstacle::getHitBox(){
	return getTransform().transformRect(mSprite.getGlobalBounds());
}

sf::Shape* Obstacle::getNarrowHitbox() const{
	return mHitbox;
}

Obstacle::Category Obstacle::getCollisionCategory(){
	if (mType == DAMAGE){
		return BG_DAMAGE;
	}
	else /*if (mType == SOLID)*/{
		return BG_SOLID;
	}
}

Obstacle::Type Obstacle::getCollisionType(){
	return REC;
}

void Obstacle::collide(CollidableEntity *collidable){
}
