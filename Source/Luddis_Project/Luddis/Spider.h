#ifndef INCLUDED_SPIDER
#define INCLUDED_SPIDER


#include "CollidableEntity.h"
#include "AnimationQueue.h"
#include <SFML/Window.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class GUIManager;
class EventManager;
class GameStateLevel;

class Spider : public Entity {
public:
	Spider(sf::RenderWindow* window, const sf::Vector2f& position, const float& activation, Transformable* aTarget, GameStateLevel* gameStateLevel);
	~Spider();

	void tick(const sf::Time& deltaTime) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	bool isAlive() const override;
	bool isActive() const override;
	void setActive(const bool& active) override;
	RenderLayer getRenderLayer() const override;
private:
	void updateMovement(const sf::Time& deltaTime);
	sf::Transformable* mTarget;
	const float mActivate;
	bool mTalk;
	bool mIsAlive;
	bool mIsActive;
	bool mWaiting;
	bool mDisplayDialogue;
	float mWait;
	sf::Sprite mSprite;
	sf::RenderWindow* mWindow;
	GameStateLevel* mGameStateLevel;
	sf::Vector2f mDirection;
	AnimationQueue mAnimation;
};

#endif