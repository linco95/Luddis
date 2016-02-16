#ifndef INCLUDED_LEVEL
#define INCLUDED_LEVEL

#include "Entity.h"
#include <SFML\Graphics.hpp>
#include <vector>
#include <rapidjson\document.h>

namespace sf{
	class Music;
}
class EntityManager;

class Level : public Entity {

public:
	Level(EntityManager* entityManager);
	~Level();

	void initializeLevel(sf::RenderWindow& aWindow, sf::Transformable* aTarget, std::string levelFilename);

	void tick(const sf::Time& deltaTime) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	bool isAlive() const override;
	bool isActive() const override;
	void setActive(const bool& active) override;
	Entity::RenderLayer getRenderLayer() const override;

private:
	void updateView(const sf::Time& deltaTime);
	void increaseMapBounds(sf::IntRect size);
	void initializeEntities(sf::RenderWindow* window, const rapidjson::Document& configDoc);
	void readInitMap();

	EntityManager* mEntityManager;
	sf::Transformable* mTarget;
	sf::RenderWindow *mWindow;
	typedef std::vector<sf::Sprite> SpriteVector;
	SpriteVector mBackgroundImages;
	sf::Music *mLevelMusic;
	sf::IntRect mMapBounds;
	std::vector<float> mPointsOfNoReturn;
	// PONR = Point Of No Return
	float mCurrentPONR;
	bool mIsActive;
};

#endif