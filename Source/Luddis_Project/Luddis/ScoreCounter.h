#ifndef _INCLUDED_SCORECOUNTER_
#define _INCLUDED_SCORECOUNTER_

#include "InterfaceElement.h"
#include "Animation.h"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

namespace sf{
	class RenderWindow;
}

class ScoreCounter : public InterfaceElement{
public:
	enum ScoreType{
		CHIPS,
		DUST
	};

	ScoreCounter(sf::RenderWindow* aWindow, std::string filename, sf::Vector2f screenPos, ScoreType type);
	virtual ~ScoreCounter();

	void tick(const sf::Time& deltaTime) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	bool isAlive() const override;
	bool isActive() const override;
	void setActive(const bool& active) override;
	Strata getRenderLayer() const override;
	void setScore(int score);
	void addToScore(int score);

private:
	bool mAlive;
	bool mIsActive;
	sf::RenderWindow* mWindow;
	sf::Vector2f mPosition;
	sf::Sprite mSprite;
	sf::Text mCounter;
	int mScore;
	ScoreType mType;
};

#endif // !_INCLUDED_SCORECOUNTER_
