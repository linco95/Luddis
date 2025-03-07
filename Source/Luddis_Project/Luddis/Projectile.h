#ifndef _INCLUDED_PROJECTILE_
#define _INCLUDED_PROJECTILE_


#include "CollidableEntity.h"
#include <SFML/Window.hpp>
#include <string>

class Projectile : public CollidableEntity{
public:
	//The max life time should be entered in seconds
	Projectile(std::string textureFilename, sf::Vector2f direction,sf::Vector2f position, float maxLifeTimeMS, int damageValue, Category collisionCategory);
	virtual ~Projectile();

	void tick(const sf::Time& deltaTime) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	bool isAlive() const override;
	bool isActive() const override;
	void setActive(const bool& active) override;
	Renderer::RenderLayer getRenderLayer() const override;
	sf::FloatRect getHitBox() override;
	sf::Shape* getNarrowHitbox() const override;
	int getCollisionDamage() const override;
	void setTexture(std::string filename);
	void stun(const sf::Time& deltatime) override;
private:
	void updateMovement(const sf::Time& deltaTime);
	void checkLifeTime();

	sf::Sprite mSprite;
	float mLifeTime;
	int mDamage;
	bool mIsAlive;
	bool mIsActive;
	Category mCollisionCategory;
	sf::Vector2f mDirection;
	Category getCollisionCategory() override;
	Type getCollisionType() override;
	void collide(CollidableEntity *collidable, const sf::Vector2f& moveAway) override;
	sf::Shape* mHitbox;
	float mTimeStunned;
};

#endif // !_INCLUDED_PROJECTILE_
