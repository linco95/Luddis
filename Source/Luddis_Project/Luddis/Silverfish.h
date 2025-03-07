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
	/*
	* 0 - SILVER
	* 1 - GOLD
	*/
	enum FishType {
		SILVER,
		GOLD
	};
	Silverfish(sf::RenderWindow* window, FishType type, const sf::Vector2f& position, const float& angle, const float& activation, Transformable* aTarget);
	~Silverfish();

	void tick(const sf::Time& deltaTime) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	bool isAlive() const override;
	bool isActive() const override;
	void setActive(const bool& active) override;
	Renderer::RenderLayer getRenderLayer() const override;
	sf::FloatRect getHitBox() override;
	sf::Shape* getNarrowHitbox() const override;
	int getCollisionDamage() const override;
	void stun(const sf::Time& deltatime) override;
private:
	void updateMovement(const sf::Time& deltaTime);
	sf::Transformable* mTarget;
	bool mIsAlive;
	bool mIsActive;
	bool mSwimAway;
	sf::Sprite mSprite;
	sf::RenderWindow* mWindow;
	sf::Vector2f mDirection;
	Category getCollisionCategory() override;
	Type getCollisionType() override;
	void collide(CollidableEntity *collidable, const sf::Vector2f& moveAway) override;
	int mLife;
	bool mBefriend;
	const float mActivate;
	sf::Shape* mHitbox;
	FishType mType;
	AnimationQueue mAnimation;
	sf::Vector2f mNextDir;
	Category mAlignment;
	float mTimeStunned;
	float mInvulnerable;
};

#endif