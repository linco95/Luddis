#ifndef _INCLUDED_TEXTBOXDECORATOR_
#define _INCLUDED_TEXTBOXDECORATOR_

#include "InterfaceElement.h"

class TextBox;

class TextBoxDecorator :public InterfaceElement{
public:
	TextBoxDecorator(sf::IntRect box, std::string text, int fontSize, bool animate = false, std::string fontName = "Resources/Fonts/arial.ttf");
	~TextBoxDecorator();

	void draw(sf::RenderTarget & target, sf::RenderStates states) const override;
	void tick(const sf::Time & deltaTime) override;
	Strata getRenderLayer() const override;
	bool isAlive() const override;
	bool isActive() const override;
	void setActive(const bool & active) override;

	void setOrigin(float x, float y);
	void setOrigin(const sf::Vector2f& vector);

private:
	TextBox* mTextBox;

	bool mIsActive;
	bool mIsAlive;
};

#endif // !_INCLUDED_TEXTBOXDECORATOR_
