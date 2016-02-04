#ifndef INCLUDED_OBSTACLE
#define INCLUDED_OBSTACLE

#include "Entity.h"
#include "Collidable.h"
#include <SFML\Graphics.hpp>

class Obstacle : public Entity, public Collidable{
public:
	enum ObstacleType{
		SOLID,
		DAMAGE
	};

	Obstacle(std::string textureFilename, sf::RenderWindow* window, ObstacleType type);
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
	virtual void collide(Collidable *collidable);

	sf::Shape* mHitbox;
	ObstacleType mType;
};


#endif