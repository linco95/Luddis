#ifndef INCLUDED_SILVERFISH
#define INCLUDED_SILVERFISH

#include <string>
#include "Entity.h"
#include <SFML/Audio/Sound.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class Silverfish : public Entity {
public:
	Silverfish(std::string textureFilename, sf::RenderWindow* window);
	~Silverfish();
	virtual void tick(const sf::Time& deltaTime);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual bool isAlive() const;
	virtual RenderLayer getRenderLayer() const;
private:
	void updateMovement(const sf::Time& deltaTime);
	bool mIsAlive;
	sf::Sprite mSprite;
	sf::RenderWindow* mWindow;
	sf::Vector2f mDirection;
};

#endif