#ifndef _INCLUDED_LUDDIS_
#define _INCLUDED_LUDDIS_

#include <string>
#include "Entity.h"
#include <SFML/Window.hpp>

class Luddis : public Entity{
public:
	Luddis(std::string textureFilename, sf::Window* window);
	~Luddis();
	virtual void tick(const sf::Time& deltaTime);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual bool isAlive() const override;
private:
	sf::Sprite mSprite;
	sf::Window* mWindow;
	bool mIsAlive;
};

#endif // !_INCLUDED_LUDDIS_
