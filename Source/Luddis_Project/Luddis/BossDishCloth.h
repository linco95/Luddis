#ifndef _INCLUDED_BOSSDISHCLOTH_
#define _INCLUDED_BOSSDISHCLOTH_

#include "AnimationQueue.h"
#include "CollidableEntity.h"
#include <SFML/Graphics/RenderWindow.hpp>

class BossDishCloth: public CollidableEntity{
public:
	BossDishCloth(sf::RenderWindow* window);
	~BossDishCloth();

	virtual void tick(const sf::Time& deltaTime);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual bool isAlive();
	virtual RenderLayer getRenderLayer() const;
	virtual Category getCollisionCategory();
	virtual Type getCollisionType();
	virtual void collide(CollidableEntity *collidable);
	virtual sf::FloatRect getHitBox();
	sf::Shape* getNarrowHitbox() const override;


private:
	void updateMovement(const sf::Time& deltaTime);
	void attack();

	AnimationQueue mAnimation;
	sf::RenderWindow* mWindow;
	bool mShooting;
	bool mIsAlive;
	float mAttackInterval;
	sf::Vector2f mDirection;
	int mLife;
	sf::Shape* mHitbox;

};

#endif // !_INCLUDED_BOSSDISHCLOTH
