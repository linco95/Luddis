#include "BossDishCloth.h"
#include "Projectile.h"
#include "EntityManager.h"
#include "CollisionManager.h"
#include "VectorMath.h"
#include <SFML\Graphics\Shape.hpp>

//Different states depending on how damaged the boss is.
//State 1
static const Animation ANIMATION_IDLE = Animation("Resources/Images/Spritesheets/Grafik_TrasanFas1_S2D3V2");
static const Animation SHOOTING_ANIMATION = Animation("Resources/Images/Spritesheets/Grafik_TrasanAttackFas1_S2D4V3");
//State 2
static const Animation ANIMATION_IDLE_2 = Animation("Resources/Images/Spritesheets/Grafik_TrasanFas2_S2D3V2");
static const Animation SHOOTING_ANIMATION_2 = Animation("Resources/Images/Spritesheets/Grafik_TrasanAttackFas2_S2D4V3");
//State 3
static const Animation ANIMATION_IDLE_3 = Animation("Resources/Images/Spritesheets/Grafik_TrasanFas3_S2D3V1");
static const Animation SHOOTING_ANIMATION_3 = Animation("Resources/Images/Spritesheets/Grafik_TrasanAttackFas3_S2D4V1");
//State 4
static const Animation ANIMATION_IDLE_4 = Animation("Resources/Images/Spritesheets/Grafik_TrasanFas4_S2D3V2");
static const Animation SHOOTING_ANIMATION_4 = Animation("Resources/Images/Spritesheets/Grafik_TrasanAttackFas4_S2D4V1");

static const std::string PROJECTILE_FILEPATH = "Resources/Images/BAWS1projectile.png";
static const int MAX_LIFE = 100;
static const float ATTACK_INTERVAL = 3.5f;
static const float PROJECTILE_LIFETIME = 2.5f;
static const float PROJECTILE_SPEED = 300;
static const sf::CircleShape HITBOX_SHAPE = sf::CircleShape(15, 8);

BossDishCloth::BossDishCloth(sf::RenderWindow* window) :
mIsAlive(true),
mIsActive(true),
mWindow(window),
mShooting(false),
mLife(MAX_LIFE),
mAttackInterval(ATTACK_INTERVAL),
mDirection(0, 1.0f),
mAnimation(Animation(ANIMATION_IDLE)),
mHitbox(new sf::CircleShape(HITBOX_SHAPE))
{
	setPosition(3000, 500);
}

BossDishCloth::~BossDishCloth(){
	delete mHitbox;
}

void BossDishCloth::tick(const sf::Time& deltaTime){
	if (mLife <= 0){
		mIsAlive = false;
	}
	mAttackInterval -= deltaTime.asSeconds();
	updateMovement(deltaTime);
	mAnimation.tick(deltaTime);
	//setScale((float)mLife / 100, (float)mLife / 100);
	if (mAttackInterval <= 0){
		attack();
		mAttackInterval = ATTACK_INTERVAL;
	}
}

void BossDishCloth::draw(sf::RenderTarget& target, sf::RenderStates states) const{
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
		|| getPosition().y> mWindow->getView().getSize().y - spriteHeight)
	{
		mDirection = mDirection*-1.0f;
	}
	if (mAttackInterval > 0.6f &&
		mAttackInterval <= ATTACK_INTERVAL - 0.4f)
	{
		mShooting = false;
		move(mDirection);
	}
	else{
		if (!mShooting){
			//Seem to cause a few frames of lagg
			mShooting = true;
			//For different states of damage (changes happen next shot if hit when shooting)
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
			
			//mAnimation.replaceAnimation(SHOOTING_ANIMATION);
		}
	}
}

void BossDishCloth::attack(){
	sf::Vector2f vec(0, 1);
	int max = 3;
	
	if(mLife < 50){
		max = 2;
	}
	for (int i = 0; i < max; i++)
	{

		vec = VectorMath::rotateVector(vec, 135/(float)max);
		Projectile* proj = new Projectile(PROJECTILE_FILEPATH, vec*PROJECTILE_SPEED, getPosition()+vec*PROJECTILE_SPEED/3.0f, PROJECTILE_LIFETIME, ENEMY_STUN);
		EntityManager::getInstance().addEntity(proj);
		CollisionManager::getInstance().addCollidable(proj);
	}
}

BossDishCloth::Category BossDishCloth::getCollisionCategory(){
	return ENEMY;
}

BossDishCloth::Type BossDishCloth::getCollisionType(){
	return REC;
}

void BossDishCloth::collide(CollidableEntity* collidable){
	if (collidable->getCollisionCategory() == HAIR){
		mLife -= 15;
		// For different states of damages (causes animation to "start over")
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
}

sf::FloatRect BossDishCloth::getHitBox(){
	return getTransform().transformRect(mAnimation.getCurrAnimation().getSprite().getGlobalBounds());
}
sf::Shape* BossDishCloth::getNarrowHitbox() const{
	return mHitbox;
}