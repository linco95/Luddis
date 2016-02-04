#ifndef INCLUDED_DUST
#define INCLUDED_DUST

#include <string>
#include "Entity.h"
#include "Collidable.h"
#include <SFML/Window.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class Dust : public Entity, public Collidable {
public:
	Dust(std::string textureFilename, sf::RenderWindow* window);
	~Dust();
	virtual void tick(const sf::Time& deltaTime);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
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
};

#endif