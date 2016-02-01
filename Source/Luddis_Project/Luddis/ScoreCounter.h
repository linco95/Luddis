#ifndef _INCLUDED_SCORECOUNTER_
#define _INCLUDED_SCORECOUNTER_

#include "Entity.h"
#include "Animation.h"
#include <SFML/Graphics/Text.hpp>

class ScoreCounter : public Entity{
public:
	ScoreCounter(std::string filename, sf::Vector2f screenPos);
	~ScoreCounter();

	virtual void tick(const sf::Time& deltaTime);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual bool isAlive();
	virtual RenderLayer getRenderLayer() const;
	void setScore(int score);
	void addToScore(int score);
private:
	bool mAlive;
	Animation mAnimation;
	sf::Text mCounter;
	int mScore;
};

#endif // !_INCLUDED_SCORECOUNTER_
