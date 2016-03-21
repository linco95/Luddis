#include "TextBoxDecorator.h"
#include "TextBox.h"
#include <SFML/Graphics/RenderTarget.hpp>

TextBoxDecorator::TextBoxDecorator(sf::IntRect box, std::string text, int fontSize, bool animate, std::string fontName) :
	mTextBox(new TextBox(box, text, fontSize, animate, fontName)) {

}

TextBoxDecorator::~TextBoxDecorator() {
	delete mTextBox;
}

void TextBoxDecorator::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform *= getTransform();
	target.draw(*mTextBox, states);
}

void TextBoxDecorator::tick(const sf::Time& deltaTime) {
	mTextBox->animate(deltaTime);
}

TextBoxDecorator::Strata TextBoxDecorator::getRenderLayer() const {
	return FIRST;
}

bool TextBoxDecorator::isAlive() const {
	return mIsAlive;
}

bool TextBoxDecorator::isActive() const {
	return mIsActive;
}

void TextBoxDecorator::setActive(const bool & active) {
	mIsActive = active;
}

void TextBoxDecorator::setOrigin(float x, float y){
	mTextBox->setOrigin(x, y);
}

void TextBoxDecorator::setOrigin(const sf::Vector2f & vector){
	mTextBox->setOrigin(vector);
}
