#ifndef _INCLUDED_PROJECTILE_
#define _INCLUDED_PROJECTILE_

#include "Entity.h"
#include "Collidable.h"
#include <SFML/Window.hpp>
#include <string>

class Projectile : public Entity, public Collidable{
public:
	//The max life time should be entered in seconds
	Projectile(std::string textureFilename, sf::Vector2f direction,sf::Vector2f position, float maxLifeTimeMS, Category collisionCategory);
	virtual ~Projectile();

	virtual void tick(const sf::Time& deltaTime);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual bool isAlive();
	virtual RenderLayer getRenderLayer() const;
	virtual sf::FloatRect getHitBox();
	sf::Shape getNarrowHitbox() const override;
	void setTexture(std::string filename);
private:
	void updateMovement(const sf::Time& deltaTime);
	void checkLifeTime();

	sf::Sprite mSprite;
	float mLifeTime;
	bool mIsAlive;
	Category mCollisionCategory;
	sf::Vector2f mDirection;
	virtual Category getCollisionCategory();
	virtual Type getCollisionType();
	virtual void collide(Collidable *collidable);
};

#endif // !_INCLUDED_PROJECTILE_
