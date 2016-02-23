#ifndef _INCLUDED_EVENTZONE_
#define _INCLUDED_EVENTZONE_

#include "CollidableEntity.h"
#include <SFML/Graphics/RenderWindow.hpp>

class GameStateLevel;

class EventZone: public CollidableEntity{
public:
	enum EventType{
		SPIDER,
		SPIDER_EASY_END,
		SPIDER_MEDIUM_END,
		SPIDER_HARD_END
	};
	EventZone(EventType eventType, sf::Vector2f pos, sf::Shape* shape, float rotation, int level);
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
