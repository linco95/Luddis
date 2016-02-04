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
	ScoreCounter(sf::RenderWindow* aWindow, std::string filename, sf::Vector2i screenPos);
	~ScoreCounter();

	virtual void tick(const sf::Time& deltaTime);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual bool isAlive();
	virtual RenderLayer getRenderLayer() const;
	void setScore(int score);
	void addToScore(int score);
private:
	bool mAlive;
	sf::RenderWindow* mWindow;
	sf::Vector2i mPosition;
	Animation mAnimation;
	sf::Text mCounter;
	int mScore;
};

#endif // !_INCLUDED_SCORECOUNTER_
