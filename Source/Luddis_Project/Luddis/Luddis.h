#ifndef _INCLUDED_LUDDIS_
#define _INCLUDED_LUDDIS_

#include <string>
#include "AnimationQueue.h"
#include "CollidableEntity.h"
#include <SFML/Graphics/RenderWindow.hpp>

#include "PowerupDisplay.h"
#include <SFML/Audio/Sound.hpp>

class EntityManager;

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

private:
	sf::Vector2f getVectorMouseToSprite() const;
	void handleInput(const sf::Time& deltaTime);
	void updateMovement(const sf::Time& deltaTime);
	void updateRotation();
	void attack();
	Category getCollisionCategory() override;
	Type getCollisionType() override;
	void collide(CollidableEntity *collidable) override;
	void changeScale();

	AnimationQueue mAnimation;
	EntityManager* mEntityManager;
	sf::RenderWindow* mWindow;
	float mProjectileCooldown;
	float mStunDuration;
	float mLoseDust;
	bool mIsAlive;
	bool mIsActive;
	bool mColliding;
	bool mIsFlipped;
	PowerupDisplay* mPowerups[4];
	sf::FloatRect mCollideBox;
	sf::Vector2f mDirectionVector;
	sf::Vector2f mPrevPos;
	sf::Shape* mHitbox;

	float mScaleX;
	float mScaleY;
};

#endif // !_INCLUDED_LUDDIS_
