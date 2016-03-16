#include "BossRobotButton.h"
#include "ResourceManager.h"

//Takes five shots to "die"
static const int MAX_LIFE = 25;
static const std::string ANIMATION = ("Resources/Images/SpriteSheets/robot/robotButton");

static const sf::RectangleShape HITBOX_SHAPE = sf::RectangleShape(sf::Vector2f(50, 70));

BossRobotButton::BossRobotButton(sf::RenderWindow * window, const sf::Vector2f & position, const float & activation, Transformable * aTarget) :
mWindow(window),
mTarget(aTarget),
mActivate(activation),
mIsAlive(true),
mIsActive(true),
mHitbox(new sf::RectangleShape(HITBOX_SHAPE)),
mLife(MAX_LIFE),
mAnimation(Animation(ANIMATION))
{
	ResourceManager::getInstance().loadTexture(ANIMATION + ".png");
	setPosition(position);
	mHitbox->setOrigin(mHitbox->getLocalBounds().width / 2, mHitbox->getLocalBounds().height / 2);
}

BossRobotButton::~BossRobotButton() {
	delete mHitbox;
}

void BossRobotButton::tick(const sf::Time & deltaTime) {

}

void BossRobotButton::draw(sf::RenderTarget & target, sf::RenderStates states) const {
	states.transform *= getTransform();
	target.draw(mAnimation.getCurrAnimation(), states);
}

bool BossRobotButton::isAlive() const {
	return mIsAlive;
}

bool BossRobotButton::isActive() const {
	return mIsActive;
}

void BossRobotButton::setActive(const bool & active) {
	mIsActive = active;
}

Renderer::RenderLayer BossRobotButton::getRenderLayer() const {
	return Renderer::FOREGROUND;
}

BossRobotButton::Category BossRobotButton::getCollisionCategory() {
	return SOLID;
}

BossRobotButton::Type BossRobotButton::getCollisionType() {
	return REC;
}

void BossRobotButton::collide(CollidableEntity * collidable, const sf::Vector2f & moveAway) {
	if (mLife <= 0) {
		mAnimation.getCurrAnimation().setFrame(mAnimation.getCurrAnimation().getSpriteAmount() - 1);
	}
	else if (collidable->getCollisionCategory() == PLAYER_PROJECTILE) {
		mLife -= 5;
	}
}

sf::FloatRect BossRobotButton::getHitBox() {
	return getTransform().transformRect(mAnimation.getCurrAnimation().getSprite().getGlobalBounds());
}

sf::Shape* BossRobotButton::getNarrowHitbox() const {
	mHitbox->setPosition(getPosition());
	mHitbox->setScale(getScale());
	mHitbox->setRotation(getRotation());
	return mHitbox;
}

void BossRobotButton::stun(const sf::Time & deltatime) {
	return;
}

int BossRobotButton::getLife() {
	return mLife;
}
