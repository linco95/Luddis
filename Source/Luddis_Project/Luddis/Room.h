#ifndef _INCLUDED_ROOM_
#define _INCLUDED_ROOM_

#include "InterfaceElement.h"
#include "Button.h"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/View.hpp>

class GUIManager;
class EventManager;

class Room : public InterfaceElement{
public:
	typedef std::vector<Button*> ButtonVector;

	Room(GUIManager* guiManager, std::string textureFilename, EventManager* eventManager, sf::RenderWindow* window);
	~Room();

	void tick(const sf::Time& deltaTime) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	RenderLayer getRenderLayer() const override;
	bool isAlive() const override;
	bool isActive() const override;
	void setActive(const bool& active) override;
	void onClick(std::string buttonFunc) override;

	void createButtons(int room);
	void addButton(std::string buttonFile, std::string buttonText, std::string buttonFunc, sf::Vector2f pos, Button::ButtonType buttonType);

	void kill();

private:
	void buttonFuncShop();
	void buttonFuncLevel(std::string);
	void buttonFuncRoom(int room);

	typedef void(Room::*FadeEffect)(std::string);
	FadeEffect mFadeEffect;
	sf::Sprite mBackground;
	sf::RenderWindow* mWindow;
	ButtonVector mLevelButtons;
	GUIManager* mGUIManager;
	EventManager* mEventManager;
	std::string mButtonFunc;

	bool mIsActive;
	bool mIsAlive;
};

#endif // !_INCLUDED_ROOM_
