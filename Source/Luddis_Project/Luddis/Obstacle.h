#ifndef INCLUDED_OBSTACLE
#define INCLUDED_OBSTACLE


#include "CollidableEntity.h"
#include <SFML\Graphics.hpp>

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

	Obstacle(std::string textureFilename, sf::RenderWindow* window, ObstacleType type, sf::Vector2f direction, const sf::Vector2f& position);
	~Obstacle();

	virtual void tick(const sf::Time& deltaTime);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states)const;
	virtual bool isAlive();
	virtual RenderLayer getRenderLayer() const;
	virtual sf::FloatRect getHitBox();
	sf::Shape* getNarrowHitbox() const override;
private:
	bool mIsAlive;
	sf::Sprite mSprite;
	sf::RenderWindow* mWindow;
	virtual Category getCollisionCategory();
	virtual Type getCollisionType();
	virtual void collide(CollidableEntity *collidable);

	sf::Shape* mHitbox;
	ObstacleType mType;
	bool mActive;
	float mActiveTime;
	float mIdleTime;
};


#endif