#ifndef _INCLUDED_MENU_
#define _INCLUDED_MENU_

#include "InterfaceElement.h"
#include "Button.h"
#include "GameState.h"
#include "EventObserver.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include <vector>

class EntityManager;
class EventManager;
class GUIManager;
class GameStateLevel;

class Menu: public InterfaceElement{
public:
	enum MenuType{
		MAINMENU,
		PAUSEMENU
	};
	Menu(sf::RenderWindow* window, EventManager* eventManager, GUIManager* gUIManager, MenuType menuType, EntityManager* entityManager);
	virtual ~Menu();

	void initialize(GameStateLevel* gameStateLevel);
	void initializeButtons(MenuType menuType);

	void tick(const sf::Time& deltaTime) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	RenderLayer getRenderLayer() const override;
	bool isAlive() const override;
	bool isActive() const  override;
	void setActive(const bool& active) override;
	void onClick(std::string) override;


private:
	void internalClear();
	void addButton(std::string buttonFile, std::string buttonText, std::string buttonFunc, sf::Vector2f pos);

	void buttonFuncNewGame();
	void buttonFuncLoadGame();
	void buttonFuncContinue();
	void buttonFuncExitLevel();
	void buttonFuncSettings();
	void buttonFuncQuitGame();
	void buttonFuncResetLevel();

	sf::RectangleShape* mBackground;
	typedef std::vector<Button*> ButtonVector;
	ButtonVector mButtons;
	EntityManager* mEntityManager;
	EventManager* mEventManager;
	GUIManager* mGUIManager;
	GameStateLevel* mGameStateLevel;
	sf::RenderWindow* mWindow;

	bool mIsAlive;
	bool mIsActive;
};

#endif // !_INCLUDED_MENU_
