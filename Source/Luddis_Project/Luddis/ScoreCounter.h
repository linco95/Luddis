#ifndef _INCLUDED_SCORECOUNTER_
#define _INCLUDED_SCORECOUNTER_

#include "Entity.h"
#include "Animation.h"
#include <SFML/Graphics/Text.hpp>

namespace sf{
	class RenderWindow;
}

class ScoreCounter : public Entity{
public:
	enum ScoreType{
		CHIPS,
		DUST
	};

	ScoreCounter(sf::RenderWindow* aWindow, std::string filename, sf::Vector2i screenPos, ScoreType type);
	~ScoreCounter();

	void tick(const sf::Time& deltaTime) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	bool isAlive() const override;
	bool isActive() const override;
	void setActive(const bool& active) override;
	RenderLayer getRenderLayer() const override;
	void setScore(int score);
	void addToScore(int score);
private:
	bool mAlive;
	bool mIsActive;
	sf::RenderWindow* mWindow;
	sf::Vector2i mPosition;
	Animation mAnimation;
	sf::Text mCounter;
	int mScore;
	ScoreType mType;
};

#endif // !_INCLUDED_SCORECOUNTER_
