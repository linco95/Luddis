#ifndef _INCLUDED_LUDDIS_
#define _INCLUDED_LUDDIS_

#include <string>
#include "AnimationQueue.h"
#include "CollidableEntity.h"
#include <SFML/Graphics/RenderWindow.hpp>

#include "PowerupDisplay.h"
#include <SFML/Audio/Sound.hpp>

class LuddisState;
class EntityManager;
class GameStateLevel;

class Luddis : public CollidableEntity{
public:
	Luddis(std::string textureFilename, sf::RenderWindow* windowm, EntityManager* entityManager);
	~Luddis();
	void tick(const sf::Time& deltaTime) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	bool isAlive() const override;
	bool isActive() const override;
	void setActive(const bool& active) override;
	RenderLayer getRenderLayer() const override;
	sf::FloatRect getHitBox() override;
	sf::Shape* getNarrowHitbox() const override;
	void reset(GameStateLevel* gameStateLevel);
	AnimationQueue* getAnimation();
	void setPlayerState(LuddisState* luddisState);
private:

	Category getCollisionCategory() override;
	Type getCollisionType() override;
	void collide(CollidableEntity *collidable, const sf::Vector2f& moveAway) override;

	AnimationQueue mAnimation;
	EntityManager* mEntityManager;
	sf::RenderWindow* mWindow;
	GameStateLevel* mGameStateLevel;
	LuddisState* mCurrentLuddState;
	bool mIsAlive;
	bool mIsActive;
	sf::Vector2f mPrevPos;
	int mLife;
	sf::FloatRect mCollideBox;
	sf::Shape* mHitbox;

};

#endif // !_INCLUDED_LUDDIS_
