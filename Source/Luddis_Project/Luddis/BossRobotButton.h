#ifndef INCLUDED_BOSSROBOTBUTTON
#define INCLUDED_BOSSROBOTBUTTON

#include "CollidableEntity.h"
#include <SFML\Graphics.hpp>
#include "AnimationQueue.h"

class BossRobotButton : public CollidableEntity {
public:
	BossRobotButton(sf::RenderWindow* window, const sf::Vector2f& position, const float& activation, Transformable* aTarget);
	~BossRobotButton();

	void tick(const sf::Time& deltaTime) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	bool isAlive() const override;
	bool isActive() const override;
	void setActive(const bool& active) override;
	Renderer::RenderLayer getRenderLayer() const override;
	Category getCollisionCategory() override;
	Type getCollisionType() override;
	void collide(CollidableEntity *collidable, const sf::Vector2f& moveAway) override;
	sf::FloatRect getHitBox() override;
	sf::Shape* getNarrowHitbox() const override;
	int getCollisionDamage() const override;
	void stun(const sf::Time& deltatime) override;
	int getLife();

private:
	sf::RenderWindow* mWindow;
	sf::Transformable* mTarget;
	const float mActivate;
	bool mIsAlive;
	bool mIsActive;
	sf::Shape* mHitbox;
	int mLife;
	AnimationQueue mAnimation;
	float mInvulnerableTimer;
	float mAnimationHitTimer;
	bool mHit;
};

#endif // !INCLUDED_BOSSROBOTBUTTON
