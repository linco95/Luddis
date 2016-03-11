#ifndef INCLUDED_BOSSROBOT
#define INCLUDED_BOSSROBOT

#include "CollidableEntity.h"
#include "EntityManager.h"
#include "AnimationQueue.h"
#include "BossRobotButton.h"

class BossRobot : public CollidableEntity {
public:
	BossRobot(sf::RenderWindow* window, const sf::Vector2f& position, const float& activation, Transformable* aTarget, BossRobotButton* button);
	~BossRobot();

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
	void stun(const sf::Time& deltatime) override;
private:
	void updateMovement(const sf::Time& deltaTime);
	void attack();

	AnimationQueue mAnimation;
	sf::Sprite mIdleSprite;
	sf::Sprite mHitSprite;
	sf::RenderWindow* mWindow;
	sf::Transformable* mTarget;
	const float mActivate;
	bool mIsAlive;
	bool mIsActive;
	float mAttackInterval;
	sf::Vector2f mDirection;
	sf::Shape* mHitbox;
	float mTimeStunned;
	float mInvulnerable;
	BossRobotButton* mButton;

	int mCurrentHealth;
	bool mHitBool;
	float mHitTimer;
	bool mDying;
	bool mAttacking;
};

#endif // !INCLUDED_BOSSROBOT