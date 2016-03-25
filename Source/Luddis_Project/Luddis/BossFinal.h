#ifndef _INCLUDED_BOSSFINAL_
#define _INCLUDED_BOSSFINAL_

#include "AnimationQueue.h"
#include "CollidableEntity.h"
#include <SFML/Graphics/RenderWindow.hpp>

class EntityManager;
class GameStateLevel;

class BossFinal : public CollidableEntity {
public:
	BossFinal(sf::RenderWindow* window, const sf::Vector2f& position, const float& activation, Transformable* aTarget, EntityManager* entityManager);
	virtual ~BossFinal();

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
private:
	void updateMovement(const sf::Time& deltaTime);
	void attack();

	AnimationQueue mAnimation;
	EntityManager* mEntityManager;
	sf::RenderWindow* mWindow;
	sf::Transformable* mTarget;
	GameStateLevel* mGameStateLevel;
	const float mActivate;
	bool mIsAlive;
	bool mDead;
	bool mIsActive;
	bool mMeet;
	float mAttackInterval1;
	float mAttackInterval2;
	float mAttackTime2;
	sf::Vector2f mDirection;
	int mLife;
	sf::Shape* mHitbox;
	float mTimeStunned;
	float mInvulnerable;

	bool mState1;
	bool mState2;
	bool mState3;
	bool mState4;
	bool mState5;
};

#endif // !_INCLUDED_BOSSFINAL
