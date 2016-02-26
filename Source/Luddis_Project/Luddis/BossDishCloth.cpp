#include "BossDishCloth.h"
#include "Projectile.h"
#include "PowerUpItem.h"
#include "EntityManager.h"
#include "CollisionManager.h"
#include "VectorMath.h"
#include <SFML\Graphics\Shape.hpp>
#include "ResourceManager.h"

//Different states depending on how damaged the boss is.
//State 1
static const std::string ANIMATION_IDLE = ("Resources/Images/Spritesheets/RagIdle_P1");
static const std::string SHOOTING_ANIMATION = ("Resources/Images/Spritesheets/RagAttack_P1");
//State 2
static const std::string ANIMATION_IDLE_2 = ("Resources/Images/Spritesheets/RagIdle_P2");
static const std::string SHOOTING_ANIMATION_2 = ("Resources/Images/Spritesheets/RagAttack_P2");
//State 3
static const std::string ANIMATION_IDLE_3 = ("Resources/Images/Spritesheets/RagIdle_P3");
static const std::string SHOOTING_ANIMATION_3 = ("Resources/Images/Spritesheets/RagAttack_P3");
//State 4
static const std::string ANIMATION_IDLE_4 = ("Resources/Images/Spritesheets/RagIdle_P4");
static const std::string SHOOTING_ANIMATION_4 = ("Resources/Images/Spritesheets/RagAttack_P4");

static const std::string PROJECTILE_FILEPATH = "Resources/Images/Rag_projectile.png";

static const std::string POWERUP1_FILEPATH = "Resources/Images/Rag_projectile.png";

static const int MAX_LIFE = 100;
static const float ATTACK_INTERVAL = 3.5f;
static const float PROJECTILE_LIFETIME = 2.5f;
static const float PROJECTILE_SPEED = 300;
static const sf::RectangleShape HITBOX_SHAPE = sf::RectangleShape(sf::Vector2f(225, 225));

void loadResources(){
	ResourceManager::getInstance().loadTexture(ANIMATION_IDLE + ".png");
	ResourceManager::getInstance().loadTexture(SHOOTING_ANIMATION + ".png");
	ResourceManager::getInstance().loadTexture(ANIMATION_IDLE_2 + ".png");
	ResourceManager::getInstance().loadTexture(SHOOTING_ANIMATION_2 + ".png");
	ResourceManager::getInstance().loadTexture(ANIMATION_IDLE_3 + ".png");
	ResourceManager::getInstance().loadTexture(SHOOTING_ANIMATION_3 + ".png");
	ResourceManager::getInstance().loadTexture(ANIMATION_IDLE_4 + ".png");
	ResourceManager::getInstance().loadTexture(SHOOTING_ANIMATION_4 + ".png");
}

BossDishCloth::BossDishCloth(sf::RenderWindow* window, const sf::Vector2f& position,
	const float& activation, Transformable* aTarget, EntityManager* entityManager) :
mIsAlive(true),
mIsActive(false),
mWindow(window),
mEntityManager(entityManager),
mShooting(false),
mActivate(activation),
mLife(MAX_LIFE),
mAttackInterval(ATTACK_INTERVAL),
mDirection(0, 1.0f),
mAnimation(Animation(ANIMATION_IDLE)),
mHitbox(new sf::RectangleShape(HITBOX_SHAPE)),
mTarget(aTarget)
{
	loadResources();
	setPosition(position);
	mHitbox->setOrigin(mHitbox->getLocalBounds().width / 2, mHitbox->getLocalBounds().height / 2);

}

BossDishCloth::~BossDishCloth(){
	delete mHitbox;
}

void BossDishCloth::tick(const sf::Time& deltaTime){
	if (mTarget->getPosition().x >= mActivate){
		mIsActive = true;
	}
	if (!mIsActive) return;
	if (mLife <= 0){
		/*PowerUpItem* pow1 = new PowerUpItem(POWERUP1_FILEPATH, getPosition());
		mEntityManager->addEntity(pow1);
		CollisionManager::getInstance().addCollidable(pow1);*/
		mIsAlive = false;
	}
	mAttackInterval -= deltaTime.asSeconds();
	updateMovement(deltaTime);
	mAnimation.tick(deltaTime);
	if (mAttackInterval <= 0){
		attack();
		mAttackInterval = ATTACK_INTERVAL;
	}
}

void BossDishCloth::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	if (!mIsActive) return;
	states.transform *= getTransform();
	target.draw(mAnimation.getCurrAnimation(), states);
}

bool BossDishCloth::isAlive() const{
	return mIsAlive;
}

bool BossDishCloth::isActive() const{
	return mIsActive;
}

void BossDishCloth::setActive(const bool& active){
	mIsActive = active;
}

BossDishCloth::RenderLayer BossDishCloth::getRenderLayer() const {
	return PLAYER;
}

