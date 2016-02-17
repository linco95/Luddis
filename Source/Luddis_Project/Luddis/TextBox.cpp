#include "TextBox.h"
#include "ResourceManager.h"
#include <iostream>

TextBox::TextBox(sf::IntRect box, std::string text, int fontSize, std::string fontName):
mTextString(text),
mBox(box),
mFontSize(fontSize),
mRows(0),
mFontName(fontName){
	setFont(fontName);
	mText.setString(wrapText(mTextString, mBox.width, ResourceManager::getInstance().getFont(mFontName), mFontSize));
	mText.setPosition((float)mBox.left, (float)mBox.top);
}
TextBox::~TextBox(){

}

void TextBox::setString(const std::string& text){
	mText.setString(wrapText(text, mBox.width, ResourceManager::getInstance().getFont(mFontName), mFontSize));
	size_t position = 0;
	mRows = 1;
	while (position = mTextString.find("\n") != sf::String::InvalidPos){
		mRows++;
	}
}

void TextBox::setFont(std::string fontName){
	mText.setFont(ResourceManager::getInstance().getFont(fontName));
	mText.setCharacterSize(mFontSize);
	mText.setColor(sf::Color::Black);
	mText.setString(wrapText(mTextString, mBox.width, ResourceManager::getInstance().getFont(mFontName), mFontSize));
}

void TextBox::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	states.transform *= getTransform();
	target.draw(mText, states);
}

sf::String TextBox::wrapText(sf::String string, unsigned width, const sf::Font &font, unsigned characterSize, bool bold){
	unsigned currentOffset = 0;
	bool firstWord = true;
	std::size_t wordBegining = 0;

	for (std::size_t pos(0); pos < string.getSize(); ++pos) {
		char currentChar = string[pos];
		if (currentChar == '\n'){
			currentOffset = 0;
			firstWord = true;
			continue;
		}
		else if (currentChar == ' ') {
			wordBegining = pos;
			firstWord = false;
		}

		sf::Glyph glyph = font.getGlyph(currentChar, characterSize, bold);
		currentOffset += (unsigned int)glyph.advance;

		if (!firstWord && currentOffset > width) {
			pos = wordBegining;
			string[pos] = '\n';
			firstWord = true;
			currentOffset = 0;
		}
	}

	return string;
}

int TextBox::getRows() const{
	return mRows;
}