#ifndef _INCLUDED_MENU_
#define _INCLUDED_MENU_

#include "InterfaceElement.h"
#include "Button.h"
#include "Slider.h"
#include "GameState.h"
#include "EventObserver.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include <vector>

class EventManager;
class GUIManager;
class GameStateLevel;

class Menu: public InterfaceElement, public EventObserver{
public:
	enum MenuType{
		MAINMENU,
		PAUSEMENU,
		ROOMMENU,
		DEATHMENU,
		PLAYMENU,
		SETTINGSMENU
	};
	Menu(sf::RenderWindow* window, EventManager* eventManager, GUIManager* gUIManager, MenuType menuType, Menu* previousMenu = nullptr);
	virtual ~Menu();

	void initialize(GameState* gameState);
	void initializeButtons();

	void tick(const sf::Time& deltaTime) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	Strata getRenderLayer() const override;
	bool isAlive() const override;
	bool isActive() const  override;
	void setActive(const bool& active) override;
	void onClick(std::string) override;
	virtual void onEvent(const sf::Event & aEvent) override;
	MenuType getMenuType() const;
	void kill();

private:
	void internalClear();
	void addButton(std::string buttonFile, std::string buttonText, std::string buttonFunc, sf::Vector2f pos, Button::ButtonType buttonType);
	void addSlider(std::string sliderFile, std::string gaugeFile, std::string attribute, float percent, std::string buttonFunc, sf::Vector2f pos);

	void buttonFuncNewGame();
	void buttonFuncPlay();
	void buttonFuncLoadGame();
	void buttonFuncPrevious();
	void buttonFuncExitLevel();
	void buttonFuncSettings();
	void buttonFuncQuitGame();
	void buttonFuncResetLevel();

	sf::RectangleShape* mBackground;
	typedef std::vector<Button*> ButtonVector;
	ButtonVector mButtons;
	typedef std::vector<Slider*> SliderVector;
	SliderVector mSliders;
	EventManager* mEventManager;
	GUIManager* mGUIManager;
	GameState* mGameState;
	sf::RenderWindow* mWindow;
	MenuType mMenuType;
	Menu* mPreviousMenu;

	bool mIsAlive;
	bool mIsActive;

};

#endif // !_INCLUDED_MENU_
