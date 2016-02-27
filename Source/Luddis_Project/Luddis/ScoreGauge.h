#ifndef _INCLUDED_SCOREGAUGE_
#define _INCLUDED_SCOREGAUGE_

#include "InterfaceElement.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <string>
#include <SFML/Graphics/Sprite.hpp>

class ResourceManager;
class Inventory;

class ScoreGauge : public InterfaceElement{
public:
	ScoreGauge(sf::RenderWindow* aWindow, std::string backgroundFilename, std::string gaugeFilename, sf::Vector2f screenPos, bool distort = true);
	virtual ~ScoreGauge();

	void tick(const sf::Time& deltaTime) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	bool isAlive() const override;
	bool isActive() const  override;
	void setActive(const bool& active) override;
	RenderLayer getRenderLayer() const override;
	void kill();
	void updateGauge(float fillPercent);

private:
	sf::RenderWindow* mWindow;
	Inventory* mInventory;
	ResourceManager* mResourceManager;
	sf::Sprite mFrame;
	sf::Sprite mGauge;
	sf::IntRect mFrameRectSize;

	bool mDistort;
	bool mIsAlive;
	bool mIsActive;
};

#endif // !_INCLUDED_SCOREGAUGE_
