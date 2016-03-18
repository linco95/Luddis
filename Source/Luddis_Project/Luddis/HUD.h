#ifndef _INCLUDED_HUD_
#define _INCLUDED_HUD_

#include "InterfaceElement.h"
#include <vector>
#include <SFML/Graphics/Sprite.hpp>

class GUIManager;
class EventManager;
class Button;
class ScoreGauge;
class ScoreCounter;
class GameState;

namespace sf {
	class RenderWindow;
}

class HUD : public InterfaceElement {
public:
	enum HUDType {
		LEVEL,
		MAP
	};

	HUD(sf::RenderWindow* window, GUIManager* guiManager, EventManager* eventManager, GameState* gameState);
	~HUD();
	void initialize(HUDType hudType);

	void draw(sf::RenderTarget & target, sf::RenderStates states) const override;
	void tick(const sf::Time & deltaTime) override;
	Strata getRenderLayer() const override;
	bool isAlive() const override;
	bool isActive() const override;
	void setActive(const bool & active) override;
	void onClick(std::string buttonFunc) override;

private:
	sf::RenderWindow* mWindow;
	GUIManager* mGUIManager;
	EventManager* mEventManager;
	GameState* mGameState;

	sf::Sprite mMuteGraphic;
	Button* mSoundButton;
	ScoreGauge* mLuddGauge;
	ScoreCounter* mScoreCounter[2];

	bool mMute;
	bool mIsActive;
	bool mIsAlive;
};

#endif // !_INCLUDED_HUD_
