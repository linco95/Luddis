#include "BossDishCloth.h"
#include "Projectile.h"
#include "EntityManager.h"
#include "CollisionManager.h"
#include "VectorMath.h"

static const std::string ANIMATION_FILEPATH = "Resources/Images/BAWS.png";
static const std::string PROJECTILE_FILEPATH = "Resources/Images/BAWS1projectile.png";

static const float ATTACK_INTERVAL = 1.3f;
static const float PROJECTILE_LIFETIME = 2.5f;
static const float PROJECTILE_SPEED = 400;

BossDishCloth::BossDishCloth(sf::RenderWindow* window) :
mIsAlive(true),
mWindow(window),
mAttackInterval(ATTACK_INTERVAL),
mDirection(0, 1.0f),
mAnimation(Animation(ANIMATION_FILEPATH, sf::Vector2i(200, 200), 4, 4, sf::seconds(0.3f))){
	setPosition(2000, 500);
}

BossDishCloth::~BossDishCloth(){

}

void BossDishCloth::tick(const sf::Time& deltaTime){
	mAttackInterval -= deltaTime.asSeconds();
	updateMovement(deltaTime);
	mAnimation.tick(deltaTime);
	if (mAttackInterval <= 0){
		attack();
		mAttackInterval = ATTACK_INTERVAL;
	}
}

void BossDishCloth::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	states.transform *= getTransform();
	target.draw(mAnimation.getCurrAnimation(), states);
}

bool BossDishCloth::isAlive(){
	return mIsAlive;
}

BossDishCloth::RenderLayer BossDishCloth::getRenderLayer() const {
	return PLAYER;
}

void BossDishCloth::updateMovement(const sf::Time& deltaTime){
	int spriteHeight = mAnimation.getCurrAnimation().getSprite().getTextureRect().height;
	if (getPosition().y < 0 + spriteHeight
		|| getPosition().y> mWindow->getSize().y - spriteHeight)
	{
		mDirection = mDirection*-1.0f;
	}
	move(mDirection);
}

void BossDishCloth::attack(){
	sf::Vector2f vec(0, 1);
	for (int i = 0; i < 3; i++)
	{

		vec = VectorMath::rotateVector(vec, 45);
		Projectile* proj = new Projectile(PROJECTILE_FILEPATH, vec*PROJECTILE_SPEED, getPosition(), PROJECTILE_LIFETIME, ENEMY_STUN);
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

}

sf::FloatRect BossDishCloth::getHitBox(){
	return getTransform().transformRect(mAnimation.getCurrAnimation().getSprite().getGlobalBounds());
}