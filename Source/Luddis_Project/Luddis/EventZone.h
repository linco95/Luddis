#ifndef _INCLUDED_EVENTZONE_
#define _INCLUDED_EVENTZONE_

#include "CollidableEntity.h"
#include <SFML/Graphics/RenderWindow.hpp>

class GameStateLevel;

class EventZone: public CollidableEntity{
public:
	enum EventType{
		SPIDER
	};
	//The sf::Shape used will be deleted in destructor.
	EventZone(GameStateLevel* gameStateLevel, EventType, sf::Vector2f pos, sf::Shape* shape, float rotation, int level);
	~EventZone();
	void tick(const sf::Time& deltaTime) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	bool isAlive() const override;
	bool isActive() const override;
	void setActive(const bool& active) override;
	RenderLayer getRenderLayer() const override;
	sf::FloatRect getHitBox() override;
	sf::Shape* getNarrowHitbox() const override;

private:
	Category getCollisionCategory() override;
	Type getCollisionType() override;
	void collide(CollidableEntity* collidable, const sf::Vector2f& moveAway) override;

	sf::Shape* mHitbox;
	sf::FloatRect mBigHitbox;
	sf::RenderWindow* mWindow;
	GameStateLevel* mGameStateLevel;
	EventType mEventType;
	Type mCollisionType;
	int mLevel;
	bool mIsAlive;
	bool mIsActive;
};

#endif // !_INCLUDED_EVENTBOX
