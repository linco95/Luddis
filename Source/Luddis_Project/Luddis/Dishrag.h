#ifndef INCLUDED_DISHRAG
#define INCLUDED_DISHRAG

#include "Entity.h"
#include "Collidable.h"
#include <SFML/Graphics/RenderWindow.hpp>

class Dishrag : public Entity, public Collidable{
public:
	Dishrag(std::string textureFilename, sf::RenderWindow* window, sf::Vector2f position);
	~Dishrag();
	virtual void tick(const sf::Time& deltaTime);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual bool isAlive();
	virtual RenderLayer getRenderLayer() const;
	virtual sf::FloatRect getHitBox();
private:
	bool mIsAlive;
	void updateMovement(const sf::Time& deltaTime);
	sf::Sprite mSprite;

	virtual Category getCollisionCategory();
	virtual Type getCollisionType();
	virtual void collide(Collidable *collidable);

	sf::RenderWindow* mWindow;
};

#endif