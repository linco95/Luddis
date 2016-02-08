#ifndef INCLUDED_LEVEL
#define INCLUDED_LEVEL

#include "Entity.h"
#include <SFML\Graphics.hpp>
#include <vector>

namespace sf{
	class Music;
}

class Level : public Entity {

public:
	Level();
	~Level();

	void initializeLevel(sf::RenderWindow& aWindow, sf::Transformable* aTarget);
	void initializeEntities();

	void tick(const sf::Time& deltaTime) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	bool isAlive() const override;
	bool isActive() const override;
	void setActive(const bool& active) override;
	Entity::RenderLayer getRenderLayer() const override;

private:
	void updateView(const sf::Time& deltaTime);

	sf::Transformable* mTarget;
	sf::RenderWindow *mWindow;
	sf::Sprite mBackground;
	sf::Music *mLevelMusic;
	std::vector<float> mPointsOfNoReturn;
	// PONR = Point Of No Return
	float mCurrentPONR;
	bool mIsActive;
};

#endif