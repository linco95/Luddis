#define _USE_MATH_DEFINES
#include "Silverfish.h"
#include "ResourceManager.h"
#include "EntityManager.h"
#include <SFML/System.hpp>
#include <stdlib.h>
#include "SoundEngine.h"
#include "VectorMath.h"
#include <cmath>
#include "Inventory.h"

static const std::string ANIMATION1_SWIM = "resources/images/spritesheets/Silverfish_Swim";
static const std::string ANIMATION1_DEATH = "resources/images/spritesheets/Silverfish_death";
static const std::string ANIMATION1_DEAD = "resources/images/spritesheets/Silverfish_dead";
static const std::string ANIMATION1_HIT = "resources/images/spritesheets/Silverfish_hit";
static const std::string ANIMATION2_SWIM = "resources/images/spritesheets/Goldfish_Swim";
static const std::string ANIMATION2_DEATH = "resources/images/spritesheets/Goldfish_death";
static const std::string ANIMATION2_DEAD = "resources/images/spritesheets/Goldfish_dead";
static const std::string ANIMATION2_HIT = "resources/images/spritesheets/Goldfish_hit";

static const float SPEED = 80;
static const Renderer::RenderLayer LAYER = Renderer::PLAYER;
static const int DAMAGE = 10;
static const int LIFE = 10;
static const sf::Vector2f FRONTVECTOR(-1, 0);
static const float INVULNERABLE_TIMER = 1.5f;

static const sf::RectangleShape HITBOX_SHAPE = sf::RectangleShape(sf::Vector2f(55, 17));

Silverfish::Silverfish(sf::RenderWindow* window, FishType type, const sf::Vector2f& position, const float& angle, const float& activation, Transformable* aTarget) :
	mIsAlive(true),
	mIsActive(false),
	mSwimAway(false),
	mBefriend(false),
	mLife(LIFE),
	mActivate(activation),
	mWindow(window),
	mType(type),
	mAnimation(ANIMATION1_SWIM),
	mHitbox(new sf::RectangleShape(HITBOX_SHAPE)),
	mAlignment(ENEMY_DAMAGE),
	mTarget(aTarget),
	mInvulnerable(0)
{
	mSprite.setOrigin((float)mSprite.getTextureRect().width / 2, (float)mSprite.getTextureRect().height / 2);
	// Get a y-spawn position
	sf::Vector2f size = mWindow->getView().getSize();
	int r1 = rand() % (int)size.y + 1;
	// Set spawn position
	setPosition(position);

	sf::Vector2f dir;
	dir = { 1, 0 };
	dir = VectorMath::rotateVector(dir, angle);
	mDirection = VectorMath::normalizeVector(dir);

	// TODO flip if changing direction when swimming aways

	if (mDirection.x > 0) {
		scale(sf::Vector2f(1, -1));
	}

	if (mType == GOLD) {
		mAnimation.setDefaultAnimation(ANIMATION2_SWIM);
	}

	mHitbox->setScale(getScale());
	mHitbox->setRotation(getRotation());
	mHitbox->setOrigin(mHitbox->getLocalBounds().width / 2, mHitbox->getLocalBounds().height / 2);

}

Silverfish::~Silverfish() {
	delete mHitbox;
}

void Silverfish::tick(const sf::Time& deltaTime) {

	if (mTarget->getPosition().x >= mActivate) {
		mIsActive = true;
	}
	if (mTimeStunned <= 0) {
		if (mBefriend) {
			mAlignment = IGNORE;
			mBefriend = false;
		}
		if (!mIsActive) return;
		updateMovement(deltaTime);
		mAnimation.tick(deltaTime);
		// TODO: Cleanup, enable fishes to be outside while spawning
		if (getPosition().y<(
			-mAnimation.getCurrAnimation().getSprite().getGlobalBounds().height / 2) ||
			getPosition().y > mWindow->getView().getSize().y + mAnimation.getCurrAnimation().getSprite().getGlobalBounds().height / 2
			) {
			mIsAlive = false;
		}
	}
	else {
		mTimeStunned -= deltaTime.asSeconds();
	}

	if (mInvulnerable >= 0) {
		mInvulnerable -= deltaTime.asSeconds();
	}

}

