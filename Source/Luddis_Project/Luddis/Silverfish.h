#ifndef INCLUDED_SILVERFISH
#define INCLUDED_SILVERFISH

#include <string>

#include "CollidableEntity.h"
#include "AnimationQueue.h"
#include <SFML/Window.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Graphics/RenderWindow.hpp>


class Silverfish : public CollidableEntity {
public:
	Silverfish(sf::RenderWindow* window, const sf::Vector2f& position, const float& angle);
	~Silverfish();

	void tick(const sf::Time& deltaTime) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	bool isAlive() const override;
	bool isActive() const override;
	void setActive(const bool& active) override;
	RenderLayer getRenderLayer() const override;
	sf::FloatRect getHitBox() override;
	sf::Shape* getNarrowHitbox() const override; 
private:
	void updateMovement(const sf::Time& deltaTime);
	bool mIsAlive;
	bool mIsActive;
	bool mSwimAway;
	sf::Sprite mSprite;
	sf::RenderWindow* mWindow;
	sf::Vector2f mDirection;
	Category getCollisionCategory() override;
	Type getCollisionType() override;
	void collide(CollidableEntity *collidable) override;
	int mLife;
	sf::Shape* mHitbox;
	AnimationQueue mAnimation;
};

#endif