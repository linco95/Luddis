#ifndef INCLUDED_MOUSECURSOR_H
#define INCLUDED_MOUSECURSOR_H
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Sprite.hpp>

namespace sf {
	class Time;
	class RenderWindow;
}
class MouseCursor : public sf::Drawable, sf::Transformable {
public:
	MouseCursor();
	void initialize(const char* filepath, sf::RenderWindow& window);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void tick();
private:
	sf::Sprite mSprite;
	sf::RenderWindow* mWindow;
};

#endif // INCLUDED_MOUSECURSOR_H