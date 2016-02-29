#ifndef INCLUDED_OBSTACLE
#define INCLUDED_OBSTACLE

#include "CollidableEntity.h"
#include <SFML\Graphics.hpp>
#include "AnimationQueue.h"

class Obstacle : public CollidableEntity{
public:
	/*
	* 0 - SOLID
	* 1 - DAMAGE
	*/
	enum ObstacleType{
		SOLID,
		DAMAGE
	};

	Obstacle(sf::RenderWindow* window, ObstacleType type, const sf::Vector2f& position, const float& angle, const sf::Vector2f& size);
	~Obstacle();

	void tick(const sf::Time& deltaTime) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states)const override;
	bool isAlive() const override;
	bool isActive() const override;
	void setActive(const bool& active) override;
	RenderLayer getRenderLayer() const override;
	sf::FloatRect getHitBox() override;
	sf::Shape* getNarrowHitbox() const override;
	void stun(const sf::Time& deltatime) override;
private:
	bool mIsAlive;
	bool mIsActive;
	sf::Sprite mSprite;
	sf::RenderWindow* mWindow;
	Category getCollisionCategory() override;
	Type getCollisionType() override;
	void collide(CollidableEntity *collidable, const sf::Vector2f& moveAway) override;

	//Two steam hitboxes
	sf::Shape* mActiveHitbox;
	sf::Shape* mIdleHitbox;
	//One solid hitbox
	sf::Shape* mHitbox;
	ObstacleType mType;

	// State managing variables
	bool mIsDamaging;
	float mDamageTime;
	float mIdleTime;

	float mAngle;
	AnimationQueue mAnimation;

	float mTimeStunned;
};

#endif