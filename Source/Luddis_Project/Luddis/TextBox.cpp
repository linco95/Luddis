#include "TextBox.h"
#include "ResourceManager.h"
#include <iostream>

const float ANIMATION_SPEED = 30.0f;

TextBox::TextBox(sf::IntRect box, std::string text, int fontSize, bool animate, std::string fontName):
mTextString(text),
mBox(box),
mFontSize(fontSize),
mAnimating(animate),
mRows(0),
mFontName(fontName),
MAX_ANIMATE_TIME((float)(text.size() / ANIMATION_SPEED)),
mAnimateTime(MAX_ANIMATE_TIME){
	if (!animate){
		mAnimateTime = 0.0f;
	}
	setFont(fontName);
	setString(mTextString);
	mText.setPosition((float)mBox.left, (float)mBox.top);
}

TextBox::~TextBox(){

}

void TextBox::setString(const std::string& text){
	mTextString = wrapText(mTextString, mBox.width, ResourceManager::getInstance().getFont(mFontName), mFontSize);
	mText.setString(mTextString);
}

void TextBox::setFont(std::string fontName){
	mText.setFont(ResourceManager::getInstance().getFont(fontName));
	mText.setCharacterSize(mFontSize);
	mText.setColor(sf::Color::Black);
	mText.setString(wrapText(mTextString, mBox.width, ResourceManager::getInstance().getFont(mFontName), mFontSize));
}

void TextBox::animate(const sf::Time& deltaTime){
	if (mAnimateTime > 0){
		mAnimateTime -= deltaTime.asSeconds();
		mAnimateTime = std::max(mAnimateTime, 0.0f);
		std::string tempString;
		int maxStringLength = (int)((MAX_ANIMATE_TIME - mAnimateTime) * 30);
		for (int i = 0; i < maxStringLength; i++){
			tempString.push_back(mTextString[i]);
		}
		mText.setString(tempString);
	}
	else if (mAnimating){
		mText.setString(mTextString);
		mAnimating = false;
	}
}

void TextBox::resetAnimation(){
	mAnimateTime = MAX_ANIMATE_TIME;
}

void TextBox::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	states.transform *= getTransform();
	target.draw(mText, states);
}

sf::String TextBox::wrapText(sf::String string, unsigned width, const sf::Font &font, unsigned characterSize, bool bold){
	unsigned currentOffset = 0;
	bool firstWord = true;
	std::size_t wordBeginning = 0;
	mRows = 1;

	for (std::size_t pos(0); pos < string.getSize(); ++pos) {
		char currentChar = string[pos];
		if (currentChar == '\n'){
			currentOffset = 0;
			firstWord = true;
			mRows++;
			continue;
		}
		else if (currentChar == ' ') {
			wordBeginning = pos;
			firstWord = false;
		}

		sf::Glyph glyph = font.getGlyph(currentChar, characterSize, bold);
		currentOffset += (unsigned int)glyph.advance;

		if (!firstWord && currentOffset > width) {
			pos = wordBeginning;
			string[pos] = '\n';
			firstWord = true;
			currentOffset = 0;
			mRows++;
		}
	}

	return string;
}

int TextBox::getRows() const{
	return mRows;
}