void BossDishCloth::updateMovement(const sf::Time& deltaTime){
	int spriteHeight = mAnimation.getCurrAnimation().getSprite().getTextureRect().height;
	if (getPosition().y < 0 + spriteHeight
		|| getPosition().y> mWindow->getView().getSize().y - spriteHeight) {
		mDirection = mDirection*-1.0f;
	}
	if (mAttackInterval > 0.6f &&
		mAttackInterval <= ATTACK_INTERVAL - 0.4f) {
		mShooting = false;
		move(mDirection);
	}
	else{
		if (!mShooting){
			mShooting = true;
			//For different states of damage (changes happen next shot if hit when shooting)
			//State 1
			if (mLife < 26) {
				//int  frame = mAnimation.getCurrAnimation().getCurrentFrame();
				mAnimation.replaceAnimation(SHOOTING_ANIMATION_4);
				//mAnimation.getCurrAnimation().setFrame(frame);
			}
			//State 2
			if (25 < mLife && mLife < 51) {
				//int  frame = mAnimation.getCurrAnimation().getCurrentFrame();
				mAnimation.replaceAnimation(SHOOTING_ANIMATION_3);
				//mAnimation.getCurrAnimation().setFrame(frame);
			}
			//State 3
			if (50 < mLife && mLife < 76) {
				//int  frame = mAnimation.getCurrAnimation().getCurrentFrame();
				mAnimation.replaceAnimation(SHOOTING_ANIMATION_2);
				//mAnimation.getCurrAnimation().setFrame(frame);
			}
			//State 4
			if (75 < mLife) {
				//int  frame = mAnimation.getCurrAnimation().getCurrentFrame();
				mAnimation.replaceAnimation(SHOOTING_ANIMATION);
				//mAnimation.getCurrAnimation().setFrame(frame);
			}
			//mAnimation.replaceAnimation(SHOOTING_ANIMATION);
		}
	}
}

void BossDishCloth::attack() {
	sf::Vector2f vec(-1, 0);
	int max = 8;
	
	if (mLife < 50){
		max = 2;
	}
	for (int i = 0; i < max; i++)
	{

		vec = VectorMath::rotateVector(vec, 360 / (float)max);
		Projectile* proj = new Projectile(PROJECTILE_FILEPATH, vec*PROJECTILE_SPEED, getPosition() + vec*PROJECTILE_SPEED / 3.0f, PROJECTILE_LIFETIME, ENEMY_STUN);
		mEntityManager->addEntity(proj);
		CollisionManager::getInstance().addCollidable(proj);
	}
	
	Projectile* proj = new Projectile(PROJECTILE_FILEPATH, vec*PROJECTILE_SPEED, sf::Vector2f(getPosition().x, 590) + vec*PROJECTILE_SPEED / 3.0f, PROJECTILE_LIFETIME, ENEMY_STUN);
	mEntityManager->addEntity(proj);
	CollisionManager::getInstance().addCollidable(proj);
}

BossDishCloth::Category BossDishCloth::getCollisionCategory() {
	return ENEMY;
}

BossDishCloth::Type BossDishCloth::getCollisionType() {
	return REC;
}

void BossDishCloth::collide(CollidableEntity* collidable, const sf::Vector2f& moveAway){
	if (collidable->getCollisionCategory() == HAIR){
		if (!mShooting){
		mLife -= 15;
		// For different states of damages
		//State 1
		if (mLife < 26){
			int  frame = mAnimation.getCurrAnimation().getCurrentFrame();
			mAnimation.setDefaultAnimation(ANIMATION_IDLE_4);
			mAnimation.getCurrAnimation().setFrame(frame);
		}
		//State 2
		if (25 < mLife && mLife < 51){
			int  frame = mAnimation.getCurrAnimation().getCurrentFrame();
			mAnimation.setDefaultAnimation(ANIMATION_IDLE_3);
			mAnimation.getCurrAnimation().setFrame(frame);
		}
		//State 3
		if (50 < mLife && mLife < 76){
			int  frame = mAnimation.getCurrAnimation().getCurrentFrame();
			mAnimation.setDefaultAnimation(ANIMATION_IDLE_2);
			mAnimation.getCurrAnimation().setFrame(frame);
		}
		//State 4
		if (75 < mLife){
			int  frame = mAnimation.getCurrAnimation().getCurrentFrame();
			mAnimation.setDefaultAnimation(ANIMATION_IDLE);
			mAnimation.getCurrAnimation().setFrame(frame);
		}
		}
		if (mShooting){
			//State 1
			if (mLife < 26){
				int  frame = mAnimation.getCurrAnimation().getCurrentFrame();
				mAnimation.replaceAnimation(SHOOTING_ANIMATION_4);
				mAnimation.getCurrAnimation().setFrame(frame);
			}
			//State 2
			if (25 < mLife && mLife < 51){
				int  frame = mAnimation.getCurrAnimation().getCurrentFrame();
				mAnimation.replaceAnimation(SHOOTING_ANIMATION_3);
				mAnimation.getCurrAnimation().setFrame(frame);
			}
			//State 3
			if (50 < mLife && mLife < 76){
				int  frame = mAnimation.getCurrAnimation().getCurrentFrame();
				mAnimation.replaceAnimation(SHOOTING_ANIMATION_2);
				mAnimation.getCurrAnimation().setFrame(frame);
			}
			//State 4
			if (75 < mLife){
				int  frame = mAnimation.getCurrAnimation().getCurrentFrame();
				mAnimation.replaceAnimation(SHOOTING_ANIMATION);
				mAnimation.getCurrAnimation().setFrame(frame);
			}
		}
	}
}

sf::FloatRect BossDishCloth::getHitBox(){
	return getTransform().transformRect(mAnimation.getCurrAnimation().getSprite().getGlobalBounds());
}
sf::Shape* BossDishCloth::getNarrowHitbox() const{
	mHitbox->setPosition(getPosition());
	mHitbox->setScale(getScale());
	mHitbox->setRotation(getRotation());
	return mHitbox;
}

void BossDishCloth::stun(const sf::Time& deltatime) {

}