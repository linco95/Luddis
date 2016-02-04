#ifndef _INCLUDED_DIALOGUE_
#define _INCLUDED_DIALOGUE_

#include <SFML/Graphics.hpp>
#include "Entity.h"
#include "Button.h"

class Dialogue : public Entity{
public:
	Dialogue(std::string text);
	~Dialogue();
	virtual void tick(const sf::Time& deltaTime);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual bool isAlive();
	virtual RenderLayer getRenderLayer() const;
	void updateText(const sf::Time& deltaTime);
private:
	bool mIsAlive;
	sf::Sprite mSprite;
	sf::Text mDialogueText;
	Button mButton1;
	Button mButton2;
};

#endif // !_INCLUDED_DIALOGUE_
