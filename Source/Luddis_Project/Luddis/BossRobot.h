#ifndef INCLUDED_BOSSROBOT
#define INCLUDED_BOSSROBOT

#include "CollidableEntity.h"
#include "EntityManager.h"

class BossRobot : public CollidableEntity {
public:
	BossRobot(sf::RenderWindow* window, const sf::Vector2f& position, const float& activation, Transformable* aTarget, EntityManager* entityManager);
	virtual ~BossRobot();

	void tick(const sf::Time& deltaTime) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	bool isAlive() const override;
	bool isActive() const override;
	void setActive(const bool& active) override;
	RenderLayer getRenderLayer() const override;
	Category getCollisionCategory() override;
	Type getCollisionType() override;
	void collide(CollidableEntity *collidable, const sf::Vector2f& moveAway) override;
	sf::FloatRect getHitBox() override;
	sf::Shape* getNarrowHitbox() const override;
	void stun(const sf::Time& deltatime) override;
private:

};

#endif // !INCLUDED_BOSSROBOT