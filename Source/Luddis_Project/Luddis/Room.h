#ifndef _INCLUDED_ROOM_
#define _INCLUDED_ROOM_

#include "InterfaceElement.h"
#include "Button.h"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/View.hpp>

class GUIManager;

class Room : public InterfaceElement{
public:
	Room(sf::Vector2f position, GUIManager* guiManager, std::string textureFilename);
	~Room();

	void tick(const sf::Time& deltaTime) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	RenderLayer getRenderLayer() const override;
	bool isAlive() const override;
	bool isActive() const override;
	void setActive(const bool& active) override;
	void onClick(std::string buttonFunc) override;

	void buttonFuncShop();
	void buttonFuncLevel(int level);
	void buttonFuncRoom(int room);
	sf::View getView() const;
	void kill();

private:
	typedef std::vector<Button*> ButtonVector;
	sf::Sprite mBackground;
	sf::View mView;
	ButtonVector mLevelButtons;
	GUIManager* mGUIManager;

	bool mIsActive;
	bool mIsAlive;
};

#endif // !_INCLUDED_ROOM_
