#ifndef INCLUDED_SPIDER
#define INCLUDED_SPIDER


#include "CollidableEntity.h"
#include "AnimationQueue.h"
#include <SFML/Window.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class Spider : public Entity {
public:
	Spider(sf::RenderWindow* window);
	~Spider();

	void tick(const sf::Time& deltaTime) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	bool isAlive() const override;
	bool isActive() const override;
	void setActive(const bool& active) override;
	RenderLayer getRenderLayer() const override;
private:
	void updateMovement(const sf::Time& deltaTime);
	bool mTalk;
	bool mIsAlive;
	bool mIsActive;
	bool mWaiting;
	float mWait;
	sf::Sprite mSprite;
	sf::RenderWindow* mWindow;
	sf::Vector2f mDirection;
	sf::Vector2f mDirection2;
	AnimationQueue mAnimation;
};

#endif