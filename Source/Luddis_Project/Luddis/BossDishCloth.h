#ifndef _INCLUDED_BOSSDISHCLOTH_
#define _INCLUDED_BOSSDISHCLOTH_

#include "AnimationQueue.h"
#include "CollidableEntity.h"
#include <SFML/Graphics/RenderWindow.hpp>

class BossDishCloth: public CollidableEntity{
public:
	BossDishCloth(sf::RenderWindow* window, const sf::Vector2f& position);
	virtual ~BossDishCloth();

	void tick(const sf::Time& deltaTime) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	bool isAlive() const override;
	bool isActive() const override;
	void setActive(const bool& active) override;
	RenderLayer getRenderLayer() const override;
	Category getCollisionCategory() override;
	Type getCollisionType() override;
	void collide(CollidableEntity *collidable) override;
	sf::FloatRect getHitBox() override;
	sf::Shape* getNarrowHitbox() const override;
private:
	void updateMovement(const sf::Time& deltaTime);
	void attack();

	AnimationQueue mAnimation;
	sf::RenderWindow* mWindow;
	bool mShooting;
	bool mIsAlive;
	bool mIsActive;
	float mAttackInterval;
	sf::Vector2f mDirection;
	int mLife;
	sf::Shape* mHitbox;
};

#endif // !_INCLUDED_BOSSDISHCLOTH
