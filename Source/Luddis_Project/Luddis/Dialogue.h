#ifndef _INCLUDED_DIALOGUE_
#define _INCLUDED_DIALOGUE_

#include <SFML/Graphics.hpp>
#include "Entity.h"

class Dialogue : public Entity{
public:
	Dialogue(std::string textfile);
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
};

#endif // !_INCLUDED_DIALOGUE_
