#ifndef _INCLUDED_INTERFACEELEMENT_
#define _INCLUDED_INTERFACEELEMENT_

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/Time.hpp>
#include <string>

class InterfaceElement: public sf::Drawable, public sf::Transformable{
public:
	enum RenderLayer{
		BACKGROUND,
		FOREGROUND
	};
	InterfaceElement();
	virtual ~InterfaceElement();

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;
	virtual void tick(const sf::Time& deltaTime) = 0;
	virtual RenderLayer getRenderLayer() const = 0;
	virtual bool isAlive() const = 0;
	virtual bool isActive() const = 0;
	virtual void setActive(const bool& active) = 0;
	virtual void onClick(std::string buttonType){}
};

#endif // !_INCLUDEDINTERFACE_ELEMENT_
