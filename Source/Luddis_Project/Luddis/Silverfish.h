#ifndef INCLUDED_SILVERFISH
#define INCLUDED_SILVERFISH

#include <string>

#include "CollidableEntity.h"
#include <SFML/Window.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

static const char* DEFAULTIMAGE = "Resources/Images/Grafik_silverfisk_prototyp_s1d3v2.png";

class Silverfish : public CollidableEntity {
public:
	Silverfish(sf::RenderWindow* window, const sf::Vector2f& position, std::string textureFilename = DEFAULTIMAGE);
	~Silverfish();
	virtual void tick(const sf::Time& deltaTime);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual bool isAlive();
	virtual RenderLayer getRenderLayer() const;
	virtual sf::FloatRect getHitBox();
	sf::Shape* getNarrowHitbox() const override; 

private:
	void updateMovement(const sf::Time& deltaTime);
	bool mIsAlive;
	bool mSwimAway;
	sf::Sprite mSprite;
	sf::RenderWindow* mWindow;
	sf::Vector2f mDirection;
	virtual Category getCollisionCategory();
	virtual Type getCollisionType();
	virtual void collide(CollidableEntity *collidable);
	int mLife;
	sf::Shape* mHitbox;
};

#endif