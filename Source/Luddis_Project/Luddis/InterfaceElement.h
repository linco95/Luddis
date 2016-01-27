#ifndef _INCLUDED_INTERFACEELEMENT_
#define _INCLUDED_INTERFACEELEMENT_

#include <SFML\Graphics.hpp>
#include <SFML\System.hpp>

class InterfaceElement{
public:
	InterfaceElement();
	~InterfaceElement();

	virtual void tick(const sf::Time& deltaTime) = 0;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;

};

#endif // !_INCLUDED_INTERFACEELEMENT_
