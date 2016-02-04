#include "Entity.h"
#include "Collidable.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <string>

class SpiderEgg : public Entity, public Collidable{
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
	virtual void collide(Collidable *collidable);

	bool mIsAlive;
	sf::Sprite mSprite;
	sf::RenderWindow* mWindow;
	sf::Shape* mHitbox;
};