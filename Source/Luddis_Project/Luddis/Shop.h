#ifndef _INCLUDED_SHOP_
#define _INCLUDED_SHOP_

#include "InterfaceElement.h"
#include "Button.h"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <vector>

class Inventory;
class GameState;
class EventManager;
class GUIManager;

namespace sf {
	class RenderWindow;
}

class Shop : public InterfaceElement{
public:
	Shop(sf::RenderWindow* window, GameState* gameState, EventManager* eventManager, GUIManager* guiManager);
	~Shop();

	void draw(sf::RenderTarget & target, sf::RenderStates states) const override;
	void tick(const sf::Time & deltaTime) override;
	Strata getRenderLayer() const override;
	bool isAlive() const override;
	bool isActive() const override;
	void setActive(const bool & active) override;
	void onClick(std::string command) override;
	void buttonFuncSwitchPage(int page);
	void buttonFuncClose();
	void buttonFunc();

private:
	void initialize();

	static const int MAX_PAGES = 10;

	sf::RenderWindow* mWindow;
	EventManager* mEventManager;
	GUIManager* mGUIManager;
	GameState* mGameState;
	Inventory* mInventory;
	sf::Sprite mBackground;
	sf::Text mText;
	int mActivePage;
	int mMaxPage;

	bool mIsAlive;
	bool mIsActive;
	typedef std::vector<Button*> ButtonVector;
	ButtonVector mButtons;
	ButtonVector mItems[MAX_PAGES];

};

#endif // !_INCLUDED_SHOP_
