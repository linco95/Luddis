#ifndef INCLUDED_SILVERFISH
#define INCLUDED_SILVERFISH

#include <string>
#include "Entity.h"
#include <SFML/Window.hpp>
#include <SFML/Audio/Sound.hpp>

class Silverfish {
public:
	Silverfish(std::string textureFilename, sf::Window* window);
	~Silverfish();
	virtual void tick(const sf::Time& deltaTime);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual bool isAlive() const;
private:
	void updateMovement();
	void updateRotation();
	bool mIsAlive;
	sf::Sprite mSprite;
	sf::Window* mWindow;

	float mDirection;

};

#endif