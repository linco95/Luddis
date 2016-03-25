#ifndef _INCLUDED_TEXTBOX_
#define _INCLUDED_TEXTBOX_

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Time.hpp>
#include <string>
#include <vector>

class TextBox: public sf::Drawable, public sf::Transformable{
public:
	//Class representing a box of text that will wrap
	//around based on the size of the box passed on to it.
	TextBox(sf::IntRect box, std::string text, int fontSize, bool animate = false, std::string fontName = "Resources/Fonts/phitradesign Handwritten-Bold.ttf");
	~TextBox();

	void setString(const std::string& text);
	void setFont(std::string fontName);
	void setColor(sf::Color color);
	void animate(const sf::Time& deltaTime);
	void resetAnimation();
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	int getRows() const;

private:
	//Wraps text to the confines of the desgnated box size
	//(Box height is not used for anything).
	sf::String wrapText(sf::String string, unsigned width, const sf::Font &font, unsigned characterSize, bool bold = false);

	sf::Text mText;
	sf::IntRect mBox;
	int mFontSize;
	int mRows;
	const float MAX_ANIMATE_TIME;
	float mAnimateTime;
	bool mAnimating;
	sf::String mTextString;
	std::string mFontName;
};

#endif // !_INCLUDED_TEXTBOX_
