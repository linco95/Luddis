#include "BossRobot.h"

BossRobot::BossRobot(sf::RenderWindow* window, const sf::Vector2f& position, const float& activation, Transformable* aTarget, EntityManager* entityManager) {

}

BossRobot::~BossRobot() {

}

void BossRobot::tick(const sf::Time& deltaTime) {

}

void BossRobot::draw(sf::RenderTarget& target, sf::RenderStates states) const {

}

bool BossRobot::isAlive() const {

}

bool BossRobot::isActive() const {

}

void BossRobot::setActive(const bool& active) {

}

BossRobot::RenderLayer BossRobot::getRenderLayer() const {

}

BossRobot::Category BossRobot::getCollisionCategory() {

}

BossRobot::Type BossRobot::getCollisionType() {

}

void BossRobot::collide(CollidableEntity *collidable, const sf::Vector2f& moveAway) {

}

sf::FloatRect BossRobot::getHitBox() {

}

sf::Shape* BossRobot::getNarrowHitbox() const {

}

void BossRobot::stun(const sf::Time& deltatime) {

}