void Silverfish::updateMovement(const sf::Time& deltaTime) {
	float rotation = VectorMath::getAngle(sf::Vector2f(FRONTVECTOR), mDirection) * 180 / (float)M_PI;
	setRotation(rotation);

	// Make the fishes swim faster when swimming away
	float speed = SPEED;
	if (mSwimAway && mAnimation.getCurrAnimation().hasLooped()) {
		if (mDirection != mNextDir) mDirection = mNextDir;
		speed = SPEED * 4;
	}
	else if (mSwimAway && !mAnimation.getCurrAnimation().hasLooped()) {
		speed = 0;
	}

	move(mDirection * speed * deltaTime.asSeconds());
}

void Silverfish::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	if (!mIsActive) return;
	states.transform *= getTransform();
	target.draw(mAnimation.getCurrAnimation(), states);
}

bool Silverfish::isAlive() const {
	return mIsAlive;
}

bool Silverfish::isActive() const {
	return mIsActive;
}

void Silverfish::setActive(const bool& active) {
	mIsActive = active;
}

Renderer::RenderLayer Silverfish::getRenderLayer() const {
	return LAYER;
}

Silverfish::Category Silverfish::getCollisionCategory() {
	return mAlignment;
}

Silverfish::Type Silverfish::getCollisionType() {
	return REC;
}

void Silverfish::collide(CollidableEntity *collidable, const sf::Vector2f& moveAway) {
	if (collidable->getCollisionCategory() == PLAYER_PROJECTILE) {
		if (mSwimAway == false) {
			mLife -= collidable->getCollisionDamage();
			SoundEngine* se = &SoundEngine::getInstance();
			se->playEvent("event:/Gameplay/Luddis/Interaction/Luddis_Hit");
			if (mLife <= 0) {
				if (mType == GOLD) {
					mAnimation.replaceAnimation(ANIMATION2_DEATH);
					mAnimation.setDefaultAnimation(ANIMATION2_DEAD);
				}

				else if (mType == SILVER) {
					mAnimation.replaceAnimation(ANIMATION1_DEATH);
					mAnimation.setDefaultAnimation(ANIMATION1_DEAD);
				}
				mSwimAway = true;
				mNextDir = VectorMath::normalizeVector(getPosition() - collidable->getPosition());
				mBefriend = true;
				mTimeStunned = 0;
			}
			else if (mLife > 0) {
				if (mType == GOLD) mAnimation.replaceAnimation(ANIMATION2_HIT);
				else if (mType == SILVER) mAnimation.replaceAnimation(ANIMATION1_HIT);
			}
		}
	}
	if (collidable->getCollisionCategory() == PLAYER_OBJECT) {
		if (mInvulnerable <= 0 && mTimeStunned <= 0) {
			if (mType == SILVER) {
				Inventory::getInstance().addDust(-1);
			}
			else if (mType == GOLD) {
				Inventory::getInstance().addDust(-15);
			}
			SoundEngine* se = &SoundEngine::getInstance();
			se->playEvent("event:/Gameplay/Luddis/Interaction/Luddis_Hit");
			mInvulnerable += INVULNERABLE_TIMER;
		}
	}
}

sf::FloatRect Silverfish::getHitBox() {
	if (mIsActive) {
		return getTransform().transformRect(mAnimation.getCurrAnimation().getSprite().getGlobalBounds());
	}
	else {
		// Not a good way to disable collision as all silverfishes would still collide with each other (if they can collide with an area of 0)
		return sf::FloatRect(-999, -999, 0, 0);
	}
}

sf::Shape* Silverfish::getNarrowHitbox() const {
	mHitbox->setPosition(getPosition());

	return mHitbox;
}

int Silverfish::getCollisionDamage() const {
	// the fish does no damage to the target, but to the inventory.
	return 0;
}

void Silverfish::stun(const sf::Time& deltatime) {
	if (mSwimAway == false) {
		mTimeStunned = float(deltatime.asSeconds());
	}
	else {
		return;
	}
}