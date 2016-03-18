#ifndef _INCLUDED_BOSSDISHCLOTH_
#define _INCLUDED_BOSSDISHCLOTH_

#include "AnimationQueue.h"
#include "CollidableEntity.h"
#include <SFML/Graphics/RenderWindow.hpp>

class EntityManager;
class GameStateLevel;

class BossDishCloth: public CollidableEntity{
public:
	BossDishCloth(sf::RenderWindow* window, const sf::Vector2f& position, const float& activation, Transformable* aTarget, EntityManager* entityManager);
	virtual ~BossDishCloth();

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
	EntityManager* mEntityManager;
	sf::RenderWindow* mWindow;
	sf::Transformable* mTarget;
	const float mActivate;
	bool mShooting;
	bool mIsAlive;
	bool mIsActive;
	bool mMeet;
	float mAttackInterval;
	float mFireInterval;
	float mAttackTimer;
	sf::Vector2f mDirection;
	int mLife;
	sf::Shape* mHitbox;
	float mTimeStunned;
	float mInvulnerable;
	GameStateLevel* mGameStateLevel;
	bool mDead;
};

#endif // !_INCLUDED_BOSSDISHCLOTH
