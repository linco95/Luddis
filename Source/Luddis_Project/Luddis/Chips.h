#ifndef INCLUDED_CHIPS
#define INCLUDED_CHIPS

#include <string>

#include "CollidableEntity.h"
#include <SFML/Window.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class Chips : public CollidableEntity {
public:
	Chips(std::string textureFilename, sf::RenderWindow* window);
	~Chips();
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
	virtual void collide(CollidableEntity *collidable);
	sf::Shape* mHitbox;

};

#endif