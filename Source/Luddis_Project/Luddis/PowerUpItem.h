#ifndef INCLUDED_POWERUPITEM
#define INCLUDED_POWERUPITEM

#include <string>

#include "CollidableEntity.h"
#include <SFML/Window.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class PowerUpItem : public CollidableEntity {
public:
	PowerUpItem(std::string textureFilename, const sf::Vector2f& position);
	~PowerUpItem();

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
	void collide(CollidableEntity *collidable, const sf::Vector2f& moveAway) override;
	sf::Shape* mHitbox;
};

#endif