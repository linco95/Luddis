#ifndef INCLUDED_SPIDER
#define INCLUDED_SPIDER

#include "Entity.h"
#include "Collidable.h"
#include <SFML/Window.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class Spider : public Entity {
public:
	Spider(std::string textureFilename, sf::RenderWindow* window);
	~Spider();

	virtual void tick(const sf::Time& deltaTime);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual bool isAlive();
	virtual RenderLayer getRenderLayer() const;
private:
	void updateMovement(const sf::Time& deltaTime);
	bool mIsAlive;
	float mWait;
	sf::Sprite mSprite;
	sf::RenderWindow* mWindow;
	sf::Vector2f mDirection;
	sf::Vector2f mDirection2;

};

#endif