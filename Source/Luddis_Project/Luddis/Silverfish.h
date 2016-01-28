#ifndef INCLUDED_SILVERFISH
#define INCLUDED_SILVERFISH

#include <string>
#include "Entity.h"
#include "Collidable.h"
#include <SFML/Window.hpp>
#include <SFML/Audio/Sound.hpp>

class Silverfish : public Entity, public Collidable {
public:
	Silverfish(std::string textureFilename, sf::Window* window);
	~Silverfish();
	virtual void tick(const sf::Time& deltaTime);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual bool isAlive();
	virtual RenderLayer getRenderLayer() const;
	virtual sf::FloatRect getHitBox();
private:
	void updateMovement(const sf::Time& deltaTime);
	bool mIsAlive;
	sf::Sprite mSprite;
	sf::Window* mWindow;
	sf::Vector2f mDirection;
	virtual Category getCollisionCategory();
	virtual Type getCollisionType();
	virtual void collide();
};

#endif