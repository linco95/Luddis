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

	void tick(const sf::Time& deltaTime) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	bool isAlive() const override;
	bool isActive() const override;
	void setActive(const bool& active) override;
	RenderLayer getRenderLayer() const override;
	sf::FloatRect getHitBox() override;
	sf::Shape* getNarrowHitbox() const override;
private:
	bool mIsAlive;
	bool mIsActive;
	sf::Sprite mSprite;
	sf::RenderWindow* mWindow;
	Category getCollisionCategory() override;
	Type getCollisionType() override;
	void collide(CollidableEntity *collidable) override;
	sf::Shape* mHitbox;

};

#endif