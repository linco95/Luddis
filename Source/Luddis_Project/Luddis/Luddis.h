#ifndef _INCLUDED_LUDDIS_
#define _INCLUDED_LUDDIS_

#include <string>
#include "Animation.h"
#include "Collidable.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include "Entity.h"
#include <SFML/Audio/Sound.hpp>

class Luddis : public Entity, public Collidable{
public:
	Luddis(std::string textureFilename, sf::RenderWindow* window);
	~Luddis();
	virtual void tick(const sf::Time& deltaTime);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual bool isAlive() ;
	virtual RenderLayer getRenderLayer() const;
	virtual sf::FloatRect getHitBox();
private:
	sf::Vector2f getVectorMouseToSprite() const;
	void handleInput(const sf::Time& deltaTime);
	void updateMovement(const sf::Time& deltaTime);
	void updateRotation();
	void attack();
	bool mIsAlive;
	Animation mAnimation;
	sf::RenderWindow* mWindow;
	sf::Sound mTestSound1;
	float mProjectileCooldown;
	virtual Category getCollisionCategory();
	virtual Type getCollisionType();
	virtual void collide();
};

#endif // !_INCLUDED_LUDDIS_
