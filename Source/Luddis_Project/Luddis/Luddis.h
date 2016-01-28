#ifndef _INCLUDED_LUDDIS_
#define _INCLUDED_LUDDIS_

#include <string>
#include "Collidable.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include "Entity.h"
#include <SFML/Window.hpp>
#include <SFML/Audio/Sound.hpp>

class Luddis : public Entity, public Collidable{
public:
	Luddis(std::string textureFilename, sf::RenderWindow* window);
	~Luddis();
	virtual void tick(const sf::Time& deltaTime);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual bool isAlive() const;
	virtual RenderLayer getRenderLayer() const;
	virtual sf::FloatRect getHitBox();
private:
	sf::Vector2f getVectorMouseToSprite() const;
	void handleInput(const sf::Time& deltaTime);
	void updateMovement(const sf::Time& deltaTime);
	void updateRotation();
	void attack();
	bool mIsAlive;
	sf::Sprite mSprite;
	sf::RenderWindow* mWindow;
	sf::Sound mTestSound1;
	float mProjectileCooldown;
	virtual Category getCategory();
	virtual Type getType();
	virtual void collide();
};

#endif // !_INCLUDED_LUDDIS_
