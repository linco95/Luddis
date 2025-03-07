#ifndef INCLUDED_LEVEL
#define INCLUDED_LEVEL

#include "Entity.h"
#include <SFML\Graphics.hpp>
#include <vector>
#include <rapidjson\document.h>
#include "Luddis.h"

namespace sf{
	class Music;
}

class EntityManager;
class GameStateLevel;

class Level : public Entity {

public:
	Level(EntityManager* entityManager, Luddis* luddis);
	~Level();

	void initializeLevel(sf::RenderWindow& aWindow, sf::Transformable* aTarget, std::string levelFilename);

	void tick(const sf::Time& deltaTime) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	bool isAlive() const override;
	bool isActive() const override;
	void setActive(const bool& active) override;
	Renderer::RenderLayer getRenderLayer() const override;
	void initializeEntities(sf::RenderWindow* window, const rapidjson::Document& configDoc);
	void readInitMap(const std:: string& filename);
	void stun(const sf::Time& deltatime) override;
private:
	void updateView(const sf::Time& deltaTime);
	void createEffects();
	void increaseMapBounds(sf::IntRect size);

	EntityManager* mEntityManager;
	GameStateLevel* mGameStateLevel;
	sf::Transformable* mTarget;
	sf::RenderWindow *mWindow;
	Luddis* mLuddis;
	typedef std::vector<sf::Sprite> SpriteVector;
	SpriteVector mBackgroundImages;
	SpriteVector mForegroundImages;
	sf::Music *mLevelMusic;
	sf::IntRect mMapBounds;
	std::vector<float> mPointsOfNoReturn;
	int mProgress;
	// PONR = Point Of No Return
	float mCurrentPONR;
	bool mIsActive;
	float mEffectInterval;
	float mTimeStunned;
};

#endif