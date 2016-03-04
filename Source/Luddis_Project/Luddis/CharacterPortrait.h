#ifndef _INCLUDED_CHARACTERPORTRAIT_
#define _INCLUDED_CHARACTERPORTRAIT_

#include "InterfaceElement.h"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <string>

class CharacterPortrait : public InterfaceElement{
public:
	CharacterPortrait(std::string textureFilename, std::string characterName, sf::Vector2f pos, bool mirror = false);
	virtual ~CharacterPortrait();
	void tick(const sf::Time& deltaTime) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	RenderLayer getRenderLayer() const override;
	bool isAlive() const override;
	bool isActive() const  override;
	void setActive(const bool& active) override;
	void kill();

	void expressEmotion(int frame);

private:
	static const int MAX_FRAMES = 16;

	bool mIsAlive;
	bool mIsActive;
	int mEmotionFrame;
	sf::Sprite mSprite;
	sf::Sprite mBubble;
	sf::Sprite mEmotion;
	sf::IntRect mFrame[MAX_FRAMES];
	sf::Text mName;
};

#endif // !_INCLUDED_CHARACTERPORTRAIT_
