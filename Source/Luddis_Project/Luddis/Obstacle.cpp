#include "Obstacle.h"
#include "VectorMath.h"
#include "ResourceManager.h"

const Entity::RenderLayer LAYER = Entity::RenderLayer::OBSTACLES;
const int DAMAGE = 0;
//static const sf::CircleShape HITBOX_SHAPE = sf::CircleShape(15, 8);
static const float IDLE_TIME = 4;
static const float DAMAGE_TIME = 2;

static const Animation ANIMATION_IDLE = Animation("Resources/Images/Spritesheets/Steam_idle");
static const Animation ANIMATION_DAMAGE = Animation("Resources/Images/Spritesheets/Steam_active");

Obstacle::Obstacle(sf::RenderWindow* window, ObstacleType type, const sf::Vector2f& position, const float& angle, const sf::Vector2f& size) :
mIsAlive(true),
mIsActive(true),
mWindow(window),
mType(type),
//mSprite(ResourceManager::getInstance().getTexture(textureFilename)),
mHitbox(new sf::RectangleShape(size)),
mIdleHitbox(new sf::RectangleShape(sf::Vector2f(53, 35))),
mActiveHitbox(new sf::RectangleShape(sf::Vector2f(150, 300))),
mIsDamaging(false),
mDamageTime(DAMAGE_TIME),
mIdleTime(IDLE_TIME),
mAnimation(ANIMATION_IDLE),
mAngle(angle)
{
	setPosition(position);
	rotate(angle);
	//If damaging obstacle (steam)
	if (type == DAMAGE) {
		
		mIdleHitbox->setOrigin(mIdleHitbox->getLocalBounds().width / 2,/* mIdleHitbox->getLocalBounds().height*/0);
		mIdleHitbox->setPosition(getPosition());
		mIdleHitbox->setScale(getScale());
		mIdleHitbox->setRotation(getRotation());

		mActiveHitbox->setOrigin(mActiveHitbox->getLocalBounds().width / 2, /*mActiveHitbox->getLocalBounds().height*/0);
		mActiveHitbox->setPosition(getPosition());
		mActiveHitbox->setScale(getScale());
		mActiveHitbox->setRotation(getRotation());
		
		mHitbox = mIdleHitbox;

		move(((float)mIdleHitbox->getLocalBounds().height / 2.0f) * VectorMath::getNormal(sf::Vector2f(cos(mAngle), sin(mAngle))));
	}
	//else solid hitbox
	else {
		mHitbox->setOrigin(mHitbox->getLocalBounds().width / 2, mHitbox->getLocalBounds().height / 2);
		mHitbox->setPosition(getPosition());
		mHitbox->setScale(getScale());
		mHitbox->setRotation(getRotation());
	}
}

Obstacle::~Obstacle(){
	delete mHitbox;
}

void Obstacle::tick(const sf::Time& deltaTime){
	// Changing obstacle
	if (mType == DAMAGE){
		float toMove = (float)mActiveHitbox->getLocalBounds().height;
		mAnimation.getCurrAnimation().setOrigin(mAnimation.getCurrAnimation().getSprite().getLocalBounds().width / 2, mAnimation.getCurrAnimation().getSprite().getLocalBounds().height);

		// Active obstacle (damaging)
		if (mIsDamaging == true){
			mDamageTime -= float(deltaTime.asSeconds());
			if (mDamageTime <= 0){
				mIsDamaging = false;
				mDamageTime = DAMAGE_TIME;
				// Move to start idle animation
				float temp = toMove - (float)mIdleHitbox->getLocalBounds().height;
				sf::Vector2f moving = -(temp / 2.0f) * VectorMath::getNormal(sf::Vector2f(cos(mAngle), sin(mAngle)));
				move(moving);
				mHitbox = mIdleHitbox;
				mAnimation.setDefaultAnimation(ANIMATION_IDLE);
			}
		}
		// Inactive
		else if (mIsDamaging == false){
			mIdleTime -= float(deltaTime.asSeconds());
			if (mIdleTime <= 0){
				mIsDamaging = true;
				mIdleTime = IDLE_TIME;
				// Move to start damaging animation
				float temp = toMove - (float)mIdleHitbox->getLocalBounds().height;
				sf::Vector2f moving = (temp / 2.0f) * VectorMath::getNormal(sf::Vector2f(cos(mAngle), sin(mAngle)));
				move(moving);
				mHitbox = mActiveHitbox;
				mAnimation.setDefaultAnimation(ANIMATION_DAMAGE);
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
	if (mType == DAMAGE) {
		return getTransform().transformRect(mAnimation.getCurrAnimation().getSprite().getGlobalBounds());
	}
	else {
		return getNarrowHitbox()->getGlobalBounds();
	}
}

sf::Shape* Obstacle::getNarrowHitbox() const{
	return mHitbox;
}

Obstacle::Category Obstacle::getCollisionCategory(){
	if (mType == DAMAGE){
		if (mIsDamaging){
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

void Obstacle::collide(CollidableEntity *collidable, const sf::Vector2f& moveAway){
}