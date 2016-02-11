#ifndef _INCLUDED_PAUSEMENU_
#define _INCLUDED_PAUSEMENU_

#include "InterfaceElement.h"
#include "Button.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include <vector>

class PauseMenu: public InterfaceElement{
public:
	PauseMenu();
	virtual ~PauseMenu();

	void tick(const sf::Time& deltaTime) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	RenderLayer getRenderLayer() const override;
	bool isAlive() const override;
	bool isActive() const  override;
	void setActive(const bool& active) override;
	void onClick(std::string) override;

private:
	void internalClear();

	sf::RectangleShape mBackground;
	typedef std::vector<Button*> ButtonVector;
	ButtonVector mButtons;

	bool mIsAlive;
	bool mIsActive;
};

#endif // !_INCLUDED_PAUSEMENU_
