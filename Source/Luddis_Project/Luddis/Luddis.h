#ifndef _INCLUDED_LUDDIS_
#define _INCLUDED_LUDDIS_

#include <string>
#include "AnimationQueue.h"
#include "Collidable.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include "Entity.h"
#include <SFML/Audio/Sound.hpp>

class Luddis : public Entity, public Collidable{
public:
	Luddis(std::string textureFilename, sf::RenderWindow* window);
	~Luddis();
	virtual void tick(const sf::Time& deltaTime);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual bool isAlive();
	virtual RenderLayer getRenderLayer() const;
	virtual sf::FloatRect getHitBox();
	sf::Shape* getNarrowHitbox() const override;

private:
	sf::Vector2f getVectorMouseToSprite() const;
	void handleInput(const sf::Time& deltaTime);
	void updateMovement(const sf::Time& deltaTime);
	void updateRotation();
	void attack();
	virtual Category getCollisionCategory();
	virtual Type getCollisionType();
	virtual void collide(Collidable *collidable);
	
	AnimationQueue mAnimation;
	sf::RenderWindow* mWindow;
	float mProjectileCooldown;
	float mStunDuration;
	bool mIsAlive;
	bool mColliding;
	sf::FloatRect mCollideBox;
	sf::Vector2f mDirectionVector;
	sf::Vector2f mPrevPos;
	sf::Shape* mHitbox;
};

#endif // !_INCLUDED_LUDDIS_
