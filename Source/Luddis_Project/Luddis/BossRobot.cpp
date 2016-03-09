#include "BossRobot.h"
#include "Inventory.h"
#include "ResourceManager.h"

static const std::string IDLE_SPRITE = ("Resources/Images/Spritesheets/robot/robotIdle.png");
static const std::string HIT_SPRITE = ("Resources/Images/Spritesheets/robot/robotHit.png");
static const std::string ATTACK_ANIMATION = ("Resources/Images/Spritesheets/robot/robotAttack");
static const std::string DYING_ANIMATION = ("Resources/Images/Spritesheets/robot/robotDying");

static const float ATTACK_INTERVAL = 3.5f;
static const float HIT_TIMER = 0.3f;

static const sf::CircleShape HITBOX_SHAPE = sf::CircleShape(225, 225);

BossRobot::BossRobot(sf::RenderWindow* window, const sf::Vector2f& position, const float& activation, Transformable* aTarget, BossRobotButton* button) :
	mIsAlive(true),
	mIsActive(false),
	mWindow(window),
	mActivate(activation),
	mAttackInterval(ATTACK_INTERVAL),
	mDirection(0, 1.0f),
	mAnimation(Animation(ATTACK_ANIMATION)),
	mHitbox(new sf::CircleShape(HITBOX_SHAPE)),
	mButton(button),
	mTarget(aTarget),
	mIdleSprite(ResourceManager::getInstance().loadTexture(IDLE_SPRITE)),
	mHitSprite(ResourceManager::getInstance().loadTexture(HIT_SPRITE)),
	mHitBool(false),
	mDying(false)
{
	ResourceManager::getInstance().loadTexture(ATTACK_ANIMATION + ".png");
	ResourceManager::getInstance().loadTexture(DYING_ANIMATION + ".png");
	setPosition(position);
	mHitbox->setOrigin(mHitbox->getLocalBounds().width / 2, mHitbox->getLocalBounds().height / 2);
	mCurrentHealth = mButton->getLife();
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

	if (!mIsActive) return;

	if (mInvulnerable >= 0) {
		mInvulnerable -= deltaTime.asSeconds();
	}

	if (mButton->getLife() < mCurrentHealth) {
		mCurrentHealth = mButton->getLife();
		mHitTimer = HIT_TIMER;
		mHitBool = true;
	}
	else if (mHitTimer>0) {
		mHitTimer -= deltaTime.asSeconds();
	}
	else if (mHitTimer <= 0) {
		mHitBool = false;
	}

	if (mButton->getLife() <= 0) {
		//TODO TURN OFF
		//mIsAlive = false;
		mDying = true;
	}
}

void BossRobot::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	if (!mIsActive) return;
	states.transform *= getTransform();

	if (mDying == true) {
		target.draw(mAnimation.getCurrAnimation(), states);
	} else if (mHitBool == true) {
		target.draw(mHitSprite, states);
	}
	

	target.draw(mIdleSprite, states);
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