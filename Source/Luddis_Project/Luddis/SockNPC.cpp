#include "SockNPC.h"
#include "ViewUtility.h"
#include "SoundEngine.h"
#include <SFML/Graphics/RenderTarget.hpp>

static const std::string SOCKANIMATION_DILE_FILEPATH = "Resources/Images/Spritesheets/Sock_idle";
static const std::string SOCKANIMATION_DIVE_FILEPATH = "Resources/Images/Spritesheets/Sock_exit";
static const std::string SOCKANIMATION_SURFACE_FILEPATH = "Resources/Images/Spritesheets/Sock_entry";

static const char* SOCK_SOUND_DIVE = "event:/Gameplay/Shop/Sock Move Down";
static const char* SOCK_SOUND_SURFACE = "event:/Gameplay/Shop/Sock Move Up";

static const float MAX_DIVE_TIMER = 0.65f;

static sf::Vector2f positions[]{
	sf::Vector2f(350, 550),
	sf::Vector2f(1360, 500)
};

SockNPC::SockNPC() :
	mIsActive(true),
	mIsAlive(true),
	mDive(false),
	mDiveDown(false),
	mLeftSide(false),
	mDiveTimer(0.0f),
	tempTimer(0.0f),
	mAnimation(SOCKANIMATION_DILE_FILEPATH) {

	setScale(-1.0f, 1.0f);
	setPosition(positions[0]);
}

SockNPC::~SockNPC() {

}

void SockNPC::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform *= getTransform();
	target.draw(mAnimation.getCurrAnimation(), states);
}

void SockNPC::tick(const sf::Time & deltaTime) {
	mAnimation.tick(deltaTime);
	if (mDive && mDiveDown) {
		mDiveTimer += deltaTime.asSeconds();
		mDiveTimer = std::min(mDiveTimer, MAX_DIVE_TIMER);
		if (mDiveTimer >= MAX_DIVE_TIMER) {
			mDiveDown = false;
			mAnimation.overrideAnimation(SOCKANIMATION_SURFACE_FILEPATH);
			SoundEngine::getInstance().playEvent(SOCK_SOUND_SURFACE);
			if (mLeftSide) {
				mLeftSide = !mLeftSide;
				scale(-1, 1);
				setPosition(positions[0]);
			}
			else {
				mLeftSide = !mLeftSide;
				scale(-1, 1);
				setPosition(positions[1]);
			}
		}
	}
	else if (mDive && !mDiveDown) {
		mDiveTimer -= deltaTime.asSeconds();
		mDiveTimer = std::max(mDiveTimer, 0.0f);
		if (mDiveTimer <= 0.0f)
			mDive = false;
	}
}

SockNPC::Strata SockNPC::getRenderLayer() const {
	return Strata::FOURTH;
}

bool SockNPC::isAlive() const {
	return mIsAlive;
}

bool SockNPC::isActive() const {
	return mIsActive;
}

void SockNPC::setActive(const bool & active) {
	mIsActive = active;
}

void SockNPC::dive() {
	mDive = true;
	mDiveDown = true;
	mAnimation.replaceAnimation(SOCKANIMATION_DIVE_FILEPATH);
	SoundEngine::getInstance().playEvent(SOCK_SOUND_DIVE);
}
