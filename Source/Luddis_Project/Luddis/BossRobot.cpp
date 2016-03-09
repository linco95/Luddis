#include "BossRobot.h"
#include "Inventory.h"
#include "ResourceManager.h"

static const std::string ANIMATION_IDLE = ("Resources/Images/Spritesheets/RagIdle_P1");
static const std::string SHOOTING_ANIMATION = ("Resources/Images/Spritesheets/RagAttack_P1");

static const int MAX_LIFE = 100;
static const float ATTACK_INTERVAL = 3.5f;

static const sf::CircleShape HITBOX_SHAPE = sf::CircleShape(225, 225);

BossRobot::BossRobot(sf::RenderWindow* window, const sf::Vector2f& position, const float& activation, Transformable* aTarget, EntityManager* entityManager) :
mIsAlive(true),
mIsActive(false),
mWindow(window),
mEntityManager(entityManager),
mActivate(activation),
mLife(MAX_LIFE),
mAttackInterval(ATTACK_INTERVAL),
mDirection(0, 1.0f),
mAnimation(Animation(ANIMATION_IDLE)),
mHitbox(new sf::CircleShape(HITBOX_SHAPE)),
mTarget(aTarget) 
{
	ResourceManager::getInstance().loadTexture(ANIMATION_IDLE + ".png");
	ResourceManager::getInstance().loadTexture(SHOOTING_ANIMATION + ".png");
	setPosition(position);
	mHitbox->setOrigin(mHitbox->getLocalBounds().width / 2, mHitbox->getLocalBounds().height / 2);
}

BossRobot::~BossRobot() {
	delete mHitbox;
}

void BossRobot::tick(const sf::Time& deltaTime) {
	if (mTarget->getPosition().x >= mActivate) {
		mIsActive = true;
	}
	if (mTimeStunned <= 0) {
		mAttackInterval -= deltaTime.asSeconds();
		updateMovement(deltaTime);
		mAnimation.tick(deltaTime);
		if (mAttackInterval <= 0) {
			attack();
			mAttackInterval = ATTACK_INTERVAL;
		}
	}
	else {
		mTimeStunned -= deltaTime.asSeconds();
	}

	if (mInvulnerable >= 0) {
		mInvulnerable -= deltaTime.asSeconds();
	}

	if (!mIsActive) return;
	if (mLife <= 0) {
		mIsAlive = false;
	}
}

void BossRobot::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	if (!mIsActive) return;
	states.transform *= getTransform();
	target.draw(mAnimation.getCurrAnimation(), states);
}

void BossRobot::updateMovement(const sf::Time& deltaTime) {

}

void BossRobot::attack() {
	//?
}

bool BossRobot::isAlive() const {
	return mIsAlive;
}

bool BossRobot::isActive() const {
	return mIsActive;
}

void BossRobot::setActive(const bool& active) {
	mIsActive = active;
}

BossRobot::RenderLayer BossRobot::getRenderLayer() const {
	return PLAYER;
}

BossRobot::Category BossRobot::getCollisionCategory() {
	return ENEMY_DAMAGE;
}

BossRobot::Type BossRobot::getCollisionType() {
	return CIRCLE;
}

void BossRobot::collide(CollidableEntity *collidable, const sf::Vector2f& moveAway) {
	if (collidable->getCollisionCategory() == PLAYER_OBJECT) {
		if (mInvulnerable <= 0) {
			Inventory::getInstance().addDust(-1);
		}
	}
}

sf::FloatRect BossRobot::getHitBox() {
	return getTransform().transformRect(mAnimation.getCurrAnimation().getSprite().getGlobalBounds());
}

sf::Shape* BossRobot::getNarrowHitbox() const {
	mHitbox->setPosition(getPosition());
	mHitbox->setScale(getScale());
	mHitbox->setRotation(getRotation());
	return mHitbox;
}

void BossRobot::stun(const sf::Time& deltatime) {
	mTimeStunned = float(deltatime.asSeconds());
}