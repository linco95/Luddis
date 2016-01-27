#ifndef _INCLUDED_PROJECTILE_
#define _INCLUDED_PROJECTILE_

#include "Entity.h"
#include <SFML/Window.hpp>
#include <string>

class Projectile : public Entity{
public:
	//The max life time should be entered in milliseconds
	Projectile(std::string textureFilename, sf::Vector2f direction,sf::Vector2f position, float maxLifeTimeMS);
	virtual ~Projectile();

	virtual void tick(const sf::Time& deltaTime);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual bool isAlive() const;
private:
	void updateMovement(const sf::Time& deltaTime);
	void checkLifeTime();

	sf::Sprite mSprite;
	float mLifeTime;
	bool mIsAlive;
	sf::Vector2f mDirection;
};

#endif // !_INCLUDED_PROJECTILE_
