#include "BossDishCloth.h"
#include "Projectile.h"
#include "EntityManager.h"
#include "CollisionManager.h"
#include "VectorMath.h"
#include <SFML\Graphics\Shape.hpp>

static const std::string ANIMATION_FILEPATH = "Resources/Images/spritesheets/Grafik_Trasan300x300_S2D3V1";
static const std::string PROJECTILE_FILEPATH = "Resources/Images/BAWS1projectile.png";

static const int MAX_LIFE = 100;
static const float ATTACK_INTERVAL = 3.5f;
static const float PROJECTILE_LIFETIME = 2.5f;
static const float PROJECTILE_SPEED = 300;
static const sf::CircleShape HITBOX_SHAPE = sf::CircleShape(15, 8);

BossDishCloth::BossDishCloth(sf::RenderWindow* window) :
mIsAlive(true),
mWindow(window),
mLife(MAX_LIFE),
mAttackInterval(ATTACK_INTERVAL),
mDirection(0, 1.0f),
mAnimation(Animation(ANIMATION_FILEPATH)),
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
	setScale((float)mLife / 100, (float)mLife / 100);
	if (mAttackInterval <= 0){
		attack();
		mAttackInterval = ATTACK_INTERVAL;
	}
}

void BossDishCloth::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	states.transform *= getTransform();
	target.draw(mAnimation, states);
}

bool BossDishCloth::isAlive(){
	return mIsAlive;
}

BossDishCloth::RenderLayer BossDishCloth::getRenderLayer() const {
	return PLAYER;
}

void BossDishCloth::updateMovement(const sf::Time& deltaTime){
	int spriteHeight = mAnimation.getSprite().getTextureRect().height;
	if (getPosition().y < 0 + spriteHeight
		|| getPosition().y> mWindow->getView().getSize().y - spriteHeight)
	{
		mDirection = mDirection*-1.0f;
	}
	if (mAttackInterval > 0.6f &&
		mAttackInterval <= ATTACK_INTERVAL - 0.3f)
	{
		move(mDirection);
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

void BossDishCloth::collide(Collidable* collidable){
	if (collidable->getCollisionCategory() == HAIR){
		mLife -= 15;
	}
}

sf::FloatRect BossDishCloth::getHitBox(){
	return getTransform().transformRect(mAnimation.getSprite().getGlobalBounds());
}
sf::Shape* BossDishCloth::getNarrowHitbox() const{
	return mHitbox;
}