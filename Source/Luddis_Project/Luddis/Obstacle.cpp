#include "Obstacle.h"
#include "VectorMath.h"
#include "ResourceManager.h"
#include "Inventory.h"

const Renderer::RenderLayer LAYER = Renderer::OBSTACLES;
const int DAMAGE = 0;
//static const sf::CircleShape HITBOX_SHAPE = sf::CircleShape(15, 8);
static const float IDLE_TIME = 1;
static const float DAMAGE_TIME = 2;
static const float EMPTY_TIME = 3;

static const Animation ANIMATION1_IDLE = Animation("Resources/Images/Spritesheets/Steam_idle");
static const Animation ANIMATION1_EMPTY = Animation("Resources/Images/Spritesheets/Steam_empty");
static const Animation ANIMATION1_DAMAGE = Animation("Resources/Images/Spritesheets/Steam_active");
static const Animation ANIMATION2_IDLE = Animation("Resources/Images/Spritesheets/Lightning_idle");
static const Animation ANIMATION2_EMPTY = Animation("Resources/Images/Spritesheets/Lightning_empty");
static const Animation ANIMATION2_DAMAGE = Animation("Resources/Images/Spritesheets/Lightning_active");

static const float INVULNERABLE_TIMER = 1.0f;

Obstacle::Obstacle(sf::RenderWindow* window, ObstacleType type, const sf::Vector2f& position, const float& angle, const sf::Vector2f& size, int level) :
	mIsAlive(true),
	mIsActive(true),
	mWindow(window),
	mType(type),
	mLevel(level),
	//mSprite(ResourceManager::getInstance().getTexture(textureFilename)),
	mHitbox(new sf::RectangleShape(size)),
	mActiveHitbox(new sf::RectangleShape(sf::Vector2f(150, 300))),
	mIsDamaging(false),
	mIsEmpty(false),
	mDamageTime(DAMAGE_TIME),
	mIdleTime(IDLE_TIME),
	mEmptyTime(EMPTY_TIME),
	mAnimation(ANIMATION1_IDLE),
	mInvulnerable(INVULNERABLE_TIMER),
mAngle(angle)
{
	setPosition(position);
	rotate(angle);
	//If damaging obstacle (steam)
	if (type == DAMAGE) {

		mActiveHitbox->setOrigin(mActiveHitbox->getLocalBounds().width / 2, mActiveHitbox->getLocalBounds().height / 2);
		mActiveHitbox->setPosition(getPosition());
		mActiveHitbox->setScale(getScale());
		mActiveHitbox->setRotation(getRotation());

		mHitbox = mActiveHitbox;
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
		if (mTimeStunned <= 0) {
			float toMove = (float)mActiveHitbox->getLocalBounds().height;
		
			// Active obstacle (damaging)
			if (mIsDamaging == true) {
				mDamageTime -= float(deltaTime.asSeconds());
				if (mDamageTime <= 0) {
					mIsDamaging = false;
					mIsEmpty = true;
					mDamageTime = DAMAGE_TIME;

					if (mLevel == 1)
					mAnimation.setDefaultAnimation(ANIMATION1_EMPTY);
					else if (mLevel == 2)
					mAnimation.setDefaultAnimation(ANIMATION2_EMPTY);
				}
			}
			// Inactive
			else if (mIsDamaging == false) {
				if (mIsEmpty == true) {
					mEmptyTime -= float(deltaTime.asSeconds());
					if (mEmptyTime <= 0) {
						mIsEmpty = false;
						mEmptyTime = EMPTY_TIME;
						if (mLevel == 1)
							mAnimation.setDefaultAnimation(ANIMATION1_IDLE);
						else if (mLevel == 2)
							mAnimation.setDefaultAnimation(ANIMATION2_IDLE);
					}
				}
				else if (mIsEmpty == false){
					mIdleTime -= float(deltaTime.asSeconds());
				if (mIdleTime <= 0) {
					mIsDamaging = true;
					mIdleTime = IDLE_TIME;
					// Move to start damaging animation

					if (mLevel == 1)
						mAnimation.setDefaultAnimation(ANIMATION1_DAMAGE);
					else if (mLevel == 2)
						mAnimation.setDefaultAnimation(ANIMATION2_DAMAGE);
					}
				}
			}
			mAnimation.tick(deltaTime);
			if (mInvulnerable >= 0) {
				mInvulnerable -= deltaTime.asSeconds();
			}
		}
		else {
			mTimeStunned -= deltaTime.asSeconds();
		}

	}
	// Solid obstacle
	else {
		rotate(deltaTime.asSeconds() * 100.0f);
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

Renderer::RenderLayer Obstacle::getRenderLayer() const{
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
			return ENEMY_DAMAGE;
		}
		else{
			return Category::IGNORE;
		}
	}
	else /*if (mType == SOLID)*/{
		return Category::SOLID;
	}
}

Obstacle::Type Obstacle::getCollisionType(){
	return REC;
}

void Obstacle::collide(CollidableEntity *collidable, const sf::Vector2f& moveAway) {
	if (mType == DAMAGE) {
		if (mIsDamaging) {
			if (collidable->getCollisionCategory() == PLAYER_OBJECT) {
				if (mInvulnerable <= 0)
					Inventory::getInstance().addDust(-1);
				mInvulnerable = INVULNERABLE_TIMER;
			}
		}
	}
}

void Obstacle::stun(const sf::Time& deltatime) {
	if (mType == DAMAGE) {
		mTimeStunned = float(deltatime.asSeconds());
	}
	else {
		return;
	}
}