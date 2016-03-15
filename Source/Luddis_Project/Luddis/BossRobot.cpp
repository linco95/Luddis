#include "BossRobot.h"
#include "Inventory.h"
#include "ResourceManager.h"
#include "Debug.h"

static const std::string IDLE_SPRITE = ("Resources/Images/SpriteSheets/robot/robotIdle");
static const std::string HIT_SPRITE = ("Resources/Images/SpriteSheets/robot/robotHit");
static const std::string ATTACK_ANIMATION = ("Resources/Images/SpriteSheets/robot/robotAttack");
static const std::string DYING_ANIMATION = ("Resources/Images/SpriteSheets/robot/robotDying");
static const std::string DEAD_SPRITE = ("Resources/Images/SpriteSheets/robot/robotDead");
static const std::string ATTACKING_SPRITE = ("Resources/Images/SpriteSheets/robot/robotAttacking");

static const float ATTACK_INTERVAL = 3.5f;
static const float HIT_TIMER = 0.3f;

static const float PHASE_ONE_INTERVAL = 10.0f;
static const float PHASE_TWO_ONE_INTERVAL = 2.0f;
static const float PHASE_TWO_TWO_INTERVAL = 2.0f;

static const sf::CircleShape HITBOX_SHAPE = sf::CircleShape(125, 10);

BossRobot::BossRobot(sf::RenderWindow* window, const sf::Vector2f& position, const float& activation, Transformable* aTarget, BossRobotButton* button) :
	mIsAlive(true),
	mIsActive(false),
	mWindow(window),
	mActivate(activation),
	mAttackInterval(ATTACK_INTERVAL),
	mDirection(0, 1.0f),
	mAnimation(Animation(IDLE_SPRITE)),
	mHitbox(new sf::CircleShape(HITBOX_SHAPE)),
	mButton(button),
	mTarget(aTarget),
	mPhaseOneTimer(PHASE_ONE_INTERVAL),
	mPhaseTwoTimerOne(PHASE_TWO_ONE_INTERVAL),
	mPhaseTwoTimerTwo(PHASE_TWO_TWO_INTERVAL),
	mState(IDLE),
	mCurrentHealth(0)
{
	ResourceManager::getInstance().loadTexture(ATTACK_ANIMATION + ".png");
	ResourceManager::getInstance().loadTexture(DYING_ANIMATION + ".png");
	ResourceManager::getInstance().loadTexture(HIT_SPRITE + ".png");
	ResourceManager::getInstance().loadTexture(IDLE_SPRITE + ".png");
	ResourceManager::getInstance().loadTexture(DEAD_SPRITE + ".png");
	ResourceManager::getInstance().loadTexture(ATTACKING_SPRITE + ".png");
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
	//If not stunned
	if (mTimeStunned <= 0) {
		mAttackInterval -= deltaTime.asSeconds();
		updateMovement(deltaTime);
		mAnimation.tick(deltaTime);
		
		//If time to attack
		if (mAttackInterval <= 0) {
			attack();
			mAttackInterval = ATTACK_INTERVAL;
		}
	}
	else {
		mTimeStunned -= deltaTime.asSeconds();
	}
	//If inactive
	if (!mIsActive) return;
	//When to damage
	if (mInvulnerable >= 0) {
		mInvulnerable -= deltaTime.asSeconds();
	}

	// Retrieve health & reset hit timer if health is less than current
	if (mButton->getLife() < mCurrentHealth) {
		mCurrentHealth = mButton->getLife();
		Debug::log("Rob Health" + std::to_string(mCurrentHealth), Debug::INFO);
		if (mCurrentHealth > 0) {
			mHitTimer = HIT_TIMER;
			mAnimation.replaceAnimation(HIT_SPRITE);
		}
	}

	//Start "dying" animation
	if (mCurrentHealth <= 0 && mState != DYING && mState != DEAD) {
		mState = DYING;
	}	

	//Different timers depending on phase
	switch (mState) {
		case BossRobot::IDLE:
			mPhaseOneTimer -= deltaTime.asSeconds();
			if (mPhaseOneTimer <= 0) {
				mState = PREPARING;
				mPhaseOneTimer = PHASE_ONE_INTERVAL;
			}
			break;
		case BossRobot::PREPARING:
			mPhaseTwoTimerOne -= deltaTime.asSeconds();
			if (mPhaseTwoTimerOne <= 0) {
				mState = ATTACKING;
				mPhaseTwoTimerOne = PHASE_TWO_ONE_INTERVAL;
			}
			break;
		case BossRobot::ATTACKING:
			mPhaseTwoTimerTwo -= deltaTime.asSeconds();
			if (mPhaseTwoTimerTwo <= 0) {
				mState = RETURNING;
				mPhaseTwoTimerTwo = PHASE_TWO_TWO_INTERVAL;
			}
			break;
		case BossRobot::RETURNING:
			
			break;
		case BossRobot::DYING:
			//Freeze dying animation
			mAnimation.setDefaultAnimation(DEAD_SPRITE);
			mAnimation.overrideAnimation(DYING_ANIMATION);
			mState = DEAD;
			break;
		case BossRobot::DEAD:
			break;
		default:
			break;
	}
}

void BossRobot::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	if (!mIsActive) return;
	states.transform *= getTransform();
	target.draw(mAnimation.getCurrAnimation(), states);
}

void BossRobot::updateMovement(const sf::Time& deltaTime) {
	//Phase 1

	//Phase 2.1

	//Phase 2.2
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

Renderer::RenderLayer BossRobot::getRenderLayer() const {
	return Renderer::PLAYER;
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