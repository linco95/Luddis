
#include "CollidableEntity.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <string>
#include "Animation.h"

class SpiderEgg : public CollidableEntity{
public:
	SpiderEgg(sf::RenderWindow* window, const std::string& textureFilename, const sf::Vector2f& aPos);
	~SpiderEgg();

	void tick(const sf::Time& deltaTime) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	bool isAlive() const override;
	bool isActive() const override;
	void setActive(const bool& active) override;
	RenderLayer getRenderLayer() const override;
	sf::FloatRect getHitBox() override;
	sf::Shape* getNarrowHitbox() const override;
	void stun(const sf::Time& deltatime) override;
private:
	Category getCollisionCategory() override;
	Type getCollisionType() override;
	void collide(CollidableEntity *collidable, const sf::Vector2f& moveAway) override;

	bool mIsAlive;
	bool mIsActive;
	Animation mAnimation;
	sf::RenderWindow* mWindow;
	sf::Shape* mHitbox;
};