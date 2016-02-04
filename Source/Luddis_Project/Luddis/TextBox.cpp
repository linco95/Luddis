#include "TextBox.h"
#include "ResourceManager.h"

TextBox::TextBox(sf::IntRect box, std::string text, int fontSize, std::string fontName):
mTextString(text),
mBox(box),
mFontSize(fontSize),
mFontName(fontName){
	makeSuperFly();
}

TextBox::~TextBox(){

}

void TextBox::setFont(std::string fontName){
	mFontName = fontName;
	for (TextVector::size_type i = 0; i < mTexts.size(); i++)
	{
		mTexts.at(i).setFont(ResourceManager::getInstance().getFont(mFontName));
	}
	makeSuperFly();
}

void TextBox::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	states.transform *= getTransform();
	for (TextVector::size_type i = 0; i < mTexts.size(); i++){
		target.draw(mTexts.at(i), states);
	}
}

void TextBox::makeSuperFly(){
	mTexts.clear();
	std::string tempString;
	int height = mBox.height;
	int width = mBox.width;
	int xPos = mBox.top;
	int yPos = mBox.left;
	addText(xPos, yPos);
	for (std::string::size_type i = 0; i < mTextString.length(); i++){
		tempString.push_back(mTextString.at(i));
		mTexts.back().setString(tempString);
		if (mTexts.back().getGlobalBounds().width >= width && tempString.back() == ' '){
			yPos += height/2;
			addText(xPos, yPos);
			tempString.clear();
		}
	}
}

void TextBox::addText(int xPos, int yPos){
	mTexts.push_back(sf::Text());
	mTexts.back().setFont(ResourceManager::getInstance().getFont(mFontName));
	mTexts.back().setCharacterSize(mFontSize);
	mTexts.back().setPosition((float)xPos, (float)yPos);
	mTexts.back().setColor(sf::Color::Black);
}