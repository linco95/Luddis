#ifndef _INCLUDED_TEXTBOX_
#define _INCLUDED_TEXTBOX_

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Text.hpp>
#include <string>
#include <vector>

class TextBox: public sf::Drawable, public sf::Transformable{
public:
	TextBox(sf::IntRect box, std::string text, int fontSize, std::string fontName = "Resources/Fonts/arial.ttf");
	~TextBox();

	void setFont(std::string fontName);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
private:
	void makeSuperFly();
	void addText(int xPos, int yPos);

	typedef std::vector<sf::Text> TextVector;
	TextVector mTexts;
	sf::IntRect mBox;
	int mFontSize;
	std::string mTextString;
	std::string mFontName;
};

#endif // !_INCLUDED_TEXTBOX_
