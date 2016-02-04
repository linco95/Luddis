#ifndef _INCLUDED_BOSSDISHCLOTH_
#define _INCLUDED_BOSSDISHCLOTH_

#include "AnimationQueue.h"
#include "Entity.h"
#include "Collidable.h"
#include <SFML/Graphics/RenderWindow.hpp>

class BossDishCloth: public Entity, public Collidable{
public:
	BossDishCloth(sf::RenderWindow* window);
	~BossDishCloth();

	virtual void tick(const sf::Time& deltaTime);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual bool isAlive();
	virtual RenderLayer getRenderLayer() const;
	virtual Category getCollisionCategory();
	virtual Type getCollisionType();
	virtual void collide(Collidable *collidable);
	virtual sf::FloatRect getHitBox();

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
};

#endif // !_INCLUDED_BOSSDISHCLOTH
