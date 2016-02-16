#include "Obstacle.h"
#include "VectorMath.h"
#include "ResourceManager.h"

const Entity::RenderLayer LAYER = Entity::RenderLayer::OBSTACLES;
const int DAMAGE = 0;
static const sf::CircleShape HITBOX_SHAPE = sf::CircleShape(15, 8);
static const float IDLE_TIME = 4;
static const float ACTIVE_TIME = 2;

static const Animation ANIMATION_IDLE = Animation("Resources/Images/Spritesheets/Grafik_Luddis shot120x90treframes_s2d3v1");
static const Animation ANIMATION_ACTIVE = Animation("Resources/Images/Spritesheets/Grafik_Luddis_hit_sprite_s2d2v1");

Obstacle::Obstacle(sf::RenderWindow* window, std::string textureFilename, ObstacleType type, const sf::Vector2f& position, const float& angle) :
mIsAlive(true),
mIsActive(true),
mWindow(window),
mType(type),
mSprite(ResourceManager::getInstance().getTexture(textureFilename)),
mHitbox(new sf::CircleShape(HITBOX_SHAPE)),
// The following three are only used by changing objects
mActive(false),
mActiveTime(ACTIVE_TIME),
mIdleTime(IDLE_TIME),
mAnimation(ANIMATION_IDLE)
{
	mSprite.setOrigin((float)mSprite.getTextureRect().width / 2, (float)mSprite.getTextureRect().height / 2);
	setPosition(position);
	rotate(angle);

	mHitbox->setOrigin(mHitbox->getLocalBounds().width / 2, mHitbox->getLocalBounds().height / 2);
	mHitbox->setPosition(getPosition());
	mHitbox->setScale(getScale());
	mHitbox->setRotation(getRotation());
}

Obstacle::~Obstacle(){
	delete mHitbox;
}

void Obstacle::tick(const sf::Time& deltaTime){
	// Changing obstacle
	if (mType == DAMAGE){
		// Active obstacle (damaging)
		if (mActive == true){
			mActiveTime -= float(deltaTime.asSeconds());
			if (mActiveTime <= 0){
				mActive = false;
				mActiveTime = ACTIVE_TIME;

				mAnimation.setDefaultAnimation(ANIMATION_IDLE);
			}
		}
		// Inactive
		else if (mActive == false){
			mIdleTime -= float(deltaTime.asSeconds());
			if (mIdleTime <= 0){
				mActive = true;
				mIdleTime = IDLE_TIME;

				mAnimation.setDefaultAnimation(ANIMATION_ACTIVE);
			}
		}
		mAnimation.tick(deltaTime);
	}
	// Solid obstacle
	else {
		return;
	}
}

void Obstacle::draw(sf::RenderTarget& target, sf::RenderStates states)const{
	states.transform *= getTransform();
	// TODO?: Support for animated solid obstacle is nonexistant
	if (mType == SOLID)
		target.draw(mSprite, states);
	if (mType == DAMAGE)
		target.draw(mAnimation.getCurrAnimation(), states);
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
		if (mIsActive){
			return BG_DAMAGE;
		}
		else{
			return BG_SOLID;
		}
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
