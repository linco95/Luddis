#include "Obstacle.h"
#include "VectorMath.h"
#include "ResourceManager.h"

const Entity::RenderLayer LAYER = Entity::RenderLayer::OBSTACLES;
const int DAMAGE = 0;
static const sf::CircleShape HITBOX_SHAPE = sf::CircleShape(15, 8);
static const float IDLE_TIME = 4;
static const float DAMAGE_TIME = 2;

static const Animation ANIMATION_IDLE = Animation("Resources/Images/Spritesheets/Grafik_steam_AnimationIdleSprite");
static const Animation ANIMATION_DAMAGE = Animation("Resources/Images/Spritesheets/Grafik_steam_AnimationSprite");

Obstacle::Obstacle(sf::RenderWindow* window, std::string textureFilename, ObstacleType type, const sf::Vector2f& position, const float& angle) :
mIsAlive(true),
mIsActive(true),
mWindow(window),
mType(type),
mSprite(ResourceManager::getInstance().getTexture(textureFilename)),
mHitbox(new sf::CircleShape(HITBOX_SHAPE)),
mIsDamaging(false),
mDamageTime(DAMAGE_TIME),
mIdleTime(IDLE_TIME),
mAnimation(ANIMATION_IDLE),
mAngle(angle)
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
		if (mIsDamaging == true){
			mDamageTime -= float(deltaTime.asSeconds());
			if (mDamageTime <= 0){
				mIsDamaging = false;
				mDamageTime = DAMAGE_TIME;
				// Move to start idle animation
				sf::Vector2f moving = -((float)mSprite.getTextureRect().height / 3.7f) * VectorMath::getNormal(sf::Vector2f(cos(mAngle), sin(mAngle)));
				move(moving);

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
				sf::Vector2f moving = ((float)mSprite.getTextureRect().height / 3.7f) * VectorMath::getNormal(sf::Vector2f(cos(mAngle), sin(mAngle)));
				move(moving);

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
	return getTransform().transformRect(mSprite.getGlobalBounds());
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

void Obstacle::collide(CollidableEntity *collidable){
}