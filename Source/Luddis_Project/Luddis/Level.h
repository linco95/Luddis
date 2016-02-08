#ifndef INCLUDED_LEVEL
#define INCLUDED_LEVEL

#include "Entity.h"
#include <SFML\Graphics.hpp>
#include <vector>
#include <rapidjson\document.h>

namespace sf{
	class Music;
}

class Level : public Entity {

public:
	Level();
	~Level();

	void initializeLevel(sf::RenderWindow& aWindow, sf::Transformable* aTarget);

	void tick(const sf::Time& deltaTime) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	bool isAlive() override;
	Entity::RenderLayer getRenderLayer() const override;

private:
	void updateView(const sf::Time& deltaTime);
	void increaseMapBounds(sf::IntRect size);
	void initializeEntities(sf::RenderWindow* window, const rapidjson::Document& configDoc);

	sf::Transformable* mTarget;
	sf::RenderWindow *mWindow;
	typedef std::vector<sf::Sprite> SpriteVector;
	SpriteVector mBackgroundImages;
	sf::Music *mLevelMusic;
	sf::IntRect mMapBounds;
	std::vector<float> mPointsOfNoReturn;
	// PONR = Point Of No Return
	float mCurrentPONR;
};

#endif