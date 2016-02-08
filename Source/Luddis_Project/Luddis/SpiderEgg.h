
#include "CollidableEntity.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <string>

class SpiderEgg : public CollidableEntity{
public:
	SpiderEgg(std::string textureFilename, sf::RenderWindow* window);
	~SpiderEgg();

	virtual void tick(const sf::Time& deltaTime);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual bool isAlive();
	virtual RenderLayer getRenderLayer() const;
	virtual sf::FloatRect getHitBox();
	sf::Shape* getNarrowHitbox() const override;
private:
	virtual Category getCollisionCategory();
	virtual Type getCollisionType();
	virtual void collide(CollidableEntity *collidable);

	bool mIsAlive;
	sf::Sprite mSprite;
	sf::RenderWindow* mWindow;
	sf::Shape* mHitbox;
};