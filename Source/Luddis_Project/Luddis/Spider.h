#ifndef INCLUDED_SPIDER
#define INCLUDED_SPIDER

#include "Entity.h"
#include "Renderer.h"
#include "AnimationQueue.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>

class GUIManager;
class EventManager;
class GameStateLevel;

class Spider : public Entity {
public:
	Spider(sf::RenderWindow* window, const sf::Vector2f& position);
	~Spider();

	void tick(const sf::Time& deltaTime) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	bool isAlive() const override;
	bool isActive() const override;
	void setActive(const bool& active) override;
	Renderer::RenderLayer getRenderLayer() const override;
	void stun(const sf::Time& deltaTime) override;
	void turn();

private:
	void updateMovement(const sf::Time& deltaTime);
	bool mTalk;
	bool mIsAlive;
	bool mIsActive;
	bool mWaiting;
	bool mLeaving;
	bool mIdleAnimation;
	bool mTurn;
	sf::Sprite mSprite;
	sf::RenderWindow* mWindow;
	GameStateLevel* mGameStateLevel;
	sf::Vector2f mDirection;
	AnimationQueue mAnimation;
	sf::RectangleShape mWeb;
};

#endif